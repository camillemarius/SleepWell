#ifndef __SD_CARD_H
#define __SD_CARD_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include "main.h"
/* Private includes ----------------------------------------------------------*/
#include "ff.h"    // Include the FatFs header file
#include <stdio.h>
#include <string.h>
#include <stdarg.h> // for va_list var arg functions
#include <inttypes.h>
#include <stdbool.h>

/* Exported types ------------------------------------------------------------*/
// Structure to represent accelerometer data
typedef struct {
    float x;
    float y;
    float z;
    float pitch;
    float roll;
} AccelData;

/* Exported constants --------------------------------------------------------*/


/* Exported macro ------------------------------------------------------------*/


/* Exported functions prototypes ---------------------------------------------*/


/* Private defines -----------------------------------------------------------*/
bool SDCard_sd_add_data(void);
bool sd_add_accel_data(AccelData data);
FRESULT sd_write_read_test(void);

FRESULT sd_add_data(const char* filename, AccelData* data);

FRESULT sd_read_data(const char* filename);

void myprintf(const char *fmt, ...);


#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
