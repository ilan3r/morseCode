/*
Ilan Rajpar 
April 2nd 2024

Turning button presses and holds into morse code, which is then turned into 
characters displayed on the screen. Button debouncing is employed, and a long press 
of the button resembles a backspace. 

At this font size on the screen, the maximum number of characters that can be printed is 15; 

The hardware used for this is Arduino Nano, Adafruit SS13-6 128x64 screen, 
pushbutton, resistors, breadboard, and cables. 


*/ 
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);


const int enterTimeLow = 500; 
const int dotsVsDashTime = 200;
long unsigned currentTime = 0; 
const int deleteTime = 1000; 

String morseCode = ""; 
String plainText = ""; 

// const int LEDpin = 3;
const int buttonPin = 2;  

// the time allowed for button debouncing 
const int debounceDelay = 5; 

unsigned long lastDetection; 

// the current and previous state of the button
int buttonState = 0;     
int lastButtonState = 0; 
bool pressed = 0; 

// the moment the button was pressed and released
unsigned long startPressed = 0;    
unsigned long endPressed = 0;      

// the time the button has held for and been released for 
long unsigned holdTime = 0;  
long unsigned idleTime = 0; 

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.display();
  pinMode(buttonPin, INPUT); 
  // pinMode(LEDpin, OUTPUT); 
  // digitalWrite(LEDpin, LOW);
  for (int i = 0; i < 5; i++){
    Serial.println("");
  }

}

void loop() {
  int reading = digitalRead(buttonPin); 
  currentTime = millis(); 

  // only accept the change in button state after debounce time
  if (reading != lastButtonState){
    lastDetection = currentTime;
  }
  if ( (currentTime - lastDetection) > debounceDelay ){
    buttonState = reading; 
  }

  if (buttonState != lastButtonState){
    // button pressed
    if (buttonState == 0){
      // digitalWrite(LEDpin, HIGH); 
      startPressed = currentTime; 
      idleTime = millis() - endPressed; 

      if (idleTime > enterTimeLow && pressed == 0){
        if (morseCode != "")
        {
          plainText += morseToChar(morseCode); 
          display.print(morseToChar(morseCode));
          display.display();
          morseCode = "";
        }
        pressed = 1; 
      } 
      pressed = 1; 
    }

    // if the button is released 
    else if (buttonState == 1){
      // digitalWrite(LEDpin, LOW); 
      endPressed = currentTime; 
      holdTime = millis() - startPressed;  

      
      if (holdTime > dotsVsDashTime && holdTime < deleteTime && pressed == 1){
        morseCode += "-";
        pressed = 0; 
      }
      else if (holdTime < dotsVsDashTime && pressed == 1){
        morseCode += ".";
        pressed = 0; 
      }

      else if (holdTime > deleteTime && pressed == 1){
        int endIndex = plainText.length() - 1;
        if (endIndex >= 0){
          plainText.remove(endIndex); 
          pressed = 0;
          display.clearDisplay(); 
          display.setCursor(0,0); 
          display.print(plainText); 
          display.display();  

        }
      }
    }
  }
  lastButtonState = reading; 
}



// morse code function
char morseToChar(String message){
  message.trim();
  if (message == ".-") return 'A';
  else if (message == "-...") return 'B';
  else if (message == "-.-.") return 'C';
  else if (message == "-..") return 'D';
  else if (message == ".") return 'E';
  else if (message == "..-.") return 'F';
  else if (message == "--.") return 'G';
  else if (message == "....") return 'H';
  else if (message == "..") return 'I';
  else if (message == ".---") return 'J';
  else if (message == "-.-") return 'K';
  else if (message == ".-..") return 'L';
  else if (message == "--") return 'M';
  else if (message == "-.") return 'N';
  else if (message == "---") return 'O';
  else if (message == ".--.") return 'P';
  else if (message == "--.-") return 'Q';
  else if (message == ".-.") return 'R';
  else if (message == "...") return 'S';
  else if (message == "-") return 'T';
  else if (message == "..-") return 'U';
  else if (message == "...-") return 'V';
  else if (message == ".--") return 'W';
  else if (message == "-..-") return 'X';
  else if (message == "-.--") return 'Y';
  else if (message == "--..") return 'Z';
  else if (message == ".....") return '_';
  else return '#';

}