#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

#define RADIO_MODULE 0 //or 1

//PA LEVEL: RF24_PA_MIN(-18dB+20dB+10dB), RF24_PA_LOW(-12dB+20dB+10dB), RF24_PA_HIGH(-6dB+20dB+10dB), RF24_PA_MAX(0dB+20dB+10dB)
//DATA RATE: RF24_1MBPS, RF24_2MBPS, RF24_250KBPS

RF24 radio(9, 8);  // CE, CS

byte address[][6] = {"00001","00002"};

void setup()
{
  Serial.begin(115200);
  printf_begin();
  radio.begin();
  if (radio.isChipConnected())
  {
    Serial.println("Conexoes Corretas");
  }
  #if RADIO_MODULE == 0
    radio.openWritingPipe(address[1]);
    radio.openReadingPipe(1,address[0]);
  #else
    radio.openWritingPipe(address[0]);
    radio.openReadingPipe(1,address[1]);
  #endif
  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_250KBPS);
  radio.setAutoAck(1); 
  radio.enableAckPayload();  
  //radio.setRetries(0,15);
  radio.startListening();
  radio.printDetails();
  delay(100);
}

void loop()
{ 
  int len = 0;

  radio.startListening();
 
  if(Serial.available() > 0)
  {
  String pacote = Serial.readString();
  radio.stopListening();
  char buf[pacote.length()] = ""; 
  pacote.toCharArray(buf,pacote.length());
  radio.write(&buf, sizeof(buf));}

  if (radio.available())
   {
    len = radio.getDynamicPayloadSize();
    char text[] = "";
    radio.read( &text, len);
    Serial.println(text);
   }
}
