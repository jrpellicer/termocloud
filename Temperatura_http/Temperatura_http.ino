#include "DHT.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <time.h>

#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

/*Put your SSID & Password*/
const char* ssid = "Asir_FHW";  // Enter SSID here
const char* password = "FHW_Asir";  //Enter Password here
const char* serverUrl = "https://estacion-jrpm.azurewebsites.net/api/escribe_lecturas?code=Ykm9tOrBZp9HbCDEAki5gz_OtiaJ3Zw0an0NPzLRb0gfAzFuOMKKYA%3D%3D";
//const char* serverUrl = "https://0v3h7sk95i.execute-api.us-east-1.amazonaws.com/default/HolaMundo";
const char* ntpServer = "es.pool.ntp.org"; // Servidor NTP para obtener la hora


long lastMsg = 0;


// DHT Sensor
uint8_t DHTPin = D2;

// Initialize DHT sensor.
DHT dht(DHTPin, DHTTYPE); 

float Temperature;
float Humidity;

String getTimestamp() {
    time_t now;
    struct tm timeinfo;
    time(&now);
    localtime_r(&now, &timeinfo);

    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);
    return String(buffer);
}

void setup() {
  // Inicializamos puerto serie para debug
  Serial.begin(115200);
  delay(100);

  // Inicializamos pin sensor
  pinMode(DHTPin, INPUT);
  dht.begin();              

  // Comenzamos conexión a WiFi
  Serial.println("Conectando a ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  //chequeamos conexión a WiFi
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  //Mostramos información de la conexión
  Serial.println("");
  Serial.println("WiFi conectada..!");
  Serial.print("IP Asignada: ");  
  Serial.println(WiFi.localIP());

 // Configurar NTP para obtener fecha y hora
   configTime(0, 0, ntpServer);
   Serial.println("Esperando sincronización de hora...");
   delay(35000);
   long now = millis();
  
  // Leemos temperatura y humedad
  Temperature = dht.readTemperature(); // Gets the values of the temperature
  Humidity = dht.readHumidity(); // Gets the values of the humidity 

  if (WiFi.status() == WL_CONNECTED) {
        WiFiClientSecure client;
        //client.setInsecure();  // Deshabilita la validación del certificado SSL
        //auto client = std::make_unique<BearSSL::WiFiClientSecure>();
        client.setInsecure();

        HTTPClient http;
        http.begin(client, serverUrl);
        //http.addHeader("Content-Type", "application/json");

        //String payload = "{\"sensor\": \"casa1\",\"timestamp\": \"2025-02-04 16:55:00\",\"temperatura\": " + String(msg) + ",\"humedad\": " + String(msg2) + "}";
        String payload = "{\"sensor\": \"casa1\",";
        payload += "\"timestamp\": \"" + getTimestamp() + "\",";
        payload += "\"temperatura\": " + String(Temperature, 1) + ",";
        payload += "\"humedad\": " + String(Humidity, 1) + "}";
        Serial.println(payload);
        int httpResponseCode = http.POST(payload);
        //int httpResponseCode = http.GET();
        if (httpResponseCode > 0) {
            Serial.print("Código de respuesta HTTP: ");
            Serial.println(httpResponseCode);
            String response = http.getString();
            Serial.println("Respuesta: " + response);
        } else {
            Serial.print("Error en la petición HTTP: ");
            Serial.println(httpResponseCode);
        }

        http.end();
        
    }
  // Esperamos 5 segundos antes de apagar para que de tiempo a enviarse el mensaje (importante)
  //delay(5000);

  //Apagamos durante 5 minutos (300 segundos)
  Serial.println("Modo ESP8266 deep sleep durante 5 minutos");
  ESP.deepSleep(3e8); // 10e6 es 10.000.000 microsegundos
}



void loop() {

}
