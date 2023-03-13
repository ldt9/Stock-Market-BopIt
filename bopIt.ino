//TODO:
// Add Speaker functionality

// https://mhsp18object.wordpress.com/2018/05/06/project-2-diy-arduino-sensor-bop-it/
#include <string.h>

//Create an Instance of the Gryroscope
#include <Arduino_LSM6DSOX.h>

//Initialize the LCD Screen
#include <LCD_I2C.h> // LCD Library
LCD_I2C lcd(0x27, 16, 2); // Default address of most PCF8574 modules, change according

//sensor pins
int buyItPin = 7;
int sellItPin = 8;
int holdItPin = 6; //Use gyroscope pitch and yaw
int bypass = 9; //To bypass the RFID Scanner during testing
int greenPin =  2;
int redPin = 4;
int buzzer = 10;

//action to perform during each step
long action = 0;

//reference variables for sensor states
int state1 = 0;
int state2 = 0;

//time user has to complete action in milliseconds and related variables for time keeping
unsigned long gameRate = 10000;
unsigned long timeOfPrompt;
unsigned long timeElapsed;

//variable to control game loop
int keepPlaying = 0;
int actionCompleted = 0;
int score = 0;
int finalScore = 0;
int bypassTrue = 0;

//define tones
#define NOTE_B0 31
#define NOTE_C1 33
#define NOTE_CS1 35
#define NOTE_D1 37
#define NOTE_DS1 39
#define NOTE_E1 41
#define NOTE_F1 44
#define NOTE_FS1 46
#define NOTE_G1 49
#define NOTE_GS1 52
#define NOTE_A1 55
#define NOTE_AS1 58
#define NOTE_B1 62
#define NOTE_C2 65
#define NOTE_CS2 69
#define NOTE_D2 73
#define NOTE_DS2 78
#define NOTE_E2 82
#define NOTE_F2 87
#define NOTE_FS2 93
#define NOTE_G2 98
#define NOTE_GS2 104
#define NOTE_A2 110
#define NOTE_AS2 117
#define NOTE_B2 123
#define NOTE_C3 131
#define NOTE_CS3 139
#define NOTE_D3 147
#define NOTE_DS3 156
#define NOTE_E3 165
#define NOTE_F3 175
#define NOTE_FS3 185
#define NOTE_G3 196
#define NOTE_GS3 208
#define NOTE_A3 220
#define NOTE_AS3 233
#define NOTE_B3 247
#define NOTE_C4 262
#define NOTE_CS4 277
#define NOTE_D4 294
#define NOTE_DS4 311
#define NOTE_E4 330
#define NOTE_F4 349
#define NOTE_FS4 370
#define NOTE_G4 392
#define NOTE_GS4 415
#define NOTE_A4 440
#define NOTE_AS4 466
#define NOTE_B4 494
#define NOTE_C5 523
#define NOTE_CS5 554
#define NOTE_D5 587
#define NOTE_DS5 622
#define NOTE_E5 659
#define NOTE_F5 698
#define NOTE_FS5 740
#define NOTE_G5 784
#define NOTE_GS5 831
#define NOTE_A5 880
#define NOTE_AS5 932
#define NOTE_B5 988
#define NOTE_C6 1047
#define NOTE_CS6 1109
#define NOTE_D6 1175
#define NOTE_DS6 1245
#define NOTE_E6 1319
#define NOTE_F6 1397
#define NOTE_FS6 1480
#define NOTE_G6 1568
#define NOTE_GS6 1661
#define NOTE_A6 1760
#define NOTE_AS6 1865
#define NOTE_B6 1976
#define NOTE_C7 2093
#define NOTE_CS7 2217
#define NOTE_D7 2349
#define NOTE_DS7 2489
#define NOTE_E7 2637
#define NOTE_F7 2794
#define NOTE_FS7 2960
#define NOTE_G7 3136
#define NOTE_GS7 3322
#define NOTE_A7 3520
#define NOTE_AS7 3729
#define NOTE_B7 3951
#define NOTE_C8 4186
#define NOTE_CS8 4435
#define NOTE_D8 4699
#define NOTE_DS8 4978
#define REST 0


// change this to make the song slower or faster
int tempo = 360;

int buyMelody[] = {
  //Cantina Band
  NOTE_B4,-4, NOTE_E5,-4, NOTE_B4,-4, NOTE_E5,-4,
};

int sellMelody[] = {

  // Super Mario Brothers 
  NOTE_G5,8, NOTE_FS5,8, NOTE_F5,8, NOTE_DS5,4,
};

int holdMelody[] = {

  //Keyboard Cat
  NOTE_C4,4, NOTE_E4,4, NOTE_G4,4, NOTE_E4,4,  
};

// sizeof gives the number of bytes, each int value is composed of two bytes (16 bits)
// there are two values per note (pitch and duration), so for each note there are four bytes
int buyNotes = sizeof(buyMelody) / sizeof(buyMelody[0]) / 2;
int sellNotes = sizeof(sellMelody) / sizeof(sellMelody[0]) / 2;
int holdNotes = sizeof(holdMelody) / sizeof(holdMelody[0]) / 2;

// this calculates the duration of a whole note in ms
int wholenote = (60000 * 4) / tempo;

int divider = 0, noteDuration = 0;

void setup() {
	//initialize software serial monitor and give a delay for lcd to load
	lcd.begin();
  lcd.backlight();
	delay(500);

  //Initialize the gyroscope
  IMU.begin();

	//setting pin modes
	// pinMode(rfidScannerPin, INPUT);
	pinMode(buyItPin, INPUT);       //Button controls this pin
	pinMode(sellItPin, INPUT);      //Sound detector controls this pin
  pinMode(bypass, INPUT);         //Button to start game
  pinMode(greenPin, OUTPUT);
  pinMode(redPin, OUTPUT);
  pinMode(buzzer, OUTPUT);

	char scoreString[10];
}

void actionDetector(int action, int actionPin, String actionText, bool digital) {
	//if action selected is one, then tell user to "Buy It"
	// lcd.clear();
  // lcd.print(actionText);

  float x, y, z;
  //reset LED States
  digitalWrite(greenPin, LOW); // Green LED
  digitalWrite(redPin, LOW); // Green LED


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
      delay(500);
		}

		//recalculate timeElapsed
		timeElapsed = millis() - timeOfPrompt;
	}

	//once while loop stops (either due to time running out or completing action)
	//check to see if action was comleted and adjust settings accordingly
	if (actionCompleted == 1 && score == 99){
    lcd.clear();
    lcd.print("Game Won!");
    delay(750);
    finalScore = score;
    gameRate = 10000;
    score = 0; //reset score to 0 for new game
		char scoreString[10];
		sprintf(scoreString,"%4d",finalScore);
		keepPlaying = 0;
    lcd.clear();
		lcd.print("GAME OVER!");
    lcd.clear();
		lcd.write("Score");
    lcd.clear();
		lcd.print(scoreString);

		delay(2000);
  }
  else if (actionCompleted == 1){
    lcd.clear();
		lcd.print("action completed");
    digitalWrite(greenPin, HIGH); // Green LED
		// tone(speakerPin, 10000, 250); // Adust to a better win sound
		delay(250);
		// noTone(speakerPin);
	}
	else if (actionCompleted == 0){
    lcd.clear();
		lcd.print("action failed");
    digitalWrite(redPin, HIGH); // Red LED
		delay(750);
    // tone(speakerPin, 10000, 250); // Adust to a better Lose sound
    delay(250);
    // noTone(speakerPin);
		finalScore = score;
    gameRate = 10000;
    score = 0; //reset score to 0 for new game
		char scoreString[10];
		sprintf(scoreString,"%4d",finalScore);
		keepPlaying = 0;

    lcd.clear();
		lcd.print("GAME OVER!");
    lcd.clear();
		lcd.print("Final Score: ");
    // lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print(scoreString);

		delay(2000);
    digitalWrite(redPin, LOW);                        
	}
}

void loop() {
	// display game start code on lcd
  lcd.clear();
  lcd.print("   Press  the"); 
  lcd.setCursor(2, 1);
  lcd.print("Start Button!");
  delay(500);

	//If button is pressed, go into game.
  bypassTrue = digitalRead(bypass);
	if (bypassTrue){

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

			//if else statements to prompt user and check action
		    switch(action) {
				case 1:
            // iterate over the notes of the melody.
            // Remember, the array is twice the number of notes (notes + durations)
            for (int thisNote = 0; thisNote < buyNotes * 2; thisNote = thisNote + 2) {

              // calculates the duration of each note
              divider = buyMelody[thisNote + 1];
              if (divider > 0) {
                // regular note, just proceed
                noteDuration = (wholenote) / divider;
              } else if (divider < 0) {
                // dotted notes are represented with negative durations!!
                noteDuration = (wholenote) / abs(divider);
                noteDuration *= 1.5; // increases the duration in half for dotted notes
              }

              // we only play the note for 90% of the duration, leaving 10% as a pause
              tone(buzzer, buyMelody[thisNote], noteDuration * 0.9);

              // Wait for the specief duration before playing the next note.
              delay(noteDuration);

              // stop the waveform generation before the next note.
              noTone(buzzer);
            }
					actionDetector(action, buyItPin, "Buy It!", 1);
					break;
				case 2:
            // iterate over the notes of the melody.
            // Remember, the array is twice the number of notes (notes + durations)
            for (int thisNote = 0; thisNote < sellNotes * 2; thisNote = thisNote + 2) {

              // calculates the duration of each note
              divider = sellMelody[thisNote + 1];
              if (divider > 0) {
                // regular note, just proceed
                noteDuration = (wholenote) / divider;
              } else if (divider < 0) {
                // dotted notes are represented with negative durations!!
                noteDuration = (wholenote) / abs(divider);
                noteDuration *= 1.5; // increases the duration in half for dotted notes
              }

              // we only play the note for 90% of the duration, leaving 10% as a pause
              tone(buzzer, sellMelody[thisNote], noteDuration * 0.9);

              // Wait for the specief duration before playing the next note.
              delay(noteDuration);

              // stop the waveform generation before the next note.
              noTone(buzzer);
            }
					actionDetector(action, sellItPin, "Sell It!", 1);
					break;
				case 3:
            // iterate over the notes of the melody.
            // Remember, the array is twice the number of notes (notes + durations)
            for (int thisNote = 0; thisNote < holdNotes * 2; thisNote = thisNote + 2) {

              // calculates the duration of each note
              divider = holdMelody[thisNote + 1];
              if (divider > 0) {
                // regular note, just proceed
                noteDuration = (wholenote) / divider;
              } else if (divider < 0) {
                // dotted notes are represented with negative durations!!
                noteDuration = (wholenote) / abs(divider);
                noteDuration *= 1.5; // increases the duration in half for dotted notes
              }

              // we only play the note for 90% of the duration, leaving 10% as a pause
              tone(buzzer, holdMelody[thisNote], noteDuration * 0.9);

              // Wait for the specief duration before playing the next note.
              delay(noteDuration);

              // stop the waveform generation before the next note.
              noTone(buzzer);
            }
					actionDetector(action, holdItPin, "Hold It!", 0);
					break;
        }
		}
	}
}
