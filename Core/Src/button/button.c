#include "button.h"
#include <stdbool.h>

static vibration_state_t vibration_state = VIBRATION_STATE_IDLE;
static vibration_functions_t vibration_function = VIBRATION_STATE_NONE;
static uint32_t vibration_start_time = 0;   // Startzeit der Vibration
static uint32_t vibration_duration = 40;   // Dauer der Vibration in Millisekunden
static uint32_t vibration_off_duration = 100;   // Dauer der Vibration in Millisekunden


// Timing for press detection
static uint32_t button_press_start_time = 0;  // When the button was first pressed
static uint32_t button_last_press_time = 0;   // Time of the last button press
static uint32_t press_threshold_short = 300;  // Max duration for short press (ms)
static uint32_t press_threshold_long = 1000;  // Duration for long press (ms)
static uint32_t double_press_threshold = 500; // Max time between presses for double press (ms)
static uint8_t press_count = 0;  // To count the number of presses for double press


void getButtonState(vibration_functions_t *vib_func) {
    *vib_func = vibration_function;
}

void HAL_GPIO_EXTI0_Callback(uint16_t GPIO_Pin) {
    uint32_t current_time = HAL_GetTick();

    if (vibration_function == VIBRATION_STATE_NONE){
        vibration_function = VIBRATION_STATE_DOUBLE;
    } else if (vibration_function == VIBRATION_STATE_SHORT){
        vibration_function = VIBRATION_STATE_DOUBLE;
    } else if (vibration_function == VIBRATION_STATE_DOUBLE){
        vibration_function = VIBRATION_STATE_SHORT;
    }
    vibration_state = VIBRATION_STATE_START;
    
}

typedef enum
{
    NO_PRESS,
    SINGLE_PRESS,
    LONG_PRESS,
    DOUBLE_PRESS
} eButtonEvent ;

bool buttonState()
{
    static const uint32_t DEBOUNCE_MILLIS = 20 ;
    static bool buttonstate = false;
    static uint32_t buttonstate_ts = 0;

    buttonstate = HAL_GPIO_ReadPin(Taster_EXTI0_GPIO_Port, Taster_EXTI0_Pin) == GPIO_PIN_SET;
    buttonstate_ts = HAL_GetTick();
    uint32_t now = HAL_GetTick() ;
    if( now - buttonstate_ts > DEBOUNCE_MILLIS ) {
        if( buttonstate != (HAL_GPIO_ReadPin(Taster_EXTI0_GPIO_Port, Taster_EXTI0_Pin) == GPIO_PIN_SET)) {
            buttonstate = !buttonstate ;
            buttonstate_ts = now ;
        }
    }
    return buttonstate ;
}


eButtonEvent getButtonEvent()
{
    static const uint32_t DOUBLE_GAP_MILLIS_MAX = 250 ;
    static const uint32_t LONG_MILLIS_MIN = 800 ;
    static uint32_t button_down_ts = 0 ;
    static uint32_t button_up_ts = 0 ;
    static bool double_pending = false ;
    static bool long_press_pending = false ;
    static bool button_down = false ; ;

    eButtonEvent button_event = NO_PRESS;
    uint32_t now = HAL_GetTick() ;

    // If state changed...
    if( button_down != buttonState() )
    {
        button_down = !button_down ;
        if( button_down )
        {
            // Timestamp button-down
            button_down_ts = now ;
        }
        else
        {
            // Timestamp button-up
            button_up_ts = now ;

            // If double decision pending...
            if( double_pending )
            {
                button_event = DOUBLE_PRESS ;
                double_pending = false ;
            }
            else
            {
                double_pending = true ;
            }

            // Cancel any long press pending
            long_press_pending = false ;
        }
    }

    // If button-up and double-press gap time expired, it was a single press
    if( !button_down && double_pending && now - button_up_ts > DOUBLE_GAP_MILLIS_MAX )
    {
        double_pending = false ;
        button_event = SINGLE_PRESS ;
    }
    // else if button-down for long-press...
    else if( !long_press_pending && button_down && now - button_down_ts > LONG_MILLIS_MIN )
    {
        button_event = LONG_PRESS ;
        long_press_pending = false ;
        double_pending = false ;

    }

    return button_event ;
}
        



void start_vibration() {
    vibration_start_time = HAL_GetTick();  // Setze den Startzeitpunkt
    HAL_GPIO_WritePin(IO_VIBRATOR_GPIO_Port, IO_VIBRATOR_Pin, GPIO_PIN_SET);
}

void stop_vibration() {
    vibration_start_time = HAL_GetTick();  // Setze den Startzeitpunkt
    HAL_GPIO_WritePin(IO_VIBRATOR_GPIO_Port, IO_VIBRATOR_Pin, GPIO_PIN_RESET);
}

// Diese Funktion überprüft, ob die Verzögerung abgelaufen ist
int is_delay_done(uint32_t duration) {
    return (HAL_GetTick() - vibration_start_time) >= duration;
}

void button_systick(void) {
    static vibration_cnt = 0;
    if(vibration_state == VIBRATION_STATE_IDLE) {
        ;
    } 
    else if(vibration_state == VIBRATION_STATE_START) {
        if(vibration_function == VIBRATION_STATE_SHORT) {
            vibration_cnt = 1;
            start_vibration();
        } else if(vibration_function == VIBRATION_STATE_DOUBLE) {
            vibration_cnt = 2;
            start_vibration();
        }
        vibration_state = VIBRATION_STATE_IN_PROGRESS;
    }
    else if(vibration_state == VIBRATION_STATE_IN_PROGRESS) {
        if (is_delay_done(vibration_duration)) {
            if(vibration_cnt>0) {
                static uint8_t low_period = 0;
                if(low_period == 0) {
                    stop_vibration();
                    vibration_state = VIBRATION_STATE_IN_PROGRESS;
                    low_period = 1;
                    vibration_cnt--;
                } else {
                    start_vibration();
                    vibration_state = VIBRATION_STATE_IN_PROGRESS;
                    low_period = 0;
                }
                
            }else {
                stop_vibration();
                vibration_state = VIBRATION_STATE_IDLE;
            }
        }
    }
}
