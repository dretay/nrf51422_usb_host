#include "application.h"

#define SPI_INSTANCE  0 /**< SPI instance index. */
static const nrf_drv_spi_t spi = NRF_DRV_SPI_INSTANCE(SPI_INSTANCE); /**< SPI instance. */
static volatile bool spi_xfer_done; /**< Flag used to indicate that SPI instance completed the transfer. */

#define TEST_STRING "Nordic"
static uint8_t       m_tx_buf[] = TEST_STRING; /**< TX buffer. */
static uint8_t       m_rx_buf[sizeof(TEST_STRING) + 1]; /**< RX buffer. */
static const uint8_t m_length = sizeof(m_tx_buf); /**< Transfer length. */

#define SPI_SS_PIN 15
#define SPI_MOSI_PIN 16
#define SPI_MISO_PIN 17
#define SPI_SCK_PIN 18
#define SPI_RST_PIN 19

#define UART_TX_PIN 9
#define UART_RX_PIN 11
#define UART_TX_BUF_SIZE                256
#define UART_RX_BUF_SIZE                256

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

uint32_t millis(void)
{
	return (app_timer_cnt_get() / 32.768);
}
volatile bool scheduled = false;
static void uart_event_handler(void * p_event_data, uint16_t event_size) {	
	u8 in_byte[256];
	u32 index = 0;
	while (app_uart_get(&in_byte[index]) == NRF_SUCCESS)
	{
		index++;
	}	
	scheduled = false;
}
void uart_event_schedule(app_uart_evt_t * p_event) 
{
	uint32_t err_code;
    
	u8 in_byte[6];
	u32 index = 0;
	switch (p_event->evt_type) {
	case APP_UART_DATA_READY:
		// We can unconditionally post the event since this function is only
		// called on first byte put into FIFO
		if(!scheduled) {
			err_code = app_sched_event_put(NULL, 0, uart_event_handler);
			APP_ERROR_CHECK(err_code);		
			scheduled = true;
		}
		break;

	case APP_UART_COMMUNICATION_ERROR:
		APP_ERROR_HANDLER(p_event->data.error_communication);
		break;

	case APP_UART_FIFO_ERROR:
		APP_ERROR_HANDLER(p_event->data.error_code);
		break;

	default:
		break;
	}	
}
void usb_host_poll_timeout_handler(void * p_context) {
	UNUSED_PARAMETER(p_context);
	USBCore.poll();	
}
static void init(void)
{	
	
	uint32_t err_code;
	
	nrf_drv_spi_config_t spi_config = NRF_DRV_SPI_DEFAULT_CONFIG;
	spi_config.ss_pin   = SPI_SS_PIN;
	spi_config.miso_pin = SPI_MISO_PIN;
	spi_config.mosi_pin = SPI_MOSI_PIN;
	spi_config.sck_pin  = SPI_SCK_PIN;
	spi_config.frequency = NRF_DRV_SPI_FREQ_8M;
	APP_ERROR_CHECK(nrf_drv_spi_init(&spi, &spi_config, NULL));

	app_uart_comm_params_t const comm_params =
	{
		.rx_pin_no = UART_RX_PIN,
		.tx_pin_no = UART_TX_PIN,		
		.flow_control = APP_UART_FLOW_CONTROL_DISABLED,
		.use_parity = false,
		.baud_rate = NRF_UART_BAUDRATE_1000000  
	};

	APP_UART_FIFO_INIT(&comm_params,
		UART_RX_BUF_SIZE,
		UART_TX_BUF_SIZE,
		uart_event_schedule,
		APP_IRQ_PRIORITY_LOW,
		err_code);
	APP_ERROR_CHECK(err_code);

	nrf_gpio_cfg_output(SPI_RST_PIN);
	
	PlatformConfig config;
	config.log_log = log_log;
	config.millis = millis;
	config.delay = nrf_delay_ms;
	config.spi_transmit = spi_transmit;
	config.spi_transmit_receive = spi_transmit_receive;
	config.hard_reset = hard_reset;

	USBDevice *usb_device = HIDUniversal.new(&config);
	usb_device->set_handler(&handler);
	USBCore.init(usb_device, &config);

	// initialize timer module, making it use the scheduler.
	//APP_TIMER_APPSH_INIT(APP_TIMER_PRESCALER, APP_TIMER_OP_QUEUE_SIZE, true);

	// create usb_host timer.
	err_code = app_timer_create(&m_usb_host_timer_id, APP_TIMER_MODE_REPEATED, usb_host_poll_timeout_handler);
	APP_ERROR_CHECK(err_code);

	//start usb_host timer
	err_code = app_timer_start(m_usb_host_timer_id, USB_HOST_POLL_INTERVAL, NULL);
	APP_ERROR_CHECK(err_code);

}


const struct myapplication MyApplication = { 
	.init = init,		
};

