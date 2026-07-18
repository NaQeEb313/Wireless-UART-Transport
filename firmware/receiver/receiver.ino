#define RX_PIN 5
#define BIT_TIME 600

void setup()
{
  Serial.begin(115200);
  pinMode(RX_PIN, INPUT);
}

byte readByte()
{
  while(digitalRead(RX_PIN)==HIGH);   // wait start bit

  delayMicroseconds(BIT_TIME*1.5);

  byte val = 0;

  for(int i=0;i<8;i++)
  {
    val |= digitalRead(RX_PIN)<<i;
    delayMicroseconds(BIT_TIME);
  }

  delayMicroseconds(BIT_TIME);   // stop bit

  return val;
}

void loop()
{
  byte b = readByte();
  Serial.write(b);
}
