#include <LiquidCrystal_I2C.h>
#define BUZZER    7
#define ENTRADA_A A0
#define ENTRADA_B A1
#define ENTRADA_C A2
#define ENTRADA_D A3
#define SDA A4
#define SCL A5

// Pines de los LED RGB
int led1R = 3;    // Rojo solo
int ledB = 2;   // Azul solo
int led2R = 5;   // Azul del amarillo
int led1G = 4;   // Verde solo
int led2G = 6;   // Verde del amarillo

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Definición de secuencia
const int NIVEL_MAX = 50;
int secuencia[NIVEL_MAX];
int secuenciaUsuario[NIVEL_MAX];
unsigned long tiempoInicio;

int melodiaAciert[] = {587, 698, 587, 698, 784, 831, 880, 988};
int duracionNotasAcierto[] = {125, 125, 125, 125, 125, 125, 125, 125};
int melodiaErr[] = {333, 308, 285, 265, 247, 231, 217};
int duracionNotasErr[] = {125, 125, 250, 125, 125, 250, 250};
int nivelActual = 0;
int velocidad = 500;

void setup() {
  Serial.begin(9600);
  int semilla = analogRead(A5);
  randomSeed(semilla);
  Serial.print("Semilla: ");
  Serial.println(semilla);

  pinMode(ENTRADA_A, INPUT_PULLUP);
  pinMode(ENTRADA_B, INPUT_PULLUP);
  pinMode(ENTRADA_C, INPUT_PULLUP);
  pinMode(ENTRADA_D, INPUT_PULLUP);

  pinMode(led1R, OUTPUT);
  pinMode(ledB, OUTPUT);
  pinMode(led2R, OUTPUT);
  pinMode(led1G, OUTPUT);
  pinMode(led2G, OUTPUT);

  // Buzzer
  pinMode(BUZZER, OUTPUT);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("LEDs y LCD");

  tiempoInicio = millis();

  // Inicia el nivel actual correctamente
  nivelActual = 1;
}

void loop() {
  if (nivelActual == 1) {
    generaSecuencia();
    muestraSecuencia();
    leeSecuencia();
  } else {
    muestraSecuencia();
    leeSecuencia();
  }
  muestraTiempo(); 
}

void muestraSecuencia() {
  for (int i = 0; i < nivelActual; i++) {
    apagaLEDs();
    Serial.print("Mostrando secuencia[");
    Serial.print(i);
    Serial.print("]: ");
    Serial.println(secuencia[i]);
    if (secuencia[i] == 0) {
      enciendeLED(led1R, led1G, ledB, 255, 0, 0); // Rojo
      tone(BUZZER, 200);
      delay(500);
      noTone(BUZZER);
      delay(50);
    } else if (secuencia[i] == 1) {
      enciendeLED(led1R, led1G, ledB, 0, 255, 0); // Verde
      tone(BUZZER, 300);
      delay(500);
      noTone(BUZZER);
      delay(50);
    } else if (secuencia[i] == 2) {
      enciendeLED(led1R, led1G, ledB, 0, 0, 255); // Azul
      tone(BUZZER, 400);
      delay(500);
      noTone(BUZZER);
      delay(50);
    } else if (secuencia[i] == 3) {
      enciendeLED(led2R, led2G, ledB, 255, 255, 0); // Amarillo
      tone(BUZZER, 500);
      delay(500);
      noTone(BUZZER);
      delay(50);
    }
    delay(500);
    noTone(BUZZER);
    apagaLEDs();
    delay(200);
  }
}

void leeSecuencia() {
  for (int i = 0; i < nivelActual; i++) {
    int flag = 0;
    while (flag == 0) {
      if (digitalRead(ENTRADA_A) == LOW) {
        secuenciaUsuario[i] = 0;
        enciendeLED(led1R, led1G, ledB, 255, 0, 0);
        tone(BUZZER, 200);
        delay(300);
        noTone(BUZZER);
        flag = 1;
        delay(200);
        apagaLEDs();
        if (secuenciaUsuario[i] != secuencia[i]) {
          Serial.print("Error en secuenciaUsuario[");
          Serial.print(i);
          Serial.print("]: ");
          Serial.println(secuenciaUsuario[i]);
          Serial.print("Esperado secuencia[");
          Serial.print(i);
          Serial.print("]: ");
          Serial.println(secuencia[i]);
          secuenciaError();
          return;
        }
      }
      if (digitalRead(ENTRADA_B) == LOW) {
        secuenciaUsuario[i] = 1;
        enciendeLED(led1R, led1G, ledB, 0, 255, 0);
        tone(BUZZER, 300);
        delay(300);
        noTone(BUZZER);
        flag = 1;
        delay(200);
        apagaLEDs();
        if (secuenciaUsuario[i] != secuencia[i]) {
          Serial.print("Error en secuenciaUsuario[");
          Serial.print(i);
          Serial.print("]: ");
          Serial.println(secuenciaUsuario[i]);
          Serial.print("Esperado secuencia[");
          Serial.print(i);
          Serial.print("]: ");
          Serial.println(secuencia[i]);
          secuenciaError();
          return;
        }
      }
      if (digitalRead(ENTRADA_C) == LOW) {
        secuenciaUsuario[i] = 2;
        enciendeLED(led1R, led1G, ledB, 0, 0, 255);
        tone(BUZZER, 400);
        delay(300);
        noTone(BUZZER);
        flag = 1;
        delay(200);
        apagaLEDs();
        if (secuenciaUsuario[i] != secuencia[i]) {
          Serial.print("Error en secuenciaUsuario[");
          Serial.print(i);
          Serial.print("]: ");
          Serial.println(secuenciaUsuario[i]);
          Serial.print("Esperado secuencia[");
          Serial.print(i);
          Serial.print("]: ");
          Serial.println(secuencia[i]);
          secuenciaError();
          return;
        }
      }
      if (digitalRead(ENTRADA_D) == LOW) {
        secuenciaUsuario[i] = 3;
        enciendeLED(led2R, led2G, ledB, 255, 255, 0);
        tone(BUZZER, 500);
        delay(300);
        noTone(BUZZER);
        flag = 1;
        delay(200);
        apagaLEDs();
        if (secuenciaUsuario[i] != secuencia[i]) {
          Serial.print("Error en secuenciaUsuario[");
          Serial.print(i);
          Serial.print("]: ");
          Serial.println(secuenciaUsuario[i]);
          Serial.print("Esperado secuencia[");
          Serial.print(i);
          Serial.print("]: ");
          Serial.println(secuencia[i]);
          secuenciaError();
          return;
        }
      }
    }
  }
  secuenciaCorrecta();
}

void generaSecuencia() {
  for (int i = 0; i < NIVEL_MAX; i++) {
    secuencia[i] = random(0, 4);
  }
}

void melodiaError() {
  for (int i = 0; i < sizeof(melodiaErr) / sizeof(melodiaErr[0]); i++) {
    tone(BUZZER, melodiaErr[i], duracionNotasErr[i]);
    delay(duracionNotasErr[i] + 62);
  }
}
void melodiaAcierto(){
  for (int i = 0; i < sizeof(melodiaAciert) / sizeof(melodiaAciert[0]); i++) {
    tone(BUZZER, melodiaAciert[i], duracionNotasAcierto[i]);
    delay(duracionNotasAcierto[i] + 65);
  }
}
void secuenciaError() {
  for (int i = 0; i < 5; i++) {
    apagaLEDs();
    delay(100);
    // Enciende todos los LEDs simultáneamente
    analogWrite(led1R, 255);  // Rojo
    analogWrite(led1G, 255);    // Verde (apagado)
    analogWrite(ledB, 255);     // Azul (apagado)
    analogWrite(led2R, 255);  // Rojo del amarillo
    analogWrite(led2G, 255);  // Verde del amarillo
    delay(100);
  }
  melodiaError();
  nivelActual = 1;
  velocidad = 500;
  tiempoInicio = millis();
  nivelActual = 0;
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("Nivel: ");
  lcd.print(nivelActual);
  delay(200);
}


void secuenciaCorrecta() {
  if (nivelActual < NIVEL_MAX)
    nivelActual++;
  velocidad -= 50 / (nivelActual / 50 + 1);
  lcd.setCursor(0, 1);
  lcd.print("Nivel: ");
  lcd.print(nivelActual);
  
  melodiaAcierto();
  
  delay(200);
}


void muestraTiempo() {
  lcd.setCursor(0, 0);
  lcd.print("Time: ");
  lcd.print((millis() - tiempoInicio) / 1000); // Muestra el tiempo en segundos
  lcd.print(" segs");
}

void enciendeLED(int pinR, int pinG, int pinB, int valorR, int valorG, int valorB) {
  // Configura los valores para los colores del LED
  analogWrite(pinR, valorR);
  analogWrite(pinG, valorG);
  analogWrite(pinB, valorB);
}

void apagaLEDs() {
  // Apaga todos los LED RGB
  analogWrite(led1R, 0);
  analogWrite(led1G, 0);
  analogWrite(ledB, 0);
  analogWrite(led2G, 0);
  analogWrite(led2R, 0);
}
