  //Biblioteca do controle de PS4
  #include <PS4Controller.h>
 
  //Setup pinos Locomocão Ponte H (TB6612fng)
  #define PWMA 27
  #define PWMB 26
  #define A1  13
  #define A2  12
  #define B1  33
  #define B2  25
  
  //Setup pinos brushless
  #include <Servo_ESP32.h>
  static const int brushPin = 4; 
  Servo_ESP32 servo;
  int angle;


  //Permite inverter a pilotagem conforme o lado do robo que esta para cima( 0-Normal; 1-Invertido)
  int inv = 1; 

  //Função consolidada de controle de motor - recebe o valor como uma função só e depois distribui para cada função individual
  void motors_control(int speedL, int speedR) {
    //manda para a funcao motor um valor de -255 a 255, o sinal signifca a direcao  
    motor_A(speedL*2); 
    motor_B(speedR*2);
  }

  // se o valor for positivo gira para um lado e se for negativo troca o sentido
  // Função de velocidade individual de cada motor.
  void motor_A(int speedA){  
    if(speedA>0){  
      digitalWrite(A1, 1);
      digitalWrite(A2, 0);
    }else{
      digitalWrite(A1, 0);
      digitalWrite(A2, 1);
    }
    ledcWrite(5,abs(speedA));

  }
  
  void motor_B(int speedB){
    if(speedB>0){
      digitalWrite(B1, 1);
      digitalWrite(B2, 0);
    }else{
      digitalWrite(B1, 0);
      digitalWrite(B2, 1);
    }
    ledcWrite(6,abs(speedB));

  }
  
  
  void setup(void) {
    Serial.begin(115200);
    while (!Serial)
      delay(10);
  
  //endereço IP do controle
    PS4.begin("64:64:64:64:64:64");
    Serial.println("Ready.");
   
   //Pinos PWA e PWB
    ledcAttachPin(PWMA,5);
    ledcAttachPin(PWMB,6);
  
    ledcSetup(5, 80000, 8);
    ledcSetup(6, 80000, 8);

   //Setup ponte H
    pinMode(A1, OUTPUT);
    pinMode(A2, OUTPUT);
    pinMode(B1, OUTPUT);
    pinMode(B2, OUTPUT);
    digitalWrite(A1, 0);
    digitalWrite(A2, 0);
    digitalWrite(B1, 0);
    digitalWrite(B2, 0);
  
  //Pino brushless, iniciado como parado
   servo.attach(brushPin);
   int angle = 0;
   servo.write(angle);
   Serial.println(angle);

 // Iniciando a coneção durante o setup, necessário para garantir o funcionamento
   while(PS4.isConnected()!= true){
    delay(20);}
  }
  
  void loop() {
    
    //Loop do Fail Safe
    while(PS4.isConnected()) {
    // PS4.RStickX() <0 <--
    // PS4.RStickX() >0 -->
    
    float velo=0;
    float veloInv=0;

    //Se o valor do joystick estiver entre -10 e 10, desconsideramos esse valor
    if(PS4.LStickY()<10 && PS4.LStickY()>-10){
      //Qualquer valor fora desses limites é considerado e entra no loop abaixo
      if(PS4.RStickX()>10 || PS4.RStickX()<-10){
        velo=map(abs(PS4.RStickX()),0,130,0,1); //Interpolação do valor do joystick para ficar proporcional (entre 0 e 1)
        veloInv=1-velo;//Velocidade invertida se necessário.
        
        //Virar para direita no proprio centro
        if(PS4.RStickX()>0){
          motors_control(PS4.RStickY()*velo,PS4.RStickY()*(-veloInv));
          }
          //virar para esquerda no proprio centro
          else{
           motors_control(PS4.RStickY()*(-veloInv),PS4.RStickY()*velo); 
          }
      
      }
    
    } 
    //Andando 
    //Virar para Direita
    else if(PS4.RStickX()>10){
    velo=map(PS4.RStickX(),0,130,0,1);
    veloInv=1-velo;
    motors_control((int)PS4.LStickY()*velo,(int)PS4.LStickY()*veloInv);
      
    }
    
    //Virar para Esquerda
    else if (PS4.RStickX()<-10){
    velo=map(abs(PS4.RStickX()),0,130,0,1);
    veloInv=1-velo;
    motors_control((int)PS4.LStickY()*veloInv,(int)PS4.LStickY()*velo);
    }
    
    // Somente frente e tras
    else {
      motors_control(PS4.LStickY(),PS4.LStickY());
    }
    
      //inicio do Brushless - seta o piso do valor (primeiro beep)- Botão quadrado
      if (PS4.Square()) { 
          angle=40;
          servo.write(angle);
          Serial.println(angle);
          delay(200);
      }

      //Função para arma ativa, aceleração discreta com o gatilho - Botão R1
      if (PS4.R1()){
        angle=angle+2;
        servo.write(angle);
        Serial.println(angle);
        delay(200);
    }
        //Sentido de locomocao invertido - Botão Seta para baixo
       if(PS4.Down()){
          inv = -1;
          delay(100);
       }
       //Sentido de locomocao principal
        if(PS4.Up()){
          inv = 1;
          delay(100);
       }
        
       //Função para arma ativa, aceleração gradativa/dinamica com o gatilho - Botão R2
        if(PS4.R2()){ 
        angle=map(PS4.R2Value(),0,255,40,140);
        servo.write(angle);
        Serial.println(angle); 
        } 
    }
    
     
    //Failsafe
    //Teste de conectividade do controle, caso esteja, desconectado zera todos os parametros
    if(PS4.isConnected()!= true){
    angle = 0;
    servo.write(angle);
    motors_control(0,0);
    Serial.println("Restart");
    ESP.restart();
    delay(20);
    }
    
  }





  //Biblioteca do controle de PS4
#include <PS4Controller.h>

//Locomocão 
#define PWMA 27
#define PWMB 26
#define A1  13
#define A2  12
#define B2  25
#define B1  33


//Setup pinos brushless
#include <Servo_ESP32.h>
static const int brushPin = 4; 
Servo_ESP32 servo;
int angle;


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
  if(speedA>0){  
    digitalWrite(A1, 1);
    digitalWrite(A2, 0);
  }else{
    digitalWrite(A1, 0);
    digitalWrite(A2, 1);
  }
  ledcWrite(5,abs(speedA));

}

void motor_B(int speedB){
  if(speedB>0){
    digitalWrite(B1, 1);
    digitalWrite(B2, 0);
  }else{
    digitalWrite(B1, 0);
    digitalWrite(B2, 1);
  }
  ledcWrite(6,abs(speedB));

}


void setup(void) {
  Serial.begin(115200);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens


  PS4.begin("01:01:01:01:01:01");
  Serial.println("Ready.");
  
  ledcAttachPin(PWMA,5);
  ledcAttachPin(PWMB,6);
  
  ledcSetup(5, 80000, 8);
  ledcSetup(6, 80000, 8);
  
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(B1, OUTPUT);
  pinMode(B2, OUTPUT);
  digitalWrite(A1, 0);
  digitalWrite(A2, 0);
  digitalWrite(B1, 0);
  digitalWrite(B2, 0);


  while(PS4.isConnected()!= true){
  delay(20);}
}

void loop() {

  while(PS4.isConnected()) {
  //motors_control(linear_speed*multiplicador, angular_speed* multiplicador2);
  // Multiplicadcor = 1.8 para aumentar a velocidade linear, o quao rapido o robo vai ser
  // Multiplicadcor2 = multiplic_curva, parametro que varia de 1 ate a 2.3 para suavisar as curvas em alta velocidade
    if(PS4.LStickY()<-15 || PS4.LStickY()>15){
      int curva = map(abs(PS4.LStickY()), 0, 127, 80, 190);
      float multiplic_curva = (float) curva/100;
      motors_control(-2*inv*PS4.LStickY(), (0.8)*PS4.RStickX()/multiplic_curva);

    }else { // Controle sobre valores pequenos devido a problemas na funcao map
      motors_control(-2*inv*PS4.LStickY(), (0.8)*PS4.RStickX()/0.8);

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
    
      //Sentido de locomocao principal
      if(PS4.L3()){
        inv = 1;
        delay(100);
      }
              
      //Função para arma ativa, aceleração gradativa/dinamica com o gatilho - Botão R2
      if(PS4.R2()){ 
      angle=map(PS4.R2Value(),0,255,40,140);
      servo.write(angle);
      Serial.println(angle); 
      } 

            //Função para arma ativa, aceleração discreta com o gatilho - Botão R1
      if (PS4.R1()){
        angle=angle+2;
        servo.write(angle);
        Serial.println(angle);
        delay(200);
    }

        //inicio do Brushless - seta o piso do valor (primeiro beep)- Botão quadrado
    if (PS4.Square()) { 
        angle=40;
        servo.write(angle);
        Serial.println(angle);
        delay(200);
    }

  }
    
  //Failsafe
  if(PS4.isConnected()!= true){
  motors_control(0,0);
  Serial.println("Restart");
  ESP.restart();
  delay(20);
  }
  
}

