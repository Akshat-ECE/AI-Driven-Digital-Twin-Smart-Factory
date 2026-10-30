# AI-Driven Digital Twin for Smart Factory Operations

HCLTech Embedded Systems & IoT project (Project 307: Knowledge-driven IoT Fault Diagnosis Assistant)

**Version:** v1.0 (TODO: confirm this label after final checks)

## Project Overview

This project is a digital-twin prototype for monitoring one machine ("machine1"). An ESP32 generates simulated machine telemetry and publishes it over MQTT. A cloud receiver stores the data in InfluxDB Cloud, a PyTorch model analyses it, and Grafana Cloud visualises the results.

## Important Notes and Limitations

- **The data is simulated.** The ESP32 firmware generates the seven machine parameters in code (random values following a repeating normal / warning / fault / recovery pattern). **No physical sensors are connected.** Real sensor integration is future work.
- **The AI model is a prototype.** It is a small PyTorch network trained on a very small hard-coded dataset. It has not been independently validated or calibrated. Its output is a model score, not a proven failure probability, and the classification threshold is a project parameter, not an industrial standard.
- **MQTT TLS is not fully verified.** The firmware encrypts traffic but does not verify the broker's certificate (`setInsecure()`), which is acceptable for a prototype but not for production.
- **Test results:** all tests in `docs/Test_Cases.xlsx` stay marked *Not Run* until they are actually executed and the evidence is recorded.

## System Architecture

```
ESP32 (simulated telemetry)
   -> MQTT broker (EMQX Cloud)
   -> Cloud Receiver (Python / Flask)
   -> InfluxDB Cloud (bucket: Smart_factory)
   -> AI analysis (PyTorch)  -> NORMAL / WARNING / FAULT
   -> Grafana Cloud dashboard
```

The architecture diagram is in `docs/Architecture_Diagram.png`.

## Monitored Parameters

Seven parameters are published every 3 seconds, one MQTT topic each:

| Parameter | MQTT topic |
|---|---|
| Temperature | `factory/machine1/temperature` |
| Pressure | `factory/machine1/pressure` |
| Motor speed | `factory/machine1/motor_speed` |
| Vibration | `factory/machine1/vibration` |
| Current | `factory/machine1/current` |
| Voltage | `factory/machine1/voltage` |
| Humidity | `factory/machine1/humidity` |

InfluxDB measurements: `factory_esp32` (telemetry, written by `cloud_receiver/app.py`) and `factory_ai` (AI output, written by `cloud_receiver/ai_worker.py`).

## Repository Structure

```
AI-Driven-Digital-Twin-Smart-Factory/
├── cloud_receiver/
│   ├── app.py                 # receiver that stores telemetry in InfluxDB
│   ├── ai_worker.py           # AI analysis worker
│   └── requirements.txt       # receiver dependencies
├── firmware/
│   └── esp32_machine_simulator/
│       └── esp32_machine_simulator.ino   # ESP32 firmware (credentials are placeholders)
├── src/
│   └── esp32_ai.py
├── grafana/
│   └── queries.md             # Flux queries for the dashboard
├── docs/                      # problem statement, SRS, report, test cases, diagram, deck
├── .env.example               # template for local secrets
├── requirements.txt
└── README.md
```

## Setup

### 1. Python environment

```bash
git clone https://github.com/Akshat-ECE/AI-Driven-Digital-Twin-Smart-Factory.git
cd AI-Driven-Digital-Twin-Smart-Factory
python -m venv venv
venv\Scripts\activate          # Windows
pip install -r requirements.txt
```

### 2. Credentials

Copy `.env.example` to `.env` and fill in your own InfluxDB values. **Never commit `.env`**; it is listed in `.gitignore`.

### 3. Run the cloud receiver

TODO: add the exact command you use to start the receiver (for example `python cloud_receiver/app.py`) and where it is deployed.

### 4. Flash the ESP32 firmware

1. Open `firmware/esp32_machine_simulator/esp32_machine_simulator.ino` in the Arduino IDE with ESP32 board support installed.
2. Install the **PubSubClient** library.
3. Replace the placeholders `YOUR_WIFI_SSID`, `YOUR_WIFI_PASSWORD`, `YOUR_EMQX_BROKER_HOST`, `YOUR_MQTT_USERNAME` and `YOUR_MQTT_PASSWORD` with your own values (do not commit them).
4. Upload to the ESP32 and open the Serial Monitor at 115200 baud.

## Usage

1. Start the cloud receiver and power the ESP32.
2. The ESP32 publishes simulated values every 3 seconds; the Serial Monitor shows each topic and value.
3. Open the Grafana dashboard to see the parameters and the AI output. Queries are in `grafana/queries.md`; make sure the dashboard time range includes recent points.

Dashboard link: TODO (add only if you want it public).

## Testing

Unit and system test cases are in `docs/Test_Cases.xlsx`. Status: **Not Run** until executed and recorded (see `docs/Test_Report_Template.docx`).

## Demo Video

TODO: add the link to the live demo video.

## Documentation

See `docs/DOCUMENTATION_INDEX.md` for the list of project documents (problem statement, SRS, project report, test cases, architecture diagram and presentation).

## Future Work

- Connect real physical sensors to the ESP32 (for example a BME680 for temperature, humidity and pressure)
- Train and validate the model on real or properly generated data
- Verify the MQTT broker's TLS certificate
- Monitor multiple machines and add automated alerts

## Author

Akshat Kaushal, B.Tech Electronics & Communication Engineering (3rd year, ECE-A), ABES Engineering College
