**Proyecto LoRa – Transmisión de Temperatura y Humedad con ESP32 + DHT11 + ThingSpeak**


Este proyecto implementa un sistema inalámbrico basado en LoRa para la transmisión de datos ambientales (temperatura y humedad) utilizando dos ESP32: un emisor equipado con un sensor DHT11 y un receptor encargado de procesar la información y enviarla a ThingSpeak para su visualización en tiempo real.

***Emisor LoRa (ESP32 + DHT11)***


El nodo emisor realiza la lectura periódica del sensor DHT11 y envía los datos estructurados a través del módulo LoRa.

Se configuraron parámetros de modulación optimizados para máximo alcance:
 
  - Spreading Factor: SF12
  
  - Bandwidth: 125 kHz
  
  - Coding Rate: 4/8
  
  - Potencia de transmisión: 20 dBm
  
  - Formato de datos enviados: ,TEMP:xx.x,HUM:yy

***Receptor LoRa (ESP32 + WiFi + ThingSpeak)***


El nodo receptor permanece en modo de recepción continua, decodifica la trama enviada y extrae los valores de temperatura y humedad.
Luego, estos datos se envían automáticamente a la plataforma ThingSpeak, utilizando:

Field 1: Temperatura
Field 2: Humedad

El sistema también incluye la activación de un actuador (motor/ventilador) cuando la temperatura supera los 30 °C.

Características principales
  
  - Comunicación inalámbrica LoRa a 433 MHz
  
  - Medición ambiental con sensor DHT11
  
  - Enlace de largo alcance configurado para máxima cobertura
  
  - Envío de datos y visualización remota mediante ThingSpeak
  
  - Activación automática de actuador con base en la temperatura
  
  - Código estructurado con función para parseo de tramas LoRa
