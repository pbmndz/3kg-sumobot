#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//motor
#define RMOTOR1 9
#define RMOTOR2 10

#define LMOTOR1 6
#define LMOTOR2 5

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C // Use 0x3D if 0x3C doesn't work
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//flag
bool buttonClicked = false; // Initialize the flag
bool startSumo = false; 
bool zigzagR = false;
bool start = false;

void attack(){         
  digitalWrite (RMOTOR1, HIGH);
  digitalWrite (RMOTOR2, LOW);
  digitalWrite (LMOTOR1, HIGH);
  digitalWrite (LMOTOR2, LOW);
  Serial.println ("ATTACK");
}
void forward(){         
  analogWrite (RMOTOR1, 125);
  digitalWrite (RMOTOR2, LOW);
  analogWrite (LMOTOR1, 125);
  digitalWrite (LMOTOR2, LOW);
  Serial.println ("Forward");
}
void backwards(){              
  digitalWrite (RMOTOR1, LOW);
  digitalWrite (RMOTOR2, HIGH);
  digitalWrite (LMOTOR1, LOW);
  digitalWrite (LMOTOR2, HIGH);
  Serial.println ("Backward");
}
void right(){              
  digitalWrite (RMOTOR1, LOW);
  digitalWrite (RMOTOR2, HIGH);
  digitalWrite (LMOTOR1, HIGH);
  digitalWrite (LMOTOR2, LOW);
  Serial.println ("right");
} 
void left(){              
  digitalWrite (RMOTOR1, HIGH);
  digitalWrite (RMOTOR2, LOW);
  digitalWrite (LMOTOR1, LOW);
  digitalWrite (LMOTOR2, HIGH);
  Serial.println ("left");
}
void stop(){
  digitalWrite (RMOTOR1, LOW);
  digitalWrite (RMOTOR2, LOW);
  digitalWrite (LMOTOR1, LOW);
  digitalWrite (LMOTOR2, LOW);
  Serial.println ("STOP");
}


void setup() {

  Serial.begin(9600);

// Display
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); 
  }
  display.setRotation(2);
  display.clearDisplay();
  display.setTextSize(1);      
  display.setTextColor(SSD1306_WHITE);   
  
// SENSORS
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(7, INPUT);
  pinMode(8, INPUT);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A6, INPUT);
  pinMode(A7, INPUT);

// MOTOR
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);

}


void loop() {
  display.clearDisplay();

  int Strategies = analogRead(A0); 
  int irStart = digitalRead(2);     
  int buttonStart = digitalRead(3); 
  int lineLeftBack = digitalRead(7);  
  int lineRightBack = digitalRead(12); 
  int lineLeft = digitalRead(11);    
  int lineRight = digitalRead(13);    
  // Sensors
  int irFront = digitalRead(4);       
  int irSensorLeft = analogRead(A3);
  int irSensorRight = analogRead(A1);
  int irSensorBack = analogRead(A2);
  int irCornerRight = analogRead(A6);
  int irCornerLeft = analogRead(A7);

if (buttonStart == HIGH) {
  if (!buttonClicked) {
    buttonClicked = true; 
    startSumo = !startSumo; 
  } else {
    buttonClicked = false; 
  }
  delay(100);
}

if (startSumo or irStart == 1) {
  display.setCursor(0, 10);
  display.print(F("Microstart: "));
  display.println("start!!");
  start = true;
} else {
  display.setCursor(0, 10);
  display.print(F("Microstart: "));
  display.println("stop");
  start = false;
}
  display.setCursor(100, 0);
  display.print(Strategies);
  // front sensors
  display.setCursor(0, 20);
  display.print(F("F: ")); display.print(irFront);
  display.setCursor(40, 20);
  display.print(F("CL: ")); display.print(irCornerLeft);
  display.setCursor(80, 20);
  display.print(F("CR: ")); display.print(irCornerRight);
  // side and back sensors
  display.setCursor(0, 30);
  display.print(F("L: ")); display.println(irSensorLeft);
  display.setCursor(40, 30);
  display.print(F("R: ")); display.print(irSensorRight);
  display.setCursor(80, 30);
  display.print(F("B: ")); display.print(irSensorBack);
  // Line sensor
  display.setCursor(0, 50);
  display.print(F("LFR: ")); display.print(lineRight);
  display.setCursor(50, 50);
  display.print(F("LFL: ")); display.print(lineLeft);
  display.setCursor(0, 40);
  display.print(F("blr:")); display.print(lineRightBack);
  display.setCursor(50, 40);
  display.print(F("bll:")); display.print(lineLeftBack);

  if (Strategies >= 360 && Strategies <= 379) {
      display.setCursor(0, 0);
      display.print(F("Strategy 1"));
      if(start){
        if (!zigzagR){
          right();
          delay(125);
          attack();
          delay(200);
          left();
          delay(125);
          attack();
          delay(200);
          zigzagR = true;
        } else if (lineRight or lineLeft){
          backwards();
          delay(250);
          right();
          delay(250);
        }else if (irFront){
          attack();
        }else if (irCornerLeft > 75 or irSensorLeft > 75){
          left();
        }else if (irCornerRight > 75 or irSensorRight > 75){
          right();
        }else if(irSensorBack > 75){
          right();
          delay(500);
        }else{
          forward();
        }  
      }else if(!irStart){
        stop();
        start = false;
      }
  } else if (Strategies >= 390 && Strategies <= 399) {
      display.setCursor(0, 0);
      display.print(F("Strategy 2"));
  } else if (Strategies >= 405 && Strategies <= 418) {
      display.setCursor(0, 0);
      display.print(F("Strategy 3"));
  } else if (Strategies >= 420 && Strategies <= 430) {
      display.setCursor(0, 0);
      display.print(F("Strategy 4"));
  } else {
      display.setCursor(0, 0);
      display.print(F("pick a strategy"));
      stop();
  }
  display.display();
} 