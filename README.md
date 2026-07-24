# Smart Sense Plug — Team AuraPlus (EN1190 EDP)

An intelligent motion-sensing add-on that improves the **safety and energy efficiency** of household appliances (particularly irons). The system automatically cuts power to an appliance that has been left idle for a user-defined period, helping prevent fire hazards and wasted energy — without replacing the appliance itself.

Built for the **EN1190 Engineering Design Project** at the University of Moratuwa, Department of Electronic & Telecommunication Engineering.

## How it works

The product is two cooperating units:

- **Iron Unit (motion unit)** — straps to the appliance. An **MPU-6050** accelerometer/gyroscope tracks motion; an **ESP32-S3** streams motion state over **ESP-NOW** (no Wi-Fi/router needed). Battery-powered (Li-Po) for full portability.
- **Plug Unit (power control unit)** — an ESP32-S3 receives the motion data, runs an inactivity timer, and switches a mains relay. A rotary encoder sets the idle timeout (1–30 min) and an RGB LED shows system state; a manual-override mode makes it behave as a normal plug.

When motion stops for the selected timeout, the plug cuts power automatically.

## My contribution

- **Sole developer of the Iron Unit firmware** — a FreeRTOS multi-threaded design with an ESP-NOW communication layer and an accelerometer-based motion-detection algorithm (acceleration-derivative thresholding).
- **Co-designed the Iron Unit PCB** — a compact SMD ESP32-S3 board (Altium Designer) with the MPU-6050, power regulation and USB-C charging.

## Repository structure

```
source_codes/iron_unit_final.ino/   # Final Iron Unit firmware (my work)
Testing/                            # Incremental bring-up sketches:
  acclerometer_test/                #   MPU-6050 read
  esp_now_test/                     #   ESP-NOW transmit/receive
  Bluetooth Testing/                #   NimBLE experiments
  sleep_modes_test/                 #   low-power sleep + ESP-NOW
  complete_test/                    #   integrated motion -> send
PCB_design/                         # Iron Unit Altium project (ESP32 + USB)
enclosure_design/                   # SolidWorks / Onshape enclosure models
```

## Tech

`ESP32-S3` · `FreeRTOS` · `ESP-NOW` · `MPU-6050` · `Altium Designer` · `SolidWorks` · Arduino/C++

## Team

Team AuraPlus — Fernando H.M.D, Induwara M.L.A.S., Ranasinghe D.P.H., Rasanjana W.P.G.R.A.
