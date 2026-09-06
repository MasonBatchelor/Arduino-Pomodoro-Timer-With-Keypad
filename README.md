# Arduino-Pomodoro-Timer-With-Keypad

<img width="2783" height="2630" alt="IMG_0407" src="https://github.com/user-attachments/assets/865aef67-09ce-42b1-8d86-3986f2349b4e" />

An Arduino Pomodoro Timer project that allows for preset and custom time inputs.
Built with a 16x2 LCD display, Active Buzzer and a 4x4 Keypad.

This is my first electronics project. I have built it to improve my understanding of C++, Arduino and working with electrical components.

---

## Features
- Buzzer notification to signal countdown has finished 
- Automatic switching between study and break sessions 
- Countdown timer                                      
- Manual input for study/break session times           
- Preset times to select from                          

---

## Hardware
- Breadboard
- 16x2 LCD display
- 4x4 Keypad
- Active Buzzer
- Potentiometer
- Jumper Wires
- Arduino Mega

---

## Controls
| KEY    | Explanation                                     |
|--------|-------------------------------------------------|
| 'ABCD' | Selects from the given times in the preset menu |
| '*'    | Select Preset option in menu / Backspace        |
| '#'    | Select Manual input option in menu / Enter      |
| '0-9'  | Enters numbers during manual input              |

---

## How the Pomodoro Timer works

### State machine
The Pomodoro timer uses enums to define the different states in the program. The variable `Current_State` is used to keep track of the state the program is currently running in. This allows for separate blocks of code to be run depending on the state of the program, rather than needing to run through multiple if statements to determine what the program should do next.

| State         | Explanation                                                                           |
|---------------|---------------------------------------------------------------------------------------|
| Default_State | Menu used to determine whether the user wants to input a time or choose from a preset |
| Preset_State  | Lists the preset times the user can choose from                                       |
| Manual_State  | Allows entry of a study session and break session time                                |
| Break_State   | Counts down from the set break time to zero                                           |
| Session_State | Counts down from the set study time to zero                                           |

---

### Countdown timer
The `UpdateTimer()` function handles counting down the timer by making use of `millis()` instead of delay(). This is because delay() would result in the program needing to stop everything whilst waiting for the countdown. This would prevent the keypad being checked for inputs and, would result in a much more sluggish user experience. 

`millis()` in this case works far better because it allows for the program to keep running whilst still being able to check how much time has passed. Every loop compares the current `millis()` value against what is stored in `Last_Executed_Millis`. If the difference is greater than 1000 milliseconds, the timer is updated and `Last_Executed_Millis` is set to the current time.

<img width="4030" height="3022" alt="IMG_0403" src="https://github.com/user-attachments/assets/bb1a2461-3381-4380-8cc0-47eb9f62faac" />

---

### Hardware Pinout

16x2 LCD
| LCD PIN | Name | Connects to:                        |
|---------|-----------|--------------------------------|
| 1       | VSS       | GND                            |
| 2       | VCC       | 5V                             |
| 3       | Vo        | Potentiometer (Middle Pin)     | 
| 4       | Rs        | Arduino Digital Pin 2          |
| 5       | R/W       | GND                            |
| 6       | E         | Arduino Digital Pin 4          |
| 7       | D0        | N/A                            |
| 8       | D1        | N/A                            |
| 9       | D2        | N/A                            |
| 10      | D3        | N/A                            |
| 11      | D4        | Arduino Digital Pin 8          |
| 12      | D5        | Arduino Digital Pin 9          |
| 13      | D6        | Arduino Digital Pin 10         |
| 14      | D7        | Arduino Digital Pin 11         |
| 15      | A (LED +) | 5V                             |
| 16      | K (LED -) | GND                            |


4x4 Keypad
| Keypad PIN | Connects to:                       |
|------------|------------------------------------|
| 1          | Arduino Digital Pin 53             |
| 2          | Arduino Digital Pin 51             |
| 3          | Arduino Digital Pin 49             |
| 4          | Arduino Digital Pin 47             |
| 5          | Arduino Digital Pin 37             |
| 6          | Arduino Digital Pin 35             |
| 7          | Arduino Digital Pin 33             |
| 8          | Arduino Digital Pin 31             |


Active Buzzer
|Buzzer Terminal | Connects to:                   |
|----------------|--------------------------------|
| +              | Arduino Digital Pin 3          |
| -              | GND                            |

---

## What I have learned
With this being my first electronics project, building a Pomodoro timer has improved my understanding of electronics and programming.

### Electronics
The decision to work with the LCD display, 4x4 keypad, buzzer and potentiometer has taught me how individual components connect to a breadboard and Arduino. For example, I have learnt that the 16x2 LCD can operate in a 4-bit mode. This means only data pins D4-D7 need to be connected to the Arduino without affecting the display. I've also learnt that a keypad detects inputs by using rows and columns with each pin representing one of them.

### C++ and Arduino
This project has made me far more comfortable with C++ and Arduino. I've learnt how to work with enums, libraries and using `millis()`. Using `millis()` rather than relying on `delay()` for the countdown has allowed me to run the timer without holding up the entire program. Using enums has particularly helped me by letting me break the program down into smaller portions rather than trying to tackle the whole project at once, something that I realized was not going to work early on.

---

## Future Improvements
The timer is currently functional, however there are a few improvements I would like to make to continue developing my skills.

### Pause and Resume
Currently, there is no way to stop the timer once it has started. I would like to add the ability to pause and resume with one of the keypad inputs to make the timer more practical to use.

### 3D printed enclosure
At the moment the components are attached to a cutting mat. This works well for testing purposes and debugging, however for a final design I would like to model and print an enclosure to house the breadboard, LCD, keypad and Arduino securely. Doing this would both improve the aesthetics of the timer and make it much more portable.

### Custom PCB
Eventually I would like to move away from using a breadboard and instead design a custom PCB for the project. A problem currently with the project is that the jumper wires become loose from the breadboard. A PCB would solve this, making the design more reliable, take up less space, and give an overall more polished experience.
