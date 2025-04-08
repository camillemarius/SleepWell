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
// State definitions
typedef enum {
    SHORT_PRESS,
    LONG_PRESS,
    DOUBLE_PRESS,
    NO_PRESS
} ButtonPressState;


/* Exported functions prototypes ---------------------------------------------*/
void EXTI0_IRQHandler_Callback(void);
ButtonPressState button_get_state(void);

/* Private defines -----------------------------------------------------------*/


#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
