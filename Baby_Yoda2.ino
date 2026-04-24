// ELE TÁ MUDANDO DE ESTADO SÓ TEM QUE COLOCAR AS CONFIG DO OLHO EM CADA ESTADO

#include <gfxfont.h>
#include <Adafruit_SPITFT_Macros.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_GFX.h>
#include <Adafruit_GrayOLED.h>
#include <Adafruit_SSD1306.h>


// Pinos dos sensores HC-SR04
#define trigPinLeft 9 // Trigger do Sensor Esquerdo
#define echoPinLeft 10 // Echo do Sensor Esquerdo
#define trigPinRight 11 // Trigger do Sensor Direito
#define echoPinRight 12 // Echo do Sensor Direito
#define MAX_DISTANCE 15 // Distância máxima em cm

unsigned long tempoAnterior1 = 0; // Armazena o tempo anterior para o sensor 1
unsigned long tempoAnterior2 = 0; // Armazena o tempo anterior para o sensor 2
const long intervalo = 100; // Intervalo entre leituras (em milissegundos)
float timer_estado = 0;
float timer_estado2 = 0;


// Variáveis para triangulação
const float L = 12.0; // Distância entre os sensores em cm
float x = 0; // Posição X calculada do objeto
float d1 = 0.0;
float d2 = 0.0;


bool soneca = true;
int counter = 0;
bool troca = false;


// On an arduino UNO:       A4(SDA), A5(SCL)
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1 //-1 se for o mesmo pin de reset do arduino
#define SCREEN_ADDRESS 0x3D //Endereço da tela
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Criação do objeto OLED
#include <FluxGarage_RoboEyes.h>
RoboEyes<Adafruit_SSD1306> roboEyes(display);

void setup() {
  Serial.begin(9600);
  // Startup OLED Display
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C or 0x3D
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
    pinMode(trigPinLeft, OUTPUT);
    pinMode(echoPinLeft, INPUT);
    pinMode(trigPinRight, OUTPUT);
    pinMode(echoPinRight, INPUT);
  }
  //Inicializando o RoboEyes
  roboEyes.begin(SCREEN_WIDTH, SCREEN_HEIGHT, 100); // Largura, Altura, Framerate
  //roboEyes.setAutoblinker(ON, 8, 3); // Animação de piscar -> bool ativo, int intervalo
  
}

enum Estado{
  esquerda,
  centro,
  direita
};

Estado estado_atual = centro;


void loop() {
  if(soneca == true){dormindo();}
  else{acordado();}
}
  
  
long getDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH, 20000UL);
  float distancia = (duration / 2.0) * 0.0343; //Converte para cm
  if (distancia >= MAX_DISTANCE || distancia <= 0.1) {
    return -1; // Valor fora do alcance
   }
  else{
    return distancia;
  }
 }



void determinarEstado(float x) {

  if (soneca == true){
    if (x < -2.0) { //Esquerda
      roboEyes.setAutoblinker(OFF, 3, 1);
      roboEyes.setHFlicker(ON, 2);
      roboEyes.setWidth(36, 36); // byte olho esquerdo, byte olho direito
      roboEyes.setHeight(10, 10); // byte olho esquerdo, byte olho direito
      roboEyes.setBorderradius(14, 14); // byte olho esquerdo, byte olho direito
      roboEyes.setSpacebetween(10); // int espaço -> pode ser negativo
      estado_atual = esquerda;
      roboEyes.setPosition(DEFAULT);
      roboEyes.setCuriosity(OFF);
      roboEyes.setMood(ANGRY);
    }
    else if (x >= -2.0 && x <= 2.0) { // Centro  
      roboEyes.setAutoblinker(ON, 3, 1);
      roboEyes.setHFlicker(OFF, 2);
      roboEyes.setWidth(36, 36); // byte olho esquerdo, byte olho direito
      roboEyes.setHeight(10, 10); // byte olho esquerdo, byte olho direito
      roboEyes.setBorderradius(14, 14); // byte olho esquerdo, byte olho direito
      roboEyes.setSpacebetween(10); // int espaço -> pode ser negativo
      estado_atual = centro;
      roboEyes.setPosition(DEFAULT);
      roboEyes.setCuriosity(OFF);
      roboEyes.setMood(DEFAULT);
    }
    else { //Direita
      roboEyes.setAutoblinker(OFF, 3, 1);
      roboEyes.setHFlicker(ON, 2);
      roboEyes.setWidth(36, 36); // byte olho esquerdo, byte olho direito
      roboEyes.setHeight(10, 10); // byte olho esquerdo, byte olho direito
      roboEyes.setBorderradius(14, 14); // byte olho esquerdo, byte olho direito
      roboEyes.setSpacebetween(10); // int espaço -> pode ser negativo
      estado_atual = direita;
      roboEyes.setPosition(DEFAULT);
      roboEyes.setCuriosity(OFF);
      roboEyes.setMood(ANGRY);
    }
  }

  else{
    if (x < -2.0) { //Esquerda
        
        if (estado_atual == esquerda) {
          roboEyes.setWidth(30, 30); // byte olho esquerdo, byte olho direito
          roboEyes.setHeight(30, 30); // byte olho esquerdo, byte olho direito
          roboEyes.setBorderradius(14, 14); // byte olho esquerdo, byte olho direito
          roboEyes.setSpacebetween(10); // int espaço -> pode ser negativo
          timer_estado +=1;
          if(timer_estado > 2000){roboEyes.setMood(TIRED); delay(100); timer_estado = 0;}
        }
      else {
        timer_estado = 0;
        estado_atual = esquerda;
        roboEyes.setWidth(30, 30); // byte olho esquerdo, byte olho direito
        roboEyes.setHeight(30, 30); // byte olho esquerdo, byte olho direito
        roboEyes.setBorderradius(15, 15); // byte olho esquerdo, byte olho direito
        roboEyes.setSpacebetween(10); // int espaço -> pode ser negativo
        roboEyes.setPosition(W);
        roboEyes.setCuriosity(ON);
        roboEyes.setMood(DEFAULT);
      }
    }
    else if (x >= -2.0 && x <= 2.0) { // Centro
      if (estado_atual == centro) {

        roboEyes.setWidth(30, 30); // byte olho esquerdo, byte olho direito
        roboEyes.setHeight(30, 30); // byte olho esquerdo, byte olho direito
        roboEyes.setBorderradius(15, 15); // byte olho esquerdo, byte olho direito
        roboEyes.setSpacebetween(10); // int espaço -> pode ser negativo
        timer_estado +=1;
        if(timer_estado > 10000){timer_estado = 0; soneca = true; dormindo();}
      }
      else {
        timer_estado = 0;
        estado_atual = centro;
        roboEyes.setWidth(30, 30); // byte olho esquerdo, byte olho direito
        roboEyes.setHeight(30, 30); // byte olho esquerdo, byte olho direito
        roboEyes.setBorderradius(15, 15); // byte olho esquerdo, byte olho direito
        roboEyes.setSpacebetween(10); // int espaço -> pode ser negativo
        roboEyes.setPosition(DEFAULT);
        roboEyes.setCuriosity(OFF);
        roboEyes.setMood(DEFAULT);

        
      }
    }
    else { //Direita
      
      if (estado_atual == direita) {
        roboEyes.setWidth(30, 30); // byte olho esquerdo, byte olho direito
        roboEyes.setHeight(30, 30); // byte olho esquerdo, byte olho direito
        roboEyes.setBorderradius(15, 15); // byte olho esquerdo, byte olho direito
        roboEyes.setSpacebetween(10); // int espaço -> pode ser negativo
        timer_estado +=1;
        if(timer_estado > 2000){roboEyes.setMood(TIRED); delay(100); timer_estado = 0;}
      }
      else {
        timer_estado = 0;
        estado_atual = direita;
        roboEyes.setWidth(30, 30); // byte olho esquerdo, byte olho direito
        roboEyes.setHeight(30, 30); // byte olho esquerdo, byte olho direito
        roboEyes.setBorderradius(15, 15); // byte olho esquerdo, byte olho direito
        roboEyes.setSpacebetween(10); // int espaço -> pode ser negativo
        roboEyes.setPosition(E);
        roboEyes.setCuriosity(ON);
        roboEyes.setMood(DEFAULT);
      }
    }

  }

}



void dormindo(){
  while (counter < 5){
    roboEyes.setHeight(3, 3);
    roboEyes.setAutoblinker(ON, 8, 3);
    roboEyes.setPosition(DEFAULT);
    unsigned long tempoAtual = millis();
    // Obter distâncias dos sensores
    // Sensor Esquerdo - Leia sem atraso
    if (tempoAtual - tempoAnterior1 >= intervalo) {
      d1 = getDistance(trigPinLeft, echoPinLeft);
      tempoAnterior1 = tempoAtual;
    }
    // Sensor Direito - Leia sem atraso
    if (tempoAtual - tempoAnterior2 >= intervalo) {
      d2 = getDistance(trigPinRight, echoPinRight);
      tempoAnterior2 = tempoAtual;
    }
    // Evitar leituras inválidas
    if (d1 == -1){d1 = MAX_DISTANCE;}
    if (d2 == -1){d2 = MAX_DISTANCE;}
    
    //Calcular posição X usando triangulação
    x = (pow(d1, 2) - pow(d2, 2)) / (2 * L);
    determinarEstado(x);
    if (estado_atual != centro){
      troca = true;
    }
    else {
      if(troca == true) {
        troca = false;
        counter +=1;
      }
    }
    roboEyes.update();
  }
  counter = 0;
  soneca = false;
}

void acordado(){
  unsigned long tempoAtual = millis();
  // Obter distâncias dos sensores
  // Sensor Esquerdo - Leia sem atraso
  if (tempoAtual - tempoAnterior1 >= intervalo) {
    d1 = getDistance(trigPinLeft, echoPinLeft);
    tempoAnterior1 = tempoAtual;
  }
  // Sensor Direito - Leia sem atraso
  if (tempoAtual - tempoAnterior2 >= intervalo) {
    d2 = getDistance(trigPinRight, echoPinRight);
    tempoAnterior2 = tempoAtual;
  }
  // Evitar leituras inválidas
  if (d1 == -1){d1 = MAX_DISTANCE;}
  if (d2 == -1){d2 = MAX_DISTANCE;}
    
  //Calcular posição X usando triangulação
  x = (pow(d1, 2) - pow(d2, 2)) / (2 * L);
  determinarEstado(x);
  roboEyes.update();
}
