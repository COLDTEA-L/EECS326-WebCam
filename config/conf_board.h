/**
 * \file
 *
 * \brief User board configuration template
 *
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */

#ifndef CONF_BOARD_H
#define CONF_BOARD_H

#define  PIN_LED              PIO_PA20_IDX
#define WIFI_CTS_PIN          PIO_PA8_IDX
#define WIFI_USART_ID            ID_USART0
#define WIFI_RESET_PIN        PIO_PA11_IDX
//#define WIFI_SET		  PIO_PB3_IDX
#define WIFI_SET	          PIO_PB0_IDX
#define WIFI_COMM			PIO_PB1_IDX
#define WIFI_SETUP_BUTTON    PIO_PA14_IDX
#define CAM_RESET_PIN			PIO_PA20_IDX
#define WIFI_USART                USART0

#define BOARD_USART_BAUDRATE       115200

#define PIN_PCK1                       (PIO_PA17_IDX)

#define PIN_PCK1_FLAGS                 (PIO_PERIPH_B | PIO_DEFAULT)



#define TWI_CLK     (400000UL)

#define OV2640_VSYNC_PIO	       OV_VSYNC_PIO

#define OV2640_VSYNC_ID		       OV_VSYNC_ID

#define OV2640_VSYNC_MASK              OV_VSYNC_MASK

#define OV2640_VSYNC_TYPE              OV_VSYNC_TYPE

// Image sensor data pin.

#define OV2640_DATA_BUS_PIO            OV_DATA_BUS_PIO

#define OV2640_DATA_BUS_ID             OV_DATA_BUS_ID



/** Enable Com Port. */

#define CONF_BOARD_UART_CONSOLE



#define TWI_CLK     (400000UL)
/* Use TC Peripheral 0. */

#define TC             TC0

#define TC_PERIPHERAL  0

//! [tc_define_peripheral]

/** TWI0 Data pins definition */

#define TWI0_DATA_GPIO                 PIO_PA3_IDX

#define TWI0_DATA_FLAGS                (PIO_PERIPH_A | PIO_DEFAULT)

#define TWI0_DATA_MASK                 PIO_PA3

#define TWI0_DATA_PIO                  PIOA

#define TWI0_DATA_ID                   ID_PIOA

#define TWI0_DATA_TYPE                 PIO_PERIPH_A

#define TWI0_DATA_ATTR                 PIO_DEFAULT



/** TWI0 clock pins definition */

#define TWI0_CLK_GPIO                  PIO_PA4_IDX

#define TWI0_CLK_FLAGS                 (PIO_PERIPH_A | PIO_DEFAULT)

#define TWI0_CLK_MASK                  PIO_PA4

#define TWI0_CLK_PIO                   PIOA

#define TWI0_CLK_ID                    ID_PIOA

#define TWI0_CLK_TYPE                  PIO_PERIPH_A

#define TWI0_CLK_ATTR                  PIO_DEFAULT

//! [tc_define_ch1]

/* Configure TC0 channel 1 as waveform output. */

#define TC_CHANNEL_WAVEFORM 1

#define ID_TC_WAVEFORM      ID_TC1

//! [tc_define_ch2]

/* Configure TC0 channel 2 as capture input. */

#define TC_CHANNEL_CAPTURE 2

#define ID_TC_CAPTURE ID_TC2

//! [tc_define_ch2]

//! [tc_define_irq_handler]

/* Use TC2_Handler for TC capture interrupt. */

#define TC_Handler  TC2_Handler

#define TC_IRQn     TC2_IRQn
#endif // CONF_BOARD_H
