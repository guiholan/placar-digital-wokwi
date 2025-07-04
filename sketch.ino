const int botaoLed[5] = {A0, A1, A2, A3, A4};
const int ledPin[5] = {1, 6, 7, 9, 11};
bool estadoLed[5] = {false, false, false, false, false};
bool estadoAnterior[5] = {true, true, true, true, true};

// Pinos do registrador
const int DS   = 8;  // data
const int SHCP = 12; // clock
const int STCP = 10; // latch

// Botões do placar
const int botaoPontoA  = 2;
const int botaoFaltaA  = 3;
const int botaoPontoB  = 4;
const int botaoFaltaB  = 5;
const int botaoPeriodo = A5; // novo botão

int pontosA = 0, pontosB = 0, faltasA = 0, faltasB = 0;
int segundos = 0, minutos = 0;
int periodo = 0; // novo período

unsigned long ultimoTempo = 0;

bool ultimoEstadoPontoA  = HIGH;
bool ultimoEstadoFaltaA  = HIGH;
bool ultimoEstadoPontoB  = HIGH;
bool ultimoEstadoFaltaB  = HIGH;
bool ultimoEstadoPeriodo = HIGH;

const byte segmentos[10] = {
  B00111111, // 0
  B00000110, // 1
  B01011011, // 2
  B01001111, // 3
  B01100110, // 4
  B01101101, // 5
  B01111101, // 6
  B00000111, // 7
  B01111111, // 8
  B01101111  // 9
};

void setup() {
  for (int i = 0; i < 5; i++) {
    pinMode(ledPin[i], OUTPUT);
    pinMode(botaoLed[i], INPUT_PULLUP);
    digitalWrite(ledPin[i], LOW);
  }

  pinMode(botaoPontoA,  INPUT_PULLUP);
  pinMode(botaoFaltaA,  INPUT_PULLUP);
  pinMode(botaoPontoB,  INPUT_PULLUP);
  pinMode(botaoFaltaB,  INPUT_PULLUP);
  pinMode(botaoPeriodo, INPUT_PULLUP); 
  pinMode(DS, OUTPUT);
  pinMode(SHCP, OUTPUT);
  pinMode(STCP, OUTPUT);
}

void loop() {
  for (int i = 0; i < 5; i++) {
    bool leitura = digitalRead(botaoLed[i]);
    if (leitura == LOW && estadoAnterior[i] == HIGH) {
      estadoLed[i] = !estadoLed[i];
      digitalWrite(ledPin[i], estadoLed[i] ? HIGH : LOW);
      delay(200);
    }
    estadoAnterior[i] = leitura;
  }

  checarBotoes();
  atualizarTempo();
  atualizarDisplays();
}

void atualizarTempo() {
  if (millis() - ultimoTempo >= 1000) {
    ultimoTempo = millis();
    segundos++;
    if (segundos >= 60) {
      segundos = 0;
      minutos++;
      if (minutos >= 45) {
        minutos = 0;
      }
    }
  }
}

void checarBotoes() {
  bool estadoAtualPontoA  = digitalRead(botaoPontoA);
  bool estadoAtualFaltaA  = digitalRead(botaoFaltaA);
  bool estadoAtualPontoB  = digitalRead(botaoPontoB);
  bool estadoAtualFaltaB  = digitalRead(botaoFaltaB);
  bool estadoAtualPeriodo = digitalRead(botaoPeriodo); // novo botão

  if (estadoAtualPontoA == LOW && ultimoEstadoPontoA == HIGH) {
    pontosA++;
    if (pontosA > 99) pontosA = 0;
    delay(200);
  }

  if (estadoAtualFaltaA == LOW && ultimoEstadoFaltaA == HIGH) {
    faltasA++;
    if (faltasA > 9) faltasA = 0;
    delay(200);
  }

  if (estadoAtualPontoB == LOW && ultimoEstadoPontoB == HIGH) {
    pontosB++;
    if (pontosB > 99) pontosB = 0;
    delay(200);
  }

  if (estadoAtualFaltaB == LOW && ultimoEstadoFaltaB == HIGH) {
    faltasB++;
    if (faltasB > 9) faltasB = 0;
    delay(200);
  }

  if (estadoAtualPeriodo == LOW && ultimoEstadoPeriodo == HIGH) {
    periodo++;
    if (periodo > 9) periodo = 0;
    delay(200);
  }

  ultimoEstadoPontoA  = estadoAtualPontoA;
  ultimoEstadoFaltaA  = estadoAtualFaltaA;
  ultimoEstadoPontoB  = estadoAtualPontoB;
  ultimoEstadoFaltaB  = estadoAtualFaltaB;
  ultimoEstadoPeriodo = estadoAtualPeriodo;
}

void atualizarDisplays() {
  byte valores[11];

  valores[0]  = pontosA / 10;
  valores[1]  = pontosA % 10;
  valores[2]  = faltasA % 10;
  valores[3]  = minutos / 10;
  valores[4]  = minutos % 10;
  valores[5]  = segundos / 10;
  valores[6]  = segundos % 10;
  valores[7]  = faltasB % 10;
  valores[8]  = pontosB / 10;
  valores[9]  = pontosB % 10;
  valores[10] = periodo % 10; // novo display

  digitalWrite(STCP, LOW);
  for (int i = 10; i >= 0; i--) {
    shiftOut(DS, SHCP, MSBFIRST, segmentos[valores[i]]);
  }
  digitalWrite(STCP, HIGH);
}

