#include "sprd_production_pubefuse.h"

static int efuse_write(unsigned int blk, unsigned int val) {
    char magic[8] = {0}, buf[16] = {0};
    off_t curpos, offset;
    int fd = -1;
    int len = 0, ret = 0;
    char *pBuf = (char *)&val;

    ALOGD("%s()->Line:%d; blk = %d, data = 0x%08x \n", __FUNCTION__, __LINE__,
        blk, val);

    fd = open(DEV_NODE_MAGIC, O_WRONLY);
    if (fd < 0) {
        ALOGE("CXK MAGIC open fail");
        return EFUSE_OPEN_FAILED_ERR;
    }
    sprintf(magic, "%x", MAGIC_NUMBER);
    len = write(fd, magic, strlen(magic));
    if (len <= 0) {
        ALOGE("%s()->Line:%d; write magic error\n", __FUNCTION__, __LINE__);
        close(fd);
        return EFUSE_WRITE_FAILED_ERR;
    }
    close(fd);
    fd = open(DEV_NODE, O_RDWR);
    if (fd < 0) {
        ALOGE("%s()->Line:%d; %s open error, fd = %d. \n", __FUNCTION__, __LINE__,
            DEV_NODE, fd);
        return EFUSE_OPEN_FAILED_ERR;
    }

    sprintf(buf, "%08x", val);

    offset = blk * BLOCK_SIZE;
    curpos = lseek(fd, offset, SEEK_CUR);
    if (curpos == -1) {
        ALOGE("%s()->Line:%d; lseek error\n", __FUNCTION__, __LINE__);
        close(fd);
        return EFUSE_LSEEK_FAILED_ERR;
    }
    ALOGD("val=%x,buf=%s\n", val, buf);

    len = write(fd, pBuf, 4);
    ALOGD("pBuf[0~3]=%02x,%02x,%02x,%02x\n", pBuf[0], pBuf[1], pBuf[2], pBuf[3]);
    if (len <= 0) {
        ALOGE(
            "%s()->Line:%d; write efuse block(%d) data(%s) error, retcode = %d; \n",
            __FUNCTION__, __LINE__, blk, buf, len);
        close(fd);
        return EFUSE_WRITE_FAILED_ERR;
    }
    close(fd);
    return EFUSE_OK;
}

static int efuse_read(unsigned int blk, unsigned char *data) {
    char buf[5] = {0};
    off_t curpos, offset;
    int fd = -1, ret = 0, i = 0;
    if (data == 0) return -1;

    fd = open(DEV_NODE, O_RDONLY);
    if (fd < 0) {
        ALOGE("%s()->Line:%d; %s open error, fd = %d. \n", __FUNCTION__, __LINE__,
            DEV_NODE, fd);
        return EFUSE_OPEN_FAILED_ERR;
    }
    offset = blk * BLOCK_SIZE;
    curpos = lseek(fd, offset, SEEK_CUR);
    if (curpos == -1) {
        ALOGE("%s()->Line:%d; lseek error\n", __FUNCTION__, __LINE__);
        close(fd);
        return EFUSE_LSEEK_FAILED_ERR;
    }
    ret = read(fd, buf, 4);
    if (ret <= 0) {
        ALOGE("%s()->Line:%d; read efuse block(%d) data error, retcode = %d; \n",
            __FUNCTION__, __LINE__, blk, ret);
        close(fd);
        return EFUSE_READ_FAILED_ERR;
    }
    close(fd);
    sprintf(data, "%02x%02x%02x%02x", buf[3], buf[2], buf[1], buf[0]);
    ALOGD("buf=%s\n", data);
    return EFUSE_OK;
}

/*
* read efuse bit, the return value is the bit value
*
*/
static int read_efuse_bit(int block, int bit){
    ALOGD("%s enter\n",__FUNCTION__);

    int result = EFUSE_OK;
    unsigned int _value1 = 0, value1 =0;
    unsigned char temp1[READ_EFUSE_LEN] = {0};

    if((block < 0) || (bit < 0) || (bit > 32)){
        return EFUSE_INPUT_PARAM_ERR;
    }

    result = efuse_read(block,temp1);
    if(result != EFUSE_OK)
        return EFUSE_READ_FAILED_ERR;
    _value1 = (unsigned int)(strtoul(temp1, 0, 16));
    value1 = (_value1 & (1 << (bit))) ? 1 : 0;

    return value1;
}

/*
* write efuse bit, the bit will be setted 1
*
*/
static int write_efuse_bit(int block, int bit){
    ALOGD("%s enter\n",__FUNCTION__);

    int result = EFUSE_OK;

    if((block < 0) || (bit < 0) || (bit > 32)){
        return EFUSE_INPUT_PARAM_ERR;
    }

    result = efuse_write(block, (1 << bit));

    return result;
}

int pro_efuse_read_block(int block, char * value){
    ALOGD("%s enter, read block: %d \n",__FUNCTION__, block);

    int result = EFUSE_OK;

    if((block < 0) || (value == NULL)){
        return EFUSE_INPUT_PARAM_ERR;
    }

    result = efuse_read(block,value);

    ALOGD("%s leave, result is %d\n",__FUNCTION__, result);
    return result;
}

int pro_efuse_write_block(int block, unsigned int value){
    ALOGD("%s enter, write block: %d ,value:%x\n",__FUNCTION__, block, value);

    int result = EFUSE_OK;

    if(block < 0){
        return EFUSE_INPUT_PARAM_ERR;
	}

    result = efuse_write(block *2,value);
    if(result != EFUSE_OK){
        return EFUSE_WRITE_FAILED_ERR;
    }

    result = efuse_write(block *2 + 1,value);
    if(result != EFUSE_OK){
        return EFUSE_WRITE_FAILED_ERR;
    }

    ALOGD("%s leave, result is %d\n",__FUNCTION__, result);
    return result;
}

int pro_efuse_read_uid(char *uid, int* uid_len){
    ALOGD("%s enter\n",__FUNCTION__);
    int result = EFUSE_OK;

    if((uid == NULL) || (uid_len == NULL))
        result = EFUSE_INPUT_PARAM_ERR;
#if 0
    #ifdef CONFIG_READ_UID
    result = efuse_uid_read_from_file(uid, uid_len);
    #else
    result = efuse_uid_read_from_block(uid, uid_len);
    #endif
#endif
    ALOGD("%s leave, result is %d\n",__FUNCTION__, result);
    return result;
}

int pro_efuse_enable_secure_bit(void){
    ALOGD("%s enter\n",__FUNCTION__);

    int result = EFUSE_OK;
    result = write_efuse_bit(SECURE_BOOT_BLOCK_1,SECURE_BOOT_BIT);
    if(result != EFUSE_OK){
        return EFUSE_WRITE_FAILED_ERR;
    }

    result = write_efuse_bit(SECURE_BOOT_BLOCK_2,SECURE_BOOT_BIT);
    if(result != EFUSE_OK){
        return EFUSE_WRITE_FAILED_ERR;
    }

    ALOGD("%s leave, result is %d\n",__FUNCTION__, result);
    return result;
}

int pro_efuse_check_secure_enable_bit(void){
    ALOGD("%s enter\n",__FUNCTION__);

    int value1 = 0, value2 =0;

    /*if block1 or block2 bit is 1, we regard this bit as be setted*/
    value1 = read_efuse_bit(SECURE_BOOT_BLOCK_1,SECURE_BOOT_BIT);
    if(1 == value1){
        ALOGD("%s leave\n",__FUNCTION__);
        return BIT_ENABLED;
    }
    value2 = read_efuse_bit(SECURE_BOOT_BLOCK_2,SECURE_BOOT_BIT);
    if(1 == value2){
        ALOGD("%s leave\n",__FUNCTION__);
        return BIT_ENABLED;
    }

    /*only read block1 and block2 fail at the same time, we regard result as error*/
    if((value1 < 0) && (value2 < 0)){
        ALOGD("%s leave\n",__FUNCTION__);
        return EFUSE_READ_FAILED_ERR;
    }

    ALOGD("%s leave\n",__FUNCTION__);
    return BIT_DISABLED;
}

int pro_efuse_disable_ptest(void){
    ALOGD("%s enter\n",__FUNCTION__);

    int result = EFUSE_OK;
#if 0
    //result = write_efuse_bit(PTEST_BLOCK_1,DISABLE_PTEST);
    if(result != EFUSE_OK){
        return EFUSE_WRITE_FAILED_ERR;
    }

    //result = write_efuse_bit(PTEST_BLOCK_2,DISABLE_PTEST);
    if(result != EFUSE_OK){
        result = EFUSE_WRITE_FAILED_ERR;
    }
#endif
    ALOGD("%s leave, result is %d\n",__FUNCTION__, result);
    return result;

}

