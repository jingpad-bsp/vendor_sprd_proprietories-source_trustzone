/*
 * Copyright (C) 2012 Spreadtrum Communications Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
/*
*  sprd_efuse_hw.h
*
*  Created on: 2014-06-05
*  Author: ronghua.yu
*/

#ifndef _SPRD_EFUSE_HW_H_
#define _SPRD_EFUSE_HW_H_

//#include <misc/fuse_driver.h>

#define EFUSE_OK                 (0)
#define EFUSE_INPUT_PARAM_ERR    (-1)
#define EFUSE_OPEN_FAILED_ERR    (-2)
#define EFUSE_READ_FAILED_ERR    (-3)
#define EFUSE_WRITE_FAILED_ERR   (-4)
#define EFUSE_LSEEK_FAILED_ERR   (-5)


int efuse_hash_read(unsigned char *hash, int count);
int efuse_hash_write(unsigned char *hash, int count);
int efuse_uid_read(unsigned char *uid, int count);
int efuse_secure_enable(void);
int efuse_secure_is_enabled(void);
int efuse_is_hash_write(void);
int efuse_uid_read_value(unsigned char *uid, int count);


#endif /* _SPRD_EFUSE_HW_H_  */
