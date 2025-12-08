# LED Trap
LED Trap is an LED "cat and mouse" game. The player or "cat" (blue LED) must catch the "mouse" (red LED). The player's LED is controlled by the joystick while the red LED is randomly generated within the 8x8 grid of RGB LEDs. The player will have 20 seconds to complete each level. After each successful level, the speed of the red LED generation will increase, making it more difficult to catch. If the player fails to capture the red LED, the game is over. 

# Components
* Arduino Uno
* WS2818 RGB LEDs (64)
* Joystick Module
* Wires
* Arduino USB Cable

# How It Works
The website will display a "Connect to Arduino" button, the level, timer, and a "Start Game button. For every level, the player will have 20 seconds to "catch" the red LED. As the levels increase, the red LED will randomly generate at faster speeds.

* To begin, the player connects the Arduino to website
* The player will press "Start Game" to begin the game
* A randomly generated red LED will appear
* The player uses the joystick to move around (as the blue LED), trying to catch the red LED
* When the player's LED is in the same position as the red LED, the LED will turn yellow, signaling the player to catch the red LED
* To catch the red LED, the player must press the joystick button
* If the player successfully catches the red LED, it will turn green and the next level will begin
* If the player fails to catch the red LED within time limit, the game is over