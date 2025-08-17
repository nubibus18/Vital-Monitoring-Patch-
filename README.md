# Low-power Wireless PPG BLE Patch

This project implements a **wearable patch** for physiological signal acquisition, focusing on **Photoplethysmography (PPG)** and **Electrocardiography (ECG)** with **Bluetooth Low Energy (BLE)** connectivity.  
Built around the **ADPD1080 Analog Front-End** and **STM32WB55 MCU**, it supports low-power operation from coin cells.

---

## 🔧 Hardware Overview
- **AFE:** ADPD1080 for optical biosignal acquisition  
- **MCU:** STM32WB55 for BLE streaming and processing  
- **Power:** Coin cell + boost converter for 3.3 V  
- **PCB:** 4-layer, impedance-controlled, Altium design  

### PCB Layout
![PCB Layout TOP](Images%20and%20Videos/PPGLEDside.png)
![PCB Layout Back](Images%20and%20Videos/PPGModuleSide.png)

### Assembled Board
![Assembled Board](Images%20and%20Videos/STMmodule.jpg)

---

## 💻 Firmware Overview

The **Firmware** folder contains:
- **ADPDEvaluation and Setup** — Arduino code for ESP32 to interface with the ADPD1080 evaluation board and connect directly to a PC.  
  - Uses `ADPD1080regconfig.py` to generate register values from parameterized input and instantly write them to the device.  
  - ESP32 continuously fetches readings and sends them via a UDP server.  
  - Data reception handled by `ServerRecieve22.py` (2-value mode) and `ServerRecieve66.py` (6-value FIFO mode).  
  - This setup enables quick testing of different register configurations to identify optimal sensor parameters.  
- Each receive script generates a **CSV** file, which can be processed using the provided MATLAB scripts for signal analysis.  
- **Final Firmware** — Complete STM32WB55 code is provided, which accesses data from the ADPD1080 AFE and streams it over BLE to an Android device for real-time monitoring and logging.

---


## 🛠 Hardware Design Files

The **Hardware Design** folder contains:
- **Evaluation Design** — Altium design files for the ADPD1080 evaluation board.  
- **Final Module Design** — Altium design files for the integrated STM32WB + ADPD1080 AFE module, including battery support.  

---

## 🎥 Assembly Video
[▶ Watch Assembly Video](Images%20and%20Videos/CompleteDesign.mp4)

---

## 📊 Results

| Raw + Processed PPG Signal | Bandpass (0.5–5 Hz) FFT |
|----------------------------|------------------------|
| ![PPG Raw](Results/Pre%20and%20Post%20Filtered%20SIgnal.jpg) | ![Bandpass](Results/FFT-Bandpass.jpg) |

---

## 🚀 How to Use
1. Clone this repository  
   ```bash
   git clone https://github.com/balodi182/PPG-BLE-Patch.git
