/*
 * wifi.h
 *
 * Created: 2020/3/2 23:11:35
 *  Author:Yuanwen Liu
 */ 


#include <asf.h>
#include <string.h>
#ifndef WIFI_H_
#define WIFI_H_

#define WIFI_USART_ID             ID_USART0

#define WIFI_USART                USART0
#define WIFI_USART_BAUDRATE       115200

#define wifi_usart_Handler              USART0_Handler

#define WIFI_USART_IRQn                 USART0_IRQn

#define ALL_INTERRUPT_MASK  0xffffffff

#define wifi_comm_ID                 ID_PIOB
#define wifi_comm_PIO                PIOB
#define wifi_comm_PIN_MSK            PIO_PB1
#define wifi_comm_ATTR               PIO_IT_RISE_EDGE
volatile uint32_t button_flag;

#define WIFI_SETUP_ID                 ID_PIOA
#define WIFI_SETUP_PIO                PIOA
#define WIFI_SETUP_PIN_MSK            PIO_PA14
#define WIFI_SETUP_ATTR              PIO_PULLUP|PIO_DEBOUNCE|PIO_IT_FALL_EDGE

#define PIN_USART0_RXD    {PIO_PA5A_RXD0, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
#define PIN_USART0_RXD_IDX        (PIO_PA5_IDX)
#define PIN_USART0_RXD_FLAGS      (PIO_PERIPH_A | PIO_DEFAULT)
/** USART1 pin TX */
#define PIN_USART0_TXD    {PIO_PA6A_TXD0, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
#define PIN_USART0_TXD_IDX        (PIO_PA6_IDX)
#define PIN_USART0_TXD_FLAGS      (PIO_PERIPH_A | PIO_DEFAULT)
/** USART1 pin CTS */
#define PIN_USART0_CTS    {PIO_PA8A_CTS0, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
#define PIN_USART0_CTS_IDX        (PIO_PA8_IDX)
#define PIN_USART0_CTS_FLAGS      (PIO_PERIPH_A | PIO_DEFAULT)
/** USART1 pin RTS */
#define PIN_USART0_RTS    {PIO_PA7A_RTS0, PIOA, ID_PIOA, PIO_PERIPH_A,PIO_DEFAULT}
#define PIN_USART0_RTS_IDX        (PIO_PA7_IDX)
#define PIN_USART0_RTS_FLAGS      (PIO_PERIPH_A | PIO_DEFAULT)
void configure_usart_wifi(void);

void configure_wifi_comm_pin(void);

volatile char input_line_wifi[1000];
volatile uint32_t received_byte_wifi;
volatile unsigned input_pos_wifi;
volatile uint32_t setup_web_flag;
volatile bool web_connected_flag ;
volatile bool image_pass_flag;

void process_data_wifi(void);
void process_incoming_byte_wifi(uint8_t in_byte);
void cofigure_wifi_web_setup_pin(void);
void write_wifi_command(char*comm, uint8_t cnt);
void write_image_to_file(void);
uint8_t find_image_len(void);
#endif /* WIFI_H_ */