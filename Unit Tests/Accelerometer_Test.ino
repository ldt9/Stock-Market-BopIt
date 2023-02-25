#include "GY521.h"
#include <LCD_I2C.h>
LCD_I2C lcd(0x27, 16, 2); // Default address of most PCF8574 modules, change according

GY521 sensor(0x68);

uint32_t counter = 0;


void setup()
{
  lcd.begin(); // If you are using more I2C devices using the Wire library use lcd.begin(false)
                // this stop the library(LCD_I2C) from calling Wire.begin()
  lcd.backlight();
  Serial.begin(115200);
  Serial.println();
  Serial.println(__FILE__);
  Serial.print("GY521_LIB_VERSION: ");
  Serial.println(GY521_LIB_VERSION);

  Wire.begin();

  delay(100);
  while (sensor.wakeup() == false)
  {
    Serial.print(millis());
    Serial.println("\tCould not connect to GY521");
    delay(1000);
  }
  sensor.setAccelSensitivity(2);  // 8g
  sensor.setGyroSensitivity(1);   // 500 degrees/s

  sensor.setThrottle();
  Serial.println("start...");

  // set calibration values from calibration sketch.
  sensor.axe = 0.574;
  sensor.aye = -0.002;
  sensor.aze = -1.043;
  sensor.gxe = 10.702;
  sensor.gye = -6.436;
  sensor.gze = -0.676;
}


void loop()
{
  sensor.read();
  float pitch = sensor.getPitch();
  float roll  = sensor.getRoll();
  float yaw   = sensor.getYaw();

  if (counter % 10 == 0)
  {
    Serial.println("\nCNT\tPITCH\tROLL\tYAW");
  }

  lcd.clear();
  // lcd.print(counter);
  // lcd.print('\t');
  // lcd.print(pitch, 3);
  // lcd.print('\t');
  lcd.print(roll, 3);
  lcd.print('\t');
  lcd.print(yaw, 3);
  lcd.println();

  counter++;
}


// -- END OF FILE --
