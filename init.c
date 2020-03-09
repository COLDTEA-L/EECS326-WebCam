/**
 * \file
 *
 * \brief User board initialization template
 *
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */

#include <asf.h>
#include <board.h>
#include <conf_board.h>
#include "camera.h"
void board_init(void)
{
	/* This function is meant to contain board-specific initialization code
	 * for, e.g., the I/O pins. The initialization can rely on application-
	 * specific board configuration, found in conf_board.h.
	 */
	 ioport_init();

	 

	 

	 //pin to read connection status of wifi chip

	 ioport_set_pin_dir(WIFI_SET,IOPORT_DIR_INPUT);

	 

	 ioport_set_pin_dir(WIFI_RESET_PIN,IOPORT_DIR_OUTPUT);

	 ioport_set_pin_level(WIFI_RESET_PIN,true);

	 

	 //set up button to reset wifi chip

	 ioport_set_pin_level(WIFI_SET, IOPORT_DIR_INPUT);

	 ioport_set_pin_mode(WIFI_SET, IOPORT_MODE_PULLDOWN);

	 

	 // Pull CTS pin of WiFi low to enable communication

	 ioport_set_pin_dir(WIFI_CTS_PIN, IOPORT_DIR_OUTPUT);

	 ioport_set_pin_level(WIFI_CTS_PIN, false);

	 

	 //configure camera pins

	 //camera twi pins

	 gpio_configure_pin(TWI0_DATA_GPIO, TWI0_DATA_FLAGS);

	 gpio_configure_pin(TWI0_CLK_GPIO, TWI0_CLK_FLAGS);

	 

	 //camera clock

	 gpio_configure_pin(PIN_PCK1, PIN_PCK1_FLAGS);

	 //gpio_configure_pin(OV_SW_OVT_GPIO, OV_SW_OVT_FLAGS);
	//gpio_configure_pin(OV_RST_GPIO, OV_RST_FLAGS);
	//gpio_configure_pin(OV_FSIN_GPIO, OV_FSIN_FLAGS);
	gpio_configure_pin(OV_HSYNC_GPIO, OV_HSYNC_FLAGS);
	gpio_configure_pin(OV_VSYNC_GPIO, OV_VSYNC_FLAGS);
	gpio_configure_pin(OV_DATA_BUS_D2, OV_DATA_BUS_FLAGS);
	gpio_configure_pin(OV_DATA_BUS_D3, OV_DATA_BUS_FLAGS);
	gpio_configure_pin(OV_DATA_BUS_D4, OV_DATA_BUS_FLAGS);
	gpio_configure_pin(OV_DATA_BUS_D5, OV_DATA_BUS_FLAGS);
	gpio_configure_pin(OV_DATA_BUS_D6, OV_DATA_BUS_FLAGS);
	gpio_configure_pin(OV_DATA_BUS_D7, OV_DATA_BUS_FLAGS);
	gpio_configure_pin(OV_DATA_BUS_D8, OV_DATA_BUS_FLAGS);
	gpio_configure_pin(OV_DATA_BUS_D9, OV_DATA_BUS_FLAGS);
}
