# ESP32 RC Car with Web-Based Control

A Wi-Fi controlled RC car built using the **ESP32 DevKit V1** and **ESP-IDF**. The car hosts its own web server, allowing users to connect directly to the ESP32 through a mobile device and control the vehicle from a responsive web interface.

## Features

- 🚗 Forward and reverse driving
- ↔️ Left and right steering
- 🛑 Emergency stop
- 🎚️ Adjustable motor speed (0–100%)
- 📶 ESP32 SoftAP (Wi-Fi hotspot)
- 🌐 Built-in web server for browser-based control
- 📱 Mobile-friendly control interface
- 🔋 Battery monitoring support (via ADC and voltage divider)

## Hardware Used

- ESP32 DevKit V1
- TB6612FNG Dual Motor Driver
- LM2596 Buck Converter
- 2×18650 Li-ion Batteries
- Rear DC Drive Motor
- Front DC Steering Motor
- Voltage Divider (220kΩ and 100kΩ) for battery monitoring
- Jumper wires
- Chassis

## Software

- ESP-IDF v6.0.2
- C
- ESP HTTP Server
- FreeRTOS
- LEDC PWM Driver
- ADC One-Shot Driver

## Wi-Fi Configuration

The ESP32 creates its own wireless network.

| Setting | Value |
|---------|-------|
| SSID | RC_CAR |
| Password | 12345678 |

Connect your phone or computer to this Wi-Fi network and open the following address in a browser:

```
http://192.168.4.1
```

## Controls

| Button | Function |
|---------|----------|
| ▲ | Move Forward |
| ▼ | Move Backward |
| ◀ | Steer Left |
| ▶ | Steer Right |
| ■ | Stop Vehicle |
| Speed Slider | Adjust Motor Speed |

## Project Structure

```
Project_RC_Car/
│
├── main/
│   ├── main.c
│   ├── motor.c
│   ├── motor.h
│   ├── wifi.c
│   ├── wifi.h
│   ├── webserver.c
│   ├── webserver.h
│   ├── webpage.h
│   ├── battery.c
│   └── battery.h
│
├── CMakeLists.txt
├── sdkconfig
└── README.md
```

## Battery Monitoring

Battery voltage is measured using the ESP32 ADC through a voltage divider.

### Voltage Divider

```
Battery (+)
     |
   220kΩ
     |
     +------ GPIO34
     |
   100kΩ
     |
Battery (-) ------ ESP32 GND
```

The battery percentage is calculated in software and displayed on the web interface.

## Motor Connections

### Rear Motor

| ESP32 GPIO | TB6612 |
|------------|---------|
| GPIO25 | PWMA |
| GPIO26 | AIN1 |
| GPIO27 | AIN2 |

### Steering Motor

| ESP32 GPIO | TB6612 |
|------------|---------|
| GPIO14 | PWMB |
| GPIO18 | BIN1 |
| GPIO19 | BIN2 |

### Enable

| ESP32 GPIO | TB6612 |
|------------|---------|
| GPIO33 | STBY |

## How to Build

1. Clone the repository.

```
git clone https://github.com/YOUR_USERNAME/Project_RC_Car.git
```

2. Open the project in **Espressif IDE** or use the ESP-IDF command line.

3. Configure the project if needed.

```
idf.py menuconfig
```

4. Build the project.

```
idf.py build
```

5. Flash the ESP32.

```
idf.py flash
```

6. Monitor the serial output.

```
idf.py monitor
```

## Future Improvements

- OTA firmware updates
- Camera streaming
- Obstacle avoidance
- Line following mode
- Bluetooth control
- Battery charging status
- Headlights and turn signals
- Joystick control
- Autonomous navigation

## Demo

*(Add photos or videos here.)*

## Author

**Edmarc Justin C. Oabel**

BS Computer Engineering  
Mapúa University

## License

This project is licensed under the MIT License.
