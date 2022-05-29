/*
 * Elaborado por: Antonio del Angel Moreno Carbajal
 * 
 * Basado en el codigo de: Hugo Escapelo
 * 
 * Este programa envía datos por Internet a través del protocolo MQTT y se comunica 
 * con una tarjeta Arduino Mega mediante comunicación serial para realizar las pruebas del
 * controlador difuso.
 * 
 * La tarjeta ESP32 funciona como un intermediario entre Node-RED y el controlador de logica
 * difusa implementado en el Arduino para controlar la planta.
 * 
 * El programa envía el dato necesario para el funcionamiento adecuado de este proyecto 
 * por comunicación serial al Arduino, por lo que el resto de comandos que tienen que ver 
 * con Serial han sido eliminados.
 * 
 * Si deseas implementar este programa necesitaras: El nombre de tu red (SSID), la contraseña 
 * de tu modem, por supuesto conexión a internet y tener instalado Node-RED en tu dispositivo
 * para manipular y visualizar los datos enviados y recibidos de este proyecto.
 */

/********Bibliotecas********/
#include <WiFi.h>                         // Biblioteca para el control de WiFi
#include <PubSubClient.h>                 //Biblioteca para conexion MQTT

/********Datos de WiFi********/
const char* ssid = "INFINITUM4956_2.4";   // Aquí debes poner el nombre de tu red
const char* password = "Carbajal2104";    // Aquí debes poner la contraseña de tu red

/********Datos del broker MQTT********/
const char* mqtt_server = "192.168.1.70"; // Si estas en una red local, coloca la IP asignada, en caso contrario, coloca la IP publica
IPAddress server(192,168,1,70);

/********Objetos********/
WiFiClient espClient;                     // Este objeto maneja los datos de conexion WiFi
PubSubClient client(espClient);           // Este objeto maneja los datos de conexion al broker

/********Variables********/
int ledPin = 4;                           // Para indicar el estatus de conexión
int ledPin2 = 2;                          // Para mostrar mensajes recibidos
long timeNow, timeLast;                   // Variables de control de tiempo no bloqueante
int wait = 5000;                          // Indica la espera cada 5 segundos para envío de mensajes MQTT
String mensajeArduino;                    // Almacena la cadena de caracteres proveniente del Arduino Mega

/********Configuración del programa********/
void setup() {
  Serial.begin (115200);                  // Iniciar comunicación serial a la misma velocidad que el Arduino
  pinMode (ledPin, OUTPUT);               // Configurando como salida al LED de estatus de conexión
  pinMode (ledPin2, OUTPUT);              // Configurando como salida al LED de mensajes recibidos
  digitalWrite (ledPin, HIGH);            // LED de estatus encendido
  digitalWrite (ledPin2, HIGH);           // LED de mensaje encendido
   
  WiFi.begin(ssid, password);             // Esta es la función que realiz la conexión a WiFi
 
  while (WiFi.status() != WL_CONNECTED)   // Este bucle espera a que se realice la conexión
  { 
    digitalWrite (ledPin, HIGH);
    delay(500);                           
    digitalWrite (ledPin, LOW);
    delay (5);
  }
  
  if (WiFi.status () > 0)                 // Si se logro la conexión, encender led
  {
    digitalWrite (ledPin, HIGH);
  }
  
  delay (1000);                           // Espera antes de iniciar la comunicación con el broker

  /********Conexión con el broker MQTT********/
  client.setServer(server, 1883);         // Conectarse a la IP del broker en el puerto indicado
  client.setCallback(callback);           // Activar función de CallBack, permite recibir mensajes MQTT y ejecutar funciones a partir de ellos
  delay(1500);                            // Esta espera es preventiva, espera a la conexión para no perder información

  timeLast = millis ();                   // Inicia el control de tiempo
}                                         // fin del void setup ()

/********Cuerpo del programa, bucle principal********/
void loop() {
  if (!client.connected())                // Verificar siempre que haya conexión al broker
  {
    reconnect();                          // En caso de que no haya conexión, ejecutar la función de reconexión, definida despues del void setup ()
  }                                       // fin del if (!client.connected())
  
  client.loop();                          // Esta función ejecuta de manera no bloqueante las funciones necesarias para la comunicación con el broker

  if (Serial.available() > 0)             // Leer el puerto serial solo si esta disponible
  {
    mensajeArduino = Serial.readString(); // Almacena el mensaje proveniente del puerto serial en mensajeArduino como tipo String
  }
  
  timeNow = millis();                     // Control de tiempo para esperas no bloqueantes
  
  if (timeNow - timeLast > wait)          // Manda un mensaje por MQTT cada cinco segundos
  { 
    timeLast = timeNow;                   // Actualización de seguimiento de tiempo
    String json = mensajeArduino;         // Se construye el string correspondiente a formato JSON tomando el dato de la variable
    int str_len = json.length() + 1;      // Se calcula la longitud del string
    char char_array[str_len];             // Se crea un arreglo de caracteres de dicha longitud
    json.toCharArray(char_array, str_len);// Se convierte el string a char array    
    client.publish("Controlador/Difuso/SICIoT/AJA", char_array); // Esta es la función que envía los datos por MQTT, especifica el tema y el valor
  }                                       // Fin del if (timeNow - timeLast > wait)
}                                         // Fin del void loop ()

/********Aquí puede agregarse funciones extras del usuario********/

void callback(char* topic, byte* message, unsigned int length) // Esta función permite tomar acciones en caso de que se reciba un mensaje correspondiente a un tema al cual se hará una suscripción
{
  String messageTemp;                     // Se declara la variable en la cual se generará el mensaje completo  
  int valorMQTT;                          // Variable que almacena el dato proveniente de Node-RED como tipo entero
  
  for (int i = 0; i < length; i++)        // Se imprime y concatena el mensaje
  {  
    messageTemp += (char)message[i];
  }
  valorMQTT = messageTemp.toInt();        // Convierte la cadena de caracteres recibidos a un numero de tipo entero

  /********Agrega las funciones que requieras para actuar segun al recibir un mensaje MQTT********/

  /********El ESP32 está suscrito al tema Controlador/IoT/output********/
  if (String(topic) == "Controlador/IoT/output")    // En caso de recibirse mensaje en el tema esp32/output
  {  
    Serial.println(valorMQTT);            // Envia por puerto serial el dato proveniente de Node-RED para mover la posición de la planta
  }                                       // Fin del if (String(topic) == "Controlador/IoT/output")
}                                         // Fin del void callback

/********Función para reconectarse********/
void reconnect()                          // Bucle hasta lograr conexión
{
  while (!client.connected())             // Pregunta si hay conexión
  {           
    if (client.connect("ESP8266Client"))  // Pregunta por el resultado del intento de conexión
    { 
      client.subscribe("Controlador/IoT/output");   // Esta función realiza la suscripción al tema
    }
    else                                  // En caso de que la conexión no se logre
    {  
      delay(5000);                        // Espera de 5 segundos bloqueante
    }                                     // Fin del if - else
  }                                       // Fin del bucle while (!client.connected())
}                                         // Fin de void reconnect()
