/* Includes ------------------------------------------------------------------*/
#include "adxl345.h"
#include <math.h> // Include math.h for trigonometric functions
#include "vibrator.h"

/* Private includes ----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/


/* Private macro -------------------------------------------------------------*/
#define ADXL345_I2C_ADDR 0xA6

/* Private variables ---------------------------------------------------------*/
// (Beschleunigung)
static float xg=0;
static float yg=0;
static float zg=0;

// pitch and roll
static float pitch_sum=0;  // Mittelwert Filter
static float roll_sum=0;
static uint16_t data_read_cnt = 0;

/* Private function prototypes -----------------------------------------------*/


/* Private user code ---------------------------------------------------------*/
static void adxl_read (uint8_t Reg, uint8_t *Buffer, size_t len)
{
	HAL_I2C_Mem_Read (&hi2c1, ADXL345_I2C_ADDR, Reg, 1, Buffer, len, 2000);
}

void adxl_init (void)
{
	uint8_t chipID=0;
	adxl_read(0x00, &chipID, 1);
	if (chipID == 0xE5)
	{
        // ADXL345 Register
        #define ADXL345_THRESH_ACT      0x24
        #define ADXL345_THRESH_INACT    0x25
        #define ADXL345_TIME_INACT      0x26
        #define ADXL345_ACT_INACT_CTL   0x27
        #define ADXL345_POWER_CTL       0x2D
        #define ADXL345_BW_RATE         0x2C
        #define ADXL345_INT_ENABLE      0x2E
        #define ADXL345_INT_MAP         0x2F
        #define ADXL345_INT_SOURCE      0x30
        #define ADXL345_DATA_FORMAT     0x31

        #define ADXL345_DUR             0x21
        #define ADXL345_LATENT          0x22
        #define ADXL345_WINDOW          0x23
        #define ADXL345_THRESH_TAP      0x1D
        #define ADXL345_TAP_AXES        0x2A
        #define ADXL345_FIFO_CTL        0x38
        #define ADXL345_FIFO_STATUS     0x39



        uint8_t data = 0x00; 
        // Setze Datenrate auf 12.5 Hz + Low Power Mode (0x16)
        data = 0x16;  // 0001 0110 → Bit 4: Low Power, Bits 3:0: 12.5 Hz
        HAL_I2C_Mem_Write(&hi2c1, ADXL345_I2C_ADDR, ADXL345_BW_RATE, 1, &data, 1, HAL_MAX_DELAY);

        // Setze Messmodus (Measurement Mode)
        data = 0x08;  // Bit 3 (Measure) = 1
        HAL_I2C_Mem_Write(&hi2c1, ADXL345_I2C_ADDR, ADXL345_POWER_CTL, 1, &data, 1, HAL_MAX_DELAY);

        // Setze Datenformat auf ±2g, 10 Bit Auflösung
        data = 0x00;  // Full_Res = 0, Range = ±2g
        HAL_I2C_Mem_Write(&hi2c1, ADXL345_I2C_ADDR, ADXL345_DATA_FORMAT, 1, &data, 1, HAL_MAX_DELAY);

        // Setze FIFO in BYPASS MODE (kein Puffer, immer aktueller Wert)
        data = 0x00;  // FIFO_MODE = 00 (Bypass)
        HAL_I2C_Mem_Write(&hi2c1, ADXL345_I2C_ADDR, ADXL345_FIFO_CTL, 1, &data, 1, HAL_MAX_DELAY);	
    }

	else  {
        Error_Handler(); 
    }

}

void adxl_init_interrupt (void)
{
	uint8_t chipID=0;
	adxl_read(0x00, &chipID, 1);
	if (chipID == 0xE5)
	{
        // ADXL345 Register
        #define ADXL345_THRESH_ACT      0x24
        #define ADXL345_THRESH_INACT    0x25
        #define ADXL345_TIME_INACT      0x26
        #define ADXL345_ACT_INACT_CTL   0x27
        #define ADXL345_POWER_CTL       0x2D
        #define ADXL345_BW_RATE         0x2C
        #define ADXL345_INT_ENABLE      0x2E
        #define ADXL345_INT_MAP         0x2F
        #define ADXL345_INT_SOURCE      0x30
        #define ADXL345_DATA_FORMAT     0x31

        #define ADXL345_DUR             0x21
        #define ADXL345_LATENT          0x22
        #define ADXL345_WINDOW          0x23
        #define ADXL345_THRESH_TAP      0x1D
        #define ADXL345_TAP_AXES        0x2A
        #define ADXL345_FIFO_CTL        0x38
        #define ADXL345_FIFO_STATUS     0x39



        uint8_t data = 0x00; 
        //HAL_I2C_Mem_Write(&hi2c1, ADXL345_I2C_ADDR, ADXL345_POWER_CTL, 1, &data, 1, HAL_MAX_DELAY);

        data = 0x18;  // Bit 3,4 Autosleep und Measure aktivieren
        HAL_I2C_Mem_Write(&hi2c1, ADXL345_I2C_ADDR, ADXL345_POWER_CTL, 1, &data, 1, HAL_MAX_DELAY);

        data = 0x00; // Deactivate all of the following register bits for initialization
        HAL_I2C_Mem_Write(&hi2c1, ADXL345_I2C_ADDR, ADXL345_DATA_FORMAT, 1, &data, 1, HAL_MAX_DELAY);
        HAL_I2C_Mem_Write(&hi2c1, ADXL345_I2C_ADDR, ADXL345_INT_ENABLE, 1, &data, 1, HAL_MAX_DELAY);
        HAL_I2C_Mem_Write(&hi2c1, ADXL345_I2C_ADDR, ADXL345_INT_MAP, 1, &data, 1, HAL_MAX_DELAY);
        HAL_I2C_Mem_Write(&hi2c1, ADXL345_I2C_ADDR, ADXL345_TIME_INACT, 1, &data, 1, HAL_MAX_DELAY);
        HAL_I2C_Mem_Write(&hi2c1, ADXL345_I2C_ADDR, ADXL345_THRESH_INACT, 1, &data, 1, HAL_MAX_DELAY);
        HAL_I2C_Mem_Write(&hi2c1, ADXL345_I2C_ADDR, ADXL345_ACT_INACT_CTL, 1, &data, 1, HAL_MAX_DELAY);
        HAL_I2C_Mem_Write(&hi2c1, ADXL345_I2C_ADDR, ADXL345_DUR, 1, &data, 1, HAL_MAX_DELAY);
        HAL_I2C_Mem_Write(&hi2c1, ADXL345_I2C_ADDR, ADXL345_LATENT, 1, &data, 1, HAL_MAX_DELAY);
        HAL_I2C_Mem_Write(&hi2c1, ADXL345_I2C_ADDR, ADXL345_THRESH_TAP, 1, &data, 1, HAL_MAX_DELAY);
        HAL_I2C_Mem_Write(&hi2c1, ADXL345_I2C_ADDR, ADXL345_TAP_AXES, 1, &data, 1, HAL_MAX_DELAY);
        HAL_I2C_Mem_Write(&hi2c1, ADXL345_I2C_ADDR, ADXL345_WINDOW, 1, &data, 1, HAL_MAX_DELAY);
        //Clear Interrupts
        adxl_read(ADXL345_INT_SOURCE, &data, 1); 
        HAL_I2C_Mem_Write(&hi2c1, ADXL345_I2C_ADDR, ADXL345_FIFO_CTL, 1, &data, 1, HAL_MAX_DELAY);
        //HAL_I2C_Mem_Write(&hi2c1, ADXL345_I2C_ADDR, ADXL345_FIFO_STATUS, 1, &data, 1, HAL_MAX_DELAY);


        
        //--------------------------------------------------------------------------------------------
        // -- Deaktiviere Interrupts
        //--------------------------------------------------------------------------------------------
        // Deaktiviere alle Interrupts
        data = 0x00;  // Interrupt deaktiveren
        HAL_I2C_Mem_Write(&hi2c1, ADXL345_I2C_ADDR, ADXL345_INT_ENABLE, 1, &data, 1, HAL_MAX_DELAY);


        //--------------------------------------------------------------------------------------------
        // -- Konfiguration
        //--------------------------------------------------------------------------------------------
        data = 0x15;// Setze die Datenrate (3.13 Hz)
        HAL_I2C_Mem_Write(&hi2c1, ADXL345_I2C_ADDR, ADXL345_BW_RATE, 1, &data, 1, HAL_MAX_DELAY);
        
        data = 0x00; // Setze das Datenformat für 10 Bit und ±2g
        HAL_I2C_Mem_Write(&hi2c1, ADXL345_I2C_ADDR, ADXL345_DATA_FORMAT, 1, &data, 1, HAL_MAX_DELAY);

        
        //--------------------------------------------------------------------------------------------
        // -- Activity/Inactivity Konfiguration
        //--------------------------------------------------------------------------------------------
        data = 0X20; // Konfiguriere die Y-Achse für Activity erkennung in dc-coupled operation
        HAL_I2C_Mem_Write(&hi2c1, ADXL345_I2C_ADDR, ADXL345_ACT_INACT_CTL, 1, &data, 1, HAL_MAX_DELAY);

        data = 1;//40;//75;  // Setze den Aktivitätsschwellenwert (X *(62.5mg/LSB))
        HAL_I2C_Mem_Write(&hi2c1, ADXL345_I2C_ADDR, ADXL345_THRESH_ACT, 1, &data, 1, HAL_MAX_DELAY);


        //--------------------------------------------------------------------------------------------
        // -- Interrupt Konfiguration
        //--------------------------------------------------------------------------------------------
        // Aktiviere die Interrupts für Aktivität/Inaktivität
        data = 0x10;  // Bit 4 (Activity)
        HAL_I2C_Mem_Write(&hi2c1, ADXL345_I2C_ADDR, ADXL345_INT_ENABLE, 1, &data, 1, HAL_MAX_DELAY);

        // Interrupt-Pins konfigurieren (optional: INT1 oder INT2)
        data = 0x00;  // Interrupts auf INT1 lassen (Standard)
        HAL_I2C_Mem_Write(&hi2c1, ADXL345_I2C_ADDR, ADXL345_INT_MAP, 1, &data, 1, HAL_MAX_DELAY);

        // Lese und lösche das Interrupt-Source Register
        data = 0x00;
        adxl_read(ADXL345_INT_SOURCE, &data, 1); 
      
        // Setze den Sensor in den Messmodus
        //data = 0x18;  // Bit 3 (Measure) setzen
        //HAL_I2C_Mem_Write(&hi2c1, ADXL345_I2C_ADDR, ADXL345_POWER_CTL, 1, &data, 1, HAL_MAX_DELAY);
	
    }

	else  {
        Error_Handler(); 
    }

}

void adxl_enable() {
    uint8_t data = 0x08;  // Set bit 3 to enable measurement mode
    HAL_I2C_Mem_Write(&hi2c1, ADXL345_I2C_ADDR, ADXL345_POWER_CTL, 1, &data, 1, HAL_MAX_DELAY);
}

void adxl_disable() {
    uint8_t data = 0x00;  // Clear bit 3 to put the sensor in standby mode
    HAL_I2C_Mem_Write(&hi2c1, ADXL345_I2C_ADDR, ADXL345_POWER_CTL, 1, &data, 1, HAL_MAX_DELAY);
}

void adxl_getXYZ(float *l_x, float *l_y, float *l_z) {
    *l_x = xg;
    *l_y = yg;
    *l_z = zg;
}

void adxl_getAngle(float *l_pitch, float *l_roll) {
    float l_pitch_sum = 0;
    float l_roll_sum = 0;
    if(data_read_cnt > 0) {
        l_pitch_sum = pitch_sum/data_read_cnt;
        l_roll_sum = roll_sum/data_read_cnt;
    }
    pitch_sum = 0;
    roll_sum = 0;
    data_read_cnt = 0;

    *l_pitch = l_pitch_sum;
    *l_roll = l_roll_sum;      
}

void adxl_readDataFromSensor() {
    uint8_t RxData[6];
    adxl_read(0x32, RxData, 6);  // Lese Daten vom ADXL345

    // Verarbeite die Daten und prüfe z.B. den X-Wert
    int16_t RAWX = ((RxData[1] << 8) | RxData[0]);
    int16_t RAWY = ((RxData[3] << 8) | RxData[2]);
    int16_t RAWZ = ((RxData[5] << 8) | RxData[4]);

    // Umrechnung der Rohdaten in 'g' (Beschleunigung)
    xg = RAWX * 0.0078;
    yg = RAWY * 0.0078;
    zg = RAWZ * 0.0078;


    // Calculate pitch and roll
    //pitch = (int16_t) atan2(yg, sqrt(xg * xg + zg * zg)) * (180.0f / M_PI);
    //roll =  (int16_t) atan2(xg, sqrt(yg * yg + zg * zg)) * (180.0f / M_PI);  
    float pitch = atan2(yg, sqrt(xg * xg + zg * zg)) * (180.0f / M_PI);
    float roll =  atan2(xg, sqrt(yg * yg + zg * zg)) * (180.0f / M_PI);  

    if(data_read_cnt <10) {
        pitch_sum += pitch;
        roll_sum += roll;
        data_read_cnt += 1;
    }

    // Überprüfe, ob der X-Wert den Schwellenwert überschreitet
    static uint16_t vibrator_pwm_pulse_start = 60;
    if (roll > 40 || roll < -40)
    {
        vibrator_pwm_pulse_start = 60;
    } else {
        if(zg < 0)
        {
            vibrator_pwm_pulses(1,vibrator_pwm_pulse_start,500);
            vibrator_pwm_pulse_start++;
        } else {
            vibrator_pwm_pulse_start = 60;
        }
    }

    //
    if(zg < 0) {
        /*if(roll > 40 || roll < -40) {
            vibrator_pwm_pulses(1,vibrator_pwm_pulse_start,500);
            vibrator_pwm_pulse_start++;
        } else {
            vibrator_pwm_pulse_start = 60;
        }*/
    } else {
        if((zg>0.5) && (roll>0)) {
            roll = 90+(90-roll);
        } else if((zg>0.5) && (roll<0)) {
            roll = -90-(90+roll);
        }
    }

}

// Interrupt service routine (ISR) for INT1 pin
void EXTI9_5_IRQHandler_Callback(void)
{
    //uint8_t RxData[6];
    //adxl_read(0x32, RxData, 6);  // Lese Daten vom ADXL345

    // Verarbeite die Daten und prüfe z.B. den X-Wert
    //int16_t RAWX = ((RxData[1] << 8) | RxData[0]);
    //int16_t RAWY = ((RxData[3] << 8) | RxData[2]);
    //int16_t RAWZ = ((RxData[5] << 8) | RxData[4]);

    // Umrechnung der Rohdaten in 'g' (Beschleunigung)
    //xg = RAWX * 0.0078;
    //yg = RAWY * 0.0078;
    //zg = RAWZ * 0.0078;


    // Calculate pitch and roll
    //pitch = atan2(yg, sqrt(xg * xg + zg * zg)) * (180.0f / M_PI);
    //roll = atan2(xg, sqrt(yg * yg + zg * zg)) * (180.0f / M_PI);  

    // Überprüfe, ob der X-Wert den Schwellenwert überschreitet
    //if (roll > 40 || roll < -40)
    //{

    //} else {
    //    vibrator_pwm_pulses(1,50,1000);
    //}

    // Lese das Interrupt-Source-Register, um das Interrupt-Flag zu löschen
    //uint8_t interruptSource = 0;
    //adxl_read(ADXL345_INT_SOURCE, &interruptSource, 1);  // Lese Interrupt-Source Register
}
