# ⚖️ Projeto Balança Digital com ESP32

Este projeto utiliza um ESP32 para implementar uma balança digital inteligente com múltiplas funcionalidades, incluindo exibição em LCD e OLED, operação via botões e teclado, calibração com EEPROM e feedback sonoro com buzzer.

---

## 📋 Funcionalidades

- Leitura de peso com célula de carga (HX711)
- Exibição do peso em display LCD I2C
- Menu interativo em display OLED I2C (barramento separado)
- Cadastro, listagem e exclusão de pacientes via teclado 4x4
- Operações rápidas com botões físicos (Tare, Calibrar, Mostrar Peso)
- Feedback com Buzzer
- Armazenamento do fator de calibração na EEPROM

---

## 🔌 Conexões Físicas

### ESP32

| Componente            | Pino ESP32 |
|-----------------------|------------|
| **LCD I2C (Display 16x2)** |        |
| SDA                   | 23         |
| SCL                   | 22         |
| **OLED I2C (Menu)**   |            |
| SDA                   | 12         |
| SCL                   | 14         |
| **HX711 (Célula de Carga)** |       |
| DT                    | 15         |
| SCK                   | 2          |
| **Botões**            |            |
| Tare                  | 25         |
| Calibrar              | 27         |
| Registrar Peso        | 32         |
| Mostrar Peso Atual    | 34         |
| **Buzzer**            | 35         |
| **Teclado 4x4**       |            |
| R1                    | 21         |
| R2                    | 19         |
| R3                    | 18         |
| R4                    | 5          |
| C1                    | 17         |
| C2                    | 16         |
| C3                    | 4          |
| C4                    | 0          |

---

## 🎛️ Menu do Teclado (OLED)

Ao ligar o sistema, o menu aparece no display OLED:

- `A`: Cadastrar peso atual
- `B`: Listar pacientes cadastrados
- `C`: Excluir paciente
- `D`: Pausar sistema *(ainda não implementado)*

---

## 📦 Bibliotecas Utilizadas

- `Wire.h` (com `TwoWire` para segundo barramento I2C)
- `LiquidCrystal_I2C.h`
- `Adafruit_SSD1306.h`
- `Adafruit_GFX.h`
- `HX711.h`
- `EEPROM.h`
- `Keypad.h`

---

## 🚀 Como Usar

1. Conecte os componentes conforme a tabela acima.
2. Faça o upload do código para o ESP32.
3. Ligue o dispositivo e siga as instruções exibidas nos displays.

---

## 🛠️ Em Desenvolvimento

- Implementar função de pausa (`D`)
- Persistência de pacientes via EEPROM
- Interface de listagem mais avançada

---


![image](https://github.com/user-attachments/assets/8a640b97-414d-4c2d-bda7-4850630d30df)
