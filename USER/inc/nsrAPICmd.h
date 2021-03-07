#ifndef __NSR_API_COMMAND_H
#define __NSR_API_COMMAND_H

#ifdef __cplusplus
extern "C" {
#endif

// Commands (@[SRAPI_STEP_WAIT_PAYL] or bstReadFirstByte of I2C)
#define SRAPI_CMD_VALUE					0x00
#define SRAPI_CMD_SET_I2C_ADDR			0x10
#define SRAPI_CMD_SET_POWER_LED			0x11
#define SRAPI_CMD_SET_PULL_MODE			0x12
#define SRAPI_CMD_SET_SEND_PERIOD		0x13
#define SRAPI_CMD_SET_NOISE_CONST		0x14
#define SRAPI_CMD_SET_TH_WARNING		0x15
#define SRAPI_CMD_SET_TH_POWER_DELTA	0x16
#define SRAPI_CMD_SET_WARN_SW_ON_DUR	0x17
#define SRAPI_CMD_GET_VERSION			0x40
#define SRAPI_CMD_RESET_CONFIG			0x41

#ifdef __cplusplus
}
#endif

#endif /* __NSR_API_COMMAND_H */
