boolean mqttReconnect()
{
  if (mqttClient.connect("S7MQTTGateway"))
  {
    // Once connected, publish an announcement...
    mqttClient.publish("S7MQTT/alive","Gateway alive!");
    
    // ... and subscribe
    mqttClient.subscribe("S7MQTT/sub");
    mqttClient.subscribe("hacs/energy/totalPower");
  }
  return mqttClient.connected();
}



void callback(char* topic, byte* payload, unsigned int length) 
{
  // handle message arrived
  char mypl[48];              // Allocate the correct amount of memory for the payload copy
  memcpy(mypl,payload,length);
  mypl[length]=char(0);
  String command(mypl);

  #ifdef debug
  Serial.print("#####################Command Receive: ");
  Serial.print(topic);
  Serial.print("->");
  Serial.println(command);
  #endif

  
  if(String(topic) == "S7MQTT/sub")
  {

//    boolean   securityLightOn, securityLightOff, securityLightMan, securityLightAuto;
  if(command == "SecurityOn")   {securityLightOn = HIGH; securityLightOff = LOW; securityLightMan = LOW; securityLightAuto = LOW;}
  if(command == "SecurityOff")  {securityLightOn = LOW; securityLightOff = HIGH; securityLightMan = LOW; securityLightAuto = LOW;}
  if(command == "SecurityMan")  {securityLightOn = LOW; securityLightOff = LOW; securityLightMan = HIGH; securityLightAuto = LOW;}
  if(command == "SecurityAuto") {securityLightOn = LOW; securityLightOff = LOW; securityLightMan = LOW; securityLightAuto = HIGH;}

  if(command == "PumpOn")   {pumpOn = HIGH; pumpOff = LOW; pumpMan = LOW; pumpAuto = LOW;}
  if(command == "PumpOff")  {pumpOn = LOW; pumpOff = HIGH; pumpMan = LOW; pumpAuto = LOW;}
  if(command == "PumpMan")  {pumpOn = LOW; pumpOff = LOW; pumpMan = HIGH; pumpAuto = LOW;}
  if(command == "PumpAuto") {pumpOn = LOW; pumpOff = LOW; pumpMan = LOW; pumpAuto = HIGH;}

// boolean   backdoorLightOn, backdoorLightOff, backdoorLightMan, backdoorLightAuto;
  if(command == "BackdoorOn")   {backdoorLightOn = HIGH; backdoorLightOff = LOW; backdoorLightMan = LOW; backdoorLightAuto = LOW;}
  if(command == "BackdoorOff")  {backdoorLightOn = LOW; backdoorLightOff = HIGH; backdoorLightMan = LOW; backdoorLightAuto = LOW;}
  if(command == "BackdoorMan")  {backdoorLightOn = LOW; backdoorLightOff = LOW; backdoorLightMan = HIGH; backdoorLightAuto = LOW;}
  if(command == "BackdoorAuto") {backdoorLightOn = LOW; backdoorLightOff = LOW; backdoorLightMan = LOW; backdoorLightAuto = HIGH;}

// boolean   garageLightOn, garageLightOff, garageLightMan, garageLightAuto;
  if(command == "GarageOn")   {garageLightOn = HIGH; garageLightOff = LOW; garageLightMan = LOW; garageLightAuto = LOW;}
  if(command == "GarageOff")  {garageLightOn = LOW; garageLightOff = HIGH; garageLightMan = LOW; garageLightAuto = LOW;}
  if(command == "GarageMan")  {garageLightOn = LOW; garageLightOff = LOW; garageLightMan = HIGH; garageLightAuto = LOW;}
  if(command == "GarageAuto") {garageLightOn = LOW; garageLightOff = LOW; garageLightMan = LOW; garageLightAuto = HIGH;}

  if(command == "ResetPLC") {resetEthernet();}
}

  if(String(topic) == "hacs/energy/totalPower")
  {
    totalPower = command.toFloat();
    energyValuesUpdate();

#ifdef debug
    Serial.print("#####################Energy Receive: ");
    Serial.print(topic);
    Serial.print("->");
    Serial.println(command);
    Serial.print(" : ");
    Serial.println(totalPower);
#endif

  }
}


void schedMQTT()
{
  char buf[4];
  
  mqttClient.publish("S7MQTT/Temp",itoa(PLCTemp*100,buf,10));
  mqttClient.publish("S7MQTT/PumpStatus",itoa(poolpumpStatus,buf,10));
  mqttClient.publish("S7MQTT/SecurityLight",itoa(SecurityLight,buf,10));
  mqttClient.publish("S7MQTT/BackDoorSolar",itoa(BackDoorSolar,buf,10));
  mqttClient.publish("S7MQTT/PoolPumpCurrent",itoa(PoolPumpCurrent*100,buf,10));
  mqttClient.publish("S7MQTT/BackDoorSolar_V",itoa(BackDoorSolar_V*100,buf,10));
  mqttClient.publish("S7MQTT/GarageLight",itoa(GarageLight,buf,10));
  mqttClient.publish("S7MQTT/GarageDoor",itoa(GarageDoor,buf,10));
  mqttClient.publish("S7MQTT/Sensors",itoa(Sensors,buf,10));
  mqttClient.publish("S7MQTT/Error",itoa(errorCode,buf,10));
  blinkLED();
}


