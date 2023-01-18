//Original Creator ProfessorBoots

//Please feel free to download and make revisions as you see necessary or to add more functionality.
//Thank you!

#include <FastLED.h>
//FastLED by Daniel Garcia
#include <NewPing.h>
//NewPing by Tim Eckel
#include <OneButton.h>
//OneButton by Matthias Hertel
#include <TM1637.h>
//TM1637(Grove 4 digit display) by Seed Studio
#include <EEPROM.h>

//Variables
double raceStart = 0;
String currentTime = "";
bool racing = false;
int pingParameter = 29;
bool oneClick = false;
//Record time variables
int milliSec1 = 0;
int milliSec2 = 0;
int sec = 0;
int tenSec = 0;

//ULTRASONIC CODE1
#define triggerPin 8
#define echoPin 7
#define maxDistance 29
NewPing sonar(triggerPin, echoPin, maxDistance);

//ULTRASONIC CODE2
#define triggerPin2 12
#define echoPin2 11
NewPing sonar2(triggerPin2, echoPin2, maxDistance);

//LEDCODE
#define LED_PIN 13
#define NUM_LEDS 60
CRGB leds[NUM_LEDS];
#define BRIGHTNESS 255   /* Control the brightness of your leds */
#define SATURATION 255   /* Control the saturation of your leds */

//BUTTONCODE
OneButton resetButton(A1, true, true);

//DISPLAYCODE
#define CLK 2
#define DIO 3
TM1637 tm(CLK, DIO);

void setup() {
  Serial.begin(9600);
 //ULTRASONIC CODE
  
 //BUTTONCODE
  resetButton.attachClick(singleClick);
  resetButton.attachLongPressStop(longClick);
 


 //LEDCODE
 FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
 FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);
 FastLED.clear();
 FastLED.show();

 //DISPLAYCODE
 tm.init();
 tm.set(4);


 lightLogic(false);

}

void loop() {
  resetButton.tick();
  if(pingCheck())
  {
    if(!racing)
    {
    raceStart = millis();
    oneClick = false;
    racing = true;
    lightLogic(true);
    delay(500);
    }
    else if(racing)
    {
      racing = false;
     lightLogic(false);
      finishedRaceLed();
    }
  }
  if(racing)
  {
    currentTime = String((millis()-raceStart)/1000);
    displayTime(currentTime); 
  }

}
bool pingCheck(){
  unsigned long pingTime     = sonar.ping();
  unsigned long distanceInCm = sonar.convert_cm(pingTime);
 Serial.print("ping: ");
  Serial.println(distanceInCm);
//delay(20);
  unsigned long pingTime2 = sonar2.ping();
  unsigned long distanceInCm2 = sonar2.convert_cm(pingTime2); 
  Serial.print("ping2: ");
  Serial.println(distanceInCm2);

  if((distanceInCm <= pingParameter && pingTime > 0)||(distanceInCm2 <= pingParameter && pingTime2 > 0))
  {
    //Serial.println(currentTime);
    return true;
    
  }
  else
  {
    return false;
  }
  
}
void lightLogic(bool command)
{
  if(command)
  {
    for(int i=0; i < NUM_LEDS; i++)
    {
      leds[i] = CRGB(255,0,0);
      FastLED.show();
    }
  }
  else if(!command)
  {
        for(int i=0; i < NUM_LEDS; i++)
    {
      leds[i] = CRGB(0,255,0);
      FastLED.show();
    }
  }
}
void finishedRaceLed() {
  for (int j = 0; j < 255; j++) {
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CHSV(i - (j * 2), SATURATION, BRIGHTNESS); /* The higher the value 4 the less fade there is and vice versa */ 
    delay(.3);   
    }
    FastLED.show();
    delay(10); /* Change this to your hearts desire, the lower the value the faster your colors move (and vice versa) */
  }
}

void displayTime(String currentTime){
  //Serial.println(currentTime);  
  milliSec1 = (String(currentTime.charAt(currentTime.length()-2))).toInt();
  milliSec2 = (String(currentTime.charAt(currentTime.length()-1))).toInt();
  sec = (String(currentTime.charAt(currentTime.length()-4))).toInt();
  tenSec = (String(currentTime.charAt(currentTime.length()-5))).toInt(); 
   // Serial.println(currentTime);
    tm.point(1);
    tm.display(3, milliSec2);   
    tm.display(2, milliSec1);   
    tm.display(1, sec);   
    tm.display(0, tenSec);
}

void singleClick()
{
  if(!oneClick)
  {
    Serial.print("Sec: ");
  Serial.println(sec);
 // currentTime = "0000";
//  displayTime(currentTime);
  tm.point(1);
  tm.display(0,EEPROM.read(0));
  tm.display(1, EEPROM.read(1));
  tm.display(2, EEPROM.read(2));
  tm.display(3, EEPROM.read(3));
  oneClick = true;
  }
  else
  {
    tm.point(1);
    tm.display(3, milliSec2);   
    tm.display(2, milliSec1);   
    tm.display(1, sec);   
    tm.display(0, tenSec);
    oneClick = false;
  }
}

void longClick()
{
  Serial.println("saving");
  EEPROM.write(0, tenSec);
  EEPROM.write(1, sec);  
  EEPROM.write(2, milliSec1);
  EEPROM.write(3, milliSec2);
  
}
