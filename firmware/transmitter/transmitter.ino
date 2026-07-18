#define TX_PIN 5
#define BIT_TIME 600

void setup()
{
  Serial.begin(115200);
  pinMode(TX_PIN, OUTPUT);
}

void sendByte(byte b)
{
  digitalWrite(TX_PIN, LOW);        // start bit
  delayMicroseconds(BIT_TIME);

  for(int i=0;i<8;i++)
  {
    digitalWrite(TX_PIN,(b>>i)&1);
    delayMicroseconds(BIT_TIME);
  }

  digitalWrite(TX_PIN, HIGH);       // stop bit
  delayMicroseconds(BIT_TIME);
}

void loop()
{
  if(Serial.available())
  {
    byte b = Serial.read();
    sendByte(b);
  }
}
