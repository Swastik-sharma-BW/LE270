/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author         Notes
 * 2020-04-15    xiangxistu      the first version
 * 2024-02       EC              modify
 */

#include "cmux.h"
#include "cmux_chat.h"
#include "osasys.h"
#include "sfdt.h"

// bits: Poll/final, Command/Response, Extension
#define CMUX_CONTROL_PF 16
#define CMUX_ADDRESS_CR 2
#define CMUX_ADDRESS_EA 1
// the types of the frames
#define CMUX_FRAME_SABM 47
#define CMUX_FRAME_UA 99
#define CMUX_FRAME_DM 15
#define CMUX_FRAME_DISC 67
#define CMUX_FRAME_UIH 239
#define CMUX_FRAME_UI 3
// the types of the control channel commands
#define CMUX_C_CLD 193
#define CMUX_C_TEST 33
#define CMUX_C_MSC 225
#define CMUX_C_NSC 17
// basic mode flag for frame start and end
#define CMUX_HEAD_FLAG (unsigned char)0xF9

#define CMUX_DHCL_MASK       63         /* DLCI number is port number, 63 is the mask of DLCI; C/R bit is 1 when we send data */
#define CMUX_DATA_MASK       127        /* when data length is out of 127( 0111 1111 ), we must use two bytes to describe data length in the cmux frame */
#define CMUX_HIGH_DATA_MASK  32640      /* 32640 ( 0111 1111 1000 0000 ), the mask of high data bits */

#define CMUX_COMMAND_IS(command, type) ((type & ~CMUX_ADDRESS_CR) == command)
#define CMUX_PF_ISSET(frame) ((frame->control & CMUX_CONTROL_PF) == CMUX_CONTROL_PF)
#define CMUX_FRAME_IS(type, frame) ((frame->control & ~CMUX_CONTROL_PF) == type)

#define min(a, b) ((a) <= (b) ? (a) : (b))

/* increases buffer pointer by one and wraps around if necessary */
#define INC_BUF_POINTER(buf, p)  \
    (p)++;                       \
    if ((p) == (buf)->end_point) \
        (p) = (buf)->data;

/* Tells, how many chars are saved into the buffer */
#define cmux_buffer_length(buff) (((buff)->read_point > (buff)->write_point) ? (CMUX_BUFFER_SIZE - ((buff)->read_point - (buff)->write_point)) : ((buff)->write_point - (buff)->read_point))

/* Tells, how much free space there is in the buffer */
#define cmux_buffer_free(buff) (((buff)->read_point > (buff)->write_point) ? ((buff)->read_point - (buff)->write_point) : (CMUX_BUFFER_SIZE - ((buff)->write_point - (buff)->read_point)))

#define CMUX_THREAD_STACK_SIZE (CMUX_RECV_READ_MAX + 1536)

static size_t cmux_send_data(int port, uint8_t type, const char *data, int length);
static err_t cmux_vcom_open(int link_port);
/* only one cmux object can be created */
static struct cmux *_g_cmux = NULL;

//EC add begin
#define CMUX_MAX_FRAME_LIST_LEN 20
#define CMUX_MAX_RETRY_TIMES 3
#define CMUX_T1 10   //unit 10ms

void slist_init(slist_t *l)
{
    l->next = NULL;
}

void slist_append(slist_t *l, slist_t *n)
{
    struct slist_node *node;

    node = l;
    while (node->next) node = node->next;

    /* append the node to the tail */
    node->next = n;
    n->next = NULL;
}

slist_t *slist_first(slist_t *l)
{
   return l->next;
}

slist_t *slist_remove(slist_t *l, slist_t *n)
{
   struct slist_node *node = l;
   while(node->next && node->next != n) node = node->next;
   if(node->next != NULL) node->next = node->next->next;
   return l->next;
}

#define container_of(ptr, type, member) \
    ((type *)((char *)(ptr) - (unsigned long)(&((type *)0)->member)))

static void spi_device_write(uint8_t* data,    int datalen)
{
    uint8_t* txBuffer = NULL;
    txBuffer = SFDT_mallocTxFrameBuffer(datalen);
    if(txBuffer)
    {
        ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_spi_device_write_1, P_VALUE, "call SFDT_masterSend.");
        memcpy(txBuffer, data, datalen);
        SFDT_masterSend(txBuffer, datalen);
    }
}

static void cmux_timer_expired(void* channel)
{
    int link_port = (int)channel;
    struct cmux *cmux = NULL;
    cmux = _g_cmux;
    struct cmux_vcoms *vcom = &cmux->vcoms[link_port];

    ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_timer_expired_1, P_VALUE, "channel %d open timer expired, retrytimes %d,", link_port, vcom->openRetryTimes);
    if(osTimerIsRunning(vcom->openTimer) != 0)
    {
        osTimerStop(vcom->openTimer);
    }
    osTimerDelete(vcom->openTimer);
    vcom->openTimer = NULL;
    if(vcom->openRetryTimes < CMUX_MAX_RETRY_TIMES){
        vcom->openRetryTimes ++;
        cmux_vcom_open(link_port);
    }else{
        vcom->openStat = CMUX_OPENFAIL;
    }
}

static void cmux_start_timer(int link_port, uint32_t ticks)
{
    struct cmux *cmux = NULL;
    cmux = _g_cmux;
    struct cmux_vcoms *vcom = &cmux->vcoms[link_port];

    ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_start_timer_1, P_VALUE, "start timer of channel %d, %d ms", link_port, ticks);
    if(vcom->openTimer == NULL)
    {
        vcom->openTimer = osTimerNew((osTimerFunc_t)cmux_timer_expired, osTimerOnce, (void*)(uint32_t)vcom->link_port, NULL);
        osTimerStart(vcom->openTimer, ticks);
    }
}

static void cmux_stop_timer(int link_port)
{
    struct cmux *cmux = NULL;
    cmux = _g_cmux;
    struct cmux_vcoms *vcom = &cmux->vcoms[link_port];

    ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_stop_timer_1, P_VALUE, "stop timer of channel %d ,", link_port);
    if(osTimerIsRunning(vcom->openTimer) != 0)
    {
        osTimerStop(vcom->openTimer);
    }
    osTimerDelete(vcom->openTimer);
    vcom->openTimer = NULL;

    vcom->openRetryTimes = 0;
}

//EC add end

/**
 *  allocate buffer for cmux object receive
 *
 * @param RT_NULL
 *
 * @return  the point of struct cmux_buffer
 */
static struct cmux_buffer *cmux_buffer_init()
{
    struct cmux_buffer *buff = NULL;
    buff = malloc(sizeof(struct cmux_buffer));
    if (buff == NULL)
    {
        return NULL;
    }
    memset(buff, 0, sizeof(struct cmux_buffer));
    buff->read_point = buff->data;
    buff->write_point = buff->data;
    buff->end_point = buff->data + CMUX_BUFFER_SIZE;
    return buff;
}

/**
 *  destroy buffer for cmux object receive
 *
 * @param frame         the point of cmux_frame
 *
 * @return  RT_NULL
 */
static void cmux_frame_destroy(struct cmux_frame *frame)
{
    if ((frame->data_length > 0) && frame->data)
    {
        free(frame->data);
    }
    if (frame)
    {
        free(frame);
    }
}

/**
 *  initial virtual serial for different channel, initial slist for channel
 *
 * @param cmux          cmux object
 * @param channel       the number of virtual serial
 *
 * @return  RT_NULL
 */
static void vcoms_cmux_frame_init(struct cmux *cmux, int channel)
{
    uint32_t mask = SaveAndSetIRQMask();
    slist_init(&cmux->vcoms[channel].flist);
    RestoreIRQMask(mask);
    cmux->vcoms[channel].frame_index = 0;

    ECPLAT_PRINTF(UNILOG_PLA_APP, vcoms_cmux_frame_init_3, P_VALUE, "init cmux data channel(%d) list.", channel);
}

/**
 *  push cmux frame data into slist for different channel virtual serial
 *
 * @param cmux          cmux object
 * @param channel       the number of virtual serial
 * @param frame         the point of frame data
 *
 * @return  RT_EOK      successful
 *          CMUX_NOMEM   allocate memory failed
 */
static err_t cmux_frame_push(struct cmux *cmux, int channel, struct cmux_frame *frame)
{
    struct frameList *frame_new = NULL;
    uint16_t frame_len = cmux->vcoms[channel].frame_index;

    if (frame_len <= CMUX_MAX_FRAME_LIST_LEN)
    {
        frame_new = malloc(sizeof(struct frameList));
        if (frame_new == NULL)
        {
            ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_frame_push_1, P_VALUE, "can't malloc <struct frame> to record data address.");
            return -CMUX_NOMEM;
        }
        else
        {
            memset(frame_new, 0, sizeof(struct frameList));

            frame_new->frame = frame;
            slist_init(&frame_new->frame_list);
        }

        uint32_t mask = SaveAndSetIRQMask();
        slist_append(&cmux->vcoms[channel].flist, &frame_new->frame_list);
        RestoreIRQMask(mask);
        cmux->vcoms[channel].frame_index++;

        ECPLAT_DUMP(UNILOG_PLA_APP, cmux_frame_push_2, P_VALUE, "CMUX_RX:", frame->data_length, frame->data);

        ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_frame_push_3, P_VALUE, "new message (len:%d) for channel (%d) is append, Message total: %d.", frame_new->frame->data_length, channel, cmux->vcoms[channel].frame_index);

        return CMUX_OK;
    }
    ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_frame_push_4, P_VALUE, "malloc failed, the message for channel(%d) is long than CMUX_MAX_FRAME_LIST_LEN(%d).", channel, CMUX_MAX_FRAME_LIST_LEN);
    return -CMUX_NOMEM;
}

/**
 *  pop cmux frame data from slist for different channel virtual serial
 *
 * @param cmux          cmux object
 * @param channel       the number of virtual serial
 *
 * @return  frame_data  successful
 *          RT_NULL     no message on the slist
 */
static struct cmux_frame *cmux_frame_pop(struct cmux *cmux, int channel)
{
    struct frameList *frameNode = NULL;
    struct cmux_frame *frame_data = NULL;
    struct slist_node *frame_list_find = NULL;
    slist_t *frame_list = NULL;

    frame_list = &cmux->vcoms[channel].flist;

    frame_list_find = slist_first(frame_list);
    if (frame_list_find != NULL)
    {
        frameNode = container_of(frame_list_find, struct frameList, frame_list);
        frame_data = frameNode->frame;

        uint32_t mask = SaveAndSetIRQMask();
        slist_remove(frame_list, frame_list_find);
        RestoreIRQMask(mask);

        cmux->vcoms[channel].frame_index--;

        ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_frame_pop_1, P_VALUE, "A message (len:%d) for channel (%d) has been used, Message remain: %d.", frame_data->data_length, channel, cmux->vcoms[channel].frame_index);
        free(frameNode);
    }

    return frame_data;
}

/**
 *  write data into cmux buffer
 *
 * @param buff        the buffer of cmux object
 * @param input       the point of data
 * @param count       the length of data
 *
 * @return length     the length of write into cmux buffer
 *
 */
size_t cmux_buffer_write(struct cmux_buffer *buff, uint8_t *input, size_t count)
{
    int c = buff->end_point - buff->write_point;

    count = min(count, cmux_buffer_free(buff));
    if (count > c)
    {
        memcpy(buff->write_point, input, c);
        memcpy(buff->data, input + c, count - c);
        buff->write_point = buff->data + (count - c);
    }
    else
    {
        memcpy(buff->write_point, input, count);
        buff->write_point += count;
        if (buff->write_point == buff->end_point)
            buff->write_point = buff->data;
    }

    return count;
}

/**
 *  parse buffer for searching cmux frame
 *
 * @param buffer        the cmux buffer for cmux object
 *
 * @return  frame       successful
 *          RT_NULL     no frame in the buffer
 */
static struct cmux_frame *cmux_frame_parse(struct cmux_buffer *buffer)
{
    int end;
    int length_needed = 5; /* channel, type, length, fcs, flag */
    uint8_t *data = NULL;
    uint8_t fcs = 0xFF;
    struct cmux_frame *frame = NULL;

    extern uint8_t cmux_crctable[256];

    /* Find start flag */
    while (!buffer->flag_found && cmux_buffer_length(buffer) > 0)
    {
        if (*buffer->read_point == CMUX_HEAD_FLAG)
            buffer->flag_found = 1;
        INC_BUF_POINTER(buffer, buffer->read_point);
    }
    if (!buffer->flag_found) /* no frame started */
        return NULL;

    /* skip empty frames (this causes troubles if we're using DLC 62) */
    while (cmux_buffer_length(buffer) > 0 && (*buffer->read_point == CMUX_HEAD_FLAG))
    {
        INC_BUF_POINTER(buffer, buffer->read_point);
    }

    if (cmux_buffer_length(buffer) >= length_needed)
    {
        data = buffer->read_point;
        frame = (struct cmux_frame *)malloc(sizeof(struct cmux_frame));
        frame->data = NULL;

        frame->channel = ((*data & 0xFC) >> 2);
        fcs = cmux_crctable[fcs ^ *data];
        INC_BUF_POINTER(buffer, data);

        frame->control = *data;
        fcs = cmux_crctable[fcs ^ *data];
        INC_BUF_POINTER(buffer, data);

        frame->data_length = (*data & 254) >> 1;
        fcs = cmux_crctable[fcs ^ *data];
        /* frame data length more than 127 bytes */
        if ((*data & 1) == 0)
        {
            INC_BUF_POINTER(buffer,data);
            frame->data_length += (*data*128);
            fcs = cmux_crctable[fcs^*data];
            length_needed++;
            ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_frame_parse_1, P_VALUE, "len_need: %d, frame_data_len: %d.", length_needed, frame->data_length);
        }
        length_needed += frame->data_length;
        if (cmux_buffer_length(buffer) < length_needed)
        {
            cmux_frame_destroy(frame);
            return NULL;
        }
        INC_BUF_POINTER(buffer, data);
        /* extract data */
        if (frame->data_length > 0)
        {
            frame->data = (unsigned char *)malloc(frame->data_length);
            if (frame->data != NULL)
            {
                end = buffer->end_point - data;
                if (frame->data_length > end)
                {
                    memcpy(frame->data, data, end);
                    memcpy(frame->data + end, buffer->data, frame->data_length - end);
                    data = buffer->data + (frame->data_length - end);
                }
                else
                {
                    memcpy(frame->data, data, frame->data_length);
                    data += frame->data_length;
                    if (data == buffer->end_point)
                        data = buffer->data;
                }
                if (CMUX_FRAME_IS(CMUX_FRAME_UI, frame))
                {
                    for (end = 0; end < frame->data_length; end++)
                        fcs = cmux_crctable[fcs ^ (frame->data[end])];
                }
            }
            else
            {
                ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_frame_parse_2, P_VALUE, "Out of memory, when allocating space for frame data.");
                frame->data_length = 0;
            }
        }
        /* check FCS */
        if (cmux_crctable[fcs ^ (*data)] != 0xCF)
        {
            ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_frame_parse_3, P_VALUE, "Dropping frame: FCS doesn't match. Remain size: %d", cmux_buffer_length(buffer));
            cmux_frame_destroy(frame);
            buffer->flag_found = 0;
            return cmux_frame_parse(buffer);
        }
        else
        {
            /* check end flag */
            INC_BUF_POINTER(buffer, data);
            if (*data != CMUX_HEAD_FLAG)
            {
                ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_frame_parse_4, P_VALUE, "Dropping frame: End flag not found. Instead: %d.", *data);
                cmux_frame_destroy(frame);
                buffer->flag_found = 0;
                return cmux_frame_parse(buffer);
            }
            else
            {
            }
            INC_BUF_POINTER(buffer, data);
        }
        buffer->read_point = data;
    }
    return frame;
}

/**
 *  handle the cmux frame data from control channel
 *
 * @param cmux          cmux object
 * @param frame         the point of frame data
*/
static void cmux_handle_ctrl_chan_frame(struct cmux *cmux, struct cmux_frame *frame)
{
    if(frame->data[0] == 0xc1){
        ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_handle_ctrl_chan_frame_1, P_VALUE, "this is CLD's response.");
        cmux->cmux_mode = CMUX_AT_MODE;
        cmux->recv_task_run = 0;
    }
}

/**
 * process CMUX mode data from serial, push frame into slist and handle UA
 *
 * @param   cmux    cmux object
 * @param   buf     the address of receive data from uart
 * @param   len     the length of receive data
 */
static void cmux_recv_processdata(struct cmux *cmux, uint8_t *buf, size_t len)
{
    size_t count = len;
    struct cmux_frame *frame = NULL;

    cmux_buffer_write(cmux->buffer, buf, count);

    while ((frame = cmux_frame_parse(cmux->buffer)) != NULL)
    {
        /* distribute different data */
        if ((CMUX_FRAME_IS(CMUX_FRAME_UI, frame) || CMUX_FRAME_IS(CMUX_FRAME_UIH, frame)))
        {
            ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_recv_processdata_1, P_VALUE, "this is UI or UIH frame from channel(%d).", frame->channel);
            if (frame->channel > CMUX_CTRL_PORT)
            {
                /* receive data from logical channel, distribution them */
                cmux_frame_push(cmux, frame->channel, frame);
            }
            else
            {
                /* control channel command */
                ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_recv_processdata_2, P_VALUE, "control channel command recv UIH.");
                cmux_handle_ctrl_chan_frame(cmux, frame);
            }
        }
        else
        {
            switch ((frame->control & ~CMUX_CONTROL_PF))
            {
            case CMUX_FRAME_UA:
                ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_recv_processdata_3, P_VALUE, "This is UA frame for channel(%d).stop time", frame->channel);
                cmux_stop_timer(frame->channel);
                cmux->vcoms[frame->channel].openStat = (uint8_t)CMUX_OPEN;
                break;
            case CMUX_FRAME_DM:
                ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_recv_processdata_4, P_VALUE, "This is DM frame for channel(%d).", frame->channel);
                break;
            case CMUX_FRAME_SABM:
                ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_recv_processdata_5, P_VALUE, "This is SABM frame for channel(%d).", frame->channel);
                break;
            case CMUX_FRAME_DISC:
                ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_recv_processdata_6, P_VALUE, "This is DISC frame for channel(%d).", frame->channel);
                break;
            }
            cmux_frame_destroy(frame);
        }
    }
}

/**
 * process AT data from serial, push frame into slist and invoke callback function
 *
 * @param   cmux    cmux object
 * @param   buf     the address of receive data from uart
 * @param   len     the length of receive data
 */
static void cmux_recv_processAtdata(struct cmux *cmux, uint8_t *buf, size_t len)
{
    size_t pos;
    uint8_t resp_match_degree[MODEM_CHAT_RESP_MAX] = { 0 },i;
    ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_recv_processAtdata_1, P_VALUE, "recv len: %d", len);
    for (pos = 0; pos < len; pos++)
    {
        ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_recv_processAtdata_2, P_VALUE, "pos: %d", pos);
        for (i = 0; i < MODEM_CHAT_RESP_MAX; i++)
        {
            resp_match_degree[i] = resp_match(i, resp_match_degree[i], buf[pos]);
            //ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_recv_processAtdata_3, P_VALUE, "resp_match_degree[%d]=%d", i, resp_match_degree[i]);
            if (resp_matched(i, resp_match_degree[i]))
            {
                //ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_recv_processAtdata_4, P_VALUE, "resp_matched");
                if (i == MODEM_CHAT_RESP_OK){
                    ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_recv_processAtdata_5, P_VALUE, "has resp correct");
                    cmux->cmux_mode = CMUX_CMUX_MODE;
                    return;
                }else{
                    ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_recv_processAtdata_6, P_VALUE, "matched error str");
                    cmux->cmux_mode = CMUX_UNKNOWN_MODE;
                    return;
                }
            }
        }
    }
    return;
}

/**
 *  assemble general data in the format of cmux
 *
 * @param port          the number of virtual serial
 * @param type          the format of cmux frame
 * @param data          general data
 * @param length        the length of general data
 *
 * @return  length
 */
static size_t cmux_send_data(int port, uint8_t type, const char *data, int length)
{
    /* flag, EA=1 C port, frame type, data_length 1-2 */
    uint8_t prefix[5] = {CMUX_HEAD_FLAG, CMUX_ADDRESS_EA | CMUX_ADDRESS_CR, 0, 0, 0};
    uint8_t postfix[2] = {0xFF, CMUX_HEAD_FLAG};
    int prefix_length = 4;
    uint8_t* sendbuf = NULL;
    int sendtotallen = 0;

    sendbuf = malloc(7+length);
    if(sendbuf == NULL)
        return -CMUX_NOMEM;

    /* EA=1, Command, let's add address */
    prefix[1] = prefix[1] | ((CMUX_DHCL_MASK & port) << 2);
    /* cmux control field */
    prefix[2] = type;

    if (length > CMUX_DATA_MASK)
    {
        prefix_length = 5;
        prefix[3] = ((CMUX_DATA_MASK & length) << 1);
        prefix[4] = (CMUX_HIGH_DATA_MASK & length) >> 7;
    }
    else
    {
        prefix[3] = 1 | (length << 1);
    }
    /* CRC checksum */
    postfix[0] = cmux_frame_check(prefix + 1, prefix_length - 1);

    memcpy(sendbuf, prefix, prefix_length);
    if (length > 0)
    {
        memcpy(sendbuf+prefix_length, data, length);
    }
    memcpy(sendbuf+prefix_length+length, postfix, 2);
    sendtotallen = prefix_length+length+2;
    spi_device_write(sendbuf, sendtotallen);
    ECPLAT_DUMP(UNILOG_PLA_APP, cmux_send_data_1, P_INFO, ":", sendtotallen, sendbuf);
    return length;
}

/**
 * Receive thread , store serial data
 *
 * @param cmux    the point of cmux object structure
 *
 * @return  RT_EOK  we shouldn't let the receive thread return data, receive thread need keep alive all the time
 */
static void cmux_recv_thread()
{
    struct cmux *cmux = NULL;
    uint8_t* rxPtr = NULL;
    uint32_t rxLen = 0, flag = 0;
    uint8_t buffer[CMUX_RECV_READ_MAX];

    cmux = _g_cmux;

    ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_recv_thread_0, P_VALUE, "ENTER");

    while (cmux->recv_task_run)
    {
        SFDT_masterReceive(&rxPtr, &rxLen, &flag);
        if (rxLen)
        {
            ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_recv_thread_1, P_VALUE, "receive %d byte from spi.", rxLen);
            if(rxLen > CMUX_RECV_READ_MAX)
            {
                ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_recv_thread_1_1, P_VALUE, "too large.");
                rxLen = CMUX_RECV_READ_MAX;
            }
            memcpy(buffer, rxPtr, rxLen);
            if(cmux->cmux_mode == CMUX_CMUX_MODE){
                ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_recv_thread_1_2, P_VALUE, "in cmux mode process cmux data");
                cmux_recv_processdata(cmux, buffer, rxLen);
            }else if(cmux->cmux_mode == CMUX_AT_MODE){
                ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_recv_thread_1_3, P_VALUE, "in at mode process at response");
                cmux_recv_processAtdata(cmux, buffer, rxLen);
            }else{
                ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_recv_thread_1_4, P_VALUE, "in unknown mode cant process data!!!!!");
            }
        }
        SFDT_freeRxFrameBuffer(rxPtr);
    }
    ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_recv_thread_2, P_VALUE, "EXIT");
    cmux->recv_tid = NULL;
    osThreadExit();
}

static err_t cmux_check_cmux_mode_ready(struct cmux *object, uint8_t waitTime)
{
    err_t result = -CMUX_TIMEOUT;
    uint32_t start, end;

    start = osKernelGetTickCount()/portTICK_PERIOD_MS/osKernelGetTickFreq();
    end=start;
    // Check mode for waitTime*2 seconds
    while(end-start < waitTime)
    {
        if(object->cmux_mode == CMUX_CMUX_MODE)
        {
            ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_check_cmux_mode_ready_1, P_INFO, "success switch to cmux mode");
            result = CMUX_OK;
            break;
        }
        ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_check_cmux_mode_read2, P_INFO, "wait");
        osDelay(500);
        end = osKernelGetTickCount()/portTICK_PERIOD_MS/osKernelGetTickFreq();
    }
    return result;

}

/**
 *  open cmux mode
 *
 * @param object    the point of cmux object
 *
 * @return  the result
 */
static err_t cmux_open_cmux_mode(struct cmux *object)
{
    char cmd[64] = "AT+CMUX=0,0,5,2048,10,3,30,10,2\r\n";
    ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_open_cmux_mode_1, P_INFO, "send at+cmux");
    //mode:0(basic option); subset:0(UIH frame only); port_speed:5(115200); N1:2048; T1:10(100ms); N2:3(3 retry); T2:30(300ms); T3:10(10s); k:2
    spi_device_write((uint8_t*)cmd, strlen(cmd)+1);
    return cmux_check_cmux_mode_ready(object, 3);
}



/**
 * initialize virtual channel, and open it
 *
 * @param link_port     the channel of virtual serial
 *
 * @return  the result
 */
static err_t cmux_vcom_open(int link_port)
{
    /* establish virtual connect channel */
    ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_vcom_open_1, P_VALUE, "establish channel(%d).", link_port);

    cmux_send_data(link_port, CMUX_FRAME_SABM | CMUX_CONTROL_PF, NULL, 0);
    //wait UA back
    cmux_start_timer(link_port, CMUX_T1 * 10);
    return CMUX_OK;
}

/**
 * close virtual channel
 *
 * @param link_port     the channel of virtual serial
 *
 * @return  the result
 */
static err_t cmux_vcom_close(int link_port)
{
    ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_vcom_close_1, P_VALUE, "close channel(%d).", link_port);
    cmux_send_data(link_port, CMUX_FRAME_DISC | CMUX_CONTROL_PF, NULL, 0);
    return CMUX_OK;
}

/**
 * cmux object init function
 *
 * @param object        the point of cmux object
 * @param vcom_num      the number of virtual channel
 * @param user_data     private data
 *
 * @return  RT_EOK      successful
 *          CMUX_NOMEM   allocate memory failed
 */
err_t cmux_init(struct cmux *object, uint8_t vcom_num, void *user_data)
{
    if (_g_cmux == NULL)
    {
        _g_cmux = object;
    }
    else
    {
        EC_ASSERT(!_g_cmux, 0, 0, 0);
    }

    object->vcom_num = vcom_num;
    object->vcoms = malloc(vcom_num * sizeof(struct cmux_vcoms));
    if (object->vcoms == NULL)
    {
        ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_init_2, P_INFO, "cmux vcoms malloc failed.");
        return -CMUX_NOMEM;
    }
    memset(object->vcoms, 0, vcom_num * sizeof(struct cmux_vcoms));

    object->buffer = cmux_buffer_init();
    if (object->buffer == NULL)
    {
        ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_init_3, P_INFO, "cmux vcoms malloc failed.");
        return -CMUX_NOMEM;
    }

    object->user_data = user_data;

    ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_init_4, P_INFO, "cmux init successful.");
    return CMUX_OK;
}

/**
 * start cmux function
 *
 * @param object    the point of cmux object
 *
 * @return  the result
 */
err_t cmux_start(struct cmux *object)
{
    err_t result = CMUX_OK;

    ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_start_0, P_VALUE, "enter.");

    osThreadAttr_t task_attr;
    memset(&task_attr, 0, sizeof(task_attr));
    task_attr.name = "cmuxTask";
    task_attr.stack_size = CMUX_THREAD_STACK_SIZE;
    task_attr.priority = osPriorityBelowNormal7;


    object->recv_task_run = 1;

    if(object->recv_tid == NULL){
        object->recv_tid = osThreadNew(cmux_recv_thread, NULL, &task_attr);
    }
    if (object->recv_tid == NULL)
    {
        ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_start_1, P_VALUE, "cmux receive thread create failed.");
        return -CMUX_ERROR;
    }

    result = cmux_open_cmux_mode(object);

    if(result != CMUX_OK)
    {
        ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_start_1_1, P_VALUE, "open cmux mode failed.");
        return result;
    }

    cmux_attach(object, CMUX_CTRL_PORT);
    cmux_vcom_open(CMUX_CTRL_PORT);
    while(object->vcoms[CMUX_CTRL_PORT].openStat == CMUX_CLOSE){
        osDelay(10);
    }
    if(object->vcoms[CMUX_CTRL_PORT].openStat == CMUX_OPENFAIL){
        ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_start_2, P_VALUE, "cmux open ctrl channel failed.");
        return -CMUX_ERROR;
    }else{
        ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_start_3, P_VALUE, "cmux open ctrl channel success.");
    }

    if (object->ops->start != NULL)
    {
        result = object->ops->start(object);
        if (result != CMUX_OK)
            return result;
    }

    return result;
}

/**
 * stop cmux function
 *
 * @param object    the point of cmux object
 *
 * @return  the result
 */
err_t cmux_stop(struct cmux *object)
{
    ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_stop_0, P_VALUE, "enter.");
    uint8_t count = 0;
    char tempchar[2] = {CMUX_C_CLD,1};

    if (object->ops->stop != NULL)
    {
        object->ops->stop(object);
    }

    osDelay(1000);

    ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_stop_1, P_VALUE, "to disc control channel.");
    /* we should send CMUX_FRAME_UIH frame with CLD command to reset the link into normal AT command mode.  */
    cmux_send_data(0, CMUX_FRAME_UIH, tempchar, 2);

    while(object->recv_task_run != 0){
        osDelay(1000);
        count++;
        if(count >= 10) break;
    }
    if(count == 10){
        ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_stop_2, P_VALUE, "disc control channel timeout.");
        object->recv_task_run = 0;
    }
    return CMUX_OK;
}

/**
 * control cmux function
 *
 * @param object        the point of cmux object
 *
 * @return  CMUX_INVAL   haven't support control function
 */
err_t cmux_control(struct cmux *object, int cmd, void *args)
{
    EC_ASSERT(object != NULL, 0, 0, 0);

    return -CMUX_INVAL;
}

/**
 * write data into virtual channel
 *
 * @param link_port     the channel of virtual serial
 * @param buffer    the data you want to send
 * @param size      the length of buffer
 *
 * @return  the result
 */
size_t cmux_vcom_write(int link_port,
                                 const void *buffer,
                                 size_t size)
{
    size_t len = 0;
    ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_vcom_write_1, P_VALUE, "send %d bytes in channel(%d).", size,link_port);

    /* use virtual serial, we can write data into actual serial directly. */
    len = cmux_send_data(link_port, CMUX_FRAME_UIH, buffer, size);
    return len;
}

/**
 * write data into virtual channel
 *
 * @param link_port     the channel of virtual serial
 * @param buffer    the buffer you want to store
 * @param size      the length of buffer
 *
 * @return  the result
 */
size_t cmux_vcom_read(int link_port,
                                void *buffer,
                                size_t size)
{
    struct cmux *cmux = NULL;
    bool using_status = false;

    cmux = _g_cmux;
    struct cmux_vcoms *vcom = &cmux->vcoms[link_port];
    using_status = vcom->frame_using_status;

    /* The previous frame has been transmitted finish. */
    if (!using_status)
    {
        ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_vcom_read_1, P_VALUE, "no previous frame.in %d channel", link_port);
        /* support fifo, we using the first frame */
        vcom->frame = cmux_frame_pop(cmux, (int)vcom->link_port);
        vcom->length = 0;
        vcom->data = NULL;

        /* can't find frame */
        if (vcom->frame == NULL)
        {
            ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_vcom_read_2, P_VALUE, "no find frame");
            return 0;
        }

        if (size >= vcom->frame->data_length)
        {
            int data_len = vcom->frame->data_length;
            memcpy(buffer, vcom->frame->data, data_len);
            cmux_frame_destroy(vcom->frame);
            ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_vcom_read_3, P_VALUE, "recv len %d all copy to buffer", data_len);

            return data_len;
        }
        else
        {
            vcom->data = vcom->frame->data;
            vcom->frame_using_status = 1;
            memcpy(buffer, vcom->data, size);
            vcom->data = vcom->data + size;
            vcom->length = vcom->length + size;
            ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_vcom_read_4, P_VALUE, "frame data len:%d, buffer size %d too small copy party of data to buffer", vcom->frame->data_length, size);
            return size;
        }
    }
    else
    {
        ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_vcom_read_5, P_VALUE, "frame has previous recv part.in %d channel", link_port);
        /* transmit the rest of frame */
        if (size >= vcom->frame->data_length - vcom->length)
        {
            size_t read_len = 0;
            memcpy(buffer, vcom->data, vcom->frame->data_length - vcom->length);
            vcom->frame_using_status = 0;

            if (vcom->frame->data_length - vcom->length >= 0) {
                read_len = vcom->frame->data_length - vcom->length;
            }
            cmux_frame_destroy(vcom->frame);
            ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_vcom_read_6, P_VALUE, "copy rest of frame. %d to buffer", read_len);
            return read_len;
        }
        else
        {
            memcpy(buffer, vcom->data, size);
            vcom->data = vcom->data + size;
            vcom->length = vcom->length + size;
            ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_vcom_read_7, P_VALUE, "frame data left len:%d, buffer size %d too small copy party of data to buffer", vcom->frame->data_length - vcom->length, size);

            return size;
        }
    }
}

err_t cmux_open(struct cmux *object,int link_port)
{
    EC_ASSERT(object != NULL, 0, 0, 0);

    cmux_vcom_open(link_port);
    while(object->vcoms[link_port].openStat == CMUX_CLOSE){
        osDelay(10);
    }
    if(object->vcoms[link_port].openStat == CMUX_OPENFAIL){
        ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_open_1, P_VALUE, "cmux open AT channel failed.");
        return -CMUX_ERROR;
    }else{
        return CMUX_OK;
    }
}


/**
 * attach cmux channel into cmux object
 *
 * @param object        the point of cmux object
 * @param link_port     the channel of virtual serial
 *
 * @return  CMUX_OK          execute successful
 *
 */
err_t cmux_attach(struct cmux *object, int link_port)
{
    EC_ASSERT(object != NULL, 0, 0, 0);

    if(link_port >= object->vcom_num)
    {
        ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_attach_0, P_VALUE, "PORT[%d] attach failed, please increase CMUX_PORT_NUMBER in the env.", link_port);
        return -CMUX_INVAL;
    }

    ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_attach_1, P_VALUE, "attach channel %d", link_port);

    object->vcoms[link_port].link_port = (uint8_t)link_port;

    vcoms_cmux_frame_init(object, link_port);

    return CMUX_OK;
}

/**
 * detach cmux channel from cmux object
 *
 * @param object            the point of cmux object
 * @param link_port     the channel of virtual serial
 *
 * @return  RT_EOK          execute successful
 *         -RT_ERROR        error
 */
err_t cmux_detach(int link_port)
{
    ECPLAT_PRINTF(UNILOG_PLA_APP, cmux_detach_1, P_VALUE, "detach channel %d", link_port);
    cmux_vcom_close(link_port);

    return CMUX_OK;
}


