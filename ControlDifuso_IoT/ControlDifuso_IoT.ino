/*
 * Elaborado por:
 *  Antonio del Angel Moreno Carbajal
 *  Jose Roman Martinez Garcia
 *  Arturo Marrero Mendez
 */

//Bibliotecas del programa
#include <ESP32Encoder.h>
#include <PubSubClient.h> //Biblioteca para conexion MQTT
#include <WiFi.h>

//Datos de WiFi
const char* ssid = "INFINITUMj2tf";  // Aquí debes poner el nombre de tu red
const char* password = "38815dc9b3";  // Aquí debes poner la contraseña de tu red

//Datos del broker MQTT
const char* mqtt_server = "192.168.1.64"; // Si estas en una red local, coloca la IP asignada, en caso contrario, coloca la IP publica
IPAddress server(192,168,1,64);

// Objetos
WiFiClient espClient; // Este objeto maneja los datos de conexion WiFi
PubSubClient client(espClient); // Este objeto maneja los datos de conexion al broker
ESP32Encoder encoder;


// Variables del sistema no bloqueante
int ledPin = 3;  // Para indicar el estatus de conexión
int ledPin2 = 2; // Para mostrar mensajes recibidos
long timeNow, timeLast; // Variables de control de tiempo no bloqueante
int wait = 5000;  // Indica la espera cada 5 segundos para envío de mensajes MQTT

/********** Variables del control difuso**********/
int stpnt = A0;
int ppr = 1023;
int pot = 0;
float integral = 0.0;
float ki = 5.0;
float IM = 24.0;
float pot1 = 0.0;
float pi_ = 3.14159;

//Set-point
float sp = 0.5*pi_;

//convertir cuentas a radianes
float esc = 2*pi_/ppr;
float input;
float dinput = 0.0;
float dVs,dMs,ds,dm,db,dMb,dVb;
float Vs,Ms,s,m,b,Mb,Vb;
float u,denominador,pos;
float sHOLD,sMLR,sMLL,sML,sMR,sMFR=0.0,sMFL;
float PHOLD=0.0; // Mantener.

//Desplazamiento a la izquierda
float PMLL=-50.33;
float PML=-75.66;
float PMFL=-100.0;

//Desplazamiento a la derecha
float PMLR=50.33;
float PMR=75.66;
float PMFR=100.0;
float R1,R2,R3,R4,R5,R6,R7,R8,R9,R10,R11,R12,R13,R14,R15,R16,R17,R18,R19,R20,R21,R22,R23,R24,R25,R26,R27,R28,R29,R30,R31,R32,R33,R34,R35,R36,R37,R38,R39,R40,R41,R42,R43,R44,R45,R46,R47,R48,R49;

// Error
float error;
int inpt;

float triangular(float a,float m,float b,float x)
{
   float mu = 0.0;
   if(x>=a && x <=m)
   {
    mu = (x-a)/(m-a);
   }
   else if(x>=m && x <= b)
   {
    mu = (x-b)/(m-b);
   }
   else
   {
    mu = 0.0;
   }
   return(mu);
}

float trapezoidal(float a,float b,float c, float d,float x)
{
   float mu = 0.0;
   if(x>=a && x <b)
   {
    mu = (x-a)/(b-a);
   }
   else if(x>=b && x<c)
   {
    mu = 1.0;
   }
   else if(x>=c && x <= d)
   {
    mu = (x-d)/(c-d);
   }
   else
   {
    mu = 0.0;
   }
   return(mu);
}

// Inicialización del programa
void setup() {
  // Iniciar comunicación serial
  Serial.begin (115200);
  pinMode (ledPin, OUTPUT);
  pinMode (ledPin2, OUTPUT);
  // Entradas y salidas del controlador difuso
  pinMode(25,OUTPUT);
  pinMode(26,OUTPUT);
  pinMode(5,INPUT); 
  // Encoder del ESP32
  encoder.attachHalfQuad(19, 18);
  encoder.setCount(0);
  // Iniciando LED apagados
  digitalWrite (ledPin, LOW);
  digitalWrite (ledPin2, HIGH);
  // Visualizar la conexion a WiFi en Monitor Serial
  Serial.println();
  Serial.println();
  Serial.print("Conectar a ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password); // Esta es la función que realiz la conexión a WiFi
 
  while (WiFi.status() != WL_CONNECTED) { // Este bucle espera a que se realice la conexión
    digitalWrite (ledPin, HIGH);
    delay(500); //dado que es de suma importancia esperar a la conexión, debe usarse espera bloqueante
    digitalWrite (ledPin, LOW);
    Serial.print(".");  // Indicador de progreso
    delay (5);
  }
  
  // Cuando se haya logrado la conexión, el programa avanzará, por lo tanto, puede informarse lo siguiente
  Serial.println();
  Serial.println("WiFi conectado");
  Serial.println("Direccion IP: ");
  Serial.println(WiFi.localIP());

  // Si se logro la conexión, encender led
  if (WiFi.status () > 0){
  digitalWrite (ledPin, HIGH);
  }
  
  delay (1000); // Esta espera es solo una formalidad antes de iniciar la comunicación con el broker

  // Conexión con el broker MQTT
  client.setServer(server, 1883); // Conectarse a la IP del broker en el puerto indicado
  client.setCallback(callback); // Activar función de CallBack, permite recibir mensajes MQTT y ejecutar funciones a partir de ellos
  delay(1500);  // Esta espera es preventiva, espera a la conexión para no perder información

  timeLast = millis (); // Inicia el control de tiempo
}// fin del void setup ()

// Cuerpo del programa, bucle principal
void loop() {
  //Verificar siempre que haya conexión al broker
  if (!client.connected()) {
    reconnect();  // En caso de que no haya conexión, ejecutar la función de reconexión, definida despues del void setup ()
  }// fin del if (!client.connected())
  client.loop(); // Esta función es muy importante, ejecuta de manera no bloqueante las funciones necesarias para la comunicación con el broker

  /********** Algoritmo difuso **********/
  input = encoder.getCount()*116/63242;
  pot = analogRead(A0);
  dinput = 20;//pot * 5.0 * 23.2 / 1023.0;
  //inpt = analogRead(A0)/10;
  //input = float(inpt/10);
  Vs = trapezoidal(0.0,0.0,10.0,19.33,input);
  Ms = triangular(0.0,19.33,38.66,input);
  s = triangular(19.33,38.66,58,input);
  m = triangular(36.66,58,77.33,input);
  b = triangular(58,77.33,93.66,input);
  Mb = triangular(77.42,93.75,116.0,input);
  Vb = trapezoidal(93.66,106,116,116.0,input);
  dVs = trapezoidal(0.0,0.0,10.0,19.33,dinput);
  dMs = triangular(0.0,19.33,38.66,dinput);
  ds = triangular(19.33,38.66,58,dinput);
  dm = triangular(36.66,58,77.33,dinput);
  db = triangular(58,77.33,93.66,dinput);
  dMb = triangular(77.42,93.75,116.0,dinput);
  dVb = trapezoidal(93.66,106,116.0,116.0,dinput);
  
  // Diagonal principal HOLD
  R1 = Vs*dVs;
  R2 = Ms*dMs;
  R3 = s*ds;
  R4 = m*dm;
  R5 = b*db;
  R6 = Mb*dMb;
  R7 = Vb*dVb;
  
  //Cálcuando el maximo de los minimos de hold
  sHOLD = MAX(R1,R2,R3);
  sHOLD = MAX(sHOLD,R4,R5);
  
  sHOLD = MAX(sHOLD,R6,R7);
  
  //MLR
  R8 = Vs*dMs;
  R9 = Ms*ds;
  R10 = s*dm;
  R11 = m*db;
  R12 = b*dMb;
  R13 = Mb*dVb;
  
  //Cálcuando el maximo de los minimos de MLR
  sMLR = MAX(R8,R9,R10);
  sMLR = MAX(sMLR,R11,R12);
  sMLR = MAX(sMLR,R13,0);
  
  //MLL
  R14 = Ms*dVs;
  R15 = s*dMs;
  R16 = m*ds;
  R17 = b*dm;
  R18 = Mb*db;
  R19 = Vb*dMb;
  
  //Cálcuando el maximo de los minimos de MLL
  sMLL = MAX(R14,R15,R16);
  sMLL = MAX(sMLL,R17,R18);
  sMLL = MAX(sMLL,R19,0);
  
  //ML
  R20 = s*dVs;
  R21 = m*dMs;
  R22 = b*ds;
  R23 = Mb*dm;
  R24 = Vb*db;
  
  //Cálcuando el maximo de los minimos de ML
  sML = MAX(R20,R21,R22);
  sML = MAX(sML,R23,R24);
  
  //MR
  R25 = Vs*ds;
  R26 = Ms*dm;
  R27 = s*db;
  R28 = m*dMb;
  R29 = b*dVb;
  
  //Cálcuando el maximo de los minimos de MR
  sMR = MAX(R25,R26,R27);
  sMR = MAX(sMR,R28,R29);
  
  //MFR
  R30 = Vs*dm;
  R31 = Vs*db;
  R32 = Vs*dMb;
  R33 = Vs*dVb;
  R34 = Ms*db;
  R35 = Ms*dMb;
  R36 = Ms*dVb;
  R37 = s*dMb;
  R38 = s*dVb;
  R39 = m*dVb;
  
  //Cálcuando el maximo de los minimos de MFR
  sMFR = MAX(R30,R31,R32);
  sMFR = MAX(sMFR,R33,R34);
  sMFR = MAX(sMFR,R35,R36);
  sMFR = MAX(sMFR,R37,R38);
  sMFR = MAX(sMFR,R39,0);
  
  // MFL
  R40 = dVs*s;
  R41 = dVs*b;
  R42 = dVs*Mb;
  R43 = dVs*Vb;
  R44 = dMs*b;
  R45 = dMs*Mb;
  R46 = dMs*Vb;
  R47 = ds*Mb;
  R48 = ds*Vb;
  R49 = dm*Vb;
  
  //Cálcuando el maximo de los minimos de MFL
  sMFL = MAX(R40,R41,R42);
  sMFL = MAX(sMFL,R43,R44);
  sMFL = MAX(sMFL,R45,R46);
  sMFL = MAX(sMFL,R47,R48);
  sMFL = MAX(sMFL,R49,0);
  denominador = sHOLD + sMLR + sMLL + sML + sMR + sMFR + sMFL;
  
  if(denominador == 0)
  {
    u = 0.0;
  }
  else
  {
    u = (sMLR*PMLR+sMLL*PMLL+sML*PML+sMR*PMR+sMFR*PMFR+sMFL*PMFL)/denominador;
  }
  
  error = dinput-pos;
  
  if((integral < IM)&&(integral > -IM))
  {
    integral = integral + ki*0.005*error;
  }
  else
  {
    if(integral >= IM)
      integral = 0.95*IM;
    if(integral <= (-IM))
      integral=-0.95*IM;
  }
  
  u = u + integral;
  pos = float(encoder.getCount())*116.0/63242.0;
  // Parte integradora
  if(u >= 0)
  {
    u = u*255/100;
    analogWrite(25,u);
  }
  else
  {
    u=u*-255/100;
    analogWrite(26,u);
  }
  delay(10);
  /* Fin del algoritmo difuso */
  
  timeNow = millis(); // Control de tiempo para esperas no bloqueantes
  if (timeNow - timeLast > wait) { // Manda un mensaje por MQTT cada cinco segundos
    timeLast = timeNow; // Actualización de seguimiento de tiempo

    //Se construye el string correspondiente al JSON que contiene 3 variables
    String json = "{\"dinput\":\""+String(dinput)+"\",\"pos\":\""+String(pos)+"\",\"error\":\""+String(error)+"\",\"u\":\""+String(u)+"\"}";
    Serial.println(json); // Se imprime en monitor solo para poder visualizar que el string esta correctamente creado
    int str_len = json.length() + 1;//Se calcula la longitud del string
    char char_array[str_len];//Se crea un arreglo de caracteres de dicha longitud
    json.toCharArray(char_array, str_len);//Se convierte el string a char array    
    client.publish("Controlador/Difuso/IoT/AJA", char_array); // Esta es la función que envía los datos por MQTT, especifica el tema y el valor
  }// fin del if (timeNow - timeLast > wait)
}// fin del void loop ()

// Esta función permite tomar acciones en caso de que se reciba un mensaje correspondiente a un tema al cual se hará una suscripción
void callback(char* topic, byte* message, unsigned int length) {
  // Indicar por serial que llegó un mensaje
  Serial.print("Llegó un mensaje en el tema: ");
  Serial.print(topic);

  // Concatenar los mensajes recibidos para conformarlos como una varialbe String
  String messageTemp; // Se declara la variable en la cual se generará el mensaje completo  
  for (int i = 0; i < length; i++) {  // Se imprime y concatena el mensaje
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }

  // Se comprueba que el mensaje se haya concatenado correctamente
  Serial.println();
  Serial.print ("Mensaje concatenado en una sola variable: ");
  Serial.println (messageTemp);

  // En esta parte puedes agregar las funciones que requieras para actuar segun lo necesites al recibir un mensaje MQTT

  // Ejemplo, en caso de recibir el mensaje true - false, se cambiará el estado del led soldado en la placa.
  // El NodeMCU está suscrito al tema esp/output
  if (String(topic) == "esp32/output") {  // En caso de recibirse mensaje en el tema esp32/output
    if(messageTemp == "true"){
      Serial.println("Led encendido");
      digitalWrite(ledPin2, LOW);
    }// fin del if (String(topic) == "esp32/output")
    else if(messageTemp == "false"){
      Serial.println("Led apagado");
      digitalWrite(ledPin2, HIGH);
    }// fin del else if(messageTemp == "false")
  }// fin del if (String(topic) == "esp32/output")
  
}// fin del void callback

// Función para reconectarse
void reconnect() {
  // Bucle hasta lograr conexión
  while (!client.connected()) { // Pregunta si hay conexión
    Serial.print("Tratando de contectarse...");
    // Intentar reconexión
    if (client.connect("ESP8266Client")) { //Pregunta por el resultado del intento de conexión
      Serial.println("Conectado");
      client.subscribe("esp32/output"); // Esta función realiza la suscripción al tema
    }// fin del  if (client.connect("ESP8266Client"))
    else {  //en caso de que la conexión no se logre
      Serial.print("Conexion fallida, Error rc=");
      Serial.print(client.state()); // Muestra el codigo de error
      Serial.println(" Volviendo a intentar en 5 segundos");
      // Espera de 5 segundos bloqueante
      delay(5000);
      Serial.println (client.connected ()); // Muestra estatus de conexión
    }// fin del else
  }// fin del bucle while (!client.connected())
}// fin de void reconnect(

float MAX(float num1, float num2, float num3) {
  float resultado = 0.0;

  if (num1 >= num2 && num1 >= num3)
  {
    resultado = num1;
  } 
  else if (num2 >= num1 && num2 >= num3)
  {
    resultado = num2;
  } 
  else if (num3 >= num1 && num3 >= num2)
  {
    resultado = num3;
  }
  return resultado;
}

void areatriangle(float a, float b, float c, float mi){
  float xab,xbc,major,minor,area;
  
  //Despejando los parametros de x
  xab = (b-a)*mi+a;
  xbc = (b-c)*mi+c;

  //Cálculo de bases
  major = c-a; //Calculando la base mayor
  minor = xbc-xab; //Calculando la base menor
  
  if (mi >= 1)
  {
    area = major*mi/2;
  }
  else
  {
    area = mi*(minor+major)/2;
  }
}
