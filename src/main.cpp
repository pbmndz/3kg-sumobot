#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define RMOTOR1 9
#define RMOTOR2 10

#define LMOTOR1 6
#define LMOTOR2 5

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C // Use 0x3D if 0x3C doesn't work
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//flag
bool buttonClicked = false; // Initialize the flag
bool startSumo = false; 

void forward(){              
  digitalWrite (RMOTOR1, LOW);
  digitalWrite (RMOTOR2, HIGH);
  digitalWrite (LMOTOR1, HIGH);
  digitalWrite (LMOTOR2, LOW);
  Serial.println ("forward");
}
void backwards(){              
  digitalWrite (RMOTOR1, LOW);
  digitalWrite (RMOTOR2, HIGH);
  digitalWrite (LMOTOR1, HIGH);
  digitalWrite (LMOTOR2, LOW);
  Serial.println ("forward");
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
  digitalWrite (LMOTOR1, HIGH);
  digitalWrite (LMOTOR2, LOW);
  Serial.println ("left");
}
void motor_stop(){
  digitalWrite (RMOTOR1, LOW);
  digitalWrite (RMOTOR2, LOW);
  digitalWrite (LMOTOR1, LOW);
  digitalWrite (LMOTOR2, LOW);
  Serial.println ("STOP");
}


void setup() {

  Serial.begin(9600);

  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); 
  }
  display.clearDisplay();
  display.setTextSize(1);      
  display.setTextColor(SSD1306_WHITE); 
  display.setCursor(0, 0);     
  display.display();
  
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(7, INPUT);
  pinMode(8, INPUT);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A6, INPUT);
  pinMode(A7, INPUT);
}


void loop() {
 
  int Strategies = analogRead(A0);  //STRAT
  int irStart = digitalRead(2);     //irStart
  int buttonStart = digitalRead(3);     //irStart

  int lineLeft = digitalRead(11);     //irStart
  int lineRight = digitalRead(13);     //irStart
  //sensors
  int irFront = digitalRead(4); //front
  int irSensorLeft = analogRead(A1); 
  int irSensorRight = analogRead(A2); 
  int irSensorBack = analogRead(A3);
  int irCornerRight = analogRead(A6);
  int irCornerLeft = analogRead(A7);
  display.clearDisplay();
  


if (buttonStart == HIGH) {
  if (!buttonClicked) {
    buttonClicked = true; // Set the flag to true when the button is clicked
    startSumo = !startSumo; // Toggle the startSumo variable
  } else {
    buttonClicked = false; // Reset the flag when the button is clicked again
  }
  delay(100); // Add a small delay to avoid rapid toggling
}

if (startSumo or irStart == 1) {
  display.setCursor(0, 10);
  display.print(F("Microstart: "));
  display.println("start!!");
} else {
  display.setCursor(0, 10);
  display.print(F("Microstart: "));
  display.println("stop");
}

  pinMode(3, INPUT);
  pinMode(4, INPUT);
  
  display.setCursor(0, 20);
  display.print(F("F: "));
  display.print(irFront);
  display.setCursor(40, 20);
  display.print(F("CL: "));
  display.print(irCornerLeft);
  display.setCursor(80, 20);
  display.print(F("CR: "));
  display.print(irCornerRight);
  
  display.setCursor(0, 30);
  display.print(F("L: "));
  display.println(irSensorLeft);
  display.setCursor(40, 30);
  display.print(F("R: "));
  display.print(irSensorRight);
  display.setCursor(80, 30);
  display.print(F("B: "));
  display.print(irSensorBack);
  
  display.setCursor(0, 40);
  display.print(F("line sensors:"));
  display.setCursor(0, 50);
  display.print(F("LFR: "));
  display.print(lineRight);
  display.setCursor(50, 50);
  display.print(F("LFL: "));
  display.print(lineLeft);


  if (Strategies >= 360 && Strategies <= 366) {
      display.setCursor(0, 0);
      display.print(F("Strategy 1"));
      right();
  } else if (Strategies >= 380 && Strategies <= 388) {
      display.setCursor(0, 0);
      display.print(F("Strategy 2"));
  } else if (Strategies >= 390 && Strategies <= 402) {
      display.setCursor(0, 0);
      display.print(F("Strategy 3"));
  } else if (Strategies >= 407 && Strategies <= 420) {
      display.setCursor(0, 0);
      display.print(F("Strategy 4"));
  } else {
      display.setCursor(0, 0);
      display.print(F("pick a strategy"));
      motor_stop();
  }
  display.display();
} 