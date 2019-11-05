#include "Led.h"

static void advertising_fast(void)
{
	u32 LED_MASK = (1 << 13);
	u32 ticks = APP_TIMER_TICKS(50, 0);

	led_sb_init_params_t led_sb_init_param = LED_SB_INIT_DEFAULT_PARAMS(LED_MASK);
	led_sb_init_param.off_time_ticks       = ticks;
	led_sb_init_param.on_time_ticks        = ticks;
	led_sb_init_param.duty_cycle_max       = 255;
	led_sb_init_param.duty_cycle_min       = 0;
	led_sb_init_param.duty_cycle_step      = 255;	

	led_softblink_uninit();
	APP_ERROR_CHECK(led_softblink_init(&led_sb_init_param));
	
	APP_ERROR_CHECK(led_softblink_start(LED_MASK));
}
static void advertising_slow(void)
{
	u32 LED_MASK = (1 << 13);
	u32 ticks = APP_TIMER_TICKS(100, 0);

	led_sb_init_params_t led_sb_init_param = LED_SB_INIT_DEFAULT_PARAMS(LED_MASK);
	led_sb_init_param.off_time_ticks       = ticks;
	led_sb_init_param.on_time_ticks        = ticks;
	led_sb_init_param.duty_cycle_max       = 255;
	led_sb_init_param.duty_cycle_min       = 0;
	led_sb_init_param.duty_cycle_step      = 255;	

	led_softblink_uninit();
	APP_ERROR_CHECK(led_softblink_init(&led_sb_init_param));
	
	APP_ERROR_CHECK(led_softblink_start(LED_MASK));
}
static void advertising_idle(void)
{
}
static void ble_connected(void)
{
	led_softblink_uninit();
	nrf_gpio_cfg_output(13);
	NRF_GPIO->OUTSET = (1UL << 13);
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
