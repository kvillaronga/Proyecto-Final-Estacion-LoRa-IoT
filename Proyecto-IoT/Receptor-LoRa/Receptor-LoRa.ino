#include <SPI.h>
#include <LoRa.h>
#include <WiFi.h>
#include "ThingSpeak.h"


const char* ssid = "iPhone de Shadows";
const char* password = "123456789";


unsigned long myChannelNumber = 3168677;       
const char* myWriteAPIKey = "JVB2OCXANU0ZKRO7";

WiFiClient client;


#define SCK 2
#define MISO 13
#define MOSI 12
#define SS 14
#define RST 35
#define DIO0 34


#define ENA 23
#define IN1 22
#define IN2 21

int paquetesRecibidos = 0;
int paquetesPerdidos = 0;
unsigned long ultimoPaquete = 0;

// ======== SETUP ========
void setup() {
  Serial.begin(115200);
  delay(2000);
  
  Serial.println("\n=== RECEPTOR LORA + THINGSPEAK ===");


  SPI.begin(SCK, MISO, MOSI, SS);
  LoRa.setPins(SS, RST, DIO0);

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);

  digitalWrite(IN1,HIGH);
  
  if (!LoRa.begin(433E6)) {
    Serial.println("FALLO INICIALIZANDO LORA!");
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
 

  LoRa.receive();  
  Serial.println("Receptor LoRa listo en modo CONTINUO");

  // --- Conectar WiFi ---
  Serial.print("Conectando a WiFi: ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("\n WiFi Conectado");

  // --- Inicializar ThingSpeak ---
  ThingSpeak.begin(client);
  Serial.println(" ThingSpeak Inicializado");
}

// ======== LOOP PRINCIPAL ========
void loop() {
  int packetSize = LoRa.parsePacket();

  if (packetSize > 0) {
    paquetesRecibidos++;
    
    // Leer mensaje LoRa
    String mensaje = "";
    while (LoRa.available()) {
      mensaje += (char)LoRa.read();
    }

    // Mostrar por consola
    

    // PARSEAR DATOS ESTRUCTURADOS
    int contador = obtenerValor(mensaje, "CNT:").toInt();
    float temperatura = obtenerValor(mensaje, "TEMP:").toFloat();
    int humedad = obtenerValor(mensaje, "HUM:").toInt();
    

    // Mostrar datos parseados?
    Serial.println("");
    Serial.println("DATOS");
    
    Serial.print("  Temperatura: "); Serial.print(temperatura); Serial.println("°C");
    Serial.print("  Humedad: "); Serial.print(humedad); Serial.println("%");
    
    if (temperatura >= 30)
    {
      digitalWrite(ENA,HIGH);

    }
    else
    {
      digitalWrite(ENA,LOW);
    }


    // ---- ENVIAR DATOS A THINGSPEAK ----
    // Usar campos específicos para cada dato
    ThingSpeak.setField(1, temperatura);    // Campo 1: Temperatura
    ThingSpeak.setField(2, humedad);        // Campo 2: Humedad
    ThingSpeak.setField(3, temperatura);

    Serial.print(" Enviando a ThingSpeak... ");
    
    int httpCode = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

    if (httpCode == 200) {
      Serial.println("OK");
    } else {
      Serial.print("Error. Código: ");
      Serial.println(httpCode);
    }

    // Volver a escuchar LoRa
    LoRa.receive();

    // ThingSpeak sólo permite subir cada 15 segundos
    Serial.println("Esperando 16 segundos...");
    delay(16000);
  }

  // Mostrar estado cada 30 segundos (sin bloquear)
  static unsigned long ultimoEstado = 0;
  if (millis() - ultimoEstado > 30000) {
    Serial.print(" Esperando datos LoRa... Paquetes recibidos: ");
    Serial.println(paquetesRecibidos);
    ultimoEstado = millis();
  }
  
  delay(50);
}

// ======== FUNCIÓN PARA EXTRAER VALORES ========
String obtenerValor(String data, String clave) {
  int inicio = data.indexOf(clave);
  if (inicio == -1) return "0";  // Valor por defecto si no encuentra
  
  inicio += clave.length();
  int fin = data.indexOf(',', inicio);
  if (fin == -1) fin = data.length();
  
  return data.substring(inicio, fin);
}
