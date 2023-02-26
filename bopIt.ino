// https://mhsp18object.wordpress.com/2018/05/06/project-2-diy-arduino-sensor-bop-it/
#include <string.h>

//Initialize the Gryroscope
#include "GY521.h" // Gyroscope Library

//Create an Instance of the Gryroscope
#include <Arduino_LSM6DSOX.h>

// Initialze the RFID Scanner
#include <SPI.h>
#include <MFRC522.h> // RFID Scanner Library

// Define Reset and SDA(SS) Pins
#define RST_PIN 5
#define SS_PIN  10

//Create an Indtance of the Scanner
MFRC522 mfrc522(SS_PIN, RST_PIN); 

//Initialize the LCD Screen
#include <LCD_I2C.h> // LCD Library
LCD_I2C lcd(0x27, 16, 2); // Default address of most PCF8574 modules, change according

//sensor pins
int buyItPin = 7;
int sellItPin = 8;
int holdItPin = 9; //Use gyroscope pitch and yaw

//"start button" pin
// int rfidScannerPin = 5;
// int rfidScannerState = 0;

//action to perform during each step
long action = 0;

//reference variables for sensor states
int state1 = 0;
int state2 = 0;

//time user has to complete action in milliseconds and related variables for time keeping
unsigned long gameRate = 3000;
unsigned long timeOfPrompt;
unsigned long timeElapsed;

//variable to control game loop
int keepPlaying = 0;
int actionCompleted = 0;
int score = 0;
int finalScore = 0;


void setup() {
	//initialize software serial monitor and give a delay for lcd to load
	lcd.begin();
  lcd.backlight();
	delay(500);

  //setup for the RFID Scanner
  Serial.begin(9600);   // Initialize comms with the PC
  while (!Serial);      // Do nothing until Serial comms are established
  SPI.begin();          //Initialize SPI Bus
  mfrc522.PCD_Init();   //Initialize the RFID Scanner
  delay(4);             //Delay for the scanner, can be increased depending on the board

	//setting pin modes
	// pinMode(rfidScannerPin, INPUT);
	pinMode(buyItPin, INPUT);       //Button controls this pin
	pinMode(sellItPin, INPUT);      //Sound detector controls this pin

	// set start button state to LOW
	// rfidScannerState = 0;

	// // display game start code on lcd
  // lcd.print("     Scan"); 
  // lcd.setCursor(3, 1);
  // lcd.print("to Start!");
  // delay(500);

	char scoreString[10];
}

void actionDetector(int action, int actionPin, String actionText, bool digital) {
	//if action selected is one, then tell user to "Buy It"
	// lcd.clear();
  // lcd.print(actionText);

  float x, y, z;

	//Read button value to track if user has changed it once prompted
	if (digital) {
		state1 = digitalRead(actionPin);
	}
	else {
		// state1 = analogRead(sensor); //Adjust this for the gyroscope
    IMU.readAcceleration(x, y, z);
    // lcd.clear();
    state1 = int(x) + int(y) + int(z);
    // lcd.print(roll);
    // lcd.clear();
  }

	//write "Buy It" to lcd to prompt user
  lcd.clear();
	lcd.print(actionText);

	//record the time you prompted the user
	timeOfPrompt = millis();
	timeElapsed = millis() - timeOfPrompt;

	//while loop checking if user completed action in allowed time
	while (timeElapsed < gameRate && actionCompleted == 0){
		//read switch for comparing to check if action was completed
		if (digital) {
			state2 = digitalRead(actionPin);
		}
		else {
			// state2 = analogRead(sensor);
      IMU.readAcceleration(x, y, z);
      // lcd.clear();
      state2 = int(x) + int(y) + int(z);
      // lcd.print(yaw);
      // lcd.clear();
		}
		//if the switch state changed, then the user completed action
		if (state2 != state1){
			actionCompleted = 1;
			score = score +1;
			gameRate = gameRate - (gameRate * .05);
		}

		//recalculate timeElapsed
		timeElapsed = millis() - timeOfPrompt;
	}

	//once while loop stops (either due to time running out or completing action)
	//check to see if action was comleted and adjust settings accordingly
	if (actionCompleted == 1){
    lcd.clear();
		lcd.print("action completed");
    digitalWrite(10, HIGH); // Green LED
		// tone(3, 10000, 250);
		delay(250);
		// noTone(3);
	}
	else if (actionCompleted == 0){
    lcd.clear();
		lcd.print("action failed");
		// tone(3, 500, 750);
		delay(750);
		// noTone(3);
		finalScore = score;
    score = 0; //reset score to 0 for new game
		char scoreString[10];
		sprintf(scoreString,"%4d",finalScore);
		//scoreString = String(finalScore);
		keepPlaying = 0;
		//Str = "Score: ";

		//write "GAME OVER" and score to LCD
		// lcd.write(254);
		// lcd.write(128);
		// lcd.write("                ");
		// lcd.write("                ");
		// lcd.write(254);
		// lcd.write(128);
    lcd.clear();
		lcd.print("GAME OVER!");
		// lcd.write(254);
		// lcd.write(192);
    lcd.clear();
		lcd.write("Score");
		// lcd.write(254);
		// lcd.write(199);
    lcd.clear();
		lcd.print(scoreString);

		delay(2000);                        
	}
}

void loop() {
	//Read start button to see if pressed. If it is pressed, go into game. 
	//If not, keep looping until user presses to start
	// rfidScannerState = digitalRead(rfidScannerPin);
	// Serial.print("rfidScannerState");
	// Serial.print(rfidScannerState);
	// Serial.println(" ");
	// Serial.print("accelerometer value");
	// Serial.print(analogRead(holdItPin));
	// Serial.println(" ");


	// display game start code on lcd
	// lcd.write(254);
	// lcd.write(128);
	// lcd.write("                ");
	// lcd.write("                ");
	// lcd.write(254);
	// lcd.write(128);
	// lcd.write("Press Start");

	// display game start code on lcd
  lcd.clear();
  lcd.print("     Scan"); 
  lcd.setCursor(3, 1);
  lcd.print("to Start!");
  delay(500);

	//If card is scanned, go into game.
	// if (! mfrc522.PICC_ReadCardSerial()){
	if (mfrc522.PICC_IsNewCardPresent()){

	//variable to control the while loop running the game until user has failed
	keepPlaying = 1;

		//while loop to keep running game until user has failed and "keepPlaying" is set to zero
		while (keepPlaying != 0) {
			//pick an action to assign the user via a random number generator – upper bound is exclusive
			action = random(1,4);
			actionCompleted = 0;
      lcd.clear();
			lcd.print(action);
      lcd.clear();
			// Serial.print(action);
			// Serial.println(" ");
		  
			//if else statements to prompt user and check action
		    switch(action) {
				case 1:
					actionDetector(action, buyItPin, "Buy It!", 1);
					break;
				case 2:
					actionDetector(action, sellItPin, "Sell It!", 1);
					break;
				case 3:
					actionDetector(action, holdItPin, "Hold It!", 0);
					break;
			}
		}
	}
}
