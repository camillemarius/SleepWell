/* Includes ------------------------------------------------------------------*/
#include "button.h"
#include "vibrator.h"
#include <stdio.h>

/* Private includes ----------------------------------------------------------*/


/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/
#define SHORT_PRESS_THRESHOLD  300    // 200 ms
#define LONG_PRESS_THRESHOLD   550   // 1000 ms
#define DOUBLE_PRESS_THRESHOLD 300    // 500 ms
#define DEBOUNCE_DELAY         50     // Debounce delay in ms (50ms)

/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/
static uint32_t press_time = 0;
static uint32_t release_time = 0;

//Double Press variable
//static uint32_t last_press_time = 0;
//static uint32_t press_count = 0;

// Time of last interrupt for debouncing
static uint32_t last_interrupt_time = 0;  
static ButtonPressState buttonState = NO_PRESS;

// Initially consider button as released
static GPIO_PinState last_button_state = GPIO_PIN_SET;  

/* Private function prototypes -----------------------------------------------*/


/* Private user code ---------------------------------------------------------*/
void EXTI0_IRQHandler_Callback(void)
{
    uint32_t current_time = HAL_GetTick();  // Get the current system tick time

    // Check if the debounce time has passed
    if (current_time - last_interrupt_time < DEBOUNCE_DELAY) {
        // If not enough time has passed since the last interrupt, return early
        return;
    }

    // Update the last interrupt time
    last_interrupt_time = current_time;


    // Check if interrupt is due to EXTI0
    if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_0) != RESET) {
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_0);

        // Read the current button state
        GPIO_PinState current_button_state = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);

        // Check if a valid transition (high-to-low or low-to-high) has occurred
        if (current_button_state != last_button_state) {
            // Update the last button state for the next iteration
            last_button_state = current_button_state;

            // If the button was released (low to high transition)
            if (current_button_state == GPIO_PIN_RESET) {
                press_time = current_time;  // Store the press time
            } else {  // Button released (high to low transition)
                release_time = current_time;  // Store the release time

                uint32_t press_duration = release_time - press_time;

                if (press_duration < SHORT_PRESS_THRESHOLD) {
                    // Short press detected
                    buttonState = SHORT_PRESS;
                } else if (press_duration >= LONG_PRESS_THRESHOLD) {
                    // Long press detected
                    buttonState = LONG_PRESS;
                }
            }

            // Check for double press
            //if ((press_time - last_press_time) <= DOUBLE_PRESS_THRESHOLD) {
            //    press_count++;
            //} else {
            //    press_count = 1;  // Reset press count for new press sequence
           //}

            //last_press_time = press_time;

            // Handle double press detection
            //if (press_count == 2) {
            //    buttonState = DOUBLE_PRESS;
            //    press_count = 0;  // Reset the press count after double press detection
            //}
        }
    }
}

// This function will be called in the main loop or from an interrupt to handle detected presses
ButtonPressState button_get_state(void) {
    if (buttonState == SHORT_PRESS) {
        // Handle short press action (e.g., toggle LED)
        //vibrator_pwm_pulses(1,500,500);
        buttonState = NO_PRESS;  // Reset state
        return SHORT_PRESS;
    }
    else if (buttonState == LONG_PRESS) {
        // Handle long press action (e.g., turn on LED)
        //HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
        //vibrator_pwm_pulses(1,1000,1000);
        buttonState = NO_PRESS;  // Reset state
        return LONG_PRESS;
    }
    //else if (buttonState == DOUBLE_PRESS) {
        // Handle double press action (e.g., toggle another LED)
        //HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_1);
    //    vibrator_pwm_pulses(2,8);
    //    buttonState = NO_PRESS;  // Reset state
    //    return DOUBLE_PRESS;
    //}
    return NO_PRESS;
}