# ESP32 Temperature and Humidity Monitoring with LED Control

## Overview

This project involves interfacing an ESP32 microcontroller with a DHT22 temperature and humidity sensor to monitor environmental conditions. The collected data is then transmitted to a local server running XAMPP with a MySQL database. Additionally, the project enables remote access to the stored sensor readings and allows for real-time monitoring of temperature, humidity, and LED status. Users can control two LEDs remotely, updating their status as needed.

## Features

- **Temperature and Humidity Monitoring**: The ESP32 reads temperature and humidity data from the DHT22 sensor.
- **Local Server**: Utilizes XAMPP with MySQL for storing sensor readings locally.
- **Remote Access**: Provides access to sensor data stored in the database, allowing users to retrieve historical readings.
- **Real-time Monitoring**: Enables real-time monitoring of temperature, humidity, and LED status.
- **LED Control**: Allows users to remotely control the status of two LEDs connected to the ESP32.
- **GraphQL with MQTT**: Experimentation with GraphQL protocol over MQTT for efficient data transmission and querying.

## Setup Instructions

1. **Hardware Setup**: Connect the DHT22 sensor and LEDs to the ESP32 according to the wiring diagram.
2. **Software Installation**:
   - Install the required libraries for ESP32, DHT22, CoAP, and any other dependencies.
   - Set up XAMPP with MySQL on your local machine.
3. **Configure ESP32**:
   - Update the SSID and password in the code to connect to your Wi-Fi network.
   - Modify the server IP address and endpoints as per your local server configuration.
4. **Upload Code**: Upload the Arduino sketch to your ESP32 board using the Arduino IDE or PlatformIO.
5. **Run Server**: Start the XAMPP server and ensure that MySQL is running.
6. **Access Data**: Access sensor data and control LEDs remotely using CoAP commands or HTTP requests.
7. **Explore GraphQL with MQTT**: Experiment with GraphQL protocol over MQTT for optimized data transmission and querying.
8. **Future Perspective**: Implement two-way communication between two ESP32 boards (esp32_01 & esp32_02).

## Usage

- Use CoAP commands or HTTP requests to retrieve sensor data and control LED status.
- Monitor temperature, humidity, and LED status in real-time through the provided endpoints.
- Access historical sensor readings stored in the MySQL database for analysis or visualization.

## Dependencies

- [Arduino IDE](https://www.arduino.cc/en/software)
- [ESP32 Board Package](https://github.com/espressif/arduino-esp32)
- [DHT Sensor Library](https://github.com/adafruit/DHT-sensor-library)
- [CoAP Simple Library](https://github.com/automote/ESP-CoAP)
- [GraphQL over MQTT Library](https://github.com/graphql/graphmqtt)
- #include <HTTPClient.h>
- #include <Arduino_JSON.h>

## Contributors

- MohamedSalah Touati (https://github.com/DevMedSalah01)
- Special thanks to [@Ahmed Logs YTB Channel]

## License

This project is licensed under the [MIT License](LICENSE).
