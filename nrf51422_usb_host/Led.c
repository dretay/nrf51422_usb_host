#include "Led.h"

static void advertising_fast(void)
{
	u32 LED_MASK = (1 << 21);
	led_sb_init_params_t led_sb_init_param = LED_SB_INIT_DEFAULT_PARAMS(LED_MASK);
	led_sb_init_param.off_time_ticks       = 1000;
	led_sb_init_param.on_time_ticks        = 2000;
	led_sb_init_param.duty_cycle_max       = 200;
	led_sb_init_param.duty_cycle_min       = 20;
	led_sb_init_param.duty_cycle_step      = 100;	

	led_softblink_uninit();
	APP_ERROR_CHECK(led_softblink_init(&led_sb_init_param));
	
	APP_ERROR_CHECK(led_softblink_start(LED_MASK));
}
static void advertising_slow(void)
{
}
static void advertising_idle(void)
{
}
static void ble_connected(void)
{
	u32 LED_MASK = (1 << 22 );
	led_sb_init_params_t led_sb_init_param = LED_SB_INIT_DEFAULT_PARAMS(LED_MASK);
	led_sb_init_param.off_time_ticks       = 1000;
	led_sb_init_param.on_time_ticks        = 2000;
	led_sb_init_param.duty_cycle_max       = 200;
	led_sb_init_param.duty_cycle_min       = 20;
	led_sb_init_param.duty_cycle_step      = 100;	

	led_softblink_uninit();
	APP_ERROR_CHECK(led_softblink_init(&led_sb_init_param));
	
	APP_ERROR_CHECK(led_softblink_start(LED_MASK));
}
static void hids_write(void)
{
}
static void init(void)
{	

}
const struct led Led= { 
	.init = init,	
	.advertising_fast = advertising_fast,
	.advertising_slow = advertising_slow,
	.advertising_idle = advertising_idle,
	.ble_connected = ble_connected,
	.hids_write = hids_write,
};
