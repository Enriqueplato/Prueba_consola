#include <ESP8266WiFi.h>
const char* ssid = "nombre de red";
const char* password = "clave de red";
int ledPin = 2; // Arduino standard is GPIO13 but lolin nodeMCU is 2 http://www.esp8266.com/viewtopic.php?f=26&t=13410#p61332
WiFiServer server(80);

//Configuración
void setup() {
  //velocidad del serial
  Serial.begin(115200);
  delay(10);
  //configuración del Pin IO para encender el LED
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
  // Mensajes de Conexión a la red inalámbrica
  Serial.println();
  Serial.println();
  Serial.print("Conectando a ");
  Serial.println(ssid);
  //configuración de la conexión a la red inalámbrica
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Conectado a WiFi");
  //Inicializar el servidor para conectarse desde otros dispositivos
  server.begin();
  Serial.println("Servidor inicializado");
  //Imprime la dirección IP a la que se pueden conectar los otros dispositivos
  Serial.print("Utiice esta URL para conectarse: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");

}

void loop() {
  //Verifica si se ha conectado algún cliente
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  //Espera hasta que el cliente envía algún dato
  Serial.println("nuevo cliente");
  while(!client.available()){
    delay(1);
  }
  //Lee la primera línea del REQUEST
  String request = client.readStringUntil('r');
  Serial.println(request);
  client.flush();
  // Match the request
  //nodeMCU LoLin utiliza una señal invertida para el LED.
  //Nivel bajo, enciende el LED
  //Nivel alto, apaga el LED
  int value = HIGH; //Inicialmente apagado
  if (request.indexOf("/LED=OFF") != -1) {
    digitalWrite(ledPin, HIGH);
    value = HIGH;
  }
  if (request.indexOf("/LED=ON") != -1) {
    digitalWrite(ledPin, LOW);
    value = LOW;
  }
  //Selecciona ledPin de acuerdo al REQUEST
  //digitalWrite(ledPin, value);
  //devuelve el RESPONSE en formato HTML
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); 
  client.println("<!DOCTYPE HTML>");
  client.println("<html><h1>Ejemplo NodeMCU</h1>");
  client.print("El LED esta: <b>");
  //High=off
  //Low=on

  if(value == HIGH) {
    client.print("Apagado");
  }
  else {
    client.print("Encendido");
  }
  client.println("</b><p></p>");
  client.println("<a href=\"/LED=ON\"\"><button>Encender </button></a>");
  client.println("<a href=\"/LED=OFF\"\"><button>Apagar </button></a><br />");
  client.println("</html>");

  delay(1);
  Serial.println("Cliente desconectado");
  Serial.println("");
}
