//Biblioteca do controle de PS4
#include <PS4Controller.h>
//Biblioteca para controle de servo p/ brushless
#include <ESP32Servo.h>

char ipControle[18] = "9F:56:45:F8:96:18"; //MAC address atrubuído ao controle
int analogicoMargemDeErro = 30; //definiçao do ponto morto

//Pinos das ESCs
int ESCRPinL = 32;   
int ESCRPinR = 33; 

//PWM
int frequenciaESC = 48;
int Ppm_Min_Throttle = 1040;
int Ppm_Max_Throttle = 1960;
unsigned long tempoMilisMax = 6;

Servo ESCL;
Servo ESCR;

int inv = 1; //Permite inverter a pilotagem conforme o lado do robo que esta para cima
void motors_control(int linear, int angular) {
  int result_R = linear - angular; //ao somar o angular com linear em cada motor conseguimos a ideia de direcao do robo
  int result_L = linear + angular;
  //não envia valores de potencia abaixo de 15, que não são fortes o suficiente para mover o robo
  if(result_R<15 && result_R >-15) result_R=0; 
  if(result_L<15 && result_L >-15 ) result_L=0;
  //Não permite valores superiores a 255 ou inferiores a -255
  if(result_R >255 ) result_R= 254; 
  if(result_R<-255 ) result_R=-254;
  if(result_L >255 ) result_L= 254;
  if(result_L<-255 ) result_L=-254;  
  
  //manda para a funcao motor um valor de -255 a 255, o sinal signifca a direcao  
  motor_A(result_R); 
  motor_B(result_L);
}

void motor_A(int speedA){  // se o valor for positivo gira para um lado e se for negativo troca o sentido
  speedA = map(speedA, -255, 255, 0, 180);  
  ESCR.write(speedA);
}

void motor_B(int speedB){
  speedB = map(speedB, -255, 255, 0, 180);  
  ESCL.write(speedB);
}


void setup(void) {
  Serial.begin(115200);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens


  PS4.begin(ipControle);
  Serial.println("Ready.");
  
  ESCL.setPeriodHertz(frequenciaESC);
  ESCL.attach(ESCRPinL, Ppm_Min_Throttle, Ppm_Max_Throttle);
  ESCR.setPeriodHertz(frequenciaESC);
  ESCR.attach(ESCRPinR, Ppm_Min_Throttle, Ppm_Max_Throttle); 

  while(PS4.isConnected()!= true){
  delay(20);}
}

void loop() {

  while(PS4.isConnected()) {
  //motors_control(linear_speed*multiplicador, angular_speed* multiplicador2);
  // Multiplicadcor = 1.8 para aumentar a velocidade linear, o quao rapido o robo vai ser
  // Multiplicadcor2 = multiplic_curva, parametro que varia de 1 ate a 2.3 para suavisar as curvas em alta velocidade
    if(PS4.LStickY()<-analogicoMargemDeErro || PS4.LStickY()>analogicoMargemDeErro){
      motors_control((1.9)*inv*PS4.LStickY(), PS4.RStickX()/(1.2));

    }else { // Controle sobre valores pequenos devido a problemas na funcao map
      motors_control((1.8)*inv*PS4.LStickY(), (1.2)*PS4.RStickX());
    }

  //Sentido de locomocao invertido
    if(PS4.Down()){
      inv = -1;
      delay(100);
    } 
    if(PS4.Up()){
      inv = 1;
      delay(100);
    }
  }
    
  //Failsafe
  if(PS4.isConnected()!= true){
  motors_control(0,0);
  Serial.println("Restart");
  PS4.end();
  ESP.restart();
  delay(20);
  }
}
 