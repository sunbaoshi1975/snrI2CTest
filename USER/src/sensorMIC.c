#include "sensorMIC.h"
#include "delay.h"

/* Private define ------------------------------------------------------------*/
#define MIC_MA_NUM             		1                   // The size of MA array

#define NOISE_DB_MAX           		210
#define NOISE_DB_MIN           		0

/* Public variables ----------------------------------------------------------*/
// Noise level (dB)
uint16_t gMIC_noiseValue = 0;

/* Private variables ---------------------------------------------------------*/
// Moving average
uint8_t m_MIC_mvPtr = 0;
uint8_t m_MIC_mvLen = 0;

uint8_t mvMICData[MIC_MA_NUM] = {0};
uint16_t MIC_mvSum = 0;

/* --------------------- Start implementation --------------------------------*/
uint8_t mic_init()
{
	// Init the sensor & read version
	uint8_t lv_times = 3;
	while (nsr_i2c_probe() != NSR_STATUS_OK && --lv_times) delay_ms(500);
	if( lv_times ) LogOutput("MIC started, address:0x%02X\r\n", nsr_i2_get_configured_address());
	else LogOutput("MIC init failed...\r\n");
	return 0;
}

uint8_t mic_IsDataReady()
{
	return( m_MIC_mvLen < MIC_MA_NUM ? 0 : 1);
}

void mic_AddData(const uint8_t _noise)
{
	// Check data range and ignore wrong data
	if( _noise > NOISE_DB_MAX ) return;
	if( _noise < NOISE_DB_MIN ) return;
	
	if( _noise != mvMICData[m_MIC_mvPtr] ) {
		MIC_mvSum += _noise;
		MIC_mvSum -= mvMICData[m_MIC_mvPtr];
		mvMICData[m_MIC_mvPtr] = _noise;
	}
	
	m_MIC_mvPtr = (m_MIC_mvPtr + 1) % MIC_MA_NUM;
	if(m_MIC_mvLen < MIC_MA_NUM) m_MIC_mvLen++;
	
	gMIC_noiseValue = MIC_mvSum / m_MIC_mvLen;
}

uint8_t mic_checkData()
{
	uint8_t lv_noise = 0;
	
	/* Measure noise (dB) and store into variable
	 */
	int8_t ret = nsr_i2_measure_blocking_read(&lv_noise);
	if (ret != NSR_STATUS_OK) return 1;		// Error
	
	mic_AddData(lv_noise);
	LogOutput("Debug noise:%ddB\r\n", lv_noise);
	return 0;	// OK
}