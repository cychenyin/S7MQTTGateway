//----------------------------------------------------------------------
// Reverse float/dword/dint
//----------------------------------------------------------------------
void Reverse4(void *ptr)
{
  byte *pb;
  byte tmp;
  pb=(byte*)(ptr);
  // Swap byte 4 with byte 1
  tmp=*(pb+3);
  *(pb+3)=*pb;
  *pb=tmp;
  // Swap byte 3 with byte 2
  tmp=*(pb+2);
  *(pb+2)=*(pb+1);
  *(pb+1)=tmp;
}
//----------------------------------------------------------------------
// Reverse word/int
//----------------------------------------------------------------------
void Reverse2(void *ptr)
{
  byte *pb;
  byte tmp;
  pb=(byte*)(ptr);
  // Swap byte 2 with byte 1
  tmp=*(pb+1);
  *(pb+1)=*pb;
  *pb=tmp;
}


void blinkLED()
{
  digitalWrite(statusLED, HIGH);
  delay(100);
  digitalWrite(statusLED, LOW);
  
}

