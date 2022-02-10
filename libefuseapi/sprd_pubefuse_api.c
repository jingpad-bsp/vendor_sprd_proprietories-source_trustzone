#include "sprd_pubefuse_api.h"
#include "sprd_production_pubefuse.h"

int pub_efuse_uid_read_value(char *uid, int* uid_len)
{
    ALOGE("func:%s enter\n", __func__);
    #ifdef PRODUCTION_READ_UID
    pro_efuse_read_uid(uid, uid_len);
    ALOGE("read efuse uid:%s, len:%d\n", uid, *uid_len);
    #endif
    return EFUSE_OK;
}

int pub_efuse_read_block(int block, char *value){
    int result = EFUSE_OK;
    ALOGE("func:%s enter\n", __func__);
    result = pro_efuse_read_block(block, value);
    return result;
}

int pub_efuse_write_block(int block, unsigned int value){
    int result = EFUSE_OK;
    ALOGE("func:%s enter\n", __func__);
    result = pro_efuse_write_block(block, value);
    return result;
}

int pub_efuse_enable_secure_bit(void)
{
    int result = EFUSE_OK;
    ALOGE("func:%s enter\n", __func__);
    result = pro_efuse_enable_secure_bit();
    return result;
}

int pub_efuse_check_secure_enable_bit(void)
{
    int result = EFUSE_OK;
    ALOGE("func:%s enter\n", __func__);
    result = pro_efuse_check_secure_enable_bit();
    return result;
}

int pub_efuse_disable_ptest(void){
    int result = EFUSE_OK;
    ALOGE("func:%s enter\n", __func__);
    result = pro_efuse_disable_ptest();
    return result;

}

int pub_efuse_ops(int ops, int block, int value, char *rsp, int* rsplen){
    ALOGE("func:%s enter, ops:%x \n", __func__,ops);
    int result = EFUSE_OK;

    if(rsp == NULL || rsplen == NULL)
        result = EFUSE_INPUT_PARAM_ERR;

    switch(ops){
        case CMD_GET_EFUSEUID:
            result = pub_efuse_uid_read_value(rsp, rsplen);
            break;

        case CMD_SET_BLOCK:
            result = pub_efuse_write_block(block, value);
            break;

        case CMD_GET_BLOCK:
            result = pub_efuse_read_block(block, rsp);
            break;

        case CMD_ENABLE_SECURE:
            result = pub_efuse_enable_secure_bit();
            break;

        case CMD_CHECK_SECURE_ENABLE:
            result = pub_efuse_check_secure_enable_bit();
            break;

        case CMD_DISABLE_PTEST:
            result = pub_efuse_disable_ptest();
            break;


        default:
            break;
    }

    return result;
}


void register_this_module(struct production_pubefuse_callback * reg)
{
    ALOGE("file:%s, func:%s\n", __FILE__, __func__);

    sprintf(reg->at_cmd, "%s", "AT+PRODUCTIONPUBEFUSE");
    reg->production_pubefuse_func = pub_efuse_ops;
    ALOGE("module cmd:%s\n", reg->at_cmd);
}
