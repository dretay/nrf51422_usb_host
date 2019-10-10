#include "Usb.h"

#define SPI_INSTANCE  0 /**< SPI instance index. */
static const nrf_drv_spi_t spi = NRF_DRV_SPI_INSTANCE(SPI_INSTANCE); /**< SPI instance. */
static volatile bool spi_xfer_done; /**< Flag used to indicate that SPI instance completed the transfer. */

#define TEST_STRING "Nordic"
static uint8_t       m_tx_buf[] = TEST_STRING; /**< TX buffer. */
static uint8_t       m_rx_buf[sizeof(TEST_STRING) + 1]; /**< RX buffer. */
static const uint8_t m_length = sizeof(m_tx_buf); /**< Transfer length. */

static u32 millis(void)
{
	return (app_timer_cnt_get() / 32.768);
}
static void handler(uint8_t *prev_buf, uint8_t *buf)
{
}
static void spi_transmit(u8* out, size_t out_size)
{
	APP_ERROR_CHECK(nrf_drv_spi_transfer(&spi, out, out_size, NULL, 0));	
}
static void spi_transmit_receive(u8* out, size_t out_size, u8* in, size_t in_size)
{
	APP_ERROR_CHECK(nrf_drv_spi_transfer(&spi, out, out_size, in, in_size));	
}
static void hard_reset(void)
{
	nrf_gpio_pin_clear(SPI_RST_PIN);	
	nrf_delay_ms(10);
	nrf_gpio_pin_set(SPI_RST_PIN);
}
void usb_host_poll_timeout_handler(void * p_context) {
	UNUSED_PARAMETER(p_context);
	USBCore.poll();	
}
static void init()
{
	nrf_drv_spi_config_t spi_config = NRF_DRV_SPI_DEFAULT_CONFIG;
	spi_config.ss_pin   = SPI_SS_PIN;
	spi_config.miso_pin = SPI_MISO_PIN;
	spi_config.mosi_pin = SPI_MOSI_PIN;
	spi_config.sck_pin  = SPI_SCK_PIN;
	spi_config.frequency = NRF_DRV_SPI_FREQ_8M;
	APP_ERROR_CHECK(nrf_drv_spi_init(&spi, &spi_config, NULL));

	nrf_gpio_cfg_output(SPI_RST_PIN);
	
	PlatformConfig config;
	config.log_log = UartLogger.log_log;
	config.millis = millis;
	config.delay = nrf_delay_ms;
	config.spi_transmit = spi_transmit;
	config.spi_transmit_receive = spi_transmit_receive;
	config.hard_reset = hard_reset;

	USBDevice *usb_device = HIDUniversal.new(&config);
	usb_device->set_handler(&handler);
	USBCore.init(usb_device, &config);
	
	// create usb_host timer.
	APP_ERROR_CHECK(app_timer_create(&m_usb_host_timer_id, APP_TIMER_MODE_REPEATED, usb_host_poll_timeout_handler));	

	//start usb_host timer
	APP_ERROR_CHECK(app_timer_start(m_usb_host_timer_id, USB_HOST_POLL_INTERVAL, NULL));	
}
const struct usb Usb= { 
	.init = init,		
};

