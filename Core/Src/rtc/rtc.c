
/* Includes ------------------------------------------------------------------*/
#include "rtc.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Private includes ----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/


/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/


/* Private user code ---------------------------------------------------------*/
/*void rtc_set_time_from_compile() {
    const char* date = __DATE__;  // The compile date, e.g., "Apr 08 2025"
    const char* time = __TIME__;  // The compile time, e.g., "12:34:56"
    
    uint8_t day = (date[4] - '0') * 10 + (date[5] - '0');  // Extract day
    uint8_t month = 0;

    // Extract month from the string (e.g., "Apr" -> 4)
    if (date[0] == 'J') {
        month = (date[1] == 'a') ? 1 : ((date[2] == 'n') ? 6 : 7);  // Jan, Jun, Jul
    } else if (date[0] == 'F') {
        month = 2;  // Feb
    } else if (date[0] == 'M') {
        month = (date[2] == 'r') ? 3 : 5;  // Mar, May
    } else if (date[0] == 'A') {
        month = (date[1] == 'r') ? 4 : 8;  // Apr, Aug
    } else if (date[0] == 'S') {
        month = 9;  // Sep
    } else if (date[0] == 'O') {
        month = 10;  // Oct
    } else if (date[0] == 'N') {
        month = 11;  // Nov
    } else if (date[0] == 'D') {
        month = 12;  // Dec
    }

    uint8_t year = (date[7] - '0') * 10 + (date[8] - '0');  // Extract year (last 2 digits)
    year += 2000;  // Convert to 4-digit year

    uint8_t hour = (time[0] - '0') * 10 + (time[1] - '0');  // Extract hours
    uint8_t minute = (time[3] - '0') * 10 + (time[4] - '0');  // Extract minutes
    uint8_t second = (time[6] - '0') * 10 + (time[7] - '0');  // Extract seconds

    // Initialize the RTC date and time structs
    RTC_DateTypeDef sDate = {0};
    RTC_TimeTypeDef sTime = {0};

    sDate.Year = year - 2000;  // RTC uses a year format (0-99)
    sDate.Month = month;
    sDate.Date = day;

    sTime.Hours = hour;
    sTime.Minutes = minute;
    sTime.Seconds = second;

    // Set the date and time on the RTC
    HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD);
    HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD);
}*/

uint8_t parse_month(const char *month_str) {
    if (strcmp(month_str, "Jan") == 0) return 1;
    if (strcmp(month_str, "Feb") == 0) return 2;
    if (strcmp(month_str, "Mar") == 0) return 3;
    if (strcmp(month_str, "Apr") == 0) return 4;
    if (strcmp(month_str, "May") == 0) return 5;
    if (strcmp(month_str, "Jun") == 0) return 6;
    if (strcmp(month_str, "Jul") == 0) return 7;
    if (strcmp(month_str, "Aug") == 0) return 8;
    if (strcmp(month_str, "Sep") == 0) return 9;
    if (strcmp(month_str, "Oct") == 0) return 10;
    if (strcmp(month_str, "Nov") == 0) return 11;
    if (strcmp(month_str, "Dec") == 0) return 12;
    return 0;
}

void rtc_set_time_from_compile(void) {
    // Parse __DATE__
    char month_str[4];
    int day, year;
    sscanf(__DATE__, "%3s %d %d", month_str, &day, &year);
    uint8_t month = parse_month(month_str);

    // Parse __TIME__
    int hour, min, sec;
    sscanf(__TIME__, "%d:%d:%d", &hour, &min, &sec);

    // Set RTC time
    RTC_TimeTypeDef sTime = {0};
    sTime.Hours   = hour;
    sTime.Minutes = min;
    sTime.Seconds = sec;
    HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);

    // Set RTC date
    RTC_DateTypeDef sDate = {0};
    sDate.Year  = year - 2000;  // RTC erwartet z. B. 25 statt 2025
    sDate.Month = month;
    sDate.Date  = day;
    sDate.WeekDay = 1; // kann auch ignoriert werden (oder Wochentag-Berechnung ergänzen)

    HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
}



void rtc_get_time_date(char *time, char *date)
{
  RTC_DateTypeDef gDate;
  RTC_TimeTypeDef gTime;

  /* Get the RTC current Time */
  HAL_RTC_GetTime(&hrtc, &gTime, RTC_FORMAT_BIN);
  /* Get the RTC current Date */
  HAL_RTC_GetDate(&hrtc, &gDate, RTC_FORMAT_BIN);

  /* Display time Format: hh:mm:ss */
  sprintf((char*)time,"%02d:%02d:%02d",gTime.Hours, gTime.Minutes, gTime.Seconds);


  /* Display date Format: dd-mm-yyyy */
  sprintf((char*)date,"%02d-%02d-%02d",gDate.Date, gDate.Month, 2000+gDate.Year);
}

void rtc_set_date (uint8_t year, uint8_t month, uint8_t date, uint8_t day)  // monday = 1
{
	RTC_DateTypeDef sDate = {0};
	sDate.WeekDay = day;
	sDate.Month = month;
	sDate.Date = date;
	sDate.Year = year;
	if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
	{
		Error_Handler();
	}

	HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, 0x2345);  // backup register
}

void rtc_set_time (uint8_t hr, uint8_t min, uint8_t sec)
{
	RTC_TimeTypeDef sTime = {0};
	sTime.Hours = hr;
	sTime.Minutes = min;
	sTime.Seconds = sec;
	sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	sTime.StoreOperation = RTC_STOREOPERATION_RESET;
	if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
	{
		Error_Handler();
	}
}

void rtc_set_alarm (uint8_t hr, uint8_t min, uint8_t sec, uint8_t date)
{
	RTC_AlarmTypeDef sAlarm = {0};
	sAlarm.AlarmTime.Hours = hr;
	sAlarm.AlarmTime.Minutes = min;
	sAlarm.AlarmTime.Seconds = sec;
	sAlarm.AlarmTime.SubSeconds = 0;
	sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
	sAlarm.AlarmMask = RTC_ALARMMASK_NONE;
	sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
	sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
	sAlarm.AlarmDateWeekDay = date;
	sAlarm.Alarm = RTC_ALARM_A;
	if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BIN) != HAL_OK)
	{
		Error_Handler();
	}
}