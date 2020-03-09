/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# "Insert system clock initialization code here" comment
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */
#include <asf.h>
#include "timer_interface.h"
#include "wifi.h"
#include "camera.h"
#include "conf_board.h"
#include "conf_clock.h"
#include "ov2640.h"
int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */

	// Initialize clock and board definitions. 
	sysclk_init();
	wdt_disable(WDT);
	board_init();

	gpio_set_pin_low(WIFI_RESET_PIN);
	delay_ms(50);
	gpio_set_pin_high(WIFI_RESET_PIN);

	//Configure and start the Timer. 
	configure_tc();

	//Configure the WiFi USART, as well as the Command pin and Web Setup pin
	configure_usart_wifi();
	configure_wifi_comm_pin();
	cofigure_wifi_web_setup_pin();
	setup_web_flag=false;

	// Reset the WiFi and wait for it to connect to a network.
	while (ioport_get_pin_level(WIFI_SET)==0) {
		if (setup_web_flag) {
			write_wifi_command("setup web\r\n",10);
			delay_ms(100);
			setup_web_flag=false;
		}
	}
	//configure_twi();
	//Initialize and configure the camera
	init_camera();
	configure_camera();

//	g_ul_vsync_flag = false;

	//Tell the WiFi to turn off the command prompt and command echo
	//usart_write_line(WIFI_USART,"set sy c p off\r\n");
	write_wifi_command("set sy c p off\r\n",2);
	write_wifi_command("set sy c e off\r\n",2);
	//write_wifi_command("get gp u\r\n",10);
	/* Insert application code here, after the board has been initialized. */
	while(1){
	// Check for Web Setup request. 
	//web_connected_flag=false;
	start_idx=0;
	end_idx=0;
	if (ioport_get_pin_level(WIFI_SET)==1){
		if (setup_web_flag){
			write_wifi_command("setup web\r\n",10);
		    setup_web_flag=false; 
			web_connected_flag==false; }
		else {
			write_wifi_command("poll all\r\n",10);
			delay_ms(50);
			if(web_connected_flag==true){
				delay_ms(1000);
				web_connected_flag==false;
				//start_capture();
				//volatile uint32_t a=find_image_len();
				//if(a){
				//volatile uint32_t b=end_idx-start_idx;
				//write_image_to_file();
				//web_connected_flag==false;}
			}
			else {
				start_capture();
				volatile uint32_t a=find_image_len();
				if(a){
				volatile uint32_t b=end_idx-start_idx;
				write_image_to_file();}
				//delay_ms(1000);
				//web_connected_flag=true;
			}
		}
	}

	

	else{

		ioport_set_pin_level(WIFI_RESET_PIN,false);

		delay_ms(50);

	ioport_set_pin_level(WIFI_RESET_PIN,true);}

	

}

	}

