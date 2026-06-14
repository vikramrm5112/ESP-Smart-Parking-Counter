# ESP32 Smart Parking System

A simple smart parking system using an ESP32-WROOM-32S, two IR sensors, a 16x2 I2C LCD, and a built-in web dashboard.

The system:

* Detects occupied parking slots using IR sensors.
* Calculates available parking spaces.
* Displays available spaces on a 16x2 I2C LCD.
* Displays available spaces on the Serial Monitor.
* Hosts a live web page that updates automatically.

---
## Hardware Required

| Qty     | Component           |
| ------- | ------------------- |
| 1       | ESP32-WROOM-32S     |
| 2       | IR Obstacle Sensors |
| 1       | 16x2 I2C LCD Module |
| 1       | USB Cable           |
| Several | Jumper Wires        |

---

# Pin Connections

## IR Sensors

### IR Sensor 1

| Sensor Pin | ESP32 Pin |
| ---------- | --------- |
| VCC        | 3.3V      |
| GND        | GND       |
| OUT        | GPIO18    |

### IR Sensor 2

| Sensor Pin | ESP32 Pin |
| ---------- | --------- |
| VCC        | 3.3V      |
| GND        | GND       |
| OUT        | GPIO19    |

---

## I2C LCD

| LCD Pin | ESP32 Pin |
| ------- | --------- |
| VCC     | VIN (5V)  |
| GND     | GND       |
| SDA     | GPIO21    |
| SCL     | GPIO22    |

---

# Wiring Diagram

```text
ESP32
│
├── GPIO18 ───── IR Sensor 1 OUT
├── GPIO19 ───── IR Sensor 2 OUT
│
├── GPIO21 ───── LCD SDA
├── GPIO22 ───── LCD SCL
│
├── 3.3V ─────── IR Sensor 1 VCC
├── 3.3V ─────── IR Sensor 2 VCC
│
├── VIN (5V) ─── LCD VCC
│
└── GND ──────── All Grounds
```

---

# Software Requirements

## Arduino IDE

Download and install Arduino IDE.

---

# Install ESP32 Board Package

Open:

```text
File → Preferences
```

Add the following URL to:

```text
Additional Board Manager URLs
```

```text
https://espressif.github.io/arduino-esp32/package_esp32_index.json
```

Click OK.

---

Open:

```text
Tools → Board → Boards Manager
```

Search for:

```text
ESP32
```

Install:

```text
esp32 by Espressif Systems
```

---

# Select Board

Choose:

```text
Tools → Board → ESP32 Arduino → ESP32 Dev Module
```

---

# Install Required Libraries

Open:

```text
Sketch → Include Library → Manage Libraries
```

Install:

## LiquidCrystal I2C

Library:

```text
LiquidCrystal I2C
```

---

# Configure Wi-Fi

Open the code and replace:

```cpp
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";
```

Example:

```cpp
const char* ssid = "MyWiFi";
const char* password = "MyPassword";
```

---

# Uploading the Program

1. Connect the ESP32 using USB.
2. Select the correct COM port.
3. Click Upload.
4. Wait for compilation and upload to finish.

---

# Serial Monitor

Open:

```text
Tools → Serial Monitor
```

Set baud rate:

```text
115200
```

Example output:

```text
Connecting to WiFi...
Connected!

IP Address: 192.168.1.123

Slots Available: 2
Slots Available: 1
Slots Available: 0
```

---

# LCD Display

When powered on:

```text
Connecting...
To WiFi
```

After Wi-Fi connects:

```text
WiFi Connected
```

Normal operation:

```text
Parking Slots
Available: 2
```

When one slot is occupied:

```text
Parking Slots
Available: 1
```

When both slots are occupied:

```text
Parking Slots
Available: 0
```

---

# Web Dashboard

After connecting to Wi-Fi, the Serial Monitor displays an IP address.

Example:

```text
192.168.1.123
```

Open a browser on any device connected to the same Wi-Fi network and visit:

```text
http://192.168.1.123
```

The dashboard displays:

```text
Parking Slots

2

Available Spaces
```

The number updates automatically every second.

No page refresh is required.

---

# Parking Logic

The system assumes:

* Total parking spaces = 2
* Each IR sensor represents one parking slot

Calculation:

```text
Available Slots = Total Slots - Occupied Slots
```

---

# Occupancy Table

| IR1     | IR2     | Occupied | Available |
| ------- | ------- | -------- | --------- |
| Clear   | Clear   | 0        | 2         |
| Blocked | Clear   | 1        | 1         |
| Clear   | Blocked | 1        | 1         |
| Blocked | Blocked | 2        | 0         |

---

# Troubleshooting

## LCD Does Not Display Anything

Check:

* SDA connected to GPIO21
* SCL connected to GPIO22
* LCD powered correctly
* Contrast potentiometer adjusted

---

## LCD Shows Random Characters

Verify the I2C address.

Common addresses:

```text
0x27
```

or

```text
0x3F
```

If necessary, run an I2C Scanner sketch.

---

## Sensors Always Show Occupied

Some sensors output HIGH instead of LOW.

Change:

```cpp
digitalRead(IR1) == LOW
digitalRead(IR2) == LOW
```

to:

```cpp
digitalRead(IR1) == HIGH
digitalRead(IR2) == HIGH
```

---

## Wi-Fi Does Not Connect

Verify:

* SSID is correct
* Password is correct
* Network is 2.4 GHz

---

## Cannot Open Dashboard

Check:

* ESP32 is connected to Wi-Fi
* Device is on the same Wi-Fi network
* IP address is correct

---

# Expected Result

The ESP32 continuously monitors parking occupancy and displays the available spaces on:

1. 16x2 I2C LCD
2. Serial Monitor
3. Web Browser Dashboard

Example:

```text
Parking Slots
Available: 1
```

The display updates automatically whenever a vehicle enters or leaves a parking space.

---

# License

This project is open source and may be modified for educational, personal, and commercial use.
