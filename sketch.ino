#include <Adafruit_SSD1306.h>
#include <FluxGarage_RoboEyes.h> // Movi o include para o topo por boas práticas

// Pinos dos sensores HC-SR04
#define trigPinLeft 9    // Trigger do Sensor Esquerdo
#define echoPinLeft 10   // Echo do Sensor Esquerdo
#define trigPinRight 11  // Trigger do Sensor Direito
#define echoPinRight 12  // Echo do Sensor Direito
#define MAX_DISTANCE 100 // Distância máxima em cm

unsigned long tempoAnterior1 = 0; // Armazena o tempo anterior para o sensor 1
unsigned long tempoAnterior2 = 0; // Armazena o tempo anterior para o sensor 2
const long intervalo = 100;       // Intervalo entre leituras (em milissegundos)

// Variáveis para triangulação
const float L = 12.0; // Distância entre os sensores em cm
float x = 0;          // Posição X calculada do objeto
float d1 = 0.0;
float d2 = 0.0;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1       // -1 se for o mesmo pin de reset do arduino
#define SCREEN_ADDRESS 0x3D // Endereço da tela

// Criação dos objetos OLED e RoboEyes
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
roboEyes roboEyes;

void setup() {
  Serial.begin(9600);

  // Startup OLED Display
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C or 0x3D
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }

  // Configuração dos pinos (Corrigido: movido para fora do if de falha do display)
  pinMode(trigPinLeft, OUTPUT);
  pinMode(echoPinLeft, INPUT);
  pinMode(trigPinRight, OUTPUT);
  pinMode(echoPinRight, INPUT);

  // Inicializando o RoboEyes
  roboEyes.begin(SCREEN_WIDTH, SCREEN_HEIGHT, 100); // Largura, Altura, Framerate
  
  // Corrigido: Linha que estava aglutinada no original foi separada
  roboEyes.setAutoblinker(ON, 8, 3); // Animação de piscar -> bool ativo, int interval
  roboEyes.setWidth(36, 36);         // byte olho esquerdo, byte olho direito
  roboEyes.setHeight(36, 36);        // byte olho esquerdo, byte olho direito
  roboEyes.setBorderradius(14, 14);  // byte olho esquerdo, byte olho direito
  roboEyes.setSpacebetween(10);      // int espaço -> pode ser negativo
}

void loop() {
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
  if (d1 == -1) { d1 = MAX_DISTANCE; }
  if (d2 == -1) { d2 = MAX_DISTANCE; }

  // Calcular posição X usando triangulação
  x = (pow(d1, 2) - pow(d2, 2)) / (2 * L);
  determinarEstado(x);
  
  roboEyes.update();
}

long getDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 20000UL);
  float distancia = (duration / 2.0) * 0.0343; // Converte para cm

  if (distancia >= MAX_DISTANCE || distancia <= 0.1) {
    return -1; // Valor fora do alcance
  } else {
    return distancia;
  }
}

void determinarEstado(float x) {
  if (x < -2.0) { // Esquerda
    roboEyes.setPosition(W);
    roboEyes.setCuriosity(ON);
    roboEyes.setMood(DEFAULT);
  } 
  else if (x >= -2.0 && x <= 2.0) { // Centro
    roboEyes.setPosition(DEFAULT);
    roboEyes.setCuriosity(OFF);
    roboEyes.setMood(HAPPY);
  } 
  else { // Direita
    roboEyes.setPosition(E);
    roboEyes.setCuriosity(ON);
    roboEyes.setMood(DEFAULT);
  }
}