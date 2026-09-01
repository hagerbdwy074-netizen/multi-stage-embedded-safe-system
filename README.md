# Multi-Stage Secure Smart Safe System (AVR)

An advanced embedded security safe box built on an AVR microcontroller (ATmega32/16) operating at 8MHz. The system implements a sequential **Multi-Factor Authentication (MFA)** pipeline controlled via a Finite State Machine (FSM).

---

## 🔒 Security Workflow

Accessing the safe requires passing three sequential verification stages:

```
[IDLE] 
   │
   ▼ (1. IR Detection)
[IR VERIFIED] 
   │
   ▼ (2. Keypad PIN: 4 Digits)
[READY FOR PASSWORD] 
   │
   ▼ (3. Morse Code: Dot-Dash)
[MORSE CODE] 
   │
   ▼
[UNLOCKED] (Auto-locks after 12s or press '*')
```

1. **Stage 1 (Proximity Check):** IR sensor detects an object/hand. Pressing keypad buttons out of sequence triggers a penalty.
2. **Stage 2 (PIN Authentication):** 4-digit PIN (`1328`) entered via a 4x4 matrix keypad.
3. **Stage 3 (Timing / Morse Code):** Push-button timing detection. Requires a **Dot-Dash (`.-`)** pattern (short press `<350ms`, followed by long press `≥350ms`).

---

## 🛡️ Anti-Tamper & Failsafe Features

* **Progress Indicators:** 3 dedicated LEDs (`STEP1_LED`, `STEP2_LED`, `STEP3_LED`) turn on sequentially as each stage is cleared.
* **Intrusion Alarm Mode:** Exceeding attempt thresholds (2 failed PINs, 2 wrong Morse sequences, or repeated sequence violations) activates an audible and visual alarm with a 20-second countdown.
* **Alarm Disarm Sequence:** The alarm can be bypassed within the 20-second window by triggering the IR sensor followed immediately by the Morse code sequence.
* **System Lockdown & Admin Override:** If the 20-second alarm expires without deactivation, the system enters `SHUTDOWN_MODE`. Restoring normal operation requires:
  1. Activating the hardware `ADMIN_PIN`.
  2. Entering the 4-digit Master Password (`1010`).

---

## 📌 Hardware Pinout (PORTB)

| Pin | Identifier | Type | Connected Component |
| :--- | :--- | :--- | :--- |
| **PB0** | `IR_DETECT` | Input | Active-Low IR Sensor Module |
| **PB1** | `RED_LED` | Output | Alarm Indicator LED |
| **PB2** | `BUZZER` | Output | Active Buzzer |
| **PB3** | `BUTTON` | Input | Morse Code Push Button |
| **PB4** | `ADMIN_PIN` | Input | Admin Physical Switch / Jumper |
| **PB5** | `STEP1_LED` | Output | Stage 1 Completion LED (IR) |
| **PB6** | `STEP2_LED` | Output | Stage 2 Completion LED (PIN) |
| **PB7** | `STEP3_LED` | Output | Stage 3 Completion LED (Morse) |

*Note: The 16x2 Character LCD and Keypad drivers utilize standard 8-bit or 4-bit parallel interfacing defined in `Lcd_Driver.h` and `Keypad.h`.*

---

## ⚙️ Software Architecture

The firmware utilizes a non-blocking state machine in `main()` with distinct states:

* `IDLE`
* `IR_VERIFIED`
* `READY_FOR_PASSWORD`
* `MORSE_CODE`
* `UNLOCKED`
* `ALARM_MODE`
* `SHUTDOWN_MODE`
* `ADMIN_OVERRIDE`
* `MISSING_COND`

### Custom LCD Characters
Custom 5x8 bitmaps are written to the LCD CGRAM at initialization:
* **Address `0x40` (Index 0):** Closed Padlock icon ($\approx$ Locked State).
* **Address `0x48` (Index 1):** Open Padlock icon ($\approx$ Unlocked State).

---

## 🚀 Getting Started

### Prerequisites
* **Toolchain:** `avr-gcc`, `avr-libc`, `avrdude` or **Microchip Studio (Atmel Studio)**.
* **Clock Frequency:** $8\text{ MHz}$ Internal/External Oscillator (`F_CPU 8000000UL`).

### Building via AVR-GCC
```bash
# Compile
avr-gcc -Wall -Os -mmcu=atmega32 -DF_CPU=8000000UL -c main.c -o main.o
avr-gcc -Wall -Os -mmcu=atmega32 -DF_CPU=8000000UL -c DIO_program.c -o DIO_program.o
avr-gcc -Wall -Os -mmcu=atmega32 -DF_CPU=8000000UL -c Keypad.c -o Keypad.o
avr-gcc -Wall -Os -mmcu=atmega32 -DF_CPU=8000000UL -c Lcd_Driver.c -o Lcd_Driver.o

# Link
avr-gcc -mmcu=atmega32 main.o DIO_program.o Keypad.o Lcd_Driver.o -o safe_system.elf

# Generate Hex
avr-objcopy -O ihex -R .eeprom safe_system.elf safe_system.hex

# Flash (Example using USBasp)
avrdude -c usbasp -p m32 -U flash:w:safe_system.hex:i
```

---

## 🔑 Default Credentials

| Role | Mechanism | Value |
| :--- | :--- | :--- |
| **User PIN** | Keypad | `1328` |
| **User Morse** | Push Button | `.` (Short) then `-` (Long) |
| **Admin Key** | Keypad (Post-Lockdown) | `1010` |
