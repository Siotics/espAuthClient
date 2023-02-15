# espAuthClient

Untuk versi bahasa Indonesia, [Klik ini](#bahasa-indonesia)

The ESP8266 and ESP32 Client for [espAuthAbsent](https://github.com/Cydnirn/espAuthAbsent)

## Contents

- [Used Library](#used-library)
- [Hardware Requirements](#hardware-requirements)
- [How to use](#how-to-use)

## Used Library

For the ESP8266:
-  <SPI.h>
- <MFRC522.h>
- <ESP8266WiFi.h>
- <WiFiManager.h>
- <PubSubClient.h>
- <ArduinoJson.h>

## Hardware Requirements

It Requires the ESP32 or the ESP8266 devkit
It also require an RFID scanner, in this repo it use RC522
You are **not required** to add an active buzzer, but in doing so makes the output vague

## How To Use

**For wiring, follow the devkit pinout referrence or follow the pin inside the code**

Follow these steps:

1. Connect all required hardware
2. Upload the code 
3. Connect the device to a power outlet
4. Connect to the device AP, which by default is **ESP CONFIG** with default password **siotics0rgConf**
5. If you have set up the mqtt server, add the IP and the port, **DO NOT ADD HTTP FOR THE IP OR WILL RESULT IN ERROR**

To send to server, make sure you have set up the backend and the mqtt server which you can get [here](https://github.com/Cydnirn/espAuthAbsent)

# Bahasa Indonesia

## Penjelasan

Klien ESP8266 dan ESP32 untuk [espAuthAbsent](https://github.com/Cydnirn/espAuthAbsent)

## Konten

- [Library yang Digunakan](#library-yang-digunakan)
- [Perangkat Keras yang Dibutuhkan](#perangkat-keras-yang-dibutuhkan)
- [Cara Penggunaan](#cara-penggunaan)

## Library yang Digunakan

Untuk ESP8266:
-  <SPI.h>
- <MFRC522.h>
- <ESP8266WiFi.h>
- <WiFiManager.h>
- <PubSubClient.h>
- <ArduinoJson.h>

## Perangkat Keras yang Dibutuhkan

Perangkat ini membutuhkan ESP8266 atau ESP32 Devkit
Perangkat ini juga membutuhkan scanner RFID, pada repo ini ia menggunakan RC522
Anda **tidak wajib** untuk menambahkan active buzzer, tetapi akan membuat outputnya tidak jelas kepada user

## Cara Penggunaan

**Untuk pengkabelan, ikuti referensi pinout untuk devkit yang digunakan atau ikuti pin yang ada di dalam kode**

Ikuti langkah di bawah:

1. Hubungkan semua perangkat yang dibutuhkan
2. Upload kodenya
3. Hubungkan perangkat ke sumber listrik
4. Hubungkan ke AP Perangkat, secara default namanya adalah **ESP CONFIG** dengan default password **siotics0rgConf**
5. Jika Anda sudah memasang server mqtt, tambahkan IP dan Portnya, **JANGAN TAMBAHKAN HTTP UNTUK IP ATAU AKAN BERAKHIR DENGAN ERROR**

Untuk mengirim ke server, pastikan Anda sudah memasang server backend dan mqtt perangkat ini [di sini](https://github.com/Cydnirn/espAuthAbsent)
