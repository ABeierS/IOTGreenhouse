/*
 * Project IoT Greenhouse
 * Description:
 * Author: ABS & LL
 * Date: 
 */

// City ID Herning : 2620423
// API Call : api.openweathermap.org/data/2.5/forecast?id=2620423&APPID=51e52edb805885bdaab7f0a70e593d4d
// API Key : 51e52edb805885bdaab7f0a70e593d4d

#include <time.h>
#include "PietteTech_DHT.h"

// defines
#define CUSTOM_DEBUG
#define EMAIL
//#define SMS


#define MAXTEMP   25        // Degrees
#define MAX_HUMIDITY 85     // Percent
#define MIN_HUMIDITY 10     // Percent
#define MAX_TIME_PUMP 10000 // Seconds
#define SLEEP_TIME 10       // Seconds
#define DEPTH_THRESHOLD 70  // cm


#define DHTTYPE   DHT22  // Sensor type DHT11/21/22/AM2301/AM2302
#define DHTPIN    3      // Digital pin for communications

#define trigPin 5
#define echoPin 6

#define pumpTrigPin 1

// Variables
int clouds = 0;
time_t dt = 0;
int humidity = 0;
float temp = 0;

int currenttemp = 0;
int currenthumid = 0;

long distance = 0;

system_tick_t lastTime = 0;

bool hasLogged = false;

char buf[128] = {0};

// Forward declarations
void dht_wrapper(void);
void gotForecast(const char *, const char *);
long getDistance(void);
void checkForecast(void);
void checkTank(void);
void getDHTVals(void);

void sendWarning(String);


// Setup DHT module
PietteTech_DHT DHT(DHTPIN, DHTTYPE, dht_wrapper);

void setup() {
#ifdef CUSTOM_DEBUG
  Serial.begin(9600);
  Serial.printlnf("Serial Monitor mounted");
#endif

  Particle.subscribe("hook-response/weatherForecast", gotForecast, MY_DEVICES);

  pinMode(pumpTrigPin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  checkTank();
  checkForecast();

  lastTime = millis();

}


void loop() {
  
#ifdef CUSTOM_DEBUG
  if(millis()-lastTime > 10000){
#endif

    DHT.acquire();

    if (!DHT.acquiring()) {
      getDHTVals();
      hasLogged = true;
    }

#ifdef CUSTOM_DEBUG
    if(hasLogged){
      checkForecast();
      checkTank();      
      lastTime = millis();
      hasLogged = false;
    }else if(millis()-lastTime > 10000){
      lastTime = millis();
      Serial.printlnf("Never acquired dht vals, %d", lastTime);
    }
  }
#else
// Timeout, if no temperature has been gathered go to sleep
  if(hasLogged) 
    System.sleep(SLEEP_MODE_DEEP, SLEEP_TIME);
  else if(millis() > 10000){
    sendWarning("No temperature or humidty values could be received from sensor, please check system.")
    System.sleep(SLEEP_MODE_DEEP, SLEEP_TIME);
  }
#endif
}

void sendWarning(String msg){
#ifdef EMAIL
  Particle.publish("MAIL_warning",  msg, PRIVATE);
#endif
#ifdef SMS
  Particle.publish("SMS_warning", msg, PRIVATE);
#endif
#ifdef CUSTOM_DEBUG
  Serial.println(msg);
#endif
}


void getDHTVals(){
  char buf[128] = {0};
  currenttemp = DHT.getCelsius();
  currenthumid = DHT.getHumidity();
  Particle.publish("IndoorTemp", String(currenttemp), PRIVATE);
  //Particle.publish("IndoorHumid", String(currenthumid), PRIVATE);

#ifdef CUSTOM_DEBUG
  Serial.printlnf("DHT: Temp = %d, Humid = %d",currenttemp,currenthumid);
#endif

  if(currenttemp > MAXTEMP){
    sprintf(buf, "Temperature in Greenhouse has exceded %d degrees, and is now %d degrees.",MAXTEMP,currenttemp);
    sendWarning(buf);

#ifdef CUSTOM_DEBUG
    Serial.printlnf("Temp exceded with: %d", currenttemp);
#endif
  }
  if(currenthumid > MAX_HUMIDITY){
    sprintf(buf, "Humidity in Greenhouse has exceded %d percent, and is now %d percent.",MAX_HUMIDITY,currenthumid);
    sendWarning(buf);
  }else if(currenthumid < MIN_HUMIDITY){
    sprintf(buf, "Humidity in Greenhouse has fallen below %d percent.",MIN_HUMIDITY);
    sendWarning(buf);
  }

}

void checkForecast(){
  Particle.publish("weatherForecast", PRIVATE);
}

void checkTank(){
  system_tick_t startTime;
  distance = getDistance();
#ifdef CUSTOM_DEBUG
  Serial.printlnf("Distance: %d",distance);
#endif
  if(distance > DEPTH_THRESHOLD){
     startTime = millis();
    while(distance > DEPTH_THRESHOLD){
      digitalWrite(pumpTrigPin,HIGH);
      delay(250);
      distance = getDistance();
#ifdef CUSTOM_DEBUG
      Serial.printlnf("Distance: %d",distance);
#endif
      if(millis()-startTime > MAX_TIME_PUMP){
        sendWarning("Could not fill tank");
#ifdef CUSTOM_DEBUG
        Serial.printlnf("Distance has not been corrected, exceeded threshold time");
#endif
        break;
      }
    }
    digitalWrite(pumpTrigPin,LOW);
  }
}

// This wrapper is in charge of calling
// must be defined like this for the lib work
void dht_wrapper() {
    DHT.isrCallback();
}

long getDistance(void){
  long distance,duration;
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;
  
  return distance;
}

void gotForecast(const char *event, const char *data) {

  char buffer[64] = {0};
  char del[] = "~";
  int counter = 0;

  memcpy(buffer, data, 32);
  
  char *tok = strtok(buffer,del);
  
  while(tok != NULL){
    switch (counter++){
    case 0:
      dt = strtol(tok,NULL,0);
      break;
    case 1:
      temp = strtof(tok, NULL);
      temp -= 273.15;
      break;
    case 2:
      humidity = atoi(tok);
      break;
    case 3:
      clouds = atoi(tok);
      break;
    default:
      break;
    }
    tok = strtok(NULL,del);
  }

  tm *timeObject = gmtime(&dt);

#ifdef CUSTOM_DEBUG
  Serial.printlnf("Date: %s Temp: %f, Humidity: %d, Clouds: %d", asctime(timeObject),temp,humidity,clouds);
#endif

  if((temp + ((101-clouds) / 10)) > MAXTEMP){
    sprintf(buf, "The temperature limit of %d degrees might be exceded %s.",MAXTEMP, asctime(timeObject));
    sendWarning(buf);
#ifdef CUSTOM_DEBUG
  Serial.printlnf("Temp Exceded, hook");
#endif
  }

}

/*void gotCurrentWeather(const char *event, const char *data) {
  char buffer[64] = {0};
  char del[] = "~";
  int counter = 0;

  memcpy(buffer, data, 32);
  
  char description[32] = {'\0'};
  int humidity = 0;
  float avgTemp = 0, minTemp = 0, maxTemp = 0;
  
  char *tok = strtok(buffer,del);
  
  while(tok != NULL){
    switch (counter++){
    case 0:
      memcpy(description,tok,16);
      break;
    case 1:
      avgTemp = strtof(tok, NULL);
      break;
    case 2:
      minTemp = strtof(tok, NULL);
      break;
    case 3:
      maxTemp = strtof(tok, NULL);
      break;
    case 4:
      humidity = atoi(tok);
      break;
    default:
      break;
    }
    tok = strtok(NULL,del);
  }

  Serial.printlnf("%s %f %f %f %d", description,avgTemp,minTemp,maxTemp,humidity);
}*/

