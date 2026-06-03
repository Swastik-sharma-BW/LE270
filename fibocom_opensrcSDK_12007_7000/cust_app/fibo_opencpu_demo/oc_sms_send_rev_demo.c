#include "app_image.h"
#include "fibo_opencpu.h"

#include "app.h"
#include "app_pdp.h"


static UINT32 g_sig_sms_sem = 0;
static uint16_t g_sms_index = 0;



void sms_callback(uint32_t ind_flag,void *ind_msg_buf,uint32_t ind_msg_len)
{
    APP_LOG_INFO("sms_callback ind_flag:%d", ind_flag);
    switch(ind_flag)
    {
        case FIBO_SMS_NEW_PDU_MSG_IND:
        {
            fibo_sms_pdu_msg_t *t_pdu_msg = (fibo_sms_pdu_msg_t *)ind_msg_buf;
            g_sms_index =  t_pdu_msg->nIndex;
            APP_LOG_INFO("index:%d,ind_msg_buf:%s,ind_msg_len:%d",t_pdu_msg->nIndex,t_pdu_msg->message,ind_msg_len);
        }
            break;

        case FIBO_SMS_NEW_MSG_IND:
        {
            fibo_sms_msg_t *t_msg = (fibo_sms_msg_t *)ind_msg_buf;
            g_sms_index =  t_msg->nIndex;
            APP_LOG_INFO("sms info,index:%d,sca:%s,sca_size:%d,tosca:%d,fo:%d,pid:%d,dcs:%d,message:%s,msglen:%d,phoneNum:%s,phoneNumLen:%d",
                t_msg->nIndex,(char *)t_msg->sca,t_msg->sca_size,t_msg->tosca,t_msg->fo,t_msg->pid,t_msg->dcs,
                t_msg->message,t_msg->msglen,(char *)t_msg->phoneNum,t_msg->phoneNumLen);
            APP_LOG_INFO("sms timestamp,year:%d,month:%d,day:%d,hour:%d,min:%d,sec:%d",
                t_msg->time.year,t_msg->time.month,t_msg->time.day,
                t_msg->time.hour,t_msg->time.min,t_msg->time.sec);
        }
            break;

         default:
            break;
    }
    if((FIBO_SMS_NEW_PDU_MSG_IND == ind_flag)||(FIBO_SMS_NEW_MSG_IND == ind_flag))
    {
        fibo_sem_signal(g_sig_sms_sem);
    }
}

char g_sms_pdu_70_ucs2[] = "0001000D91685128799155F200088C0031003200330034003500360037003800390030003100320033003400350036003700380039003000310032003300340035003600370038003900300031003200330034003500360037003800390030003100320033003400350036003700380039003000310032003300340035003600370038003900300031003200330034003500360037003800390030";

char g_sms_pdu_70_7bit[] = "0001000D91685128799155F20000A031D98C56B3DD7039584C36A3D56C375C0E1693CD6835DB0D9783C564335ACD76C3E56031D98C56B3DD7039584C36A3D56C375C0E1693CD6835DB0D9783C564335ACD76C3E56031D98C56B3DD7039584C36A3D56C375C0E1693CD6835DB0D9783C564335ACD76C3E56031D98C56B3DD7039584C36A3D56C375C0E1693CD6835DB0D9783C564335ACD76C3E560";


void send_and_rev_sms_demo()
{
    int i = 0;
    for (i = 0; i < 5; i++)
    {
        APP_LOG_INFO("Hello world %d", i);
        fibo_taskSleep(1000);
    }
    
    int ret = 0;

     g_sig_sms_sem = fibo_sem_new(0);
     //del all message
     //send pdu message
    ret = fibo_sms_addrxmsghandler(0, sms_callback);
    APP_LOG_INFO("fibo_sms_addrxmsghandler ret = %d", ret);

    fibo_sms_storage_info_t pt_sms_storage = {0};
    pt_sms_storage.storage_idx = 0xFFFFFFFF;
    pt_sms_storage.e_storage = 0;
    ret = fibo_sms_deletefromstorage(0, &pt_sms_storage);
    APP_LOG_INFO("fibo_sms_deletefromstorage ret = %d", ret);

    
    fibo_sms_send_raw_message_data_t raw_message_data = {0};
    fibo_sms_raw_send_resp_t raw_resp  = {0};
    //char sms_pdu_text[] = "0001000D91685128799155F2000806003100320033";
    //char sms_pdu_text[] = "0001000D91688107921167F1000806003100320033";
    //char sms_pdu_text[] = "0001000D9XXXXXXXXXX000806003300320031";//xxx... is phone number
    
    char sms_pdu_70_ucs2[] = "0001000D91685128799155F200088C0031003200330034003500360037003800390030003100320033003400350036003700380039003000310032003300340035003600370038003900300031003200330034003500360037003800390030003100320033003400350036003700380039003000310032003300340035003600370038003900300031003200330034003500360037003800390030";

    raw_message_data.format = 0;
    strcpy((char *)raw_message_data.raw_message,sms_pdu_70_ucs2);
    raw_message_data.raw_message_len = 20;

    ret = fibo_sms_send_smspdu(0,raw_message_data,&raw_resp);
    APP_LOG_INFO("fibo_sms_send_smspdu ret = %d,message_id = %d", ret,raw_resp.message_id);
    fibo_sem_wait(g_sig_sms_sem);
    APP_LOG_INFO("rev sms");

    fibo_sms_read_message_info_t  read_info = {0};
    fibo_sms_read_message_req_info_t read_req_info = {0};

    read_req_info.storage_index = g_sms_index;
    read_req_info.storage_type = E_FIBO_SMS_STORAGE_TYPE_UIM;
    read_req_info.message_mode_valid = 1;
    read_req_info.message_mode = E_FIBO_SMS_MESSAGE_MODE_PDU;
    ret = fibo_sms_read_message(0, &read_info, &read_req_info);
    APP_LOG_INFO("fibo_sms_read_message ret = %d,message_len = %d,message_body = %s,extra_da = %s,type = %d", 
        ret,read_info.length,read_info.message_text,read_info.mobile_number,read_info.tag_type);
    if(read_info.data_head_valid)
	{
        APP_LOG_INFO("total_segments %d,segment_index %d,ref_number %d",read_info.data_head.total_segments,read_info.data_head.seg_number,read_info.data_head.reference_number);
    }

    if(read_info.timestamp_valid)
    {
        APP_LOG_INFO("timestamp %d,%d %d %d %d %d %d %d",read_info.timestamp.year,read_info.timestamp.month,read_info.timestamp.day,read_info.timestamp.hours,read_info.timestamp.minutes,read_info.timestamp.seconds,read_info.timestamp.timezone);
    }

    
    read_req_info.storage_index = g_sms_index;
    read_req_info.storage_type = E_FIBO_SMS_STORAGE_TYPE_UIM;
    read_req_info.message_mode_valid = 0;
    ret = fibo_sms_read_message(0, &read_info, &read_req_info);
    APP_LOG_INFO("fibo_sms_read_message ret = %d,message_len = %d,message_body = %s,extra_da = %s,type = %d", 
        ret,read_info.length,read_info.message_text,read_info.mobile_number,read_info.tag_type);
    if(read_info.data_head_valid)
	{
        APP_LOG_INFO("total_segments %d,segment_index %d,ref_number %d",read_info.data_head.total_segments,read_info.data_head.seg_number,read_info.data_head.reference_number);
    }

    if(read_info.timestamp_valid)
    {
        APP_LOG_INFO("timestamp %d,%d %d %d %d %d %d %d",read_info.timestamp.year,read_info.timestamp.month,read_info.timestamp.day,read_info.timestamp.hours,read_info.timestamp.minutes,read_info.timestamp.seconds,read_info.timestamp.timezone);
    }

    int j = 0;
    fibo_sms_msg_list_req_info_t req_info = {0};
    fibo_sms_msg_list_info_t resp_info = {0};

    req_info.storage_type = E_FIBO_SMS_STORAGE_TYPE_UIM;
    req_info.tag_type = E_FIBO_SMS_TAG_TYPE_MT_READ_V01;
    ret = fibo_sms_getmsglist(0, &req_info, &resp_info);
    if(ret == 0)
    {
        APP_LOG_INFO("list count = %d", resp_info.message_list_length);
        for(j = 0;j<resp_info.message_list_length;j++)
        {
            APP_LOG_INFO("tag_type:%d index = %d", resp_info.message_list[j].tag_type,resp_info.message_list[j].message_index);
        }
    }
    //APP_LOG_INFO("fibo_sms_getmsglist type %d,ret = %d", i,ret);

    
    //delete sms
    pt_sms_storage.storage_idx = g_sms_index;
    pt_sms_storage.e_storage = E_FIBO_SMS_STORAGE_TYPE_UIM;
    ret = fibo_sms_deletefromstorage(0, &pt_sms_storage);
    APP_LOG_INFO("fibo_sms_deletefromstorage ret = %d", ret);

    //send text message
    fibo_sms_send_msg_info_t sms_info = {0};
    //char sms_text[] = "123";
    char sms_text[] = "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890";

    sms_info.format = E_FIBO_SMS_FORMAT_UCS2;
    sms_info.mode = FIBO_SMS_MSG_FORMAT_GW_PP_V01;
    sms_info.message_class_valid = false;
    sms_info.message_class = 0;
    strcpy((char *)sms_info.mobile_number ,"15829719552");//xxx... is phone number
    strcpy((char *)sms_info.message_text,sms_text);
    sms_info.length = strlen(sms_text);

    ret = fibo_sms_send_message(0, &sms_info);
    APP_LOG_INFO("fibo_sms_send_message ret = %d", ret);

    fibo_sem_wait(g_sig_sms_sem);
    APP_LOG_INFO("rev txt sms");


    read_req_info.storage_index = g_sms_index;
    read_req_info.storage_type = E_FIBO_SMS_STORAGE_TYPE_UIM;
    read_req_info.message_mode_valid = 1;
    read_req_info.message_mode = E_FIBO_SMS_MESSAGE_MODE_PDU;
    ret = fibo_sms_read_message(0, &read_info, &read_req_info);
    APP_LOG_INFO("fibo_sms_read_message ret = %d,message_len = %d,message_body = %s,extra_da = %s,type = %d", 
    ret,read_info.length,read_info.message_text,read_info.mobile_number,read_info.tag_type);
    if(read_info.data_head_valid)
    {
        APP_LOG_INFO("total_segments %d,segment_index %d,ref_number %d",read_info.data_head.total_segments,read_info.data_head.seg_number,read_info.data_head.reference_number);
    }

    if(read_info.timestamp_valid)
    {
        APP_LOG_INFO("timestamp %d%02d%02d-%02d%02d%02d-%+d",read_info.timestamp.year,read_info.timestamp.month,read_info.timestamp.day,read_info.timestamp.hours,read_info.timestamp.minutes,read_info.timestamp.seconds,read_info.timestamp.timezone);
    
    }


    
    read_req_info.storage_index = g_sms_index;
    read_req_info.storage_type = E_FIBO_SMS_STORAGE_TYPE_UIM;
    read_req_info.message_mode_valid = 0;
    ret = fibo_sms_read_message(0, &read_info, &read_req_info);
    APP_LOG_INFO("fibo_sms_read_message ret = %d,message_len = %d,message_body = %s,extra_da = %s,type = %d", 
        ret,read_info.length,read_info.message_text,read_info.mobile_number,read_info.tag_type);
    if(read_info.data_head_valid)
    {
        APP_LOG_INFO("total_segments %d,segment_index %d,ref_number %d",read_info.data_head.total_segments,read_info.data_head.seg_number,read_info.data_head.reference_number);
    }

    if(read_info.timestamp_valid)
    {
        APP_LOG_INFO("timestamp %d%02d%02d-%02d%02d%02d-%+d",read_info.timestamp.year,read_info.timestamp.month,read_info.timestamp.day,read_info.timestamp.hours,read_info.timestamp.minutes,read_info.timestamp.seconds,read_info.timestamp.timezone);
    
    }


    
    fibo_sem_free(g_sig_sms_sem);
    g_sig_sms_sem = 0;

    return ;
}

void receive_long_sms_demo()
{
    int i = 0;
    for (i = 0; i < 5; i++)
    {
        APP_LOG_INFO("Hello world %d", i);
        fibo_taskSleep(1000);
    }

    int ret = 0;

     g_sig_sms_sem = fibo_sem_new(0);
     //del all message
     //send pdu message
    ret = fibo_sms_addrxmsghandler(0, sms_callback);
    APP_LOG_INFO("fibo_sms_addrxmsghandler ret = %d", ret);

    while (1)
    {
        fibo_sms_read_message_info_t  read_info = {0};
        fibo_sms_read_message_req_info_t read_req_info = {0};
        fibo_sem_wait(g_sig_sms_sem);
        APP_LOG_INFO("rev sms");
        read_req_info.storage_index = g_sms_index;
        read_req_info.storage_type = E_FIBO_SMS_STORAGE_TYPE_UIM;
        read_req_info.message_mode_valid = 0;
        ret = fibo_sms_read_message(0, &read_info, &read_req_info);
        APP_LOG_INFO("fibo_sms_read_message ret = %d,message_len = %d,message_body = %s,extra_da = %s,type = %d", 
            ret,read_info.length,read_info.message_text,read_info.mobile_number,read_info.tag_type);
        if(read_info.data_head_valid)
        {
            APP_LOG_INFO("total_segments %d,segment_index %d,ref_number %d",read_info.data_head.total_segments,read_info.data_head.seg_number,read_info.data_head.reference_number);
        }

        if(read_info.timestamp_valid)
        {
            APP_LOG_INFO("timestamp %d%02d%02d-%02d%02d%02d-%+d",read_info.timestamp.year,read_info.timestamp.month,read_info.timestamp.day,read_info.timestamp.hours,read_info.timestamp.minutes,read_info.timestamp.seconds,read_info.timestamp.timezone);

        }

        read_req_info.storage_index = g_sms_index;
        read_req_info.storage_type = E_FIBO_SMS_STORAGE_TYPE_UIM;
        read_req_info.message_mode_valid = 1;
        read_req_info.message_mode = E_FIBO_SMS_MESSAGE_MODE_PDU;
        ret = fibo_sms_read_message(0, &read_info, &read_req_info);
        APP_LOG_INFO("fibo_sms_read_message ret = %d,message_len = %d,message_body = %s,extra_da = %s,type = %d", 
            ret,read_info.length,read_info.message_text,read_info.mobile_number,read_info.tag_type);
        if(read_info.data_head_valid)
        {
            APP_LOG_INFO("total_segments %d,segment_index %d,ref_number %d",read_info.data_head.total_segments,read_info.data_head.seg_number,read_info.data_head.reference_number);
        }

        if(read_info.timestamp_valid)
        {
            APP_LOG_INFO("timestamp %d%02d%02d-%02d%02d%02d-%+d",read_info.timestamp.year,read_info.timestamp.month,read_info.timestamp.day,read_info.timestamp.hours,read_info.timestamp.minutes,read_info.timestamp.seconds,read_info.timestamp.timezone);

        }


      }
    return ;
}


void *appimg_enter(void *param)
{
    APP_LOG_INFO("application appimg_enter11");
    return app_init(send_and_rev_sms_demo);
}
