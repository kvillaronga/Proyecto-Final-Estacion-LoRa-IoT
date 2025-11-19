#include <SPI.h>
#include <LoRa.h>
#include <DHT.h>


#define DHT_PIN 5
#define DHT_TYPE DHT11
DHT dht(DHT_PIN, DHT_TYPE);

#define SCK 2
#define MISO 13
#define MOSI 12
#define SS 14
#define RST 35
#define DIO0 34

#define led 5

float temperatura=0;
int humedad=0;

void setup() {
  Serial.begin(115200);
  delay(2000);
  
  SPI.begin(SCK, MISO, MOSI, SS);
  LoRa.setPins(SS, RST, DIO0);

  dht.begin();
  Serial.println(" Sensor DHT11 inicializado");
  
  
  if (!LoRa.begin(433E6)) {
    Serial.println("LoRa init failed!");
    while (1);
  }

  LoRa.setTxPower(20);               
  LoRa.setSpreadingFactor(12);       
  LoRa.setSignalBandwidth(125E3);     
  LoRa.setCodingRate4(8);             
  LoRa.setPreambleLength(8);         
  LoRa.enableCrc();                   
  
  Serial.println("Parámetros LoRa optimizados para MÁXIMO ALCANCE");
  Serial.println("TX Power: 20dBm | SF: 12 | BW: 125kHz | CR: 4/8");
  
  Serial.println("LoRa optimizado para máximo alcance");
  
  Serial.println("Emisor LoRa listo - Enviando datos estructurados");
}

void loop() {
  
  humedad = dht.readHumidity();
  temperatura = dht.readTemperature();

  
  // Crear string estructurado
  String datos = ",TEMP:" + String(temperatura, 1) +  // 1 decimal
                 ",HUM:" + String(humedad);
                 
            
  
  // Enviar
  LoRa.beginPacket();
  LoRa.print(datos);
  LoRa.endPacket();
  
  Serial.print(" Enviado: ");
  Serial.println(datos);
  
  
  delay(20000);  
}
