# Stock Market Bopit

## Project Overview

The Stock Market Bopit is a game that allows players to simulate trading stocks by using physical actions to make decisions. The game has three different actions that players can take: hitting a "Buy" button to buy a stock, yelling into the microphone to sell a stock, and shaking the device to hold onto a stock.

The game was created by my group for a Junior Design project as part of my ECE Courses at the University of Pittsburgh. It is modeled to look like a toy cash register, but with more functionality.

## Technologies Used

- Arduino Uno
- Microphone
- Buttons
- Gyroscope
- LEDs

## How to Play

1. Flip the switch on the rear of the unit to turn the machine on.
2. Press the start button to begin the game.
2. The game will display a required action such as Buy, Sell or Hold.
3. To buy the stock, hit the "Buy" button.
4. To sell the stock, scream "Sell" into the microphone.
5. To hold onto the stock, shake the Bopit. 
6. Each time a player successfully completes an action, they will recieve one point, and the game time per action will decrease to make it faster and more difficult.
6. The game will continue until the player reaches a score of 99 or fails to complete an action in the required amount of time.
7. The player's final score will be based on the total number of successful bopits.

## Files in the Repo

There are a few files in this repository that are noteable. First is the code. It has the file extension `.ino` which is meant to be loaded into an Arduino. The code contains the game loop, sounds, and over functionality of the Bop It. Next is the enclosure design. This has the file extension `.svg` which is meant to be loaded into a laser cutter or another milling machine. We used laser cut arylic for our design as you can see in the photos below. Lastly are the Altium files. These contain gerber files that are meant to be sent to PCB manufacturer for milling. You could use these, along with the included schematics in order to recreate our Bop It.

## Photos

![IMG_0332](https://user-images.githubusercontent.com/84938803/227387752-dab8af04-1969-4115-8a43-e3d4ce1f8fee.jpg)
![IMG_0333](https://user-images.githubusercontent.com/84938803/227387756-0bec212a-b4f8-4248-ba62-880725726c80.jpg)
![IMG_0334](https://user-images.githubusercontent.com/84938803/227387758-cd4d3add-f4da-46e2-89a1-70abab7909e6.jpg)
![IMG_0335](https://user-images.githubusercontent.com/84938803/227387760-4ea66dc1-79f9-46b9-bfc0-d586f3dd409a.jpg)
![IMG_0336](https://user-images.githubusercontent.com/84938803/227387761-63d0ba67-eacb-4e3a-856c-ee60ad71a1e7.jpg)
![IMG_0331](https://user-images.githubusercontent.com/84938803/227387762-42d7e02f-0ec5-4a61-89c0-94ea753f6daa.jpg)

## Conclusion

The Stock Market Bopit was a fun and interactive way to learn about Arduino's and how to interface with them. My group and I had a fun time making this project, and it taught us many valuable lessons about circuit design, programming, and most of all, teamwork!
