# Grafana Flux Query Templates
# InfluxDB bucket: Smart_factory
# Confirm the exact bucket name/case in the deployed InfluxDB account.
# Telemetry measurement written by cloud_receiver/app.py: factory_esp32
# AI measurement written by cloud_receiver/ai_worker.py: factory_ai

## Temperature
```flux
from(bucket: "Smart_factory")
  |> range(start: -15m)
  |> filter(fn: (r) => r._measurement == "factory_esp32")
  |> filter(fn: (r) => r.machine == "machine1")
  |> filter(fn: (r) => r._field == "temperature")
```

## Pressure
```flux
from(bucket: "Smart_factory")
  |> range(start: -15m)
  |> filter(fn: (r) => r._measurement == "factory_esp32")
  |> filter(fn: (r) => r.machine == "machine1")
  |> filter(fn: (r) => r._field == "pressure")
```

## Motor speed
```flux
from(bucket: "Smart_factory")
  |> range(start: -15m)
  |> filter(fn: (r) => r._measurement == "factory_esp32")
  |> filter(fn: (r) => r.machine == "machine1")
  |> filter(fn: (r) => r._field == "motor_speed")
```

## Vibration
```flux
from(bucket: "Smart_factory")
  |> range(start: -15m)
  |> filter(fn: (r) => r._measurement == "factory_esp32")
  |> filter(fn: (r) => r.machine == "machine1")
  |> filter(fn: (r) => r._field == "vibration")
```

## Current
```flux
from(bucket: "Smart_factory")
  |> range(start: -15m)
  |> filter(fn: (r) => r._measurement == "factory_esp32")
  |> filter(fn: (r) => r.machine == "machine1")
  |> filter(fn: (r) => r._field == "current")
```

## Voltage
```flux
from(bucket: "Smart_factory")
  |> range(start: -15m)
  |> filter(fn: (r) => r._measurement == "factory_esp32")
  |> filter(fn: (r) => r.machine == "machine1")
  |> filter(fn: (r) => r._field == "voltage")
```

## Humidity
```flux
from(bucket: "Smart_factory")
  |> range(start: -15m)
  |> filter(fn: (r) => r._measurement == "factory_esp32")
  |> filter(fn: (r) => r.machine == "machine1")
  |> filter(fn: (r) => r._field == "humidity")
```

## AI fault probability
```flux
from(bucket: "Smart_factory")
  |> range(start: -15m)
  |> filter(fn: (r) => r._measurement == "factory_ai")
  |> filter(fn: (r) => r.machine == "machine1")
  |> filter(fn: (r) => r._field == "fault_probability")
```

## AI status (use a Stat/Table panel; status is a string field)
```flux
from(bucket: "Smart_factory")
  |> range(start: -15m)
  |> filter(fn: (r) => r._measurement == "factory_ai")
  |> filter(fn: (r) => r.machine == "machine1")
  |> filter(fn: (r) => r._field == "ai_status")
  |> last()
```

## Notes
- The current receiver does not map a `power` field. Do not create a power panel until the firmware/receiver is updated to calculate or receive it.
- The original query file used `factory_machine`; the supplied receiver writes `factory_esp32`.
- Use the same organization and bucket configured in the InfluxDB data source.
- Confirm that the dashboard time range includes recent points.
