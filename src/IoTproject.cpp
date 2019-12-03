/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "application.h"
#include "PietteTech_DHT.h"
#line 1 "c:/Users/lassl/OneDrive/Documents/VisualStudio_workspace/TembWebHook/src/TembWebHook.ino"
/**
 * @file:        IoT Project
 * @brief:       Testcode
 * @author:      Lasse Lundholm
 * @date:        06/11-2019
 */

// defines
#define MAXTEMP   25                  // Maximum temperature before a warning is published
#define DHTTYPE   DHT22               // Sensor type DHT11/21/22/AM2301/AM2302
#define DHTPIN    D2         	        // Digital pin for communications

// variables
int TEMPPIN = A0;

// setup() runs once, when the device is first turned on.
// prototypes
void setup();
void loop();
void dht_wrapper();
// Lib instantiate
PietteTech_DHT DHT(DHTPIN, DHTTYPE, dht_wrapper);

// For LM35 testing purposes
float LM35_GetTemp(int pinNo);
int getTemp();

#line 9 "c:/Users/lassl/OneDrive/Documents/VisualStudio_workspace/TembWebHook/src/TembWebHook.ino"
void setup() {


}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  int currenttemp = 0;
  int currenthumid = 0;
  int temp_flag = 0;
  
  //DHT 
  DHT.acquire();

  if (!DHT.acquiring())
  {
    // get DHT status can be omitted from the final project
	    int result = DHT.getStatus();

      switch (result) {
		    case DHTLIB_OK:
		      Serial.println("OK");
		      break;
		    case DHTLIB_ERROR_CHECKSUM:
		        Serial.println("Error\n\r\tChecksum error");
		        break;
        case DHTLIB_ERROR_ISR_TIMEOUT:
            Serial.println("Error\n\r\tISR time out error");
            break;
        case DHTLIB_ERROR_RESPONSE_TIMEOUT:
            Serial.println("Error\n\r\tResponse time out error");
            break;
        case DHTLIB_ERROR_DATA_TIMEOUT:
            Serial.println("Error\n\r\tData time out error");
            break;
        case DHTLIB_ERROR_ACQUIRING:
            Serial.println("Error\n\r\tAcquiring");
            break;
        case DHTLIB_ERROR_DELTA:
            Serial.println("Error\n\r\tDelta time to small");
            break;
        case DHTLIB_ERROR_NOTSTARTED:
            Serial.println("Error\n\r\tNot started");
            break;
        default:
            Serial.println("Unknown error");
            break;
        }

        //Read out temperature and humidity
        /*
        currenttemp = DHT.getCelsius();
        currenthumid = DHT.getHumidity();
        */
  }


  //LM35
  currenttemp = getTemp();
  String Temp = String(currenttemp);

  //Send the indoor temperature to Thingspeak/Thingsview
  Particle.publish("IndoorTemp", Temp, PRIVATE);

  //If alarmcriteria met => publish messages
  if(currenttemp >= MAXTEMP && temp_flag == 0)
  {
    //Mail and sms messages
    String mail_warning = "WARNING: Greenhouse temperature high";
    String sms_warning = "WARNING: Greenhouse temperature high";

    Particle.publish("MAIL_warning",  mail_warning, PRIVATE);
    Particle.publish("SMS_warning", sms_warning, PRIVATE);

    //temp_flag is set to indicate a warning has been send
    //will reset when temperature drops below threshold
    temp_flag = 1;
  }

  //reset temp_flag when temperature is below threshold
  if(currenttemp < MAXTEMP && temp_flag == 1)
  {
    temp_flag = 0;
  }
  
  //wait one minut
  delay(15000);

  //DEEP SLEEP 15 min = 900 sec
  //System.sleep(SLEEP_MODE_DEEP,20);
}

// This wrapper is in charge of calling
// must be defined like this for the lib work
void dht_wrapper() {
    DHT.isrCallback();
}

//Testing with LM35
int getTemp() {
  float temp = 0;
  temp = LM35_GetTemp(TEMPPIN);

  return temp;
}

float LM35_GetTemp(int pinNo) {
  float Res = 0.0;
  //Assume 12 bit ADC
  //Take 10 measurements and average
  for (int i = 0; i < 10; ++i)
  {
    Res += (3.3 / 4096) * analogRead(pinNo);
  }
  //Divide with 10 to get average
  Res = Res / 10;
  //LM35 - 10mV per deg. C
  Res = Res / 0.01;
  return Res;
}