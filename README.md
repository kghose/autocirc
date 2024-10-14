# Autocirc
Integrated control for oil fired baseboard and heat pumps. 

# Block diagram

```
                       ┌────────────────────────────────┐                          
                       │                                │                          
                       │  ┌──────────────┐              │                          
┌──────────────┐       │  │              │              │                          
│ Outdoor temp ├───────┼─►│              │              │   ┌───────────────────┐  
└──────────────┘       │  │ Boiler       │              │   │                   │  
                       │  │ Controller   ┼──────────────┼──►│ Circulator Input  │  
┌──────────────┐       │  │              │              │   │                   │  
│ Pipe temp    ├───────┼─►│              ┼─────────┐    │   └───────────────────┘  
└──────────────┘       │  │              │         │    │                          
                       │  └───┬──▲──── ──┘         │    │                          
                       │      │  │                 │    │                          
                       │      │  │                 │    │                          
                       │      │  │                 │    │                          
                       │  ┌─ ─▼──┴───────┐         │    │                          
┌──────────────┐       │  │              │         │    │                          
│ Thermostat   ├───────┼─►│ Integrated   │    ┌────▼──┐ │                          
└──────────────┘       │  │ Controller   │◄───┤ Shared│ │                          
                       │  │              │    │ Data  │ │                          
                       │  └──────┬───────┘    └────▲──┘ │                          
                       │         │                 │    │                          
                       │         │                 │    │                          
                       │  ┌──────▼──── ──┐         │    │                          
                       │  │              │         │    │                          
                       │  │ Heat Pump    │         │    │                          
                       │  │ Controller   │         │    │                          
                       │  │              │         │    │                          
                       │  └──────┬───────┘         │    │                          
                       │         │                 │    │                          
                       │         ▼                 │    │                          
┌──────────────┐       │  ┌──────────────┐         │    │                          
│ User Input   ┼───────┼─►│              │         │    │                          
│ and Display  │       │  │ CoAP         ┼─────────┘    │    ┌────────────────────┐
│ (Browser     │       │  │ Server       │              │    │ Heat Pump          │
│  Interface)  │       │  │              │◄─────────────┼────┼ Airhandler         │
│              │◄──────┼──┤              │              │    ┤ Driver             │
└──────────────┘       │  └──────────────┘              │    └────────────────────┘
                       │                                │                          
                       └────────────────────────────────┘                          
```

1. The Boiler Controller has a fail-safe mode that operates independently of 
   everything else, where it runs the circulator based on outdoor temp
1. All modules read from (and sometimes write to) a shared state protected by a 
   mutex
1. The Boiler Controller writes out the outdoor temp to the shared state
1. The CoAP server reads the shared state/data to display it to the user via the 
   browser interface
1. The CoAP server writes to the shared state/data based on user input
1. The Integrated Controller decides to pass on the thermostat state (on/off) to 
   the boiler controller based on outdoor temp and the cross over temp
1. The Integrated Controller decides to switch on/off heat pumps based on cross 
   over temp
1. The heat pump controller handles CoAP responses based on what the integrated 
   controller tells it.









# Check ESP32 connection via USB

(https://docs.espressif.com/projects/esp-idf/en/v5.3.1/esp32/get-started/establish-serial-connection.html)

List all the serial devices
```
ls /dev/tty*
```

Try `screen` or `cu` to call up the serial terminal on the ESP32 board

```
screen /dev/ttyUSB0 115200
cu -l /dev/ttyUSB0 -s 115200
```

If screen terminates (unhelpfully silently) or `cu` gives the useful error 
message
```
cu: open (/dev/ttyUSB0): Permission denied
cu: /dev/ttyUSB0: Line in use
```

Add current user to `dialout`

```
sudo usermod -a -G dialout $USER
```


# Build and flash

(https://docs.espressif.com/projects/esp-idf/en/stable/esp32/get-started/linux-macos-setup.html#get-started-linux-macos-first-steps)

```
idf.py menuconfig
idf.py build
```

- Needs a flash size of 4 MB
- In menuconfig enable two OTA partitions if you want OTA









# Requirements
0. Hardware bypass to wire thermostat directly to baseboard (circulator) 
   controls. Hardware switch to turn off microcontroller (relinquishes control 
of heat pumps) 
1. Basic fail-safe mode (Always runs to prevent hydronic pipes from freezing)
  1. When outdoor temperature goes below T, run the hydronic circulators for C 
     seconds, N times per hour.
  2. T, C, N are user settable.
  3. Warn user if circulators don't work as intended.
2. Integrated control mode (Switches from heat pumps to baseboard and 
   vice-versa)
  1. When outdoor temperature goes below T', pass the thermostat command on to 
     the circulators, switch off the heat pumps
  1. When outdoor temperature has been above T' for more than H hours, block 
     thermostat command, restore heat pumps (turn them back on)
3. Heat pump control mode (Allows setting of individual heads)
  1. Temperature can be set on P heat pump heads independently
4. Device should be easily accessible/removable for reprogramming

# Design overview
1. ESP32 microcontroller based design
2. DS18B20s for temperature sensing
3. 3V relay to close the thermostat circuit from the zone circulator controls
4. CoAP for communication
5. Simple web UI for control
6. DNS service to connect from outside internet
7. CN105 + Wemos D1 mini board control for heads

# Design considerations




# Pin connection

- GPIO15 = DS18B20 "bus"
- GPIO13 = Thermostat relay out

# UI Module

- `T<d>` = Set T to `d` celcius
- `C<s>` = Set C to `s` seconds
- `N<n>` = Set N to `n` times per hour (can be a fraction)
- `F`    = Force circulator to run now
- `I`    = Return status string. Status string is returned by every command too.

## Status string example

```
T = -2.0C
C = 60s
N = 1/h

Current time         = yyyy.mm.dd hh:mm:ss
Time of last run     = yyyy.mm.dd hh:mm:ss (Y min ago)
Current Outdoor Temp = 10C
Current Pipe Temp    = 15C, ... (for multiple pipes)

Uptime 4.2h
RAM free 10
```

# Possible future extensions

Integrate this with heat pump control to intelligently run boiler or heat pumps during heat mode in addition to anti-freeze function. Heat pump wireless control is a separate project.

# Compile

(Standard procedure: 
https://docs.espressif.com/projects/esp-idf/en/stable/esp32/get-started/linux-macos-setup.html#get-started-linux-macos-first-steps)

```
idf.py set-target esp32
idf.py build
idf.py flash  # Auto detects which USB the ESP32 is connected to
```
