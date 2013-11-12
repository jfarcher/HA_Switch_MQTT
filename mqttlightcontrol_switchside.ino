/*
 Basic MQTT example with Authentication
 
  - connects to an MQTT server, providing username
    and password
  - publishes "hello world" to the topic "outTopic"
  - subscribes to the topic "inTopic"
*/

#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <OneWire.h>



// Update these with values suitable for your network.
byte mac[]    = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
OneWire ds(2);

#define MQTT_SERVER "192.168.1.3"
IPAddress ip(192,168,1, 178);
IPAddress gateway(192,168,1, 1);
IPAddress subnet(255, 255, 255, 0);

// constants won't change. They're used here to 
// set pin numbers:
const int buttonPin = 7;     // the number of the pushbutton pin
const int ledPin =  13;      // the number of the LED pin

// Variables will change:
int ledState = HIGH;         // the current state of the output pin
int buttonState;
char* swState;
// the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin

// the following variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers


void callback(char* topic, byte* payload, unsigned int length) {
  // handle message arrived
}

EthernetClient ethClient;
PubSubClient client(MQTT_SERVER, 1883, callback, ethClient);

void setup()
{
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);
    Serial.begin(9600);
     byte i;
  byte dsAddress[8]; //ds on pin 8
 
  Serial.begin(9600);
  delay( 500 );
  Serial.print ("Searching for DS18B20...");
 ds.reset_search(); // Start the search with the first device
  
  if ( !ds.search(dsAddress) )
  {
    Serial.println("none found. Using specified MAC Address.");
  } else {
    Serial.print( "Success! \nSetting MAC address...." );

  mac[1] = dsAddress[3];
  mac[2] = dsAddress[4];
  mac[3] = dsAddress[5];
  mac[4] = dsAddress[6];
  mac[5] = dsAddress[7];
  }
  Serial.print( "Ethernet MAC =" );
  for( i = 0; i < 6; i++ )
  {
    Serial.write( ' ' );
    Serial.print( mac[i], HEX );
  }
  Serial.println();
  
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    for(;;)
      ;
  }
  Serial.print("IP address: ");
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    // print the value of each byte of the IP address:
    Serial.print(Ethernet.localIP()[thisByte], DEC);
  }
  delay(10);
  Serial.println();
//if (client.connect("SW1")) {      // connecting to MQTT server
//   Serial.println("Connected to MQTT");      // Milestone for connection debugging
//  }
}

void loop()
{
 // read the state of the switch into a local variable:
  int reading = digitalRead(buttonPin);

 

  // check to see if you just pressed the button 
  // (i.e. the input went from LOW to HIGH),  and you've waited 
  // long enough since the last press to ignore any noise:  

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
    
   // this is all that's new to the code 
   // toggles the ledState variable each time the button is pressed 
      if (buttonState == HIGH) {
      ledState = !ledState;
      Serial.println(ledState);
      Serial.println("sending state");
      client.connect("SW1");
      if (ledState == 0){
        swState = "2-40";
      }
      if (ledState == 1){
        swState = "2-41";
      }
      client.publish("house/room1/1",swState);
    } 
  } 
  
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:
    buttonState = reading;
  }
  

  
  // set the LED using the state of the button:
//  digitalWrite(ledPin, ledState);

  // save the reading.  Next time through the loop,
  // it'll be the lastButtonState:
  lastButtonState = reading;
}

