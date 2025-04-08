/* Includes ------------------------------------------------------------------*/
#include "vibrator.h"
#include <stdbool.h>

/* Private includes ----------------------------------------------------------*/


/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/


/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/
static bool overalDisable = true;
static uint32_t remaining_pulses = 0;  // Variable to track how many pulses to generate
static uint32_t state = 0;  // State for toggling GPIO
static uint32_t high_time_reload = 500;  // High time (in microseconds) for 50% duty cycle
static uint32_t low_time_reload = 500;   // Low time (in microseconds) for 50% duty cycle
static uint32_t isPulseInProgress = 0;  // Flag to track if a pulse is ongoing
/* Private function prototypes -----------------------------------------------*/


/* Private user code ---------------------------------------------------------*/
void vibrator_enable()
{
    overalDisable = false;
}
void vibrator_disable()
{
    overalDisable = true;
}
void vibrator_pwm_pulses(uint32_t f_pulses, uint32_t f_high_time_ms, uint32_t f_low_time_ms)
{
    // Check if a pulse is already in progress
    if (isPulseInProgress || overalDisable) {
        return;  // If a pulse is ongoing, don't start another one
    }
    // Set the flag indicating a pulse is in progress
    isPulseInProgress = 1;

    remaining_pulses = f_pulses;  // Set the number of pulses to generate
    state = 0;  // Ensure the state is reset (GPIO is initially low)

    // Get the clock frequency for the timer (typically APB1 clock)
    uint32_t timer_clock = HAL_RCC_GetPCLK1Freq();  // Get APB1 clock frequency (for TIM2)




    uint32_t prescaler = 255;


    // Calculate the reload value for the high time (in microseconds)
    //high_time_reload = (f_high_time_ms*1000) / (prescaler + 1);  // Reload value for high time
    //high_time_reload = timer_clock/()(f_high_time_ms*1000) / (prescaler + 1);  // Reload value for high time
    high_time_reload = (((timer_clock/(prescaler+1))*f_low_time_ms)/1000)-1;

    // Calculate the reload value for the low time (in microseconds)
    //low_time_reload = (f_low_time_ms*1000) / (prescaler + 1);  // Reload value for low time
    low_time_reload = (((timer_clock/(prescaler+1))*f_high_time_ms)/1000)-1;




    // Update the prescaler for the timer
    __HAL_TIM_SET_PRESCALER(&htim2, prescaler);

    // Start with GPIO high for the initial high time
    HAL_GPIO_WritePin(IO_VIBRATOR_GPIO_Port, IO_VIBRATOR_Pin, GPIO_PIN_SET);  // Set GPIO high

    // Set the timer's auto-reload value for the first state (high time)
    __HAL_TIM_SET_AUTORELOAD(&htim2, high_time_reload);  // Set the period for high time in microseconds

    // Start the timer interrupt to generate pulses
    HAL_TIM_Base_Start_IT(&htim2);
}

void TIM2_IRQHandler_Callback(void)
{
    if (__HAL_TIM_GET_FLAG(&htim2, TIM_FLAG_UPDATE) != RESET) {
        if (__HAL_TIM_GET_IT_SOURCE(&htim2, TIM_IT_UPDATE) != RESET) {
            __HAL_TIM_CLEAR_IT(&htim2, TIM_IT_UPDATE);  // Clear interrupt flag

            // Generate a PWM pulse
            if (state == 0) {
                HAL_GPIO_WritePin(IO_VIBRATOR_GPIO_Port, IO_VIBRATOR_Pin, GPIO_PIN_RESET);  // Set GPIO high
                state = 1;
                // Set timer to handle the high time duration
                __HAL_TIM_SET_AUTORELOAD(&htim2, low_time_reload);  // Set auto-reload to high time

                // If there are remaining pulses, decrement the count
                if (remaining_pulses > 0) {
                    remaining_pulses--;
                }
                

            } else {
                // If no more pulses are remaining, stop the timer or disable the interrupt
                if (remaining_pulses == 0) {
                    state = 0;
                    HAL_TIM_Base_Stop_IT(&htim2);  // Stop the timer interrupt
                    HAL_GPIO_WritePin(IO_VIBRATOR_GPIO_Port, IO_VIBRATOR_Pin, GPIO_PIN_RESET);  // Set GPIO low
                    // Set the flag indicating a pulse is no more in progress
                    isPulseInProgress = 0;
                    return;
                }

                HAL_GPIO_WritePin(IO_VIBRATOR_GPIO_Port, IO_VIBRATOR_Pin, GPIO_PIN_SET);  // Set GPIO low
                state = 0;
                // Set timer to handle the low time duration
                __HAL_TIM_SET_AUTORELOAD(&htim2, high_time_reload);  // Set auto-reload to low time
            }
        }
    }
}