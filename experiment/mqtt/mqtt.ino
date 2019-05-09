#include "EspMQTTClient.h"



EspMQTTClient client(

  "ME588G4",
  "1n1t1al0",
  "192.168.1.147",   
  "i-robot07",
  8883              
);



void setup()

{

  Serial.begin(115200);



  // Optionnal functionnalities of EspMQTTClient : 
  client.enableDebuggingMessages(); // Enable debugging messages sent to serial output
//  client.enableHTTPWebUpdater(); // Enable the web updater. User and password default to values of MQTTUsername and MQTTPassword. These can be overrited with enableHTTPWebUpdater("user", "password").
//  client.enableLastWillMessage("TestClient/lastwill", "I am going offline");  // You can activate the retain flag by setting the third parameter to true

}

void onConnectionEstablished()

{

  // Subscribe to "mytopic/test" and display received message to Serial
  client.subscribe("irobot/test", [](const String & payload) {
    Serial.println(payload);
  });
  
  client.publish("irobot/test", "This is a message"); // You can activate the retain flag by setting the third parameter to true

  client.executeDelayed(5 * 1000, []() {
    client.publish("irobot/test", "This is a message sent 5 seconds later");
  });

}



void loop()
{

  client.loop();

}
