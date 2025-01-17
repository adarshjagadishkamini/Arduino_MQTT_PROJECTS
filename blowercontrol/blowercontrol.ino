
#include <dht.h>

dht DHT;

#define DHT11_PIN     5

int blowpin = 9;
int relayonoff = 8;
int flag = 0;

void setup()
{
  Serial.begin(115200);
  Serial.println("DHT TEST PROGRAM ");
  Serial.print("LIBRARY VERSION: ");
  Serial.println(DHT_LIB_VERSION);
  Serial.println();
  Serial.println("Type,\tstatus,\tHumidity (%),\tTemperature (C)");
  //pinMode(fastblowpin, OUTPUT);
  pinMode(blowpin, OUTPUT);
  pinMode(relayonoff, OUTPUT);
  
}


void loop()
{ if(flag == 0)
  {  
    digitalWrite(blowpin, LOW); }
  // READ DATA
  Serial.print("DHT11, \t");
  int chk = DHT.read11(DHT11_PIN);
  switch (chk)
  {
    case DHTLIB_OK:  
    Serial.print("OK,\t"); 
    break;
    case DHTLIB_ERROR_CHECKSUM: 
    Serial.print("Checksum error,\t"); 
    break;
    case DHTLIB_ERROR_TIMEOUT: 
    Serial.print("Time out error,\t"); 
    break;
    case DHTLIB_ERROR_CONNECT:
        Serial.print("Connect error,\t");
        break;
    case DHTLIB_ERROR_ACK_L:
        Serial.print("Ack Low error,\t");
        break;
    case DHTLIB_ERROR_ACK_H:
        Serial.print("Ack High error,\t");
        break;
    default: 
    Serial.print("Unknown error,\t"); 
    break;
  }
  
  Serial.print(DHT.humidity, 1);
  Serial.print(",\t");
  Serial.println(DHT.temperature, 1);

  if (DHT.temperature>=30)
  { 
    digitalWrite(blowpin,HIGH);
   
  flag = 1;}
  
  else 
  flag = 0;


  if (DHT.temperature>=35)
  { digitalWrite(relayonoff, HIGH);}

  if (DHT.temperature<35)
  { digitalWrite(relayonoff, LOW);}
  
  delay(2000);
}
