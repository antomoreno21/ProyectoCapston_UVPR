/*
 * 
 * 
 */
#include <ESP32Encoder.h>

ESP32Encoder encoder;

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

// Mensaje
String mensaje;

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

void setup() {
  pinMode(26,OUTPUT);
  pinMode(25,OUTPUT);
  pinMode(2,INPUT);
  Serial.begin(115200); 
  
  //ESP32Encoder::useInternalWeakPullResistors=DOWN;
  // Enable the weak pull up resistors
  ESP32Encoder::useInternalWeakPullResistors=UP;

  // use pin 19 and 18 for the first encoder
  encoder.attachHalfQuad(19, 18);
  
  // set starting count value after attaching
  encoder.setCount(0);
}

void loop() {
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

  mensaje = "{\"dinput\":\""+String(dinput)+"\"pos\":\""+String(pos)+"\",\"error\":\""+String(error)+"\",\"u\":\""+String(u)+"\"}";
  //mensaje = "{\"temp\":\""+String(temp)+"\",\"humedad\":\""+String(humedad)+"\"}";
//  Serial.print(dinput);
//  Serial.print("\t");
//  Serial.print(pos);
//  Serial.print("\t");
//  Serial.print(error);
//  Serial.print("\t");
//  Serial.println(u);
  Serial.println(mensaje);
  
  delay(10);
  
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
}

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
