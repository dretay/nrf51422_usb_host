#include "Button.h"

static uint32_t         m_app_ticks_per_100ms = APP_TIMER_TICKS(100, 0);
static button_config button_configs[BUTTON_LIST_MAX];
static app_button_cfg_t app_buttons[BUTTON_LIST_MAX];


static void button_event_handler(uint8_t pin_no, uint8_t button_action)
{
	static uint8_t     current_long_push_pin_no; 
	uint32_t           err_code;
	u32 button = 0;
	while ((button < BUTTON_LIST_MAX) && (button_configs[button].pin != pin_no))
	{
		button++;
	}

	if (button < BUTTON_LIST_MAX)
	{
		switch (button_action)
		{
		case APP_BUTTON_PUSH:
			my_log_debug("APP_BUTTON_PUSH!");
			
			if (button_configs[button].long_push != NULL)
			{
				my_log_debug("registering longpoll listener!");
				err_code = app_timer_start(button_timer_id, MS_TO_TICK(LONG_PUSH_TIMEOUT_MS), (void*)&current_long_push_pin_no);
				button_configs[button].push();
				if (err_code == NRF_SUCCESS)
				{
					current_long_push_pin_no = pin_no;
				}
				else
				{
					my_log_debug("cant create timer!");
				}
			}
			break;
		case APP_BUTTON_RELEASE:
			my_log_debug("APP_BUTTON_RELEASE!");

			(void)app_timer_stop(button_timer_id);
			if (button_configs[button].release != NULL)
			{
				button_configs[button].release();
			}
			break;
		case BUTTON_LONG_PUSH:
			my_log_debug("BUTTON_LONG_PUSH!");
			if (button_configs[button].long_push != NULL)
			{
				button_configs[button].long_push();
			}
		}
	}	
}
static void button_timer_handler(void * p_context)
{
	my_log_debug("button_timer_handler!");
	button_event_handler(*(uint8_t *)p_context, BUTTON_LONG_PUSH);
}



static u8 NEXT_UNASSIGNED_BUTTON_IDX = 0;
static bool add_button(u8 pin, void* push_callback, void* long_push_callback, void* release_callback, void* startup_callback)
{
	if (NEXT_UNASSIGNED_BUTTON_IDX <= BUTTON_LIST_MAX)
	{
		button_configs[NEXT_UNASSIGNED_BUTTON_IDX] = (button_config)
		{
			.pin = pin,
			.push = push_callback,
			.long_push = long_push_callback,
			.release = release_callback,
			.startup = startup_callback,
		};
		NEXT_UNASSIGNED_BUTTON_IDX++;
		return true;
	}
	return false;
}

static void start(void)
{	
	for (u32 i = 0; i < NEXT_UNASSIGNED_BUTTON_IDX; i++)
	{
		app_buttons[i] = (app_button_cfg_t)
		{ 
			.pin_no = button_configs[i].pin, 
			.active_state = false, 
			.pull_cfg = NRF_GPIO_PIN_PULLUP, 
			.button_handler = button_event_handler 
		};

	}
	APP_ERROR_CHECK(app_button_init((app_button_cfg_t *)app_buttons, NEXT_UNASSIGNED_BUTTON_IDX, APP_TIMER_TICKS(100, 0) / 2));
	for (u32 i = 0; i < NEXT_UNASSIGNED_BUTTON_IDX; i++)
	{
		if (button_configs[i].startup != NULL && !nrf_drv_gpiote_in_is_set(button_configs[i].pin))
		{
			button_configs[i].startup();
		}
	}

	APP_ERROR_CHECK(app_button_enable());
	APP_ERROR_CHECK(app_timer_create(&button_timer_id, APP_TIMER_MODE_SINGLE_SHOT, button_timer_handler));

}
const struct button Button= { 
	.add_button = add_button,
	.start = start,		
};

