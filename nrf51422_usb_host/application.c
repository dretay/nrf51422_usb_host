#include "application.h"

static PlatformConfig platform_config;

struct MOUSEINFO {

	struct {
		uint8_t bmLeftButton : 1;
		uint8_t bmRightButton : 1;
		uint8_t bmMiddleButton : 1;
		uint8_t bmDummy : 5;
	};
	int8_t dX;
	int8_t dY;
};
extern ble_hids_t m_hids; 
extern bool m_in_boot_mode;

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

static void init(USBDevice **usb_device, bool *erase_bonds)
{	
	platform_config.log_log = UartLogger.log_log;

	UartLogger.init();
	*usb_device = Usb.init(usb_callback);
	Led.init();
	Button.init(erase_bonds);	
	if (*erase_bonds)
	{
		log_debug("ERASE BONDS!");
	}
	else
	{
		log_debug("DONT ERASE BONDS");
	}

	
}

const struct application Application = { 
	.init = init,		
};

