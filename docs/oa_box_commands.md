# OA Box Commands

## Run mode display

```text
pH=7.487   7.945
T=12.74  C 11.00
```

* 7.487 - Current pH
* 7.945 - pH setpoint
* 12.74 - Current Temperature
* C - Temperature control set to control a chiller (C) or a heater (H)
* 11.00 - Temperature setpoint

## Keys during run mode

* A: Set pH setpoint
* B: Set temperature setpoint
* C: Calibration
* 1: One point pH calibration
* 2: Two point pH calibration
* D: Calibration Management
  * 1: See calibration slopes
  * 2: Clear current calibration
* 1: Display Unit Information
  * 1: IP Address
  * 2: MAC Address
  * 3: Software Version (v. 0.196 and later)
* 2: Clear LCD screen
* 3: Display Tank ID # and logfile name
* 4: Display PID constants
* 5: PID Tuning Menu
  * 1: Start Autotuning
  * 2: Manual Tuning
    * 1: Tune Kp (See below for Tuning Keys)
    * 2: Tune Ki
    * 3: Tune Kd
* 6: Temperature Calibration
* 7: Manually set device time
* 8: Enable/Disable PID control
* 9: Set Temperature control to chill or heat
* *: Change Google Sheet upload interval
* 0: Display current device time and uptime
* #: Change Tank ID #

## PID Manual Tuning Keys

* 1: +10000
* 4: -10000
* 2: +1000
* 5: -1000
* 3: +100
* 6: -100
* A: +10
* B: -10
* 7: +1
* *: -1
* 8: +0.1
* 0: -0.1
* #: End manual tuning
