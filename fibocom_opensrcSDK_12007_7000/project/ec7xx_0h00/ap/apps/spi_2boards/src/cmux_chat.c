/*
 * Copyright (c) 2019 xiaofan <xfan1024@live.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author          Notes
 * 2019-09-19     xiaofan         the first version
 * 2020-04-13     xiangxistu      transplant into cmux
 * 2024-02       EC              modify
 */
 
#include <osasys.h>

#include "cmux_chat.h"

#define CHAT_READ_BUF_MAX 256

/**
* In order to match response, we need a string search algorithm
* KMP and AC algorithm both are good choice, But we need the code
* is simple, readable and use lower RAM/ROM.
* So We use a simplified search algorithm, this alg is like the KMP.
* Specifically we assume the failure vecotor is [-1, 0, 0, 0, ...]
* This assuming is not work for all pattern string. Fortunately,
* it's work  for this scene.
*/

#define DEFINE_MODEM_RESP_STRDATA_TABLE(id, str) [id] = str
#define DEFINE_MODEM_RESP_STRLEN_TABLE(id, str)  [id] = (sizeof(str)-1)


static char *resp_strdata[] =
{
    MODEM_CHAT_RESP_LIST(DEFINE_MODEM_RESP_STRDATA_TABLE)
};

static uint8_t resp_strlen[] =
{
    MODEM_CHAT_RESP_LIST(DEFINE_MODEM_RESP_STRLEN_TABLE)
};

#define CHAT_DATA_FMT           "<tx: %s, want: %s, retries: %u, timeout: %u>"
#define CHAT_DATA_STR(data)     (data)->transmit, resp2str((data)->expect), (data)->retries, (data)->timeout

static const char* resp2str(uint8_t resp_id)
{
    if (resp_id == MODEM_CHAT_RESP_NOT_NEED)
        return "(not need)";
    EC_ASSERT(resp_id < MODEM_CHAT_RESP_MAX, 0, 0, 0);
    return resp_strdata[resp_id];
}

uint8_t resp_match(uint8_t resp_id, uint8_t state, char ch)
{
    while (1)
    {
        //ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_resp_match_1, P_VALUE, "resp2str(%d)[%d] = 0x%x, ch = 0x%x", resp_id, state, resp2str(resp_id)[state], ch);
        if (resp2str(resp_id)[state] == ch){
            ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_resp_match_2, P_VALUE, "has %d byte match", state+1);
            return state + 1;
        }
        if (state == 0){
            //ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_resp_match_3, P_VALUE, "state == 0");
            return 0;
        }
        state = 0;
    };
}

uint8_t resp_matched(uint8_t resp_id, uint8_t match_degree)
{
    //ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_resp_matched_1, P_VALUE, "resp_strlen[%d] = %d, state = %d", resp_id, resp_strlen[resp_id],match_degree);
    return match_degree == resp_strlen[resp_id];
}

/**
 * chat_read_until, waitting for recieve data from serial
 *
 * @param obj       the point of cmux object
 * @param link_port     the channel of virtual serial
 * @param buffer    the buffer is waitting for recieve uart data from ppp
 * @param size      the max length of CHAT_READ_BUF_MAX
 * @param stop      the max of tick time
 *
 * @return  >=0:   the length of read data
 *          <0 :   rt_device_read failed
 */
static size_t chat_read_until(struct cmux *obj, int link_port, void *buffer, size_t size, uint32_t stop)
{
    size_t rdlen = 0;
    uint32_t now;
    ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_chat_read_until_0, P_VALUE, "enter.");
    rdlen = cmux_vcom_read(link_port, buffer, size);
    if(rdlen){
        ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_chat_read_until_1, P_VALUE, "has UIH data in frame list.");
        return rdlen;
    }
    now = osKernelGetTickCount()/portTICK_PERIOD_MS/osKernelGetTickFreq();
    if(now > stop){
        ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_chat_read_until_2, P_VALUE, "has time out.");
        return 0;
    }
    return 0;
}

/**
 * modem_flush_rx , clear frame what is in the channel of virtual serial
 *
 * @param link_port     the channel of virtual serial
 */
static void modem_flush_rx(int link_port)
{
    char rdbuf[CHAT_READ_BUF_MAX] = {0};
    ECPLAT_PRINTF(UNILOG_PLA_APP, modem_flush_rx_1, P_VALUE, "enter.");
    while (cmux_vcom_read(link_port, rdbuf, CHAT_READ_BUF_MAX));
}

/**
 * modem_chat_once , send an order to control modem
 *
 * @param object    the point of cmux
 * @param link_port the channel of virtual serial
 * @param data      the AT command
 *
 * @return  =0:   modem_chat_once successful
 *          <0:   modem_chat_once failed
 */
static err_t modem_chat_once(struct cmux *obj, int link_port, const struct modem_chat_data *data)
{
    uint8_t resp_match_degree[MODEM_CHAT_RESP_MAX] = { 0 }, i;
    uint32_t stop = osKernelGetTickCount()/portTICK_PERIOD_MS/osKernelGetTickFreq() + data->timeout;
    uint32_t now = 0;
    size_t rdlen, pos;
    char rdbuf[CHAT_READ_BUF_MAX];
    char* tempstr = NULL;
    int sendDataLen = 0;
    
    if (data->transmit)
    {
        ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_modem_chat_once_1, P_VALUE, CHAT_DATA_FMT" transmit --> modem", CHAT_DATA_STR(data));
        if (data->ignore_cr == FALSE){
            sendDataLen = strlen(data->transmit)+1;
            tempstr = malloc(sendDataLen+1);
            strcpy(tempstr, data->transmit);
            strcat(tempstr,"\r");
            cmux_vcom_write(link_port, (uint8_t*)tempstr, sendDataLen);
        }else{
            cmux_vcom_write(link_port, (uint8_t*)data->transmit, strlen(data->transmit));
        }
    }

    if (data->expect == MODEM_CHAT_RESP_NOT_NEED)
    {
        osDelay(1000*data->timeout);
        return CMUX_OK;
    }

    do
    {
        rdlen = chat_read_until(obj, link_port, rdbuf, CHAT_READ_BUF_MAX, stop);
        if(rdlen != 0){
            ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_modem_chat_once_1_1, P_VALUE, "read: %s", (uint8_t*)rdbuf);
        }
        for (pos = 0; pos < rdlen; pos++)
        {
            ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_modem_chat_once_1_2, P_VALUE, "pos: %d", pos);
            for (i = 0; i < MODEM_CHAT_RESP_MAX; i++)
            {
                resp_match_degree[i] = resp_match(i, resp_match_degree[i], rdbuf[pos]);
                //ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_modem_chat_once_1_3, P_VALUE, "resp_match_degree[%d]=%d", i, resp_match_degree[i]);
                if (resp_matched(i, resp_match_degree[i]))
                {
                    //ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_modem_chat_once_1_4, P_VALUE, "resp_matched data->expect = %d", data->expect);
                    if (i == data->expect){
                        ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_modem_chat_once_2, P_VALUE, "has resp correct");
                        return CMUX_OK;
                    }else{
                        ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_modem_chat_once_3, P_VALUE, CHAT_DATA_FMT" not matched, got: %d", CHAT_DATA_STR(data), i);
                        return CMUX_OK;
                    }
                }
            }
        }
        osDelay(100);
        now = osKernelGetTickCount()/portTICK_PERIOD_MS/osKernelGetTickFreq();
        ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_modem_chat_once_3_1, P_VALUE, "now:%d.", now);
    } while (now < stop);
    ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_modem_chat_once_4, P_VALUE, CHAT_DATA_FMT" timeout", CHAT_DATA_STR(data));
    return -CMUX_TIMEOUT;
}

/**
 * modem_chat , sent AT commands
 *
 * @param port      the channel
 * @param data      the AT command, it is the address of chat strcuture, a collection of AT command
 * @param len       the length of this collection of AT command
 *
 * @return  =0:   successful
 *          <0:   failed
 */
err_t modem_chat(struct cmux *obj, int link_port, const struct modem_chat_data *data, size_t len)
{
    err_t err = CMUX_OK;
    size_t i;
    uint8_t retry_time;

    for (i = 0; i < len; i++)
    {
        ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_modem_chat_1, P_VALUE, CHAT_DATA_FMT" running", CHAT_DATA_STR(&data[i]));
        for (retry_time = 0; retry_time < data[i].retries; retry_time++)
        {
            modem_flush_rx(link_port);
            err = modem_chat_once(obj, link_port, &data[i]);
            if (err == CMUX_OK)
                break;
        }
        if (err != CMUX_OK)
        {
            ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_modem_chat_2, P_VALUE, CHAT_DATA_FMT" fail", CHAT_DATA_STR(&data[i]));
            break;
        }
        ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_modem_chat_3, P_VALUE, CHAT_DATA_FMT" success", CHAT_DATA_STR(&data[i]));
    }
    return err;
}

