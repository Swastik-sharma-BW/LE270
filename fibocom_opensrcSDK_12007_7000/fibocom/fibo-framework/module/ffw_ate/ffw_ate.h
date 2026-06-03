#ifndef DDE92F1A_25DB_4E5D_95ED_3DAD7BADDEBF
#define DDE92F1A_25DB_4E5D_95ED_3DAD7BADDEBF

#include <stdint.h>
#include <stdbool.h>
#include <cJSON.h>

#include "ffw_types.h"
#include "ffw_utils.h"

#define FFW_AT_MSG_TYPE_AT_DATA 0
#define FFW_AT_MSG_TYPE_CALLBACK 1
#define FFW_CONFIG_NUMBER_OF_SIM (2)

typedef enum
{
    FFW_ODM_EVT_ENTER = 0,
    FFW_ODM_EVT_EXIT_TMP,
    FFW_ODM_EVT_REENTER, /// recv ato
    FFW_ODM_EVT_EXIT,
}ffw_odm_switch_evt_t;


typedef int (*ffw_ate_send_t)(ffw_handle_t transport, const void *data, int len);
typedef int (*ffw_ate_bypass_cb_t)(void *ctx, const void *data, uint32_t size);
typedef void (*ffw_odm_switch_t)(ffw_handle_t engine, ffw_odm_switch_evt_t evt, void *arg);

typedef struct
{
    bool atv;
    bool ate;
    bool atq;
    uint8_t atx; // not used now
    bool crc;
    uint8_t s3;
    uint8_t s4;
    uint8_t s5;
    uint8_t cmee;
    uint8_t andc;
    uint8_t andd;
} ffw_at_channel_conf_t;

typedef struct
{
    uint8_t creg;
    uint8_t cgreg;
    uint8_t cereg;
    uint8_t cscon;
    uint8_t cgerep_mode;
    uint8_t cgerep_bfr;
    uint8_t clip;
    uint8_t clir; // NO_SAVE
    uint8_t colp; // NO_SAVE
    uint8_t cssu;
    uint8_t cssi;
    uint8_t ccwa;
    uint8_t cgauto;
    uint8_t cr;
    uint8_t s0;
    uint8_t ecsq;
    uint8_t csdh;
    uint8_t cmgf;
    uint8_t cpms_mem1;
    uint8_t cpms_mem2;
    uint8_t cpms_mem3;
    uint8_t cnmi_mode;
    uint8_t cnmi_mt;
    uint8_t cnmi_bm;
    uint8_t cnmi_ds;
    uint8_t cnmi_bfr;
    uint8_t cind_signal;
    uint8_t cind_service;
    uint8_t cind_sounder;
    uint8_t cind_message;
    uint8_t cind_call;
    uint8_t cind_roam;
    uint8_t cind_smsfull;
    uint8_t alphabet;     // sta_cs;
    uint8_t cops_format;  // AUTO_SAVE
    uint8_t cops_mode;    // AUTO_SAVE
    uint8_t cops_oper[6]; // AUTO_SAVE
    uint8_t cpol_format;  // AUTO_SAVE
    uint8_t cpol_index;   // AUTO_SAVE
    uint8_t cpol_oper[6]; // AUTO_SAVE
    uint8_t cops_act;     // AUTO_SAVE
    uint8_t volte;
    uint8_t flight_mode; // AUTO_SAVE
    uint32_t cfgNvFlag;
} ffw_sim_config_t;

typedef struct
{
    uint8_t profile;
    uint8_t ifc_rx;
    uint8_t ifc_tx;
    uint8_t icf_format;
    uint8_t icf_parity;
    uint32_t ipr;

    uint8_t cmer_mode;
    uint8_t cmer_ind;
    uint8_t csclk;
    uint8_t cscs;
    uint8_t vtd;
    uint8_t ctzu;
    uint8_t ctzr;
    uint8_t csdf_mode;
    uint8_t csdf_auxmode;
    uint8_t cind_battchg;
    int8_t timezone;                                          // AUTO_SAVE
    uint8_t timezone_dst;                                     // AUTO_SAVE
    uint8_t bcchmode;                                         // AUTO_SAVE
    // AT_Gprs_CidInfo g_staAtGprsCidInfo[4][AT_PDPCID_MAX + 1]; // AUTO_SAVE

    uint8_t gprsAuto;
    uint8_t tempStorage;
    uint8_t drx_mode;
    uint8_t callmode;
    uint8_t smsmode;
    ffw_at_channel_conf_t atch_conf;
    ffw_sim_config_t sim[FFW_CONFIG_NUMBER_OF_SIM];
    uint8_t self_register;
    uint8_t psm_mode;
    uint8_t mcuNotifySleepMode;
    uint32_t mcuNotifySleepDelayMs;
    uint8_t csta;
    uint8_t csvm;
    uint8_t detectMBS;
    uint8_t virtual_sim_card[FFW_CONFIG_NUMBER_OF_SIM];
    uint8_t nLockedFrequencyBand[FFW_CONFIG_NUMBER_OF_SIM];
    uint32_t save_ipr;
    uint8_t lte_decline_power;
	uint8_t csmp_fo;
	uint8_t csmp_vp;
	uint8_t csmp_pid;
	uint8_t csmp_dcs;
} ffw_at_config_t;

typedef struct
{
    int type; /// FFW_AT_MSG_TYPE_DATA OR FFW_AT_MSG_TYPE_CALLBACK

    int atindex; /// 串口id
    union {
        struct
        {
            void *data;
            uint32_t length;
        } data;

        struct
        {
            void *arg;
            void (*cb)(void *arg);
        } callback;
    } content;

} ffw_at_msg_t;

/**
 * @brief 
 * 
 * @param engine 
 * @return true 
 * @return false 
 */
bool ffw_ate_is_first_text(ffw_handle_t engine);

/**
 * @brief 
 * 
 * @param engine 
 * @param first 
 * @return int 
 */
int ffw_ate_set_first_text(ffw_handle_t engine, bool first);

/**
 * @brief 
 * 
 * @param idx 
 * @param transport 
 * @param send_func 
 * @return int 
 */
int ffw_ate_register(int idx, ffw_handle_t transport, ffw_ate_send_t send_func);

/**
 * @brief 
 * 
 * @param engine 
 * @param data 
 * @param len 
 * @return int 
 */
int ffw_ate_send(ffw_handle_t engine, const void *data, int len);

/**
 * @brief 
 * 
 * @param engine 
 */
void ffw_ate_finish(ffw_handle_t engine);

/**
 * @brief 
 * 
 * @param engine 
 * @param cb 
 * @param cb_ctx 
 * @return true 
 * @return false 
 */
int ffw_ate_enter_bypass_mode(ffw_handle_t engine, ffw_ate_bypass_cb_t cb, void *cb_ctx);

/**
 * @brief 
 * 
 * @param engine 
 * @return int 
 */
int ffw_ate_exit_bypass_mode(ffw_handle_t engine);

/**
 * @brief 进入ODM模式
 * 
 * @param engine 
 * @param data_cb 
 * @param switch_cb 
 * @param arg 
 * @return int 
 */
int ffw_ate_enter_odm_mode(ffw_handle_t engine, ffw_ate_bypass_cb_t data_cb, ffw_odm_switch_t switch_cb, void *arg);

/**
 * @brief 
 * 
 * @param engine 
 * @return int 
 */
int ffw_ate_exit_odm_mode(ffw_handle_t engine);

/**
 * @brief 处理ATO命令
 * 
 * @param engine 
 * @return int 
 */
int ffw_ate_reenter_odm_mode(ffw_handle_t engine);


/**
 * @brief 
 * 
 * @param atindex 
 * @param data 
 * @param len 
 * @return int 
 */
int ffw_ate_send_msg(int atindex, const void *data, uint32_t len);

/**
 * @brief 
 * 
 * @param cb 
 * @param arg 
 * @return int 
 */
int ffw_ate_callback(void (*cb)(void *arg), void *arg);

/**
* @brief 
* 
* @param cb 
* @param arg 
* @return int 
 */
int ffw_ate_callback_timeout(void (*cb)(void *arg), void *arg, uint32_t timeout);

/**
 * @brief 
 * 
 * @return int 
 */
int ffw_ate_start_srv();

/**
 * @brief 给AT引擎喂数据
 * 
 * @param engine 
 * @param data 
 * @param len 
 * @return int 
 */
int ffw_ate_push_data(ffw_handle_t engine, uint8_t *data, uint32_t len);

/**
 * @brief 
 * 
 * @param engine 
 * @return ffw_at_channel_conf_t* 
 */
ffw_at_channel_conf_t *ffw_ate_atconf(ffw_handle_t engine);

#endif /* DDE92F1A_25DB_4E5D_95ED_3DAD7BADDEBF */
