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
#include "adxl345.h"


void ADXL_Write_Reg (uint8_t Reg, uint8_t Byte);
void ADXL_Read (uint8_t Reg, uint8_t *Buffer, size_t len);
void ADXL_Init (void);


void ADXL345_InterruptHandler(void);


#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
