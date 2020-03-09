/*
 * camera.c
 *
 * Created: 2020/3/2 23:11:54
 *  Author: Yuanwen Liu
 */ 
 #include <asf.h>
#include "camera.h"
#include "ov2640.h"
#include "timer_interface.h"
#include "twi.h"

/* Pointer to the image data destination buffer */
uint8_t *g_p_uc_cap_dest_buf;

/* Rows size of capturing picture */
//uint16_t g_us_cap_rows = IMAGE_HEIGHT;

/* Vsync signal information (true if it's triggered and false otherwise) */
static volatile uint32_t g_ul_vsync_flag = false;

volatile uint32_t image_len=0;

 volatile  uint32_t start_idx=0;
 volatile uint32_t   end_idx = 0;
 //Handler for rising-edge of VSYNC signal. Should set a ?ag indicating a rising edge of VSYNC. 
static void vsync_handler(uint32_t ul_id, uint32_t ul_mask)
{
	unused(ul_id);
	unused(ul_mask);

	g_ul_vsync_flag = true;
}

// Con?guration of VSYNC interrupt.
static void init_vsync_interrupts(void)
{
	/* Initialize PIO interrupt handler, see PIO definition in conf_board.h
	**/
	pio_handler_set(OV2640_VSYNC_PIO, OV2640_VSYNC_ID, OV2640_VSYNC_MASK,
			OV2640_VSYNC_TYPE, vsync_handler);

	/* Enable PIO controller IRQs */
	NVIC_EnableIRQ((IRQn_Type)OV2640_VSYNC_ID);
	pio_enable_interrupt(OV2640_VSYNC_PIO, OV2640_VSYNC_MASK);
}

//Con?guration of TWI (two wire interface). 
void configure_twi(void)
{
twi_options_t opt;
/* Enable TWI peripheral */
pmc_enable_periph_clk(ID_BOARD_TWI);

/* Init TWI peripheral */
opt.master_clk = sysclk_get_cpu_hz();
opt.speed      = TWI_CLK;
twi_master_init(BOARD_TWI, &opt);

/* Configure TWI interrupts */
NVIC_DisableIRQ(BOARD_TWI_IRQn);
NVIC_ClearPendingIRQ(BOARD_TWI_IRQn);
NVIC_SetPriority(BOARD_TWI_IRQn, 0);
NVIC_EnableIRQ(BOARD_TWI_IRQn);
}

//Configuration and initialization of parallel capture. 
void pio_capture_init(Pio *p_pio, uint32_t ul_id)
{
/* Enable periphral clock */
pmc_enable_periph_clk(ul_id);

/* Disable pio capture */
p_pio->PIO_PCMR &= ~((uint32_t)PIO_PCMR_PCEN);

/* Disable rxbuff interrupt */
p_pio->PIO_PCIDR |= PIO_PCIDR_RXBUFF;

/* 32bit width*/
p_pio->PIO_PCMR &= ~((uint32_t)PIO_PCMR_DSIZE_Msk);
p_pio->PIO_PCMR |= PIO_PCMR_DSIZE_WORD;

/* Only HSYNC and VSYNC enabled */
p_pio->PIO_PCMR &= ~((uint32_t)PIO_PCMR_ALWYS);
p_pio->PIO_PCMR &= ~((uint32_t)PIO_PCMR_HALFS);

//#if !defined(DEFAULT_MODE_COLORED)
/* Samples only data with even index */
//p_pio->PIO_PCMR |= PIO_PCMR_HALFS;
//p_pio->PIO_PCMR &= ~((uint32_t)PIO_PCMR_FRSTS);
//#endif
}

//Uses parallel capture and PDC to store image in buffer. 
static uint8_t pio_capture_to_buffer(Pio *p_pio, uint8_t *uc_buf,
uint32_t ul_size)
{
	/* Check if the first PDC bank is free */
	if ((p_pio->PIO_RCR == 0) && (p_pio->PIO_RNCR == 0)) {
		p_pio->PIO_RPR = (uint32_t)uc_buf;
		p_pio->PIO_RCR = ul_size;
		p_pio->PIO_PTCR = PIO_PTCR_RXTEN;
		return 1;
		} else if (p_pio->PIO_RNCR == 0) {
		p_pio->PIO_RNPR = (uint32_t)uc_buf;
		p_pio->PIO_RNCR = ul_size;
		return 1;
		} else {
		return 0;
	}
}

//Configuration of camera pins, camera clock (XCLK), and calling the configure twi function. 
void init_camera(void){
pmc_enable_pllbck(7, 0x1, 1);
/* Init Vsync handler*/
init_vsync_interrupts();
//pio_disable_interrupt(OV2640_VSYNC_PIO, OV2640_VSYNC_MASK);

g_ul_vsync_flag = 0;
/* Init PIO capture*/
pio_capture_init(OV_DATA_BUS_PIO, OV_DATA_BUS_ID);

PMC->PMC_PCK[1] = (PMC_PCK_PRES_CLK_4 | PMC_PCK_CSS_PLLB_CLK);
PMC->PMC_SCER = PMC_SCER_PCK1;
while (!(PMC->PMC_SCSR & PMC_SCSR_PCK1)) {
}
configure_twi();
}

//Configuration of OV2640 registers for desired operation. 
void configure_camera(void){

/* ov2640 Initialization */
while (ov_init(BOARD_TWI) == 1) {                                                                                                           
}

ov_configure(BOARD_TWI, JPEG_INIT);
ov_configure(BOARD_TWI, YUV422);
ov_configure(BOARD_TWI, JPEG);
ov_configure(BOARD_TWI, JPEG_640x480);

/* Wait 3 seconds to let the image sensor to adapt to environment */
delay_ms(3000);
}

uint8_t start_capture(void){
/* Set capturing destination address*/
	//g_p_uc_cap_dest_buf = (uint8_t *)CAP_DEST;

	/* Set cap_rows value*/
	//g_us_cap_rows = IMAGE_HEIGHT;

	/* Enable vsync interrupt*/
	pio_enable_interrupt(OV2640_VSYNC_PIO, OV2640_VSYNC_MASK);

	/* Capture acquisition will start on rising edge of Vsync signal.
	 * So wait g_vsync_flag = 1 before start process
	 */
	while (!g_ul_vsync_flag) {
	}

	/* Disable vsync interrupt*/
	pio_disable_interrupt(OV2640_VSYNC_PIO, OV2640_VSYNC_MASK);

	/* Enable pio capture*/
	pio_capture_enable(OV2640_DATA_BUS_PIO);

	/* Capture data and send it to external SRAM memory thanks to PDC
	 * feature */
	//pio_capture_to_buffer(OV2640_DATA_BUS_PIO, g_p_uc_cap_dest_buf,
			//(g_us_cap_line * g_us_cap_rows) >> 2);


     pio_capture_to_buffer(OV2640_DATA_BUS_PIO, image_buff,MAX_IMAGE_SIZE >> 2);



	/* Wait end of capture*/
	while (!((OV2640_DATA_BUS_PIO->PIO_PCISR & PIO_PCIMR_RXBUFF) ==
			PIO_PCIMR_RXBUFF)) {
	}
	/* Reset vsync flag*/
	g_ul_vsync_flag = false;

	if(!find_image_len()){
		return 0;
	}

	else{
		return 1;

	}
	
}

uint8_t find_image_len(void){
volatile bool start_flag = false
;


for(volatile uint32_t i = 0; i < (sizeof(image_buff)-1); i++){
	if(image_buff[i] == 0xff && image_buff[i+1] == 0xd8){
		start_idx = i;
        start_flag = true;

	}
	if(start_flag){
	if(image_buff[i] == 0xff && image_buff[i+1] == 0xd9 && start_flag == true){
      	end_idx = i+2;
		image_len = end_idx-start_idx;
		return 1;
	}
}
}
return 0;
}
