//ESTA PARTE DO CÓDIGO SÓ INCLUI O FUNCIONAMENTO DO DISPLAY LCD COM AS CELULAS DE CARGA
// FUNCIONA PELO SIMULADOR MAS COM CERTEZA TERÁ DE HAVER AJUSTES NA PARTE DE CALIBRAÇÃO PELO CÓDIGO 


#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <HX711.h>
#include <EEPROM.h>

#define SDA_PIN 23
#define SCL_PIN 22

#define DT 15
#define SCK 2
#define BUTTON_TARE 25
#define BUTTON_CALIBRATE 27
#define BUTTON_REGISTER 32
#define BUTTON_LIVE 34
#define BUTTON_LIST 18

#define EEPROM_FLAG_ADDR 0
#define EEPROM_CALIB_ADDR 1
#define VALID_FLAG 123

#define MAX_PESSOAS 50
#define EEPROM_START_ADDR 10
#define REGISTRO_SIZE 16

HX711 scale;
LiquidCrystal_I2C lcd(0x27, 16, 2);

float calibration_factor = 420.0;

bool lastButtonLiveState = HIGH;
bool currentButtonLiveState = HIGH;
bool lastButtonCalibrateState = HIGH;
bool currentButtonCalibrateState = HIGH;
bool lastButtonTareState = HIGH;
bool currentButtonTareState = HIGH;
bool lastButtonRegisterState = HIGH;
bool currentButtonRegisterState = HIGH;
bool lastButtonListState = HIGH;
bool currentButtonListState = HIGH;

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_LIVE, INPUT_PULLUP);
  pinMode(BUTTON_CALIBRATE, INPUT_PULLUP);
  pinMode(BUTTON_TARE, INPUT_PULLUP);

  Wire.begin(SDA_PIN, SCL_PIN);
  lcd.begin(16, 2);
  lcd.backlight();

  scale.begin(DT, SCK);

  byte flag;
  EEPROM.get(EEPROM_FLAG_ADDR, flag);
  if (flag != VALID_FLAG) {
    calibration_factor = 420.0;
    EEPROM.put(EEPROM_FLAG_ADDR, VALID_FLAG);
    EEPROM.put(EEPROM_CALIB_ADDR, calibration_factor);
  } else {
    EEPROM.get(EEPROM_CALIB_ADDR, calibration_factor);
  }

  scale.set_scale(calibration_factor);
  scale.tare();
  delay(500);

  lcd.setCursor(0, 0);
  lcd.print("Sistema Pronto");
  delay(2000);
  lcd.clear();
}

void loop() {
  currentButtonLiveState = digitalRead(BUTTON_LIVE);
  if (lastButtonLiveState == HIGH && currentButtonLiveState == LOW) {
    float peso = scale.get_units(10);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Peso atual:");
    lcd.setCursor(0, 1);
    lcd.print(peso, 3);
    lcd.print(" kg");
    Serial.print("Peso: ");
    Serial.print(peso, 3);
    Serial.println(" kg");
  }

  currentButtonCalibrateState = digitalRead(BUTTON_CALIBRATE);
  if (lastButtonCalibrateState == HIGH && currentButtonCalibrateState == LOW) {
    calibrarBalanca();
  }

  currentButtonTareState = digitalRead(BUTTON_TARE);
  if (lastButtonTareState == HIGH && currentButtonTareState == LOW) {
    scale.tare();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Tare aplicado");
    Serial.println("Tare aplicado");
    delay(2000);
  }

  lastButtonLiveState = currentButtonLiveState;
  lastButtonCalibrateState = currentButtonCalibrateState;
  lastButtonTareState = currentButtonTareState;
  lastButtonRegisterState = currentButtonRegisterState;
  lastButtonListState = currentButtonListState;

  delay(50);
}

void calibrarBalanca() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Retire o peso");
  Serial.println("Retire qualquer peso e pressione o botao Tare.");
  while (digitalRead(BUTTON_TARE) == HIGH) {
    delay(10);
  }
  scale.tare();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Tare concluido");
  delay(2000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Coloque peso");
  Serial.println("Coloque o peso conhecido e pressione o botao Calibrar novamente.");
  while (digitalRead(BUTTON_CALIBRATE) == HIGH) {
    delay(10);
  }
  float leitura = scale.get_units(20);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Digite valor");
  Serial.println("Digite o peso conhecido no Serial Monitor:");
  while (Serial.available() == 0) {
    delay(10);
  }
  float peso_conhecido = Serial.parseFloat();

  if (peso_conhecido > 0) {
    calibration_factor = scale.get_scale() * (peso_conhecido / leitura);
    scale.set_scale(calibration_factor);
    EEPROM.put(EEPROM_CALIB_ADDR, calibration_factor);
    Serial.print("Novo fator calibracao: ");
    Serial.println(calibration_factor);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Calibrado!");
    lcd.setCursor(0, 1);
    lcd.print("Fator:");
    lcd.print(calibration_factor, 2);
    delay(3000);
  } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Erro: valor 0");
    delay(2000);
  }
}

