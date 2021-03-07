/*
* Copyright (c) 2021, DataTellIt Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of DataTellIt nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * \file
 *
 * \brief DataTellIt Noise Sensor (NSR) driver implementation
 *
 * This module provides access to the NSR functionality over a generic I2C
 * interface. It supports measurements without clock stretching only.
 *
 */

#include "nsr_i2c.h"
#include "i2c.h"
#include "nsrAPICmd.h"

#ifdef DTIT_NSR_ADDRESS
static const uint8_t NSR_I2C_ADDRESS = DTIT_NSR_ADDRESS;
#else
static const uint8_t NSR_I2C_ADDRESS = 0x7E;
#endif

uint8_t nsr_i2_read(uint8_t _cmd, uint16_t* pWord, const uint16_t _delay)
{
	uint8_t buf[BST_BUFFER_SIZE] = {0};
    uint8_t ret;
	// [cmd][len][data1][data2], where len should be 3
	const uint8_t len = 3;
	
	// Read data: num_bytes = len + 2 (cmd & CRC8)
	ret = I2C1_Read_NByte(NSR_I2C_ADDRESS, _cmd, buf, len + 2, _delay);
	if( ret != I2C_ST_OK ) return NSR_STATUS_TIMEOUT;
	
	// Check CRC, from [len] to [data2]
	if ( CRC8_Check(buf + 1, len) != buf[len + 1]) return NSR_STATUS_CRC_FAIL;
	// Check Cmd
	if( _cmd != buf[0] ) return NSR_STATUS_ERR_BAD_DATA;
	// Check length
	if( len != buf[1] ) return NSR_STATUS_ERR_PAYL_LEN;
	*pWord = (uint16_t)buf[2] << 8 | buf[3];
	return NSR_STATUS_OK;
}

uint8_t nsr_i2_measure_blocking_read(uint8_t* pNoise) {
	uint16_t lv_word;
    uint8_t ret = nsr_i2_read(SRAPI_CMD_VALUE, &lv_word, NSR_MEASURE_DURATION_USEC);
	if( ret != NSR_STATUS_OK ) return ret;
	
	// Return value
	*pNoise = lv_word & 0xFF;
	return NSR_STATUS_OK;
}

uint8_t nsr_i2c_probe(void) {
    //uint16_t version;
	//return nsr_i2_read_version(&version);
	return I2C1_CheckDevice(NSR_I2C_ADDRESS);
}

uint8_t nsr_i2_read_version(uint16_t* _pVersion) {
    return(nsr_i2_read(SRAPI_CMD_GET_VERSION, _pVersion, NSR_CMD_DURATION_USEC));
}

uint8_t nsr_i2_reset_config() {
	uint16_t lv_word;
    uint8_t ret = nsr_i2_read(SRAPI_CMD_RESET_CONFIG, &lv_word, NSR_CMD_DURATION_USEC);
	if( ret != NSR_STATUS_OK ) return ret;
	
	// Return value
	return(lv_word == 0 ? NSR_STATUS_OK : NSR_STATUS_ERR_BAD_DATA);
}

uint8_t nsr_i2_change_config(const uint8_t _cmd, uint16_t _value) {
	uint8_t buf[BST_BUFFER_SIZE] = {0};
    uint8_t ret;
	// [len][data1][data2][CRC8], where len is 3 and CRC8 counts from [len] to [data2]
	buf[0] = 3;
	buf[1] = _value >> 8;
	buf[2] = _value & 0xFF;
	buf[3] = CRC8_Check(buf, buf[0]);
    ret = I2C1_Write_NByte(NSR_I2C_ADDRESS, _cmd, buf, buf[0] + 1);
    if (ret) return NSR_STATUS_TIMEOUT;
	return NSR_STATUS_OK;
}

uint8_t nsr_i2_get_configured_address(void) {
    return NSR_I2C_ADDRESS;
}
