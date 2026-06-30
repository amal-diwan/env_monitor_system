# 🌡️ Environmental Monitoring System

> A modular, non-blocking embedded firmware for an Arduino-based environmental monitor — it tracks **temperature, humidity, and ambient light**, raises tiered visual/audible alerts against user-configurable thresholds, and persists its configuration across power cycles.

<p align="center"> <img src="circuit.png" alt="Environmental Monitoring System" width="480"> </p>

---

## Overview

This project is a complete embedded firmware for a real-time environmental monitor, built on bare Arduino with **no RTOS and no blocking `delay()` calls**. It continuously samples three sensors, presents live readings on an LCD, and warns the user when any reading crosses a configurable limit.

Rather than cramming everything into one `.ino` sketch, the firmware is split into focused, single-responsibility modules coordinated by a small cooperative task scheduler — the kind of layered structure you'd expect in production embedded code, scaled down to a microcontroller.

Thresholds can be tuned three ways — on-device buttons, a serial command line, or sensible defaults — and are saved to EEPROM with an integrity check so they survive reboots.

---

## What this project demonstrates

- **Embedded C++ architecture** — a clean, layered codebase (sensor, display, alert, input, storage, serial, scheduler) with shared state and configuration isolated into their own headers.
- **Non-blocking, cooperative scheduling** — a table-driven scheduler runs each task on its own interval, keeping the main loop responsive and free of `delay()`-based stalls.
- **Finite state machines** — explicit state machines drive both the UI (display vs. threshold-editing modes) and the alert system (normal / warning / sensor-fault / no-data).
- **Robust signal handling** — hardware button debouncing and alarm hysteresis prevent input bounce and alert "chatter" near thresholds.
- **Persistent configuration** — EEPROM storage guarded by a magic-number + range-validation scheme, with automatic fallback to defaults on first boot or corruption.
- **Fault tolerance** — detects sensor read failures (NaN) and missing data, surfacing each as a distinct, clearly-signalled state.
- **Human interfaces** — a 16×2 LCD UI plus a serial command-line interface for configuration and live status.

---

## System architecture
 
<p align="center"> <img src="sa.png" alt="System Architecture" width="480"> </p>
 
The firmware is organized around a central **shared state** with a small **cooperative scheduler** on top. The scheduler holds a table of tasks, each with its own interval, and runs only the ones whose time has come — so reading a sensor once per second never blocks polling the buttons every 20 ms, giving multitasking-like responsiveness without an RTOS.
 
Each module owns a single concern (sensors, display, alerts, input, serial) and never calls the others directly. Instead they all communicate through the shared state — sensors and the input/serial interfaces *write* to it, while the display and alert modules *read* from it. This keeps every module decoupled and independently testable. Beneath it, a persistence layer saves validated thresholds to **EEPROM** and restores them on boot, falling back to defaults if the stored data is missing or corrupt.
 
---

## Features

- **Live monitoring** of temperature & humidity (DHT11) and ambient light (LDR).
- **Configurable alarm thresholds** for all three channels, with min/max safety bounds.
- **Tiered RGB-LED + buzzer alerts:**
  - 🟢 Green — all readings normal
  - 🔴 Red + buzzer — a threshold has been crossed
  - 🟡 Blinking Yellow — sensor fault detected
  - ⚫ Off — no data yet
- **Hysteresis** on every alarm so readings hovering at a limit don't flicker the alert on and off.
- **On-device editing** via three debounced buttons (menu / increment / decrement) with a guided edit menu.
- **Serial command line** for remote configuration and status readout.
- **EEPROM persistence** with corruption detection and graceful fallback to defaults.

---

## Tech stack

| Area              | Details                                                        |
|-------------------|----------------------------------------------------------------|
| Language          | Embedded C++ (Arduino framework)                               |
| Target            | Arduino Uno / any ATmega328-based board                        |
| Sensors           | DHT11 (temperature + humidity), LDR / photoresistor (light)    |
| Output            | 16×2 HD44780 LCD, RGB LED, passive buzzer                       |
| Input             | 3 momentary push buttons (debounced)                           |
| Persistence       | On-chip EEPROM                                                  |
| Libraries         | `DHT`, `LiquidCrystal`, `EEPROM`                               |

---

## Engineering highlights

A few design choices that lift this above a typical single-file Arduino sketch:

- **A tiny cooperative kernel.** Tasks are stored in a table of `{function, interval, lastRun}` and dispatched by elapsed-time comparison — a minimal scheduler that gives multitasking-like behavior without an RTOS, and keeps the loop fully non-blocking.
- **Separation of concerns.** State (`state.h`), configuration (`config.h`), and each subsystem live in their own translation units, so any module can be modified or tested in isolation. Internal helpers are `static` to keep module interfaces small.
- **Hysteresis-based alarms.** Each alarm arms above its limit and only disarms after the reading falls back by a defined margin, eliminating rapid on/off toggling at the boundary.
- **Validated persistence.** Stored config carries a magic number and is range-checked on load; anything invalid triggers a clean reset to defaults — no garbage values ever drive the system.
- **Explicit fault states.** "No data," "sensor fault," "warning," and "normal" are modeled as distinct states with their own LCD messages and indicator behavior, rather than being lumped into a single error path.

---

## Repository structure

```
.
├── main.ino          # Entry point: wires modules together, registers scheduler tasks
├── scheduler.*       # Cooperative, interval-based task scheduler
├── state.h           # Shared data structures, enums, and global state
├── config.h          # Pin assignments, timing intervals, limits, defaults
├── sensor.*          # DHT11 + light sensor reading and validation
├── display.*         # LCD rendering for live data and edit modes
├── alert.*           # Alarm logic (hysteresis) + RGB LED / buzzer control
├── input.*           # Debounced buttons and threshold-editing menu FSM
├── storage.*         # EEPROM save/load with validation
└── serial_cmd.*      # Serial command-line interface
```

---

## Serial command reference

Connect at **9600 baud**:

| Command          | Action                                    |
|------------------|-------------------------------------------|
| `get`            | Print current thresholds                  |
| `save`           | Persist thresholds to EEPROM              |
| `defaults`       | Restore default thresholds                |
| `status`         | Print live sensor data, mode, and alerts  |
| `set temp <x>`   | Set temperature threshold (°C)            |
| `set hum <x>`    | Set humidity threshold (%)                |
| `set light <x>`  | Set light threshold (0–1023)              |

On-device, the **menu** button cycles through edit modes (temp → humidity → light → save & exit) while **inc/dec** adjust the selected value.

---

## Wiring

| Component        | Pin(s)                          |
|------------------|---------------------------------|
| LCD (16×2)       | RS 7, EN 8, D4 9, D5 10, D6 11, D7 12 |
| DHT11            | 6                               |
| Light sensor (LDR)| A0                             |
| RGB LED          | Red A1, Green A2, Blue A3       |
| Buzzer           | 5                               |
| Buttons          | Dec 2, Menu 3, Inc 4 (`INPUT_PULLUP`) |

---


## Possible extensions

- Add data logging (SD card or serial-to-host) for long-term trend analysis
- Replace the DHT11 with a higher-precision sensor (e.g. SHT31) and add calibration
- Add wireless reporting (ESP8266/ESP32) to push readings to a dashboard
- Implement a unit-test harness for the alarm/hysteresis logic on host hardware

