/*
 * wifi.c
 *
 * Created: 2020/3/2 23:11:11
 *  Author: Yuanwen Liu
 */ 
 #include <asf.h>
#include "wifi.h"
#include "camera.h"
#include "timer_interface.h"

volatile uint32_t recived_byte_wifi=0;
volatile unsigned input_pos_wifi=0;
volatile bool wifi_comm_success = false;
volatile bool image_pass_flag = false;
volatile bool web_connected_flag = false;


// Handler for incoming data from the WiFi. Should call process incoming byte wi? when a new byte arrives. 
void wifi_usart_Handler(void)
{
	
	uint32_t ul_status;


	/* Read USART status. */
	ul_status = usart_get_status(WIFI_USART);

	/* Receive buffer is full. */
	if (ul_status & US_CSR_RXBUFF) {
		usart_read(WIFI_USART,&received_byte_wifi);
		process_incoming_byte_wifi((uint8_t)received_byte_wifi);
	}
	
}

//Stores every incoming byte (in byte) from the AMW136 in a buffer.
void process_incoming_byte_wifi(uint8_t in_byte){
	input_line_wifi[input_pos_wifi++]=in_byte;

}
// Handler for ¡°command complete¡± rising-edge interrupt from AMW136. When this is triggered, it is time to process the response of the AMW136. 
static void wifi_command_response_handler(uint32_t ul_id, uint32_t ul_mask)
{
	unused(ul_id);
	unused(ul_mask);
	process_data_wifi();
	wifi_comm_success = true;
	
	for (int jj=0;jj<1000;jj++)
	 {input_line_wifi[jj]=0;}
	input_pos_wifi=0;
}

//Processes the response of the AMW136, which should be stored in the buffer
//Called by process incoming byte wi?. This processing should be looking for certain responses that the AMW136 should give,
//such as ¡°start transfer¡± when it is ready to receive the image.
void process_data_wifi() {
	if (strstr(input_line_wifi, "Start transfer")) {
		image_pass_flag=true;
	}
	if(strstr(input_line_wifi, "None")){
		web_connected_flag=false;
	}
	//wifi_comm_success = false;
}

//Handler for button to initiate web setup of AMW136. Should set a flag indicating a request to initiate web setup. 
void wifi_web_setup_handler(uint32_t ul_id, uint32_t ul_mask)
{
	unused(ul_id);
	unused(ul_mask);
	setup_web_flag = true;		
}

//Configuration of USART port used to communicate with the AMW136.
void configure_usart_wifi(void)
{
	
	gpio_configure_pin(PIN_USART0_RXD_IDX, PIN_USART0_RXD_FLAGS);
	gpio_configure_pin(PIN_USART0_TXD_IDX, PIN_USART0_TXD_FLAGS);
	gpio_configure_pin(PIN_USART0_CTS_IDX, PIN_USART0_CTS_FLAGS);
	//gpio_configure_pin(PIN_USART1_RTS_IDX, PIN_USART1_RTS_FLAGS);
	
	static uint32_t ul_sysclk;
	const sam_usart_opt_t usart_console_settings = {
		WIFI_USART_BAUDRATE,
		US_MR_CHRL_8_BIT,
		US_MR_PAR_NO,
		US_MR_NBSTOP_1_BIT,
		US_MR_CHMODE_NORMAL,
		/* This field is only used in IrDA mode. */
		0
	};

	/* Get peripheral clock. */
	ul_sysclk = sysclk_get_peripheral_hz();

	/* Enable peripheral clock. */
	sysclk_enable_peripheral_clock(WIFI_USART_ID);

	/* Configure USART. */
	usart_init_hw_handshaking(WIFI_USART, &usart_console_settings, ul_sysclk);

	/* Disable all the interrupts. */
	usart_disable_interrupt(WIFI_USART, ALL_INTERRUPT_MASK);
	
	/* Enable TX & RX function. */
	usart_enable_tx(WIFI_USART);
	usart_enable_rx(WIFI_USART);

	usart_enable_interrupt(WIFI_USART, US_IER_RXRDY);
	/* Configure and enable interrupt of USART. */
	NVIC_EnableIRQ(WIFI_USART_IRQn);
}

//Configuration of¡°command complete¡±rising-edgeinterrupt. 
void configure_wifi_comm_pin(void)
{
	/* Configure PIO clock. */
	pmc_enable_periph_clk(wifi_comm_ID);

	/* Adjust PIO debounce filter using a 10 Hz filter. */
	pio_set_debounce_filter(wifi_comm_PIO, wifi_comm_PIN_MSK, 10);

	/* Initialize PIO interrupt handler, see PIO definition in conf_board.h
	**/
	pio_handler_set(wifi_comm_PIO,wifi_comm_ID, wifi_comm_PIN_MSK,
			wifi_comm_ATTR,  wifi_command_response_handler);

	/* Enable PIO controller IRQs. */
	NVIC_EnableIRQ((IRQn_Type)wifi_comm_ID);

	/* Enable PIO interrupt lines. */
	pio_enable_interrupt(wifi_comm_PIO,wifi_comm_PIN_MSK);
}

//Configuration of button interrupt to initiate websetup.
void cofigure_wifi_web_setup_pin(void)
{
 /* Configure PIO clock. */
	pmc_enable_periph_clk(WIFI_SETUP_ID);

	/* Adjust PIO debounce filter using a 10 Hz filter. */
	pio_set_debounce_filter(WIFI_SETUP_PIO,WIFI_SETUP_PIN_MSK, 10);

	/* Initialize PIO interrupt handler, see PIO definition in conf_board.h
	**/
	pio_handler_set(WIFI_SETUP_PIO, WIFI_SETUP_ID,WIFI_SETUP_PIN_MSK,
			WIFI_SETUP_ATTR,  wifi_web_setup_handler);

	/* Enable PIO controller IRQs. */
	NVIC_EnableIRQ((IRQn_Type)WIFI_SETUP_ID);

	/* Enable PIO interrupt lines. */
	pio_enable_interrupt(WIFI_SETUP_PIO, WIFI_SETUP_PIN_MSK);
}




 
 
 // Writes a command(comm) to the AMW136, and waits either for an acknowledgment or a timeout. 
 //The timeout can be created by setting the global variable counts to zero, 
 //which will automatically increment every second, and waiting while counts < cnt. 
void write_wifi_command(char*comm, uint8_t cnt)
{
  counts = 0;
  usart_write_line(WIFI_USART, comm); 
 while(counts < cnt){

	 if(wifi_comm_success){

		 wifi_comm_success = false;

		 break;

	 }

 }
}

void write_image_to_file(void){
	volatile uint32_t len = end_idx-start_idx;
	if (len==0)
	{	return 0;
	}

	else{
		char comm[30];
		//char dummy[30];
		sprintf(comm, "image_transfer  %u \r\n\0", len);
		write_wifi_command(comm, 10);
		//dummy[]=comm[];
		if (!image_pass_flag)
		{ return;
		}
		for(size_t i = start_idx; i <= start_idx+len; i++){
			usart_putchar(WIFI_USART, image_buff[i]);
		}
		delay_ms(20);
		image_pass_flag=false;
	}

	}