#pragma once

//nrf libraries
#include "app_button.h"
#include "app_timer.h"
#include "nrf_drv_gpiote.h"

//project libraries
#include "log.h"
#include "UartLogger.h"

#define BSP_BUTTON_ACTION_LONG_PUSH (2)
typedef enum
{
	BUTTON_EVENT_NOTHING = 0,
	BUTTON_EVENT_DEFAULT,
	BUTTON_EVENT_KEY_0,
	
} button_event_t;
#define TOTAL_BUTTONS 1
#define BUTTON_1_PIN       20
#define BUTTON_PIN_LIST { BUTTON_1_PIN }

#define BSP_LONG_PUSH_TIMEOUT_MS (1000) /**< The time to hold for a long push (in milliseconds). */
typedef struct
{
	button_event_t push_event; /**< The event to fire on regular button press. */
	button_event_t long_push_event; /**< The event to fire on long button press. */
	button_event_t release_event; /**< The event to fire on button release. */
} button_event_cfg_t;
#define my_log_debug(...) UartLogger.log_log(false, false, LOG_DEBUG, __FILENAME__, __FUNCTION__, __LINE__, __VA_ARGS__)
#define BSP_MS_TO_TICK(MS) (m_app_ticks_per_100ms * (MS / 100))
typedef void(* event_callback_t)(button_event_t);
APP_TIMER_DEF(button_timer_id);
#define BUTTON_ACTION_PUSH      (APP_BUTTON_PUSH)
#define BUTTON_ACTION_RELEASE   (APP_BUTTON_RELEASE)
typedef uint8_t bsp_button_action_t;

struct button {
	void(*init)(bool *erase_bonds);			
};
extern const struct button Button;