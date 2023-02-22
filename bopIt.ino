// https://mhsp18object.wordpress.com/2018/05/06/project-2-diy-arduino-sensor-bop-it/
//include the library for lcd screen use
#include <SoftwareSerial.h>

//setting pins for lcd screen serial communication
SoftwareSerial mySerial(3,2); //pin 2 = TX, pin 3 = RX (unused)

//sensor pins
int buyItPin = 4;
int sellItPin = 6;
int holdItPin = A1;

//"start button" pin
int rfidScannerPin = 5;
int rfidScannerState = 0;

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
	mySerial.begin(9600);
	Serial.begin(9600);
	delay(500);

	// randomSeed(analogRead(A2)); // Not sure what this does yet

	//setting pin modes
	pinMode(rfidScannerPin, INPUT);
	pinMode(buyItPin, INPUT);  
	pinMode(sellItPin, INPUT);
	//holdItPin automatically set as an input due to its analog nature

	// set start button state to LOW
	rfidScannerState = 0;

	// display game start code on lcd
	mySerial.write(254);
	mySerial.write(128);
	mySerial.write("                ");
	mySerial.write("                ");
	mySerial.write(254);
	mySerial.write(128);
	mySerial.write("Press to Start");

	char scoreString[10];
}

void actionDetector(int action, int actionPin, string actionText, bool digital) {
	//if action selected is one, then tell user to "Buy It"
	Serial.println(actionText);

	//Read button value to track if user has changed it once prompted
	if digital {
		state1 = digitalRead(actionPin);
	}
	else {
		state1 = analogRead(actionPin);
	}

	//write "Buy It" to lcd to prompt user
	mySerial.write(254);
	mySerial.write(128);
	mySerial.write("                ");
	mySerial.write("                ");
	mySerial.write(254);
	mySerial.write(128);
	mySerial.write(actionText);

	//record the time you prompted the user
	timeOfPrompt = millis();
	timeElapsed = millis() – timeOfPrompt;

	//while loop checking if user completed action in allowed time
	while (timeElapsed < gameRate && actionCompleted == 0){
		//read switch for comparing to check if action was completed
		if digital {
			state2 = digitalRead(actionPin);
		}
		else {
			state2 = analogRead(actionPin);
		}
		//if the switch state changed, then the user completed action
		if (state2 != state1){
			actionCompleted = 1;
			score = score +1;
			gameRate = gameRate – (gameRate * .05);
		}

		//recalculate timeElapsed
		timeElapsed = millis() – timeOfPrompt;
	}

	//once while loop stops (either due to time running out or completing action)
	//check to see if action was comleted and adjust settings accordingly
	if (actionCompleted == 1){
		//do nothing
		Serial.println("action completed");
		tone(3, 10000, 250);
		delay(250);
		noTone(3);
	}
	else if (actionCompleted == 0){
		Serial.println("action failed");
		tone(3, 500, 750);
		delay(750);
		noTone(3);
		finalScore = score;
		char scoreString[10];
		sprintf(scoreString,"%4d",finalScore);
		//scoreString = String(finalScore);
		keepPlaying = 0;
		//Str = "Score: ";

		//write "GAME OVER" and score to LCD
		mySerial.write(254);
		mySerial.write(128);
		mySerial.write("                ");
		mySerial.write("                ");
		mySerial.write(254);
		mySerial.write(128);
		mySerial.write("GAME OVER!");
		mySerial.write(254);
		mySerial.write(192);
		mySerial.write("Score");
		mySerial.write(254);
		mySerial.write(199);
		mySerial.write(scoreString);

		delay(2000);                        
	}
}

void loop() {
	//Read start button to see if pressed. If it is pressed, go into game. 
	//If not, keep looping until user presses to start
	rfidScannerState = digitalRead(rfidScannerPin);
	Serial.print("rfidScannerState");
	Serial.print(rfidScannerState);
	Serial.println(" ");
	Serial.print("accelerometer value");
	Serial.print(analogRead(holdItPin));
	Serial.println(" ");


	// display game start code on lcd
	mySerial.write(254);
	mySerial.write(128);
	mySerial.write("                ");
	mySerial.write("                ");
	mySerial.write(254);
	mySerial.write(128);
	mySerial.write("Press Start");


	//If it is pressed, go into game.
	if (rfidScannerState == 1){

	//variable to control the while loop running the game until user has failed
	keepPlaying = 1;

		//while loop to keep running game until user has failed and "keepPlaying" is set to zero
		while (keepPlaying != 0) {
			//pick an action to assign the user via a random number generator – upper bound is exclusive
			action = random(1,3);
			actionCompleted = 0;
			Serial.print("action");
			Serial.print(action);
			Serial.println(" ");
		  
			//if else statements to prompt user and check action
		    switch(action) {
				case 1:
					actionDetector(action, buyItPin, "Buy It!", 1)
					break;
				case 2:
					actionDetector(action, sellItPin, "Sell It!", 1)
					break;
				case 3:
					actionDetector(action, holdItPin, "Hold It!", 0)
					break;
			}
		}
	}
}
