# Baby Yoda - Robô Interativo com Expressões Faciais

## Visão Geral

O **Baby Yoda** é um projeto de robô interativo desenvolvido no trainee do Eixo de Inovação no final de 2024. O sistema exibe diferentes expressões faciais em um display OLED de 128x64 pixels, utilizando sensores ultrassônicos HC-SR04 para detecção de proximidade.

O robô "emocional" pode expressar diferentes estados: **neutro**, **curioso**, **feliz**, além de possuir um efeito de **piscar** natural.


---

## Índice

- [Componentes Utilizados](#componentes-utilizados)
- [Funcionalidades Principais](#funcionalidades-principais)
- [Configuração dos Pinos](#configuração-dos-pinos)
- [Lógica de Funcionamento](#lógica-de-funcionamento)
- [Expressões Faciais](#expressões-faciais)
- [Instalação e Configuração](#instalação-e-configuração)
- [Como Usar](#como-usar)
- [Créditos](#créditos)

## Componentes Utilizados

### Hardware Principal
- **Microcontrolador**: Arduino Uno
- **Display**: OLED SSD1306 (128x64 pixels) com interface I2C
- **Sensores**: 2x HC-SR04 (Ultrassônicos)
- **Acessórios**: Jumpers e 2x Protoboards mini
- **Carcaça**: Modelagem do Baby Yoda para abrigar o circuito

### Bibliotecas de Software
- **Wire** - Comunicação I2C
- **FluxGarage_RoboEyes** - Renderização e animação procedural dos olhos
- **Adafruit SSD1306** - Controle do display OLED

## Funcionalidades Principais

### Movimentação dos Olhos
- **O Cérebro e a Personalidade (Arduino)**: O Arduino funciona como a consciência do Baby Yoda, ele recebe os "estímulos" do mundo através dos sensores e decide qual "sentimento" ou "reação" o personagem deve expressar em seu rosto digital.

- **Percepção Extrassensorial (Triangulação)**: Ao invés de botões, o robô usa matemática. Ele lê a distância de ambos os sensores simultaneamente de forma assíncrona (a cada 100ms). Usando a diferença de distância entre os dois sensores, o código calcula a posição `X` do objeto através de uma fórmula de triangulação.

- **A Janela da Alma Digital (Display e RoboEyes)**: Diferente de desenhar pixels manualmente, o projeto utiliza a biblioteca **RoboEyes** para gerar animações fluidas. O display emite alto contraste, criando um olhar vivo que pisca automaticamente e reage de forma natural à presença de usuários.

## Configuração dos Pinos

### Sensores Ultrassônicos
```cpp
// Sensor Esquerdo
#define trigPinLeft 9   // Trigger 
#define echoPinLeft 10  // Echo

// Sensor Direito  
#define trigPinRight 11 // Trigger
#define echoPinRight 12 // Echo
```

### Display OLED
```cpp
// Comunicação I2C
// SDA -> A4 (Arduino Uno)
// SCL -> A5 (Arduino Uno)
// #define SCREEN_ADDRESS 0x3D // Endereço da tela
```

## Lógica de Funcionamento

### O foco principal do projeto é a Triangulação e a Leitura Assíncrona:
1. **Leitura sem Bloqueio**: A função millis() garante que o Arduino leia os sensores a cada 100ms sem travar as animações de piscar dos olhos da biblioteca RoboEyes.
2. **Cálculo de Distância**: Os sensores medem o tempo do eco sonoro e convertem para centímetros usando o fator multiplicador 0.0343.
3. **Triangulação de Posição (X)**: Com a distância do sensor esquerdo ($d_1$) e direito ($d_2$), e sabendo que a distância fixa entre eles ($L$) é de 12 cm, calculamos a posição do objeto com a fórmula:
   $$x = \frac{d_1^2 - d_2^2}{2 \times 12}$$
5. **Determinação de Estado**: O valor calculado de X define para qual lado os olhos devem se voltar e qual será a emoção do robô.

## Expressões Faciais
🧐 Objeto à Esquerda (x < -2.0): Os olhos se movem para a esquerda (W), o robô entra em estado de curiosidade (setCuriosity(ON)).

😊 Objeto ao Centro (-2.0 <= x <= 2.0): Os olhos centralizam, a curiosidade desativa e ele fica com uma expressão feliz (setMood(HAPPY)).

🧐 Objeto à Direita (x > 2.0): Os olhos se movem para a direita (E) e o modo curiosidade é reativado.

😑 Piscar Natural: Configurado na inicialização com roboEyes.setAutoblinker(ON, 8, 3), rodando ininterruptamente em segundo plano.


## Instalação e Configuração

### Pré-requisitos
- **Arduino IDE** 1.8.0 ou superior
- **Bibliotecas Arduino**:
  - Adafruit GFX Library
  - Adafruit SSD1306
  - FluxGarage_RoboEyes
  - Wire (nativa)

### Instalação das Bibliotecas
1. Abra o Arduino IDE
2. Vá em **Sketch** → **Include Library** → **Manage Libraries**
3. Procure e instale:
   - `Adafruit GFX Library`
   - `Adafruit SSD1306`
   - `FluxGarage_RoboEyes`

### Configuração do Hardware
1. **Monte o circuito** conforme o diagrama de conexões
2. **Conecte o Arduino** ao computador via USB
3. **Carregue o código** (`sketch.ino`) no Arduino
4. **Teste as conexões** através do Monitor Serial

## Como Usar

### Operação Básica
1. **Ligue o Arduino** - O robô inicializa no estado neutro
2. **Aproxime objetos** dos sensores ultrassônicos para ver os olhos se moverem
3. **Observe as expressões automáticas**:

### Interação com Sensores
- **Sensor Esquerdo**: Detecta objetos à esquerda, move olhos para a direita
- **Sensor Direito**: Detecta objetos à direita, move olhos para a esquerda
- **Ambos os Sensores**: Cria movimento dinâmico baseado na diferença de distâncias
- **Alcance Efetivo**: 5cm a 100cm para melhor resposta


## Créditos

**Desenvolvedores**:
- Artur Melo - Colaborador
- Davi Jesseh - Colaborador
- Maria Clara - Colaborador
- Gabriel Mapurunga - Colaborador


**Projeto desenvolvido em**: Final de 2024  
**Plataforma**: Arduino IDE + Wokwi Simulator  
**Tema**: Robótica Interativa e Expressões Faciais

---

**Inspiração Visual**: Personagem Yoda da franquia Star Wars

**Demonstração de projeto semelhante**: [Vídeo dos Olhos Seguidores 8-Bits em ação.](https://www.youtube.com/watch?v=ng629rU_N2Q)
