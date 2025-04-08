/* Includes ------------------------------------------------------------------*/
#include "sd_card.h"
#include "rtc.h"

/* Private includes ----------------------------------------------------------*/


/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/


/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/
// Declare global FatFs handle and file handle
FATFS FatFs;      // FatFs handle
FIL fil;          // File handle
FRESULT fres;     // Result after operations

/* Private function prototypes -----------------------------------------------*/


/* Private user code ---------------------------------------------------------*/
// Function to initialize the SD card and mount the file system
FRESULT sd_init(void) {
    fres = f_mount(&FatFs, "", 1); // 1 = mount now
    if (fres != FR_OK) {
        //myprintf("f_mount error (%i)\r\n", fres);
        return fres;
    }
    return FR_OK;
}

// Function to get the current timestamp in milliseconds
unsigned long GetTimestamp(void) {
  // Assuming HAL_GetTick() gives the system uptime in milliseconds (you can adjust based on your system)
  return HAL_GetTick(); // or any other timestamp function you have in your environment
}
// Function to write accelerometer data to the SD card
FRESULT sd_add_data(const char* filename, AccelData* data) {
    // Initialize SD card
    fres = sd_init();
    if (fres != FR_OK) {
        return fres;
    }



    // Get timestamp for the current reading
    unsigned long timestamp = GetTimestamp();
    
    char timeData[10];
    char dateData[10];
    rtc_get_time_date(timeData, dateData);
    

    // Open the file for writing (create if it doesn't exist)
    fres = f_open(&fil, filename, FA_WRITE | FA_OPEN_ALWAYS);
    if (fres != FR_OK) {
        //myprintf("f_open error (%i)\r\n", fres);
        return fres;
    }

    // Seek to the end of the file (so we don't overwrite existing data)
    fres = f_lseek(&fil, f_size(&fil)); 
    if (fres != FR_OK) {
        //myprintf("f_lseek error (%i)\r\n", fres);
        f_close(&fil); // Close before returning error
        return fres;
    }

    // Prepare the data in CSV format with timestamp (timestamp,x,y,z)
    char buffer[128];
    int len = snprintf(buffer, sizeof(buffer), "%lu; %d; %d\r\n", timestamp, (int)data->pitch, (int)data->roll);
    //int len = snprintf(buffer, sizeof(buffer), "%s; %s; %d; %d\r\n", dateData, timeData, (int)data->pitch, (int)data->roll);

    //dtostrf(3.14159, 6, 4, buffer);  // Converts float to string with 4 decimal places
    //int len = snprintf(buffer, sizeof(buffer), "%lu,%.4f,%.4f,%.4f\r\n", timestamp, data->x, data->y, data->z);

    // Write the data to the file
    UINT bytesWrote;
    fres = f_write(&fil, buffer, len, &bytesWrote);
    if (fres != FR_OK) {
        //myprintf("f_write error (%i)\r\n", fres);
        f_close(&fil); // Close before returning error
        return fres;
    }

    //myprintf("Wrote %u bytes to '%s'.\r\n", bytesWrote, filename);

    // Close the file after writing
    f_close(&fil);

    // Unmount the SD card
    fres = f_mount(NULL, "", 0);
    if (fres != FR_OK) {
        //myprintf("f_mount unmount error (%i)\r\n", fres);
        return fres;
    }
    return FR_OK;
}

// Function to read accelerometer data from the SD card
FRESULT sd_read_data(const char* filename) {
    // Open the file for reading
    fres = f_open(&fil, filename, FA_READ);
    if (fres != FR_OK) {
        //myprintf("f_open error (%i)\r\n", fres);
        return fres;
    }

    // Read file line by line (assuming CSV format)
    char buffer[128];
    while (f_gets(buffer, sizeof(buffer), &fil)) {
        unsigned long timestamp;
        uint16_t x, y, z;

        // Parse the CSV data (timestamp,x,y,z)
        int itemsRead = sscanf(buffer, "%lu,%" PRIu16 ",%" PRIu16 ",%" PRIu16, &timestamp, &x, &y, &z);
        if (itemsRead == 4) {
            //myprintf("Read accelerometer data: Timestamp=%lu, x=%" PRIu16 ", y=%" PRIu16 ", z=%" PRIu16 "\r\n", timestamp, x, y, z);
        } else {
            //myprintf("Error reading data from file\r\n");
        }
    }

    // Close the file after reading
    f_close(&fil);
    return FR_OK;
}

// Main function to demonstrate writing and reading accelerometer data with timestamps
FRESULT sd_write_read_test(void) {
  // Initialize SD card
  fres = sd_init();
  if (fres != FR_OK) {
      return fres;
  }

  // Create an accelerometer reading example
  AccelData data = {1.23f, 4.56f, 7.89f, 5, 4};

  // Write the accelerometer data with timestamp to the SD card
  fres = sd_add_data("accel.csv", &data);
  if (fres != FR_OK) {
      return fres;
  }

  // Now, read back the data from the SD card
  fres = sd_read_data("accel.csv");
  if (fres != FR_OK) {
      return fres;
  }

  // Unmount the SD card
  fres = f_mount(NULL, "", 0);
  if (fres != FR_OK) {
      //myprintf("f_mount unmount error (%i)\r\n", fres);
      return fres;
  }

  return FR_OK;
}




/*--------------------------------------------------------
---------------------TO DELETE----------------------------
--------------------------------------------------------*/
bool sd_add_accel_data(AccelData data) {
    // Initialize SD card
    fres = sd_init();
    if (fres != FR_OK) {
        return fres;
    }
  
  
    // Write the accelerometer data with timestamp to the SD card
    fres = sd_add_data("accel.csv", &data);
    if (fres != FR_OK) {
        return fres;
    }
  
    // Unmount the SD card
    fres = f_mount(NULL, "", 0);
    if (fres != FR_OK) {
        //myprintf("f_mount unmount error (%i)\r\n", fres);
        return fres;
    }
  
    return fres != FR_OK ? false: true;
  }
  



