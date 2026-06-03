#ifndef FFW_ENGINE_FFW_CMD
#define FFW_ENGINE_FFW_CMD

#include <stdio.h>
#include <stdint.h>

#include "ffw_types.h"
#include "ffw_list.h"
#include "ffw_ate.h"
#include "ffw_buffer.h"

#define FFW_AT_CMD_PARAM_MAX (32)
#define FFW_AT_CMD_MAX_LENGTH (1024)
#define FFW_AT_CMD_PARAM_MAX_LENGTH (40960)
#define FFW_AT_CMD_NAME_LENGTH (64)

#define FFW_DECLARE_CMD_HANDLER(x) int32_t x(ffw_atcmd_t *cmd)

typedef struct ffw_at_params_s ffw_at_params_t;
typedef struct ffw_atcmd_s ffw_atcmd_t;
typedef void (*ffw_cmd_async_destory_t)(struct ffw_atcmd_s *cmd);
typedef int32_t (*cmd_func_t)(ffw_atcmd_t *cmd);
typedef struct cmd_desc_s cmd_desc_t;
typedef struct ffw_cmd_parser_s ffw_cmd_parser_t;

typedef enum
{
    /// AT SET
    FFW_AT_CMD_SET = 1,

    /// AT READ
    FFW_AT_CMD_READ,

    /// AT TEST
    FFW_AT_CMD_TEST,

    /// AT EXE
    FFW_AT_CMD_EXEC
} ffw_cmd_op_t;

typedef enum
{
    /// 初始化状态
    FFW_PARSER_STAT_INIT,

    /// hit A or a
    FFW_PARSER_STAT_PREFIX_A,

    /// hit AT
    FFW_PARSER_STAT_PREFIX_T,

    /// parse cmd name
    FFW_PARSER_STAT_CMD_NAME,

    /// parse cmd param
    FFW_PARSER_STAT_CMD_MAY_PARAM,

    /// parse cmd param
    FFW_PARSER_STAT_CMD_PARAM,

    /// parse cmd finish
    FFW_PARSER_STAT_CMD_ALMOST_FINISH,

    /// parse cmd finish
    FFW_PARSER_STAT_CMD_FINISH,

    /// end
    FFW_PARSER_STAT_END
} ffw_parser_stat_t;

struct cmd_desc_s
{
    const char *name;
    cmd_func_t func;
};

struct ffw_atcmd_s
{
    /// AT命令操作类型
    ffw_cmd_op_t op;

    /// raw cmd for echo
    ffw_buffer_t rawcmd;

    /// AT命令名字
    ffw_buffer_t name;

    /// 参数数组
    ffw_buffer_t params[FFW_AT_CMD_PARAM_MAX];

    /// 参数个数
    uint8_t count;

    /// AT引擎
    ffw_handle_t engine;

    /// 处理函数
    const cmd_desc_t *desc;

    /// 链表
    struct list_head forlist;
};

struct ffw_cmd_parser_s
{
    ffw_parser_stat_t stat;
    ffw_buffer_t *param;
    ffw_atcmd_t *cmd;
    struct list_head *cmdlist;
};

/**
 * @brief 
 * 
 * @param cmd 
 * @return int 
 */
int ffw_ate_cmd_free(ffw_atcmd_t *cmd);

/**
 * @brief 
 * 
 * @param cmdout 
 * @return int 
 */
int ffw_ate_cmd_create(ffw_atcmd_t **cmdout);

/**
 * @brief 
 * 
 * @param parser 
 * @param str 
 * @param len 
 * @param e 
 * @return int 
 */
int ffw_ate_parse_atcmd(ffw_cmd_parser_t *parser, const char *str, uint32_t len, ffw_handle_t e);

/**
 * @brief 
 * 
 * @param name 
 * @return cmd_func_t 
 */
const cmd_desc_t *ffw_ate_find_cmd(const char *name);

#endif /* FFW_ENGINE_FFW_CMD */
