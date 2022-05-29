String lectura;
int distancia=0;;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(13,OUTPUT);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  //String json = "{\\\"dinput\\\":\\\""+String(10)+"\\\",\\\"pos\\\":\\\""+String(10.1)+"\\\",\\\"error\\\":\\\""+String(2.0)+"\\\",\\\"u\\\":\\\""+String(0.5)+"\\\"}";
  
  if (Serial.available() > 0)
  {
    lectura = Serial.readString();
    distancia = lectura.toInt();
  }
  
  String json = "{\"distancia\":\""+String(distancia)+"\",\"pos\":\""+String(10.1)+"\",\"error\":\""+String(2.0)+"\",\"u\":\""+String(0.5)+"\"}";
  //String json = "{\'distancia\':\'"+String(10)+"\',\'pos\':\'"+String(10.1)+"\',\'error\':\'"+String(2.0)+"\',\'u\':\'"+String(0.5)+"}";

  //digitalWrite(13,HIGH);
  Serial.print(json);
  //delay(5);
  //digitalWrite(13,LOW);
  delay (2000);
}
