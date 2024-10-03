# Autocirc
Automation to periodically run baseboard circulators to prevent hydronic pipes from freezing.

# Requirements
1. When outdoor temperature goes below T, run the hydronic circulators for C seconds, N times per hour.
2. T, C, N are user settable.
3. Warn user if circulators don't work as intended.
4. Device should be easily accessible so that algorithm can be tweaked as needed

# Design overview
1. Use a ESP32 microcontroller to periodically sample an outdoor temperature sensor (DS18B20).
2. Based on algorithm, activate a 3V relay to drive the circulator pumps
3. Monitor the boiler out pipe temperature (It should rise) as a proxy for monitoring water circulation
4. Possibly, monitor other pipes too
5. A UI module that responds to commands over WiFi     


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

