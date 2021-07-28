#include <LiquidCrystal.h>

#define echoPin 2
#define trigPin 3
#define fsrpin A0
#define tilt 4
#define motor 6
#define backLight 13
#define safe A1
#define close A3
#define buzzer A4

int fsrreading; //weight
long duration;
int distance; //water level
int tiltreading; //1 if no tilt, 0 otherwise
int gateclosed = 0;

LiquidCrystal lcd(12, 11, 10, 9, 8, 7, 5);


void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(tilt, INPUT);
  pinMode(fsrpin, INPUT);
  pinMode(motor,OUTPUT);
  pinMode(backLight, OUTPUT);
  digitalWrite(backLight, HIGH); 
  lcd.begin(16,2);             
  lcd.clear();
  
  lcd.setCursor(1,0);          
  lcd.print("BRIDGE SAFETY!"); 
  Serial.begin(9600); 
  delay(1000);

}
void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(2);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  
  fsrreading = analogRead(fsrpin);

  
  tiltreading = digitalRead(tilt);

  
  if(gateclosed == 0 && (distance < 150 || fsrreading > 300 || tiltreading ==0)){
    analogWrite(safe,0);
    analogWrite(close,255);
    lcd.clear();
    lcd.setCursor(0,0);          
 	lcd.print("EVACUATE BRIDGE!");
    lcd.setCursor(0,1); 
    tone(buzzer,100);
    digitalWrite(motor,HIGH);
    delay(4000);
    digitalWrite(motor,LOW);
    gateclosed = 1;
    noTone(buzzer);
    
  }
  

  if(gateclosed == 1 && (distance > 150 && fsrreading < 300 && tiltreading ==1)){
    tone(buzzer,100);
    digitalWrite(motor,HIGH);
    delay(4000);
    digitalWrite(motor,LOW);
    noTone(buzzer);
    gateclosed = 0;
    lcd.clear();
    lcd.setCursor(3,0);          
 	lcd.print("SAFE TO USE");
    analogWrite(safe,255);
    analogWrite(close,0);
  	lcd.display();
  }
  delay(1000);
  Serial.println(distance);

}