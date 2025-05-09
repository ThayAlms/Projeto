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

// ----- EEPROM -----
#define EEPROM_FLAG_ADDR 0
#define EEPROM_CALIB_ADDR 1
#define VALID_FLAG 123

void setup() {
  Serial.begin(115200);

  // Iniciar LCD no barramento padrão
  Wire.begin(23, 22);
  lcd.begin(16, 2);
  lcd.backlight();

  // Iniciar OLED no segundo barramento
  WireMenu.begin(12, 14);
  if (!displayMenu.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("Falha ao iniciar OLED"));
    while (true); // trava aqui se não achar
  }
  displayMenu.clearDisplay();

  // Iniciar Balança
  scale.begin(DT, SCK);

  // Ler fator de calibração da EEPROM
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

  // Mensagem de Boas-vindas no LCD
  lcd.setCursor(0, 0);
  lcd.print("Sistema Pronto");
  delay(2000);
  lcd.clear();

  // Mostrar Menu no OLED
  mostrarMenu();
}

void loop() {
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
