//----------------------------------------------------------------------
// S7 Helper
//----------------------------------------------------------------------
// BitAt     - bool BitAt(void *Buffer, int ByteIndex, byte BitIndex);
//           IsOn=S7.BitAt(140, 2);   // “IsOn” is true if (PDU.DATA[140] & 0x04)!=0
// ByteAt    - byte ByteAt(void *Buffer, int Index);
//           MyByte=S7.ByteAt(&MyBuffer, 140);
// WordAt    - word WordAt(void *Buffer, int Index);
//           MyWord=S7.WordAt(&MyBuffer, 140);
// DWordAt   - dword DWordAt(void *Buffer, int Index);
//           MyDWord=S7.DWordAt(&MyBuffer, 140);
// FloatAt   - float FloatAt(void *Buffer, int Index);
//           MyFloat=S7.FloatAt(&MyBuffer, 140);
// IntegerAt - integer IntegerAt(void *Buffer, int Index);
//           MyInteger=S7.IntegerAt(&MyBuffer, 140);
// DIntAt    - dint DIntAt(void *Buffer, int Index);
//           MyDInt=S7.DIntAt(&MyBuffer, 140);
//
// ERROR CODES
// errTCPConnectionFailed 0x0001
// errTCPConnectionReset  0x0002
// errTCPDataRecvTout     0x0003
// errTCPDataSend         0x0004
// errTCPDataRecv         0x0005
// errISOConnectionFailed 0x0006
// errISONegotiatingPDU   0x0007
// errISOInvalidPDU       0x0008
//
// errS7InvalidPDU        0x0100
// errS7SendingPDU        0x0200
// errS7DataRead          0x0300
// errS7DataWrite         0x0400
// errS7Function          0x0500
// errBufferTooSmall      0x0600
//

//----------------------------------------------------------------------
// Reset Ethernet Connection
//----------------------------------------------------------------------
void resetEthernet() 
{
  ethClient.stop();
  Alarm.delay(1000);
  Ethernet.begin(mac, Local);
  Alarm.delay(1000);
  errorCode = "20";
}

//----------------------------------------------------------------------
// Connect to the PLC
//----------------------------------------------------------------------
bool PLCConnect()
{
    int Result=PLCClient.ConnectTo(PLC, 
                                  0,  // Rack (see the doc.)
                                  2); // Slot (see the doc.)
#ifdef debug
    Serial.print("Connecting to ");Serial.println(PLC);  
#endif
    if (Result==0) 
    {
#ifdef debug
      Serial.print("Connected ! PDU Length = ");Serial.println(PLCClient.GetPDULength());
#endif
    }
    else
    {
#ifdef debug
      Serial.println("Connection error");
#endif
      //resetEthernet();
      resetFunc();
    }
    return Result==0;
}

//----------------------------------------------------------------------
// Prints the Error number
//----------------------------------------------------------------------
void CheckError(int ErrNo)
{
#ifdef debug
  Serial.print("Error No. 0x");
  Serial.println(ErrNo, HEX);
#endif  
  // Checks if it's a Severe Error => we need to disconnect
  if (ErrNo & 0x00FF)
  {
#ifdef debug
    Serial.println("SEVERE ERROR, disconnecting.");
#endif
    PLCClient.Disconnect(); 
      errorCode = "21";
  }
}




void poolpumpCommands()
{
int Size, Result;
//  pumpCommand, SecLightCommand, BDoorSolarCommand, GarageLightCommand, GarageDoorCommand;
    
    if(pumpOn == HIGH)
    {
      bitSet(pumpCommand,0);
      Result=PLCClient.WriteArea(S7AreaDB, 105, 64, 1, &pumpCommand);
    }
    if(pumpOff == HIGH)
    {
      bitSet(pumpCommand,1);
      Result=PLCClient.WriteArea(S7AreaDB, 105, 64, 1, &pumpCommand);
    }
    if(pumpMan == HIGH)
    {
      bitSet(pumpCommand,3);
      Result=PLCClient.WriteArea(S7AreaDB, 105, 64, 1, &pumpCommand);
    }
    if(pumpAuto == HIGH)
    {
      bitSet(pumpCommand,2);
      Result=PLCClient.WriteArea(S7AreaDB, 105, 64, 1, &pumpCommand);
    }

  pumpOn = LOW;
  pumpOff = LOW;
  pumpMan = LOW;
  pumpAuto = LOW;
  Alarm.delay(100);
  pumpCommand = 0;
  Result=PLCClient.WriteArea(S7AreaDB, 105, 64, 1, &pumpCommand);
}

void securityLightCommands()
{
int Size, Result;
//  pumpCommand, SecLightCommand, BDoorSolarCommand, GarageLightCommand, GarageDoorCommand;
//    boolean   securityLightOn, securityLightOff, securityLightMan, securityLightAuto;
//    boolean   backdoorLightOn, backdoorLightOff, backdoorLightMan, backdoorLightAuto;

    if(securityLightOn == HIGH)
    {
      bitSet(SecLightCommand,0);
      Result=PLCClient.WriteArea(S7AreaDB, 105, 66, 1, &SecLightCommand);
    }
    if(securityLightOff == HIGH)
    {
      bitSet(SecLightCommand,1);
      Result=PLCClient.WriteArea(S7AreaDB, 105, 66, 1, &SecLightCommand);
    }
    if(securityLightMan == HIGH)
    {
      bitSet(SecLightCommand,3);
      Result=PLCClient.WriteArea(S7AreaDB, 105, 66, 1, &SecLightCommand);
    }
    if(securityLightAuto == HIGH)
    {
      bitSet(SecLightCommand,2);
      Result=PLCClient.WriteArea(S7AreaDB, 105, 66, 1, &SecLightCommand);
    }

  securityLightOn = LOW;
  securityLightOff = LOW;
  securityLightMan = LOW;
  securityLightAuto = LOW;
  Alarm.delay(100);
  SecLightCommand = 0;
  Result=PLCClient.WriteArea(S7AreaDB, 105, 66, 1, &SecLightCommand);
}


void backdoorLightCommands()
{
int Size, Result;
//  pumpCommand, SecLightCommand, BDoorSolarCommand, GarageLightCommand, GarageDoorCommand;
//    boolean   backdoorLightOn, backdoorLightOff, backdoorLightMan, backdoorLightAuto;

    if(backdoorLightOn == HIGH)
    {
      bitSet(BDoorSolarCommand,0);
      Result=PLCClient.WriteArea(S7AreaDB, 105, 68, 1, &BDoorSolarCommand);
    }
    if(backdoorLightOff == HIGH)
    {
      bitSet(BDoorSolarCommand,1);
      Result=PLCClient.WriteArea(S7AreaDB, 105, 68, 1, &BDoorSolarCommand);
    }
    if(backdoorLightMan == HIGH)
    {
      bitSet(BDoorSolarCommand,3);
      Result=PLCClient.WriteArea(S7AreaDB, 105, 68, 1, &BDoorSolarCommand);
    }
    if(backdoorLightAuto == HIGH)
    {
      bitSet(BDoorSolarCommand,2);
      Result=PLCClient.WriteArea(S7AreaDB, 105, 68, 1, &BDoorSolarCommand);
    }

  backdoorLightOn = LOW;
  backdoorLightOff = LOW;
  backdoorLightMan = LOW;
  backdoorLightAuto = LOW;
  Alarm.delay(100);
  BDoorSolarCommand = 0;
  Result=PLCClient.WriteArea(S7AreaDB, 105, 68, 1, &BDoorSolarCommand);
}


void garageLightCommands()
{
int Size, Result;
//  pumpCommand, SecLightCommand, BDoorSolarCommand, GarageLightCommand, GarageDoorCommand;
// boolean   garageLightOn, garageLightOff, garageLightMan, garageLightAuto;

    if(garageLightOn == HIGH)
    {
      bitSet(GarageLightCommand,0);
      Result=PLCClient.WriteArea(S7AreaDB, 105, 70, 1, &GarageLightCommand);
    }
    if(garageLightOff == HIGH)
    {
      bitSet(GarageLightCommand,1);
      Result=PLCClient.WriteArea(S7AreaDB, 105, 70, 1, &GarageLightCommand);
    }
    if(garageLightMan == HIGH)
    {
      bitSet(GarageLightCommand,3);
      Result=PLCClient.WriteArea(S7AreaDB, 105, 70, 1, &GarageLightCommand);
    }
    if(garageLightAuto == HIGH)
    {
      bitSet(GarageLightCommand,2);
      Result=PLCClient.WriteArea(S7AreaDB, 105, 70, 1, &GarageLightCommand);
    }

  garageLightOn = LOW;
  garageLightOff = LOW;
  garageLightMan = LOW;
  garageLightAuto = LOW;
  Alarm.delay(100);
  GarageLightCommand = 0;
  Result=PLCClient.WriteArea(S7AreaDB, 105, 70, 1, &GarageLightCommand);
}


void energyValuesUpdate()
{
  int Size, Result;
  Reverse4(&totalPower);
  Result = PLCClient.WriteArea(S7AreaDB, 105, 72, sizeof(float), &totalPower);
}


