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
 * \brief DataTellIt Noise Sensor (NSR) driver interface
 *
 * This module provides access to the NSR functionality over a generic I2C
 * interface. It supports measurements without clock stretching only.
 */

#ifndef DTIT_NSR_I2C_H
#define DTIT_NSR_I2C_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "common.h"
  
// I2C user buffer format
// [len][cmd][param_1]..[param_n][CRC8], where len = n + 2, n >= 0, CRC8 from [len] to [param_n]
#define BST_BUFFER_SIZE					8

#define NSR_STATUS_OK 					0
#define NSR_STATUS_PROGRESS 			1
#define NSR_STATUS_ERR_BAD_DATA 		2
#define NSR_STATUS_ERR_PAYL_LEN			3
#define NSR_STATUS_CRC_FAIL 			4
#define NSR_STATUS_UNKNOWN_DEVICE 		5
#define NSR_STATUS_TIMEOUT 				6
#define NSR_STATUS_WAIT_RESET 			255

#define NSR_CMD_DURATION_USEC 			500
#define NSR_MEASURE_DURATION_USEC		1000
  
/**
 * Detects if a sensor is connected by reading out the ID register.
 * If the sensor does not answer or if the answer is not the expected value,
 * the test fails.
 *
 * @return 0 if a sensor was detected
 */
uint8_t nsr_i2c_probe(void);

/**
 * Reads out the results of a measurement that was previously started by
 * nsr_i2_measure(). If the measurement is still in progress, this function
 * returns an error.
 *
 * @param pWord   		the address for the result of word value
 * @return              0 if the command was successful, else an error code.
 */
uint8_t nsr_i2_read(uint8_t _cmd, uint16_t* pWord, const uint16_t _delay);

/**
 * Read out the sensor version number
 *
 * @param _pVersion the address for the result of the version number
 * @return          0 if the command was successful, else an error code.
 */
uint8_t nsr_i2_read_version(uint16_t* _pVersion);

/**
 * Starts a measurement and then reads out the results. This function blocks
 * while the measurement is in progress. The duration of the measurement depends
 * on the sensor in use, please consult the datasheet.
 *
 * @param pNoise   the address for the result of the noise value (dB)
 * measurement
 * @return              0 if the command was successful, else an error code.
 */
uint8_t nsr_i2_measure_blocking_read(uint8_t* pNoise);

/**
 * Reset sensor configuration
 *
 * @param			None
 * @return          0 if the command was successful, else an error code.
 */
uint8_t nsr_i2_reset_config();

/**
 * Change sensor configuration
 *
 * @param _cmd		Command or parameter ID
 * @param value		the new value
 * @return          0 if the command was successful, else an error code.
 */
uint8_t nsr_i2_change_config(const uint8_t _cmd, uint16_t _value);

/**
 * Returns the configured SNR address.
 *
 * @return The configured i2c address
 */
uint8_t nsr_i2_get_configured_address(void);

#ifdef __cplusplus
}
#endif

#endif /* DTIT_NSR_I2C_H */
