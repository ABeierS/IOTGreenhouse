/*
 * Project IoT Greenhouse
 * Description:
 * Author: ABS & LL
 * Date: 
 */

// City ID Herning : 2620423
// API Call : api.openweathermap.org/data/2.5/forecast?id=2620423&APPID=51e52edb805885bdaab7f0a70e593d4d
// API Key : 51e52edb805885bdaab7f0a70e593d4d


// setup() runs once, when the device is first turned on.
void setup() {
  // Put initialization like pinMode and begin functions here.
  Particle.subscribe("hook-response/currentWeather", gotForecast, MY_DEVICES);
  Particle.publish("currentWeather", PRIVATE);
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  // The core of your code will likely live here.

}

void gotForecast(const char *event, const char *data) {

}
