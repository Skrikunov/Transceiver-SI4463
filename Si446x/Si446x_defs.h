/*
 * Project: Si4463 Radio Library for AVR and Arduino
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2017 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/si4463-radio-library-avr-arduino/
 */

#ifndef SI446X_DEFS_H_
#define SI446X_DEFS_H_

#define SI446X_CMD_POWER_UP				0x02
#define SI446X_CMD_NOP					0x00
#define SI446X_CMD_PART_INFO			0x01
#define SI446X_CMD_FUNC_INFO			0x10
#define SI446X_CMD_SET_PROPERTY			0x11
#define SI446X_CMD_GET_PROPERTY			0x12
#define SI446X_CMD_GPIO_PIN_CFG			0x13
#define SI446X_CMD_FIFO_INFO			0x15
#define SI446X_CMD_GET_INT_STATUS		0x20
#define SI446X_CMD_REQUEST_DEVICE_STATE	0x33
#define SI446X_CMD_CHANGE_STATE			0x34
#define SI446X_CMD_READ_CMD_BUFF		0x44
#define SI446X_CMD_READ_FRR_A			0x50
#define SI446X_CMD_READ_FRR_B			0x51
#define SI446X_CMD_READ_FRR_C			0x53
#define SI446X_CMD_READ_FRR_D			0x57
#define SI446X_CMD_IRCAL				0x17
#define SI446X_CMD_IRCAL_MANUAL			0x1a
#define SI446X_CMD_START_TX				0x31
#define SI446X_CMD_TX_HOP				0x37
#define SI446X_CMD_WRITE_TX_FIFO		0x66
#define SI446X_CMD_PACKET_INFO			0x16
#define SI446X_CMD_GET_MODEM_STATUS		0x22
#define SI446X_CMD_START_RX				0x32
#define SI446X_CMD_RX_HOP				0x36
#define SI446X_CMD_READ_RX_FIFO			0x77
#define SI446X_CMD_GET_ADC_READING		0x14
#define SI446X_CMD_GET_PH_STATUS		0x21
#define SI446X_CMD_GET_CHIP_STATUS		0x23

#define SI446X_INT_CTL_CHIP_LOW_BATT_EN	1
#define SI446X_INT_CTL_CHIP_WUT_EN		0

typedef enum
{
	SI446X_ADC_CONV_TEMP = 16,
	SI446X_ADC_CONV_BATT = 8,
	SI446X_ADC_CONV_GPIO = 4
} si446x_adc_conv_t;

typedef enum
{
	SI446X_ADC_RANGE_0P8 = 0,
	SI446X_ADC_RANGE_1P6 = 4,
	SI446X_ADC_RANGE_3P2 = 5,
	SI446X_ADC_RANGE_2P4 = 8,
	SI446X_ADC_RANGE_3P6 = 9
} si446x_adc_range_t;

#define SI446X_FIFO_CLEAR_RX			0x02
#define SI446X_FIFO_CLEAR_TX			0x01

#define GLOBAL_PROP(prop)	((SI446X_PROP_GROUP_GLOBAL<<8) | prop)
#define INT_PROP(prop)		((SI446X_PROP_GROUP_INT<<8) | prop)
#define PKT_PROP(prop)		((SI446X_PROP_GROUP_PKT<<8) | prop)
#define PA_PROP(prop)		((SI446X_PROP_GROUP_PA<<8) | prop)
#define MATCH_PROP(prop)	((SI446X_PROP_GROUP_MATCH<<8) | prop)

#define SI446X_GLOBAL_CONFIG			GLOBAL_PROP(0x03)
#define SI446X_FIFO_MODE_HALF_DUPLEX	0x10

#define SI446X_GLOBAL_CLK_CFG			GLOBAL_PROP(0x01)
#define SI446X_DIVIDED_CLK_DIS			0x00
#define SI446X_DIVIDED_CLK_EN			0x40
#define SI446X_DIVIDED_CLK_SEL_DIV_1	0<<5
#define SI446X_DIVIDED_CLK_SEL_DIV_2	1<<5
#define SI446X_DIVIDED_CLK_SEL_DIV_3	2<<5
#define SI446X_DIVIDED_CLK_SEL_DIV_7_5	3<<5
#define SI446X_DIVIDED_CLK_SEL_DIV_10	4<<5
#define SI446X_DIVIDED_CLK_SEL_DIV_15	5<<5
#define SI446X_DIVIDED_CLK_SEL_DIV_30	6<<5
#define SI446X_DIVIDED_CLK_32K_SEL_OFF	0x00
#define SI446X_DIVIDED_CLK_32K_SEL_RC	0x01
#define SI446X_DIVIDED_CLK_32K_SEL_XTAL	0x02

#define SI446X_GLOBAL_LOW_BATT_THRESH	GLOBAL_PROP(0x02)
#define SI446X_GLOBAL_WUT_CONFIG		GLOBAL_PROP(0x04)
#define SI446X_GLOBAL_WUT_M				GLOBAL_PROP(0x05)
#define SI446X_GLOBAL_WUT_R				GLOBAL_PROP(0x07)
#define SI446X_GLOBAL_WUT_LDC			GLOBAL_PROP(0x08)
#define SI446X_WUT_SLEEP				5
#define SI446X_LDC_MAX_PERIODS_FOREVER	0<<6
#define SI446X_LDC_MAX_PERIODS_TWO		1<<6
#define SI446X_LDC_MAX_PERIODS_FOUR		2<<6
#define SI446X_LDC_MAX_PERIODS_EIGHT	3<<6
#define SI446X_GLOBAL_WUT_CONFIG_WUT_LDC_EN_RX	1<<6
#define SI446X_GLOBAL_WUT_CONFIG_WUT_EN			1
#define SI446X_GLOBAL_WUT_CONFIG_WUT_LBD_EN		2

#define SI446X_INT_CTL_ENABLE			INT_PROP(0x00)
#define SI446X_INT_CTL_PH_ENABLE		INT_PROP(0x01)
#define SI446X_INT_CTL_MODEM_ENABLE		INT_PROP(0x02)
#define SI446X_INT_CTL_CHIP_ENABLE		INT_PROP(0x03)
#define SI446X_FILTER_MATCH_PEND		7
#define SI446X_FILTER_MISS_PEND			6
#define SI446X_PACKET_SENT_PEND			5
#define SI446X_PACKET_RX_PEND			4
#define SI446X_CRC_ERROR_PEND			3
#define SI446X_INVALID_SYNC_PEND		5
#define SI446X_SYNC_DETECT_PEND			0
#define SI446X_PREAMBLE_DETECT_PEND		1 //детектор преамбулы дописал сам
#define SI446X_INVALID_PREAMBLE_PEND	3 //детектор ошибки преамбулы дописал сам
#define SI446X_LOW_BATT_PEND			1
#define SI446X_WUT_PEND					0

#define SI446X_MATCH_VALUE_1			MATCH_PROP(0x00)
#define SI446X_MATCH_EN					0x40

#define	SI446X_PA_PWR_LVL				PA_PROP(0x01)

#define SI446X_PKT_FIELD_1_LENGTH		PKT_PROP(0x0D)
#define SI446X_PKT_FIELD_2_LENGTH		PKT_PROP(0x11)
#define SI446X_PKT_FIELD_2_LENGTH_LOW	PKT_PROP(0x12)


#ifndef ARDUINO

	#include "Si446x_config.h"

	#define CONCAT(a, b) a ## b
	#define CONCAT2(a, b, c) a ## b ## c

	#define PORT(port)			CONCAT(PORT, port)
	#define PORTBIT(port, bit)	CONCAT2(PORT, port, bit)
	#define DDR(port)			CONCAT(DDR, port)
	#define PINPORT(port)		CONCAT(PIN, port)
	#define PINBIT(port, bit)	CONCAT2(PIN, port, bit)
	#define PCINT(pcint)		CONCAT(PCINT, pcint)
	#define PUE(port)			CONCAT(PUE, port)
	#define PUEBIT(port, bit)	CONCAT2(PUE, port, bit)

	#define CSN_DDR			DDR(SI446X_CSN_PORT)
	#define CSN_PORT		PORT(SI446X_CSN_PORT)
	#define CSN_BIT			PORTBIT(SI446X_CSN_PORT, SI446X_CSN_BIT)

	#define SDN_DDR			DDR(SI446X_SDN_PORT)
	#define SDN_PORT		PORT(SI446X_SDN_PORT)
	#define SDN_BIT			PORTBIT(SI446X_SDN_PORT, SI446X_SDN_BIT)

	#if defined(SI446X_IRQ_PORT) && defined(SI446X_IRQ_BIT)
		#define IRQ_DDR			DDR(SI446X_IRQ_PORT)
		#define IRQ_PORT		PORT(SI446X_IRQ_PORT)
		#define IRQ_BIT			PORTBIT(SI446X_IRQ_PORT, SI446X_IRQ_BIT)
		#define IRQ_PUE			PUE(SI446X_IRQ_PORT)
	#endif

	#define INTCONCAT(num) CONCAT(INT, num)
	#define ISCCONCAT(num, bit)	CONCAT2(ISC, num, bit)
	#define INTVECTCONCAT(num)	CONCAT2(INT, num, _vect)

	#define REG_EXTERNAL_INT SI446X_REG_EXTERNAL_INT
	#define BIT_EXTERNAL_INT SI446X_BIT_EXTERNAL_INT

	/*
	#ifndef REG_EXTERNAL_INT
		#ifdef EIMSK
			#define REG_EXTERNAL_INT EIMSK
		#elif defined GICR
			#define REG_EXTERNAL_INT GICR
		#else
			#define REG_EXTERNAL_INT GIMSK
		#endif
	#endif
	*/
	#ifndef REG_EXTERNAL_INT_CTL
		#ifdef EICRA
			#if SI446X_INTERRUPT_NUM < 4
				#define REG_EXTERNAL_INT_CTL EICRA
			#else
				#define REG_EXTERNAL_INT_CTL EICRB
			#endif
		#else
			#define REG_EXTERNAL_INT_CTL MCUCR
		#endif
	#endif
	/*
	#ifndef BIT_EXTERNAL_INT
		#define BIT_EXTERNAL_INT INTCONCAT(SI446X_INTERRUPT_NUM)
	#endif
	*/
	#ifndef BIT_EXTERNAL_INT_CTL
		#define BIT_EXTERNAL_INT_CTL (_BV(ISCCONCAT(SI446X_INTERRUPT_NUM, 1))|_BV(ISCCONCAT(SI446X_INTERRUPT_NUM, 0)))
	#endif

	#ifndef INT_VECTOR
		#define INT_VECTOR INTVECTCONCAT(SI446X_INTERRUPT_NUM)
	#endif

#endif

#endif /* SI446X_DEFS_H_ */
