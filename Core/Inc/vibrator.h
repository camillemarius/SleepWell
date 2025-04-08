#ifndef __VIBRATOR_H
#define __VIBRATOR_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* Private includes ----------------------------------------------------------*/


/* Exported types ------------------------------------------------------------*/
extern TIM_HandleTypeDef htim2;

/* Exported constants --------------------------------------------------------*/


/* Exported macro ------------------------------------------------------------*/


/* Exported functions prototypes ---------------------------------------------*/
//void vibrator_pwm_pulses(uint32_t pulses);
void vibrator_enable();
void vibrator_disable();

void vibrator_pwm_pulses(uint32_t f_pulses, uint32_t f_high_time_ms, uint32_t f_low_time_ms);
void TIM2_IRQHandler_Callback(void);

/* Private defines -----------------------------------------------------------*/


#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
