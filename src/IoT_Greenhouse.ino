/*
 * Project IoT Greenhouse
 * Description:
 * Author: ABS & LL
 * Date: 
 */

// City ID Herning : 2620423
// API Call : api.openweathermap.org/data/2.5/forecast?id=2620423&APPID=51e52edb805885bdaab7f0a70e593d4d
// API Key : 51e52edb805885bdaab7f0a70e593d4d

system_tick_t lastTime = 0;
// setup() runs once, when the device is first turned on.
void setup() {
  // Put initialization like pinMode and begin functions here.
  Serial.begin(9600);
  Particle.subscribe("hook-response/weatherCurrent", gotForecast, MY_DEVICES);
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  // The core of your code will likely live here. 
  Serial.printlnf("%d",millis());
  if(millis()-lastTime > 10000){
    Particle.publish("weatherCurrent", PRIVATE);
    lastTime = millis();
    Serial.printlnf("Last time: %d",lastTime);
  }
  delay(1000);
  //System.sleep(SLEEP_MODE_DEEP, 10);
}

void gotForecast(const char *event, const char *data) {
  char buffer[32] = {0};
  char del[2] = "~";

  memcpy(buffer, data, 32);

  Serial.printlnf("event: %s, data: %s", event,buffer);
  char description[16] = {0};
  int humidity = 0;
  float avgTemp = 0, minTemp = 0, maxTemp = 0;
  int counter = 0;
  char * tok = strtok(buffer,del);
  while(tok != NULL){
    Serial.printlnf("Token:%s, Counter:%d",tok,counter);
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
}
