// Libraries for the LCD Display and 4x4 Keypad
#include <LiquidCrystal.h>
#include <Keypad.h>

// Keypad Dimensions
const int Row_Num = 4;
const int Column_Num = 4;

//Pomodoro session and break times (minutes)
int Session_Time;
int Break_Time;

//Variables used for user manual time entries
int Manual_Input = 0;
int Max_Manual = 0;

//Buzzer pin
int buzzerPin = 3;

//Countdown timer variables
int Remaining_Minutes;
int Remaining_Seconds;

//Used in comparison to current millis to determine the time between checks
//Allows program to avoid being halted during countdown such as with delay()
unsigned long Last_Executed_Millis = 0;

//Switches between entering session time and break time for manual entires
bool Entering_Session = true;

//Keypad
char key[Row_Num][Column_Num] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

//Defines arduino pins for keypad rows and columns
byte Pin_Row[Row_Num] = {53, 51, 49, 47};
byte Pin_Column[Column_Num] = {37, 35, 33, 31};

Keypad keypad = Keypad( makeKeymap(key), Pin_Row, Pin_Column, Row_Num, Column_Num);


//LCD

//LCD pins: RS, Enable, D4, D5, D6, D7
const int rs = 2, en = 4, d4 = 8, d5 = 9, d6 = 10, d7 = 11;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);



//States

//Each state represents a different stage of the pomodoro program 
enum Pomo_State
{
  Default_State, //Menu
  Preset_State, //Select from preset times
  Manual_State, //User inputs custom times
  Break_State, //Break countdown
  Session_State //Session countdown
};

//Starts program in Main menu
Pomo_State Current_State = Default_State;



//Code

void setup()
{
  lcd.begin(16,2);
  pinMode(buzzerPin, OUTPUT);
  UpdateDisplay();
}

void loop()
{
  Pomo_State Previous_State = Current_State;
  HandleInput(Current_State);

  if(Current_State != Previous_State){
    UpdateDisplay();
  }
  UpdateTimer();
}


//Handles updating text that is displayed on the LCD depending on the state
void UpdateDisplay()
{

  lcd.clear();

  switch(Current_State)
  {
    case Default_State:
      lcd.setCursor(0,0);
      lcd.print("Pomodoro");
    
      for(int i = 0; i <= 8; i++)
      {
        delay(600);
        lcd.scrollDisplayLeft();
      }
      lcd.clear();

      lcd.print("*: Presets");
      lcd.setCursor(0,1);
      lcd.print("#: Manual Input");
      break;
    
    case Preset_State:
      lcd.clear();  
      lcd.print("A: 25/5  B: 50/10");
      lcd.setCursor(0,1);
      lcd.print("C: 45/15 D: 55/5");
      break;

    case Manual_State:
      if(Entering_Session)
      {
        lcd.print("Session Time:");
      }
      else
      {
        lcd.print("Break Time:");
      }
      break;
    
    case Session_State:
      lcd.print("Study");
      lcd.setCursor(0,1);
      lcd.print(Remaining_Minutes);
      lcd.print(":");
      lcd.print(Remaining_Seconds);
      break;
    
    case Break_State:
      lcd.print("Break");
      lcd.setCursor(0,1);
      lcd.print(Remaining_Minutes);
      lcd.print(":");
      if(Remaining_Seconds < 10){
        lcd.print('0');
      }
      lcd.print(Remaining_Seconds);

  }
}

//Counts down the timer and switches between Session/Break.
void UpdateTimer()
{
  if(Current_State != Session_State && Current_State != Break_State){
    return;
  }

  unsigned long Current_Millis = millis();

  //Checks the amount of time passed
  if(Current_Millis - Last_Executed_Millis >= 1000){
    Last_Executed_Millis = Current_Millis;

    if(Remaining_Seconds > 0){
      Remaining_Seconds--;
      UpdateDisplay();
    }
    else if(Remaining_Minutes > 0){
      Remaining_Minutes--;
      Remaining_Seconds = 59;
      UpdateDisplay();
    }
    else
    {
      digitalWrite(buzzerPin, HIGH);
      delay(2000);
      digitalWrite(buzzerPin, LOW);
      if(Current_State == Session_State){
        Current_State = Break_State;
        Remaining_Minutes = Break_Time;
        Remaining_Seconds = 0;
      }         
      else if(Current_State == Break_State){
        Current_State = Session_State;
        Remaining_Minutes = Session_Time;
        Remaining_Seconds = 0;
      }
      UpdateDisplay();
    }

  }
}

//Handles the keypad inputs. 
void HandleInput(Pomo_State &Current_State)
{
  char Key_Input = keypad.getKey();
  if(Key_Input == NO_KEY){
    return;
  }

  switch(Current_State)
  {
    case(Default_State):
      
      if(Key_Input == '*')
      {
        Current_State = Preset_State;
      }
      else if(Key_Input == '#')
      {
        Current_State = Manual_State; 
      }
      break;

    case Preset_State:

      switch(Key_Input)
      {
        case 'A':
          Session_Time = 25;
          Break_Time = 5;
          
          Remaining_Minutes = Session_Time;
          Remaining_Seconds = 0;

          Current_State = Session_State;

          break;

        case 'B':
          Session_Time = 50;
          Break_Time = 10;

          Remaining_Minutes = Session_Time;
          Remaining_Seconds = 0;

          Current_State = Session_State;
          break;

        case 'C':
          Session_Time = 45;
          Break_Time = 15;
          Current_State = Session_State;

          Remaining_Minutes = Session_Time;
          Remaining_Seconds = 0;
          break;

        case 'D':
          Session_Time = 55;
          Break_Time = 5;

          Remaining_Minutes = Session_Time;
          Remaining_Seconds = 0;
          Current_State = Session_State;
          break;
        
      }
    break;

    case Manual_State:
      if(Key_Input >= '0' && Key_Input <= '9' && Max_Manual < 4)
      {
        //Maths allows for appending number to integer, removing the zero at the end
        Manual_Input = Manual_Input * 10 + (Key_Input - '0');
        lcd.setCursor(0,1);
        lcd.print(Manual_Input);
        Max_Manual += 1;
      }
      
      //'#' Checks if a value has been inputed and stores it
      else if(Key_Input == '#')
      {
        if(Manual_Input > 0)
        {
          if(Entering_Session)
          {
            Session_Time = Manual_Input;
            Manual_Input = 0;
            Max_Manual = 0;
            Entering_Session = false;
            Remaining_Minutes = Session_Time;
            Remaining_Seconds = 0;

            UpdateDisplay();
          }
          else
          {
            Break_Time = Manual_Input;
            Current_State = Session_State;
            Remaining_Minutes = Session_Time;
            Remaining_Seconds = 0;
            Max_Manual = 0;
               
            UpdateDisplay();
          }
        }
      }

      //Allows user to remove last number inputed, stops removing when no numbers are left
      else if(Key_Input == '*'){

        Manual_Input = Manual_Input / 10;
        lcd.setCursor(0,1);
        lcd.print("                ");
        lcd.setCursor(0,1);
        lcd.print(Manual_Input);
        if(Max_Manual > 0){
          Max_Manual -= 1;
        }
      }


      break;

  
  }
  return;
  
}

