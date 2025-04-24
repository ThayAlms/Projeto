💡 Projeto: Sistema de Balança com ESP32, Teclado e Displays I2C
Este projeto é um sistema de pesagem inteligente baseado no ESP32, ideal para aplicações clínicas, laboratoriais ou educacionais. Ele utiliza uma célula de carga com o módulo HX711, um display LCD 16x2 e um display OLED 128x64 em barramentos I2C separados, além de um teclado matricial 4x4 e botões físicos para interação com o usuário.

Link com o circuito:
https://wokwi.com/projects/429130214230912001

⚙️ Funcionalidades
📟 LCD 16x2: exibe informações de pesagem e mensagens do sistema.

🖥️ Display OLED: exibe um menu interativo com as opções de controle via teclado.

⚖️ Célula de carga + HX711: faz a leitura do peso com alta precisão.

⌨️ Teclado 4x4:

A: Cadastrar peso atual

B: Listar pacientes (simulado)

C: Excluir paciente (simulado)

D: Pausar sistema

🔘 Botões físicos:

Tare (zera a balança)

Calibração (ajusta o fator de calibração com peso conhecido)

Exibir peso atual

🔔 Buzzer: emite sinais sonoros para feedback do usuário

💾 EEPROM: armazena o fator de calibração mesmo após desligar o dispositivo

🧠 Tecnologias Utilizadas
ESP32

HX711

Teclado 4x4

Display LCD I2C 16x2

Display OLED I2C (com barramento separado)

EEPROM interna do ESP32

Biblioteca Keypad, Adafruit_SSD1306, HX711, LiquidCrystal_I2C

Componente | Pino ESP32
LCD SDA | 23
LCD SCL | 22
OLED SDA | 12
OLED SCL | 14
HX711 DT | 15
HX711 SCK | 2
Botão Tare | 25
Botão Calibrar | 27
Botão Registrar | 32
Botão Peso Atual | 34
Buzzer | 35
Keypad R1 | 21
Keypad R2 | 19
Keypad R3 | 18
Keypad R4 | 5
Keypad C1 | 17
Keypad C2 | 16
Keypad C3 | 4
Keypad C4 | 0


![image](https://github.com/user-attachments/assets/8a640b97-414d-4c2d-bda7-4850630d30df)
