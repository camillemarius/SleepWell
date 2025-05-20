#ifndef __ADXL345_H
#define __ADXL345_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stddef.h>
#include "main.h"
/* Private includes ----------------------------------------------------------*/


/* Exported types ------------------------------------------------------------*/
extern I2C_HandleTypeDef hi2c1;

/* Exported constants --------------------------------------------------------*/


/* Exported macro ------------------------------------------------------------*/


/* Exported functions prototypes ---------------------------------------------*/


/* Private defines -----------------------------------------------------------*/
//static void adxl_write_reg (uint8_t Reg, uint8_t Byte);
//static void adxl_read (uint8_t Reg, uint8_t *Buffer, size_t len);

void adxl_init (void);
void adxl_init_interrupt (void);
void adxl_enable();
void adxl_disable();
void adxl_readDataFromSensor();
void adxl_getXYZ(float *x, float *y, float *z);
void adxl_getAngle(float *pitch, float *roll);
void EXTI9_5_IRQHandler_Callback(void);




#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
