#include "app_image.h"
#include "fibo_opencpu.h"

#include "app.h"
#include "app_pdp.h"

void sms_callback(uint32_t ind_flag,void *ind_msg_buf,uint32_t ind_msg_len)
{
    APP_LOG_INFO("sms_callback ind_flag:%d", ind_flag);
    switch(ind_flag)
    {
        case FIBO_SMS_NEW_PDU_MSG_IND:
        {
            fibo_sms_pdu_msg_t *t_pdu_msg = (fibo_sms_pdu_msg_t *)ind_msg_buf;
            g_index = t_pdu_msg->nIndex;
            APP_LOG_INFO("index:%d,ind_msg_buf:%s,ind_msg_len:%d",t_pdu_msg->nIndex,t_pdu_msg->message,ind_msg_len);
        }
            break;

        case FIBO_SMS_NEW_MSG_IND:
        {
            fibo_sms_msg_t *t_msg = (fibo_sms_msg_t *)ind_msg_buf;
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
}
static void hello_world_demo()
{
    int ret = 0;
    fibo_taskSleep(5000);

    //send text message
    fibo_sms_send_msg_info_t sms_info = {0};
    char sms_text[] = "123";

    sms_info.format = 0;
    sms_info.mode = 0x06;
    sms_info.message_class_valid = false;
    sms_info.message_class = 0;
    strcpy((char *)sms_info.mobile_number ,"xxxxxxxxxxxx");//xxx... is phone number
    strcpy((char *)sms_info.message_text,sms_text);
    sms_info.length = strlen(sms_text);

    ret = fibo_sms_send_message(0, &sms_info);
    APP_LOG_INFO("fibo_sms_send_message ret = %d", ret);

    //send pdu message
    fibo_taskSleep(2000);
    fibo_sms_send_raw_message_data_t raw_message_data = {0};
    fibo_sms_raw_send_resp_t raw_resp  = {0};
    char sms_pdu_text[] = "0001000D9XXXXXXXXXX000806003300320031";//xxx... is phone number

    raw_message_data.format = 0;
    strcpy((char *)raw_message_data.raw_message,sms_pdu_text);
    raw_message_data.raw_message_len = 20;

    ret = fibo_sms_send_smspdu(0,raw_message_data,&raw_resp);

    APP_LOG_INFO("fibo_sms_send_smspdu ret = %d,message_id = %d", ret,raw_resp.message_id);

    //read message
    fibo_taskSleep(2000);
    fibo_sms_read_message_info_t  read_info = {0};
    fibo_sms_read_message_req_info_t read_req_info = {0};

    read_req_info.storage_index = 2;
    read_req_info.storage_type = E_FIBO_SMS_STORAGE_TYPE_UIM;
    ret = fibo_sms_read_message(0, &read_info, &read_req_info);
    APP_LOG_INFO("fibo_sms_read_message ret = %d,message_len = %d,message_body = %s,extra_da = %s,type = %d", 
        ret,read_info.length,read_info.message_text,read_info.mobile_number,read_info.tag_type);

    //set message handler
	fibo_taskSleep(2000);   
    ret = fibo_sms_addrxmsghandler(0, sms_callback);
    APP_LOG_INFO("fibo_sms_addrxmsghandler ret = %d", ret);

    fibo_sms_store_max_size_info_t pt_info = {0};
    fibo_sms_get_store_max_size_info_t pt_sms_storage = {0};

    //get message storage info
    fibo_taskSleep(2000);
    pt_sms_storage.e_storage = E_FIBO_SMS_STORAGE_TYPE_UIM;
    ret = fibo_sms_getmaxstoresize(0, &pt_sms_storage, &pt_info);
    APP_LOG_INFO("fibo_sms_getmaxstoresize SM ret = %d,max:%d,used:%d", ret,pt_info.store_max_size,pt_info.store_used_size);
    
    memset(&pt_info, 0, sizeof(fibo_sms_store_max_size_info_t));
    pt_sms_storage.e_storage = E_FIBO_SMS_STORAGE_TYPE_NV;
    ret = fibo_sms_getmaxstoresize(0, &pt_sms_storage, &pt_info);
    APP_LOG_INFO("fibo_sms_getmaxstoresize ME ret = %d,max:%d,used:%d", ret,pt_info.store_max_size,pt_info.store_used_size);

    //get sms service center
    fibo_taskSleep(2000);
    fibo_sms_service_center_cfg_t service_center_cfg = {0};

    ret = fibo_sms_getsmscenteraddress(0, &service_center_cfg);
    APP_LOG_INFO("fibo_sms_getsmscenteraddress 1 ret = %d,smsc:%s,type:%s", ret,service_center_cfg.service_center_addr,service_center_cfg.service_center_addr_type);

    //set sms service center
    fibo_taskSleep(2000);
    strcpy((char *)service_center_cfg.service_center_addr,"+8613010815501");
    strcpy((char *)service_center_cfg.service_center_addr_type,"145");
    ret = fibo_sms_setsmscenteraddress(0, &service_center_cfg);
    APP_LOG_INFO("fibo_sms_setsmscenteraddress ret = %d", ret);

    fibo_taskSleep(2000);
    memset(&service_center_cfg, 0, sizeof(fibo_sms_service_center_cfg_t));
    ret = fibo_sms_getsmscenteraddress(0, &service_center_cfg);
    APP_LOG_INFO("fibo_sms_getsmscenteraddress 2 ret = %d,smsc:%s,type:%s", ret,service_center_cfg.service_center_addr,service_center_cfg.service_center_addr_type);

    //delete sms
    fibo_taskSleep(2000);
    fibo_sms_storage_info_t pt_sms_storage = {0};
    pt_sms_storage.storage_idx = 0;
    pt_sms_storage.e_storage = 1;
    ret = fibo_sms_deletefromstorage(0, &pt_sms_storage);
    APP_LOG_INFO("fibo_sms_deletefromstorage ret = %d", ret);

    //get sms list
    fibo_taskSleep(2000);
    int i = 0;
    int j = 0;
    fibo_sms_msg_list_req_info_t req_info = {0};
    fibo_sms_msg_list_info_t resp_info = {0};

    for(i = E_FIBO_SMS_TAG_TYPE_MT_READ_V01;i<E_FIBO_SMS_TAG_TYPE_MO_NOT_SENT_V01;i++)
    {
        req_info.storage_type = E_FIBO_SMS_STORAGE_TYPE_NV;
        req_info.tag_type = i;
        ret = fibo_sms_getmsglist(0, &req_info, &resp_info);
        if(ret == 0)
        {
            APP_LOG_INFO("list count = %d", resp_info.message_list_length);
            for(j = 0;j<resp_info.message_list_length;j++)
            {
                APP_LOG_INFO("tag_type:%d index = %d", resp_info.message_list[j].tag_type,resp_info.message_list[j].message_index);
            }
        }
        APP_LOG_INFO("fibo_sms_getmsglist type %d,ret = %d", i,ret);
    }

    return ;
}

void *appimg_enter(void *param)
{
    APP_LOG_INFO("application appimg_enter");
    return app_init(hello_world_demo);
}
