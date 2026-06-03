/*
 * Copyright (c) 2019 xiaofan <xfan1024@live.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author          Notes
 * 2019-09-19     xiaofan         the first version
 * 2020-04-13     xiangxistu      transplant into cmux
 */

#ifndef __CMUX_CHAT_H__
#define __CMUX_CHAT_H__

#include "cmux.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MODEM_CHAT_RESP_LIST(F) \
    F(MODEM_CHAT_RESP_OK,         "OK"), \
    F(MODEM_CHAT_RESP_ERROR,      "ERROR") \

#define DEFINE_MODEM_RESP_ID_TABLE(id, s) id

enum {
    MODEM_CHAT_RESP_LIST(DEFINE_MODEM_RESP_ID_TABLE),
    MODEM_CHAT_RESP_MAX,
    MODEM_CHAT_RESP_NOT_NEED,
};

struct modem_chat_data {
    const char* transmit;
    uint8_t expect;      // use CHAT_RESP_xxx
    uint8_t retries;
    uint8_t timeout;     // second
    bool  ignore_cr;   // ignore CR character if it is RT_TRUE
};

err_t modem_chat(struct cmux *obj, int link_port, const struct modem_chat_data *data, size_t len);

uint8_t resp_match(uint8_t resp_id, uint8_t state, char ch);
uint8_t resp_matched(uint8_t resp_id, uint8_t match_degree);

#ifdef  __cplusplus
    }
#endif

#endif  /* __CMUX_CHAT_H__ */
