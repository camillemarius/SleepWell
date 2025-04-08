#ifndef __RTC_H
#define __RTC_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include "main.h"
/* Private includes ----------------------------------------------------------*/


/* Exported types ------------------------------------------------------------*/
extern RTC_HandleTypeDef hrtc;

/* Exported constants --------------------------------------------------------*/


/* Exported macro ------------------------------------------------------------*/


/* Exported functions prototypes ---------------------------------------------*/
#include "rtc.h"
#include <stdio.h>

void rtc_set_time_from_compile();
void rtc_get_time_date(char *time, char *date);
void rtc_set_date (uint8_t year, uint8_t month, uint8_t date, uint8_t day);
void rtc_set_time (uint8_t hr, uint8_t min, uint8_t sec);
void rtc_set_alarm (uint8_t hr, uint8_t min, uint8_t sec, uint8_t date);

/* Private defines -----------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
