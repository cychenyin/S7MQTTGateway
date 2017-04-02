/*----------------------------------------------------------------------
Arduino MQTT to S7-315 gateway

 Created: 2017/03/04
 by Kemp Maritz

DB105 in S7-315 is the data exchange interface
    Client.ConnectTo(<IP>, <Rack>, <Slot>);
    Client.SetConnectionParams(<IP>, <LocalTSAP>, <Remote TSAP>);
    Client.Connect();

------------------------------------------------------------------------*/
#include <SPI.h>
#include <TimeAlarms.h>
#include "Settimino.h"
#include <PubSubClient.h>
#include <Ethernet2.h>

//#define debug

byte mac[] = { 0x90, 0xA2, 0xDA, 0x0F, 0x08, 0xA5 };
IPAddress Local(192,168,1,245); // Local Address
IPAddress PLC(192,168,1,246);   // PLC Address
IPAddress MQTT(192,168,1,240);  // MQTT Server address

// definition
void callback(char* topic, byte* payload, unsigned int length);
void(* resetFunc) (void) = 0; //declare reset function @ address 0

byte      PLCBuffer[1024];
float     PLCTemp, PoolPumpCurrent, BackDoorSolar_V, totalPower;
word      poolpumpStatus, SecurityLight, BackDoorSolar, GarageLight, GarageDoor, Sensors;
long      lastReconnectAttempt = 0;
boolean   pumpOn, pumpOff, pumpMan, pumpAuto;
boolean   securityLightOn, securityLightOff, securityLightMan, securityLightAuto;
boolean   backdoorLightOn, backdoorLightOff, backdoorLightMan, backdoorLightAuto;
boolean   garageLightOn, garageLightOff, garageLightMan, garageLightAuto;
word      pumpCommand, SecLightCommand, BDoorSolarCommand, GarageLightCommand, GarageDoorCommand;
int       errorCode;

S7Client        PLCClient;
EthernetClient  ethClient;
PubSubClient    mqttClient(ethClient);

int             statusLED = 13;

//----------------------------------------------------------------------
// Setup : Init Ethernet and Serial port
//----------------------------------------------------------------------
void setup() 
{
  // Open serial communications and wait for port to open:
  Serial.begin(115200);

  mqttClient.setServer(MQTT, 1883);
  mqttClient.setCallback(callback);
  lastReconnectAttempt = 0;

  // Start the Ethernet Library
  Ethernet.begin(mac, Local);
  delay(2000); 
#ifdef debug
  Serial.println(Ethernet.localIP());
#endif
  Alarm.timerRepeat(1, schedS7);           // timer for every 1 second
  Alarm.timerRepeat(1, schedMQTT);           // timer for every 1 second
  pinMode(statusLED, OUTPUT);

  digitalWrite(statusLED, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(statusLED, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second

}

//----------------------------------------------------------------------
// Main Loop
//----------------------------------------------------------------------
void loop() 
{
  int Size, Result;
  void *Target;

  Size=64;
  Target = NULL; // Uses the internal Buffer (PDU.DATA[])

  // Connection
  while (!PLCClient.Connected)
  {
    if (!PLCConnect())
      Alarm.delay(2000);
  }
#ifdef debug
  Serial.print("Reading ");Serial.print(Size);Serial.println(" bytes from DB105"); 
#endif
  Result=PLCClient.ReadArea(S7AreaDB, // We are requesting DB access
                         105,         // DB Number = 1
                         0,           // Start from byte N.0
                         Size,        // We need "Size" bytes
                         Target);     // Put them into our target (Buffer or PDU)
  if (Result==0)
  {
    poolpumpStatus = S7.WordAt(2);      // Read Poolpump Status Word
    PLCTemp = S7.FloatAt(4);            // Read PLC Temperature value
    SecurityLight = S7.WordAt(8);
    BackDoorSolar = S7.WordAt(10);
    PoolPumpCurrent = S7.FloatAt(12);
    BackDoorSolar_V = S7.FloatAt(16);
    GarageLight = S7.WordAt(20);
    GarageDoor = S7.WordAt(22);
    Sensors = S7.WordAt(24);
    
#ifdef debug
    Serial.print("Temperature: ");
    Serial.println(PLCTemp);
    Serial.print("Poolpump: ");
    Serial.println(poolpumpStatus);
    Serial.print("SecurityLight: ");
    Serial.println(SecurityLight);
    Serial.print("BackDoorSolar: ");
    Serial.println(BackDoorSolar);
    Serial.print("PoolPumpCurrent: ");
    Serial.println(PoolPumpCurrent);
    Serial.print("BackDoorSolar_V: ");
    Serial.println(BackDoorSolar_V);
    Serial.print("GarageLight: ");
    Serial.println(GarageLight);
    Serial.print("GarageDoor: ");
    Serial.println(GarageDoor);
    Serial.println();
#endif
    poolpumpCommands();
    securityLightCommands();
    backdoorLightCommands();
    garageLightCommands();
  }
  else
  {
    CheckError(Result);
  }

  if (!mqttClient.connected()) 
  {
    long now = millis();
    if (now - lastReconnectAttempt > 5000)
    {
      lastReconnectAttempt = now;
      // Attempt to reconnect
      if (mqttReconnect())
      {
        lastReconnectAttempt = 0;
      }
    }
  }
  else
  {
    // Client connected
    mqttClient.loop();
  }

  Alarm.delay(1000); // wait one second
}



void schedS7()
{
  
}



