#include "Button.h"

static const uint32_t m_buttons_list[TOTAL_BUTTONS] = BUTTON_PIN_LIST;
static button_event_cfg_t m_events_list[TOTAL_BUTTONS] = { { BUTTON_EVENT_NOTHING, BUTTON_EVENT_NOTHING } };
static uint32_t         m_app_ticks_per_100ms = APP_TIMER_TICKS(100, 0);
static event_callback_t   m_registered_callback         = NULL;


static void button_action_callback(button_event_t event)
{
	uint32_t err_code;

	switch (event)
	{
	case BUTTON_EVENT_KEY_0:
		my_log_debug("BUTTON_EVENT_KEY_0!");
		break;
	default:
		break;
	}
}
static void button_event_handler(uint8_t pin_no, uint8_t button_action)
{
	button_event_t        event  = BUTTON_EVENT_NOTHING;
	uint32_t           button = 0;
	uint32_t           err_code;
	static uint8_t     current_long_push_pin_no; /**< Pin number of a currently pushed button, that could become a long push if held long enough. */
	static button_event_t release_event_at_push[1]; /**< Array of what the release event of each button was last time it was pushed, so that no release event is sent if the event was bound after the push of the button. */

	while ((button < TOTAL_BUTTONS) && (m_buttons_list[button] != pin_no))
	{
		button++;
	}

	if (button < TOTAL_BUTTONS)
	{
		switch (button_action)
		{
		case APP_BUTTON_PUSH:
			my_log_debug("APP_BUTTON_PUSH!");
			event = m_events_list[button].push_event;
			if (m_events_list[button].long_push_event != BUTTON_EVENT_NOTHING)
			{
				my_log_debug("registering longpoll listener!");
				err_code = app_timer_start(button_timer_id, BSP_MS_TO_TICK(BSP_LONG_PUSH_TIMEOUT_MS), (void*)&current_long_push_pin_no);
				if (err_code == NRF_SUCCESS)
				{
					current_long_push_pin_no = pin_no;
				}
				else
				{
					my_log_debug("cant create timer!");

				}
			}
			release_event_at_push[button] = m_events_list[button].release_event;
			break;
		case APP_BUTTON_RELEASE:
			my_log_debug("APP_BUTTON_RELEASE!");

			(void)app_timer_stop(button_timer_id);
			if (release_event_at_push[button] == m_events_list[button].release_event)
			{
				event = m_events_list[button].release_event;
			}
			break;
		case BSP_BUTTON_ACTION_LONG_PUSH:
			my_log_debug("BSP_BUTTON_ACTION_LONG_PUSH!");
			event = m_events_list[button].long_push_event;
		}
	}

	if ((event != BUTTON_EVENT_NOTHING) && (m_registered_callback != NULL))
	{
		m_registered_callback(event);
	}
}
static void button_timer_handler(void * p_context)
{
	my_log_debug("button_timer_handler!");
	button_event_handler(*(uint8_t *)p_context, BSP_BUTTON_ACTION_LONG_PUSH);
}
static const app_button_cfg_t app_buttons[TOTAL_BUTTONS] =
{
	{ BUTTON_1_PIN, false, NRF_GPIO_PIN_PULLUP, button_event_handler },
};

uint32_t event_to_button_action_assign(uint32_t button, bsp_button_action_t action, button_event_t event)
{
	uint32_t err_code = NRF_SUCCESS;

#if TOTAL_BUTTONS > 0
	if (button < TOTAL_BUTTONS)
	{
		if (event == BUTTON_EVENT_NOTHING)
		{
			// Setting default action: BSP_EVENT_KEY_x for PUSH actions, BSP_EVENT_NOTHING for RELEASE and LONG_PUSH actions.
			event = (action == BUTTON_ACTION_PUSH) ? (button_event_t)(BUTTON_EVENT_KEY_0 + button) : BUTTON_EVENT_NOTHING;
		}
		switch (action)
		{
		case BUTTON_ACTION_PUSH:
			m_events_list[button].push_event = event;
			break;
		case BSP_BUTTON_ACTION_LONG_PUSH:
			m_events_list[button].long_push_event = event;
			break;
		case BUTTON_ACTION_RELEASE:
			m_events_list[button].release_event = event;
			break;
		default:
			err_code = NRF_ERROR_INVALID_PARAM;
			break;
		}
	}
	else
	{
		err_code = NRF_ERROR_INVALID_PARAM;
	}
#else
	err_code = NRF_ERROR_INVALID_PARAM;
#endif // BUTTONS_NUMBER > 0

	return err_code;
}

static void init(void)
{	
	m_registered_callback = button_action_callback;
	for (int num = 0; (num < TOTAL_BUTTONS); num++)
	{
		APP_ERROR_CHECK(event_to_button_action_assign(num, BUTTON_ACTION_PUSH, BUTTON_EVENT_DEFAULT));
	}
	APP_ERROR_CHECK(app_button_init((app_button_cfg_t *)app_buttons, TOTAL_BUTTONS, APP_TIMER_TICKS(100, 0) / 2));
	APP_ERROR_CHECK(app_button_enable());
	APP_ERROR_CHECK(app_timer_create(&button_timer_id, APP_TIMER_MODE_SINGLE_SHOT, button_timer_handler));

}
const struct button Button= { 
	.init = init,		
};

