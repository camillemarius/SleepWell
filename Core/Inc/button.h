#ifndef __BUTTON_H
#define __BUTTON_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* Private includes ----------------------------------------------------------*/


/* Exported types ------------------------------------------------------------*/
//extern I2C_HandleTypeDef hi2c1;
typedef enum {
    VIBRATION_STATE_NONE,
    VIBRATION_STATE_SHORT,
    VIBRATION_STATE_DOUBLE,
} vibration_functions_t;

typedef enum {
    VIBRATION_STATE_IDLE,
    VIBRATION_STATE_START,
    VIBRATION_STATE_IN_PROGRESS
} vibration_state_t;

/* Exported constants --------------------------------------------------------*/


/* Exported macro ------------------------------------------------------------*/
// Zustandsmaschine für die Vibration


/* Exported functions prototypes ---------------------------------------------*/
void getButtonState(vibration_functions_t *vib_func);

void HAL_GPIO_EXTI0_Callback(uint16_t GPIO_Pin);

void start_vibration();

void stop_vibration();

// Diese Funktion überprüft, ob die Verzögerung abgelaufen ist
int is_delay_done(uint32_t duration);

void button_systick(void);

/* Private defines -----------------------------------------------------------*/


#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
