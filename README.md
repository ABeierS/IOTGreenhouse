# IOTGreenhouse  
Team consisting of: Lasse Lundholm, Andreas Beier Søndergård  
We have thought of developing some IOT features for a greenhouse.  
This can include temperature and humidity sensors both in the greenhouse and on the outside. With this you can make sure that the conditions for optimal growth. This can be connected to the internet so that a history of the enviroment in the greenhouse is available. Weather API can also be connected to warn the owner of possible harsh weather like heat, to help you remember to open a window or similar.  
For actuators we have thought of using pumps to fill up a capillary box froma rainwater collection system. This can further be developed by attaching a water atomizing/misting nozzle to a pump. With this it would be possible to raise the relative humidity in the greenhouse. 


Requirements:  
Operating in temperature range of -10 to 50 degrees  
Logging temperature and humidity in the greenhouse every 15 minutes  
Post the data gathered to visualize the history of the climate in greenhous  
Can send email/sms  
Get weather forecast to warn the owner  
Prefferably the whole system can be powered through solar panels  


Sensors:  
DHT22 (Humidity and temperature) - 0-100%RH, (-40)-80 degrees celcius, resolution of 0.1C and 0.1% RH  
Water level sensor, maybe just a distance sensor measuring on something floating on the water  

Actuators:  
Small 12V waterpump, possible to power through batter charged by solarpanel.  
https://www.banggood.com/DC-12V-Solar-Powered-Water-Pump-700LH-Brushless-Magnetic-Submersible-Water-Pumps-p-1275049.html?akmClientCountry=DK&&cur_warehouse=CN  
Water misting nozzle (don't know if pressure of smaller pumps is adequate):  
https://www.banggood.com/22PcsSet-5M-Hose-Outdoor-Cool-Patio-Misting-System-Fan-Cooler-Water-Mist-Automatic-Sprayer-Mist-Coolant-Drip-DIY-Garden-Irrigation-System-p-1512114.html?akmClientCountry=DK&rmmds=search&cur_warehouse=CN  
