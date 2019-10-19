#include "application.h"

static PlatformConfig platform_config;


extern ble_hids_t m_hids; 
static bool erase_bonds = false;

static void usb_callback(uint8_t *prev_buf, uint8_t *buf, u16 read)
{
	u32 err_code = NRF_SUCCESS;
	err_code = ble_hids_inp_rep_send(&m_hids, 0, read, buf);
	
	if ((err_code != NRF_SUCCESS) &&
        (err_code != NRF_ERROR_INVALID_STATE) &&
        (err_code != BLE_ERROR_NO_TX_PACKETS) &&
        (err_code != BLE_ERROR_GATTS_SYS_ATTR_MISSING))
	{
		APP_ERROR_HANDLER(err_code);
	}
}
static void push_callback(void)
{
	log_debug("push_callback");
}
static void release_callback(void)
{
	log_debug("release_callback");	
}
static void long_push_callback(void)
{
	log_debug("long_push_callback");	
	NVIC_SystemReset();
}
static void startup_callback(void)
{
	log_debug("startup_callback");
	erase_bonds = true;
}
static void init(USBDevice **usb_device, bool *erase_bonds_in)
{	
	platform_config.log_log = UartLogger.log_log;

	UartLogger.init();
	*usb_device = Usb.init(usb_callback);
	Led.init();
	Button.add_button(20, push_callback, long_push_callback, release_callback, startup_callback);
	Button.start();
	*erase_bonds_in = erase_bonds;

	
}

const struct application Application = { 
	.init = init,		
};

