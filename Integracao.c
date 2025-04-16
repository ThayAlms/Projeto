#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <HX711.h>
#include <EEPROM.h>
#include <Keypad.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

// ----- Barramentos I2C -----
TwoWire WireMenu = TwoWire(1); // Segundo barramento para OLED

// ----- Definições do display OLED -----
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 displayMenu(SCREEN_WIDTH, SCREEN_HEIGHT, &WireMenu, OLED_RESET);

// ----- LCD -----
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ----- Balança -----
#define DT 15
#define SCK 2
HX711 scale;
float calibration_factor = 420.0;

// ----- Teclado -----
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {21, 19, 18, 5};
byte colPins[COLS] = {17, 16, 4, 0};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// ----- Botões -----
#define BUTTON_TARE 25
#define BUTTON_CALIBRATE 27
#define BUTTON_REGISTER 32
#define BUTTON_LIVE 34
#define BUTTON_LIST 18

bool lastButtonStates[5] = {HIGH, HIGH, HIGH, HIGH, HIGH};
bool currentButtonStates[5];

// ----- EEPROM -----
#define EEPROM_FLAG_ADDR 0
#define EEPROM_CALIB_ADDR 1
#define VALID_FLAG 123

void setup() {
  Serial.begin(115200);

  pinMode(BUTTON_TARE, INPUT_PULLUP);
  pinMode(BUTTON_CALIBRATE, INPUT_PULLUP);
  pinMode(BUTTON_REGISTER, INPUT_PULLUP);
  pinMode(BUTTON_LIVE, INPUT_PULLUP);
  pinMode(BUTTON_LIST, INPUT_PULLUP);

  // Iniciar LCD no barramento padrão
  Wire.begin(23, 22);
  lcd.begin(16, 2);
  lcd.backlight();

  // Iniciar OLED no segundo barramento
  WireMenu.begin(12, 14);
  if (!displayMenu.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("Falha ao iniciar OLED"));
    while (true);
  }
  displayMenu.clearDisplay();

  // Iniciar Balança
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

  lcd.setCursor(0, 0);
  lcd.print("Sistema Pronto");
  delay(2000);
  lcd.clear();

  mostrarMenu();
}

void loop() {
  // Checar teclado
  char tecla = keypad.getKey();
  if (tecla) {
    switch (tecla) {
      case 'A':
        cadastrarPeso();
        break;
      case 'B':
        listarPacientes();
        break;
      case 'C':
        excluirPaciente();
        break;
      case 'D':
        pausarSistema();
        break;
      default:
        break;
    }
  }

  // Checar botões
  currentButtonStates[0] = digitalRead(BUTTON_TARE);
  currentButtonStates[1] = digitalRead(BUTTON_CALIBRATE);
  currentButtonStates[2] = digitalRead(BUTTON_REGISTER);
  currentButtonStates[3] = digitalRead(BUTTON_LIVE);
  currentButtonStates[4] = digitalRead(BUTTON_LIST);

  if (lastButtonStates[3] == HIGH && currentButtonStates[3] == LOW) {
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

  if (lastButtonStates[1] == HIGH && currentButtonStates[1] == LOW) {
    calibrarBalanca();
  }

  if (lastButtonStates[0] == HIGH && currentButtonStates[0] == LOW) {
    scale.tare();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Tare aplicado");
    Serial.println("Tare aplicado");
    delay(2000);
    lcd.clear();
  }

  for (int i = 0; i < 5; i++) {
    lastButtonStates[i] = currentButtonStates[i];
  }

  delay(50);
}

void mostrarMenu() {
  displayMenu.clearDisplay();
  displayMenu.setTextSize(1);
  displayMenu.setTextColor(SSD1306_WHITE);
  displayMenu.setCursor(0, 0);
  displayMenu.println("A: Cadastrar Peso");
  displayMenu.println("B: Listar Pacientes");
  displayMenu.println("C: Excluir Paciente");
  displayMenu.println("D: Pausar Sistema");
  displayMenu.display();
}

void cadastrarPeso() {
  lcd.clear();
  float peso = scale.get_units(10);
  lcd.setCursor(0,0);
  lcd.print("Peso:");
  lcd.setCursor(0,1);
  lcd.print(peso, 3);
  lcd.print(" kg");
  Serial.print("Peso cadastrado: ");
  Serial.println(peso, 3);
  delay(3000);
  lcd.clear();
}

void listarPacientes() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Listando...");
  Serial.println("Listar pacientes...");
  delay(3000);
  lcd.clear();
}

void excluirPaciente() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Excluindo...");
  Serial.println("Excluir paciente...");
  delay(3000);
  lcd.clear();
}

void pausarSistema() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Sistema Pausado");
  Serial.println("Sistema Pausado");
  delay(3000);
  lcd.clear();
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
