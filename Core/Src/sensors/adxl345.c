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
static float pitch=0;
static float roll=0;

/* Private function prototypes -----------------------------------------------*/


/* Private user code ---------------------------------------------------------*/
void adxl_write_reg (uint8_t Reg, uint8_t Byte)
{
	HAL_I2C_Mem_Write(&hi2c1, ADXL345_I2C_ADDR, Reg, 1, &Byte, 1, 100);
}

void adxl_read (uint8_t Reg, uint8_t *Buffer, size_t len)
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
    *l_pitch = pitch;
    *l_roll = roll;      
}

// Interrupt service routine (ISR) for INT1 pin
void EXTI9_5_IRQHandler_Callback(void)
{
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
    pitch = atan2(yg, sqrt(xg * xg + zg * zg)) * (180.0f / M_PI);
    roll = atan2(xg, sqrt(yg * yg + zg * zg)) * (180.0f / M_PI);  

    // Überprüfe, ob der X-Wert den Schwellenwert überschreitet
    if (roll > 40 || roll < -40)
    {

    } else {
        vibrator_pwm_pulses(1,50,1000);
    }

    // Lese das Interrupt-Source-Register, um das Interrupt-Flag zu löschen
    uint8_t interruptSource = 0;
    adxl_read(ADXL345_INT_SOURCE, &interruptSource, 1);  // Lese Interrupt-Source Register
}
