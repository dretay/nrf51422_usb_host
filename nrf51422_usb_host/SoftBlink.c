#include "SoftBlink.h"

static void init()
{
	u32 LED_MASK = (1 << 21 | 1 << 22 | 1 << 23);
	led_sb_init_params_t led_sb_init_param = LED_SB_INIT_DEFAULT_PARAMS(LED_MASK);
	led_sb_init_param.off_time_ticks       = 1000;
	led_sb_init_param.on_time_ticks        = 1000;
	led_sb_init_param.duty_cycle_max       = 200;
	led_sb_init_param.duty_cycle_min       = 1;
	led_sb_init_param.duty_cycle_step      = 50;	

	APP_ERROR_CHECK(led_softblink_init(&led_sb_init_param));
	
	APP_ERROR_CHECK(led_softblink_start(LED_MASK));
}
const struct softblink SoftBlink = { 
	.init = init,		
};

