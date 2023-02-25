#include <LCD_I2C.h>
LCD_I2C lcd(0x27, 16, 2); // Default address of most PCF8574 modules, change according
const int sound = 8;
int soundState = 0;

void setup() {

  lcd.begin(); // If you are using more I2C devices using the Wire library use lcd.begin(false)
                // this stop the library(LCD_I2C) from calling Wire.begin()
  lcd.backlight();

  pinMode(sound, INPUT);

	Serial.begin(9600);		// Initialize serial communications with the PC
}

void loop() {

  soundState = digitalRead(sound);

  	if (soundState == HIGH) {
    lcd.clear();
    lcd.print("Game Started!");
    delay(500);
		return;
	}
  else {
    lcd.clear();
    lcd.print("First IF"); // You can make spaces using well... spaces
  }
}
