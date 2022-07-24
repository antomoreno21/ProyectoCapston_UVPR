/*
 * Elaborado por:
 *  Antonio del Angel Moreno Carbajal
 *  José Roman Martinez García 
 *  Arturo Marrero Mendez
 * 
 * Fecha:
 *  AGREGAR
 * 
 * Descripción:
 *  AGREGAR
 * 
 * Componentes:
 *  Arduino MEGA 2560 
 *  Encoder
 * 
 * Conexión:
 *  AGREGAR
 */

int stpnt=A0;
volatile long int contador=0;                 //Contador de pulsos
byte ant=0, act = 0;                          //variable anterior y actual
int ppr = 1023;
int pot = 0;
float integral = 0.0;
float ki = 5.0;
float IM = 24.0;
float pot1 = 0.0;
float pi_ = 3.14159;

/***************Set-point***************/
float sp = 0.5*pi_;

/***************Convertir cuentas a radianes***************/
float esc = 2*pi_/ppr;
float input;
float dinput=0.0;
float dVs,dMs,ds,dm,db,dMb,dVb;
float Vs,Ms,s,m,b,Mb,Vb;
float u,denominador,pos;
float HOLD,MLR,MLL,ML,MR,MFR,MFL;
float PHOLD=0.0;

/***************Desplazamiento a la izquierda***************/
float PMLL=-50.33;
float PML=-75.66;
float PMFL=-100.0;

/***************Desplazamiento a la derecha***************/
float PMLR=50.33;
float PMR=75.66;
float PMFR=100.0;
float R1,R2,R3,R4,R5,R6,R7,R8,R9,R10,R11,R12,R13,R14,R15,
      R16,R17,R18,R19,R20,R21,R22,R23,R24,R25,R26,R27,R28,
      R29,R30,R31,R32,R33,R34,R35,R36,R37,R38,R39,R40,R41,  
      R42,R43,R44,R45,R46,R47,R48,R49;

/***************Lectura del puerto Serial***************/
String lectura;
String json;

/***************Variables de control de tiempo no bloqueante***************/
long timeNow, timeLast; 
int wait = 100;  //Indica la espera cada 100 milisegundos

/***************AGREGAR TEXTO***************/
float error;
int inpt;

float triangular(float a,float m,float b,float x)
{
  float mu = 0.0;
  if(x>=a && x <=m){
    mu = (x-a)/(m-a);
  }
  else if(x>=m && x <= b){
    mu = (x-b)/(m-b);
  }
  else{
    mu = 0.0;}
  
  return(mu);
}

float trapezoidal(float a,float b,float c, float d,float x)
{
  float mu = 0.0;
  
  if(x>=a && x <b){
    mu = (x-a)/(b-a);
  }
  else if(x>=b && x<c){
    mu = 1.0;
  }
  else if(x>=c && x <= d){
    mu = (x-d)/(c-d);
  }
  else{
    mu = 0.0;
  }
  
  return(mu);
}

void setup() {
  pinMode(12,OUTPUT);
  pinMode(13,OUTPUT);
  pinMode(2,INPUT);
  Serial.begin(115200);
  attachInterrupt(digitalPinToInterrupt(18), encoder, CHANGE);  //Canal B
  attachInterrupt(digitalPinToInterrupt(19), encoder, CHANGE);  //Canal A
  timeLast = millis (); // Inicia el control de tiempo
}

void loop(){
  if (Serial.available() > 0){
    lectura = Serial.readString();
    dinput = lectura.toInt();
    dinput = (float) dinput;
  }
  
  input=contador*116/63242;
  Vs=trapezoidal(0.0,0.0,10.0,19.33,input);
  Ms=triangular(0.0,19.33,38.66,input);
  s=triangular(19.33,38.66,58,input);
  m=triangular(36.66,58,77.33,input);
  b=triangular(58,77.33,93.66,input);
  Mb=triangular(77.42,93.75,116.0,input);
  Vb=trapezoidal(93.66,106,116,116.0,input);
  dVs=trapezoidal(0.0,0.0,10.0,19.33,dinput);
  dMs=triangular(0.0,19.33,38.66,dinput);
  ds=triangular(19.33,38.66,58,dinput);
  dm=triangular(36.66,58,77.33,dinput);
  db=triangular(58,77.33,93.66,dinput);
  dMb=triangular(77.42,93.75,116.0,dinput);
  dVb=trapezoidal(93.66,106,116.0,116.0,dinput);
  
  // Diagonal principal HOLD
  R1=Vs*dVs;
  R2=Ms*dMs;
  R3=s*ds;
  R4=m*dm;
  R5=b*db;
  R6=Mb*dMb;
  R7=Vb*dVb;
  
  //Cálcuando el maximo de los minimos de hold
  HOLD=MAX(R1,R2,R3);
  HOLD=MAX(HOLD,R4,R5);
  HOLD=MAX(HOLD,R6,R7);

  //MLR
  R8=Vs*dMs;
  R9=Ms*ds;
  R10=s*dm;
  R11=m*db;
  R12=b*dMb;
  R13=Mb*dVb;
  
  //Cálcuando el maximo de los minimos de MLR
  MLR=MAX(R8,R9,R10);
  MLR=MAX(MLR,R11,R12);
  MLR=MAX(MLR,R13,0);
  
  //MLL
  R14=Ms*dVs;
  R15=s*dMs;
  R16=m*ds;
  R17=b*dm;
  R18=Mb*db;
  R19=Vb*dMb;
  
  //Cálcuando el maximo de los minimos de MLL
  MLL=MAX(R14,R15,R16);
  MLL=MAX(MLL,R17,R18);
  MLL=MAX(MLL,R19,0);

  //ML
  R20=s*dVs;
  R21=m*dMs;
  R22=b*ds;
  R23=Mb*dm;
  R24=Vb*db;

  //Cálcuando el maximo de los minimos de ML
  ML=MAX(R20,R21,R22);
  ML=MAX(ML,R23,R24);

  //MR
  R25=Vs*ds;
  R26=Ms*dm;
  R27=s*db;
  R28=m*dMb;
  R29=b*dVb;
  
  //Cálcuando el maximo de los minimos de MR
  MR=MAX(R25,R26,R27);
  MR=MAX(MR,R28,R29);
  
  //MFR
  R30=Vs*dm;
  R31=Vs*db;
  R32=Vs*dMb;
  R33=Vs*dVb;
  R34=Ms*db;
  R35=Ms*dMb;
  R36=Ms*dVb;
  R37=s*dMb;
  R38=s*dVb;
  R39=m*dVb;
  
  //Cálcuando el maximo de los minimos de MFR
  MFR=MAX(R30,R31,R32);
  MFR=MAX(MFR,R33,R34);
  MFR=MAX(MFR,R35,R36);
  MFR=MAX(MFR,R37,R38);
  MFR=MAX(MFR,R39,0);
  
  // MFL
  R40=dVs*s;
  R41=dVs*b;
  R42=dVs*Mb;
  R43=dVs*Vb;
  R44=dMs*b;
  R45=dMs*Mb;
  R46=dMs*Vb;
  R47=ds*Mb;
  R48=ds*Vb;
  R49=dm*Vb;
  
  //Cálcuando el maximo de los minimos de MFL
  MFL=MAX(R40,R41,R42);
  MFL=MAX(MFL,R43,R44);
  MFL=MAX(MFL,R45,R46);
  MFL=MAX(MFL,R47,R48);
  MFL=MAX(MFL,R49,0);
  denominador=HOLD+MLR+MLL+ML+MR+MFR+MFL;
  
  if(denominador==0){
    u=0.0;
  }
  else{
    u=(MLR*PMLR+MLL*PMLL+ML*PML+MR*PMR+MFR*PMFR+MFL*PMFL)/denominador;
  }
  
  error=dinput-pos;
  
  if((integral < IM)&&(integral>-IM)){
    integral = integral + ki*0.005*error;
  }
  else{
    if(integral>=IM)
      integral=0.95*IM;
    
    if(integral<=(-IM))
      integral=-0.95*IM;      
  }

  u = u+integral;
  pos=float(contador)*116.0/63242.0;
  timeNow = millis(); // Control de tiempo para esperas no bloqueantes

  // Manda un mensaje por Serial cada 100 milisegundos
  if (timeNow - timeLast > wait) { 
    timeLast = timeNow; // Actualización de seguimiento de tiempo
    String json = "{\"dinput\":\""+String(dinput)+"\",\"pos\":\""+String(pos)+
                  "\",\"error\":\""+String(error)+"\",\"u\":\""+String(u)+"\"}";
    Serial.println(json);
  }
  
  if(u>=0){
    u=u*255/100;
    analogWrite(13,u);
  }
  else{
    u=u*-255/100;
    analogWrite(12,u);
  }
  delay(10);
}

float MAX(float num1, float num2, float num3) {
  float resultado = 0.0;
  if (num1 >= num2 && num1 >= num3){
    resultado = num1;
  } 
  else if (num2 >= num1 && num2 >= num3){
    resultado = num2;
  } 
  else if (num3 >= num1 && num3 >= num2){
    resultado = num3;
  }
    return resultado;
}

void areatriangle(float a, float b, float c, float mi){
  float xab,xbc,major,minor,area;

  //Despejando los parametros de x
  xab=(b-a)*mi+a;
  xbc=(b-c)*mi+c;

  //Cálculo de bases
  major=c-a; //Calculando la base mayor
  minor=xbc-xab; //Calculando la base menor

  if (mi>=1){
    area=major*mi/2;
  }
  else{
    area=mi*(minor+major)/2;
  }
}

void encoder(){
  ant = act;                    //Guardar el valor actual en la variable ant
  act = PIND&12;                //enmascaramiento del valor actual
  
  //cuenta ascendente
  if(ant == 12 && act == 4)  
    contador++;
    
  //cuenta descendente
  if(ant == 4 && act == 12) 
    contador--;
}
