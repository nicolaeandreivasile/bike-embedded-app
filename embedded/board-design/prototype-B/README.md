# Prototype B

This is prototype B of the bike embedded application. It implements the rework of prototype-A, signifying hardware bug fixes from the previous version and additional feature integration.

## Main processing unit
The main processing unit of the embedded hardware application is the ESP32-WROVER-E-N16R8. It is a generic Wi-Fi + Bluetooth + Bluetooth LE MCU module that target a wide variety of applications, this variant coming with 16MB of flash and 8MB of PSRAM.
Additional compoenents of the ESP32 module are:
1. JTAG debug connnector
2. Auto-program circuit (flash capabilities)
3. Voltage supervisor (when the voltage drops under a certain treshold, the module shuts down preventing faults)

More information about the ESP32 and additional parts can be found in the official RMs and datasheets:

* [ESP32-WROVER-E & ESP32-WROVER-IE Datasheet](https://www.espressif.com/sites/default/files/documentation/esp32-wrover-e_esp32-wrover-ie_datasheet_en.pdf)
* [ESP32 Series Datasheet](https://www.espressif.com/sites/default/files/documentation/esp32_datasheet_en.pdf)
* [ESP32 Technical Reference Manual](https://www.espressif.com/sites/default/files/documentation/esp32_technical_reference_manual_en.pdf)
* [ESP Hardware Design Guidelines](https://docs.espressif.com/projects/esp-hardware-design-guidelines/en/latest/esp32/index.html)
* [ESP-IDF Programming Guide](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/index.html)
* [Standard CMOS Voltage Detector IC Datasheet](https://fscdn.rohm.com/en/products/databook/datasheet/ic/power/voltage_detector/bd48xxg-m-e.pdf)

### Auto-program circuit
Additional circuitry is required to ensure that the firmware is loaded correctly.

| DTR | RTS |    | EN  | IO0 |
|-----|-----|----|-----|-----|
|  1  |  1  |    |  1  |  1  |
|  0  |  0  | -> |  1  |  1  |
|  1  |  0  |    |  0  |  1  |
|  0  |  1  |    |  1  |  0  |

The chip enters the serial bootloader when GPIO0 pin is held low on reset, as well as GPIO2 being left unconnected (floating) or being pulled down to GND. The esptool.py connects the EN pin of the ESP32 to RTS and the GPIO0 pin to DTR automatically to enter the serial bootloader, unlike other serial terminal programs, which pull RTS and DTR low while opening the serial port, holding the chip in reset. Auto-program (Boot & Enable) circuitry of the ESP32 is required to ensure proper flashing procedure.

### Voltage supervisor
A voltage supervisor is also added for undervoltage protection. The BD48E30G-M IC is used in this scenario, following an open-drain output type implementation. When the input voltage drops below $3.0V$, the ESP32 EN pin is pulled low, shutting down the IC.


## USB-to-UART converter
TODO


## Battery management
The module is compoesed of two main parts, a battery charger and a battery monitor. For more information about the battery management components, please refer to the official documentation:
* [LTC4054-4.2/LTC4054X-4.2 Datasheet](https://www.analog.com/media/en/technical-documentation/data-sheets/405442xf.pdf)
* [LTC2959 Datasheet](https://www.analog.com/media/en/technical-documentation/data-sheets/ltc2959.pdf)

### Battery charger
The application has the ability to charge the battery from USB or an alternative $5V$ DC power source by using the LTC4054-4.2 IC. It feeds $4.2V$ preset voltage. The charging current can be programmed using a resistor and is calculated below.

$$ R_{PROG} = \left(\frac{V_{PROG}}{I_{BAT}} * 1000 \right) = \left(\frac{1V}{0.5A} * 1000 \right) = 2K\Omega $$

$500mA$ is the chosen value for $I_{BAT}$, therefore $R_{PROG} = 2K\Omega$. In addition, the voltage drop across the gas gauge $R_{SENSE}$ is $0.025V$.

The charging module also has available a status LED and the cabability to be shut down by an external actuator (ESP32 in this case).

The IC can be powered by two sources, ALT and VBUS. FDN340P ($20V$, $-2A$ MOSFET-P) is used to prevent back conducting into VBUS, as well as a low $V_F$ diode, PMEG3020EP ($30V$, $2A$ Schottky barrier rectifier), to prevent USB power loss through the
100k pull-down resistor. Taking into consideration a $0.25-0.3V$ diode forward, the LTC4054 will have a safe 4.7-5V power supply, well within the operating limits ($4.25-6.5V$).

### Gas gauge
An additional gas gauge has been added to the implementation to measure various characteristics of the battery (i.e. voltage levels, current, temperature, etc.). The application follow the recommended schematics from the datasheet and connects to the ESP32 via I2C. It also has an additional GPIO that can be used in multiple scenarios (charge complete logic input, ADC input, etc.). 


## Power supply
$3.3V$ is achieved using the TPS63000 Buck-Boost converter, ended for systems powered by one-cell Li-ion or Li-polymer battery with
a typical voltage between $2.3V$ and $4.5V$. The power regulator has multiple sources and it is programmed to output the desired power specifications of the applications.

### Sources
There are 3 main sources that can power the application listed below from the highest priority to the lowest:

1. USB
2. An alternative $5V$ DC power source
3. 18650 Li-ion sincle cell battery

The power lines VBAT, ALT and VBUS follow a similar back conducting protection circuit to the one described in the previous module. First, the VBAT and ALT sources are depended on an outside rocker switch to give the application the possibility to be turned of when desired. Then, the output is united via another back conducting protection circuit with VBUS to feed the DC/DC converter.

We have to take into consideration the diode forward voltage ($0.25-0.3V$) only on the ALT and VBUS lines (these two are constant $5V$ power lines, therefore we can afford a voltage drop of $0.3V$). The aproximate range of the TPS61090 $V_{BAT}$ in this case is between $2.8V$ and $4.7V$.

### Programming the output voltage
The TPS63000 is the adjustable version for the TPS6300x family, therefore, a voltage divider is required at the FB pin. The resistor values are calculated using the below formula, keeping in mind that the desired $V_{OUT} = 3.3V$, $V_{FB} = 0.5V$ and the recommended value of the $R_2$ is less that $500k\Omega$ (~$200k\Omega$ recommended).

$$R_1 = R_2 * \left(\frac{V_{OUT}}{V_{FB}} - 1\right) = 180k\Omega * \left(\frac{3.3V}{0.5V} - 1\right) = 1M\Omega$$

Using a feedforward capacitor in parallel to R1 is recommended to improve control performance. The value can be calculated using the below formula.

$$C_{ff} = \frac{2.2\mu s}{R_1} = \frac{2.2\mu s}{1M\Omega} = 2.2pF$$

### Inductor Selection
When choosing the inductor, the following requirements need to be taken into consideration: DC resistance, current rating, saturation current, inductance. The peak current of the inductor can be calculated as seen below.

$$D = \frac{V_{OUT} - V_{IN}}{V_{OUT}} = \frac{3.3V - 2.4V}{3.3V} = 0.2727 \approx 0.273$$
$$I_{PEAK} = \frac{I_{OUT}}{n * (1 - D)} + \frac{V_{IN} * D}{2 * f * L} = \frac{1A}{0.75 * (1 - 0.273)} + \frac{2.4V * 0.273}{2 * 1500kHz * 1.5\mu H} = 1.9796A \approx 1.98A$$

The calculations are performed using the plotted graphs from the datasheet for a typical $V_{IN} = 2.4V$, $V_{OUT} = 3.3V$, and a $I_{OUT} = 1A$.

An inductor with a saturation current higher with at least 20% than the $I_{PEAK}$ is desired, preferably with low DC resistance, therefore NRS6010T1R5MMGF, with $1.5\mu F$, part is selected.

### Input capacitor
At least a $4.7\mu F$ input capacitor is recommended. To minimize EMI behavior, 22&micro;F electrolytic, 10&micro;F and 0.1&micro;F ceramic capacitors are placed near the input supply pin.

In addition, a RC filter is recommended for VINA, EN and PS/SYNC pins with $R=100\Omega$ and $C=0.1\mu F$. We can calculate the low-pass filter cut-off frequency as seen below.

$$f_C = \frac{1}{2\pi RC} = \frac{1}{2\pi * 100\Omega * 0.1\mu F} = 15.915kHz \approx 16kHz$$

### Output capacitor
There is no upper limit for the output capacitance value, although $15\mu F$ is recommended for standard applications. Larger capacitors causes lower output voltage ripple as well as lower output voltage drop during load transients. 22&micro;F electrolytic, 10&micro;F ceramic and 220&micro;F tantalum capacitors are placed on the $V_{OUT}$ power signal.

### Power LED
A status LED is placed as well. It can be turned off by the ESP32 using the BC807 PNP transistor.

##

For more information about the general purpose parts used in this implementation, please refer to each individual datasheets:

* [TBD]()

# Issues

## Main processing unit
Strapping pins overlay with JTAG pins pulled to $3.3V$ by $10\Omega$ resistors, therefore it cannot flash applications correctly via UART (GPIO12 and other strapping pins require to be pulled to GND). 
GPIO2 should be added to auto-program logic as it controls the internal LDO (VDD_SDIO) voltage.

Tasks:
* [ ] Introduce switches/jumpers to be able to configure the common pins
* [ ] Add GPIO2 to the auto-program circuit to imitate GPIO0 implementation.


## USB-to-UART converter
Currently, red is for receiving data from host to board and green for transmiting data from board to host.
Tasks:
* [ ] Switch LED colors for transmit/receive data. 


## Battery management
18650 battery holder too big for current battery cell. Currently the Keystone 1024P is mounted (polarized holder).
Battery charger needs rework (current program part). FDN340 P-MOSFET body diode forward voltage messes up the programmign of the charging current.

Tasks:
* [ ] The requirement is the Keystone 1024 (leaf springs).
* [ ] Switch the P-MOSFET from the charging current programming logic with a N-MOSFET (FDN335N perhaps) transistor for low-side switching.
* [ ] Erase the ALT alternative power source for the next revision.


## Power supply
Observed that when voltage is decreasing, $2.5V$ is the minimum threshold the regulator can sustain, while when increasing the voltage, it comes back to normal at around $3.4V$. To be investigated further.
FDN340P has $0.7V$ diode voltage drop at around $0.5A I_S$, therefore an alternative is required (impacts battery charge current programming and power input lines).

Tasks:
* [ ] Erase the ALT alternative source for the next revision.
* [ ] Protection (TVS/Zenner) diodes to be added to the cicuit. 
* [ ] Switch PNP (BC807) to high-side placememnt of the power LED.
* [ ] Rethink the input power source for the DC-DC converter. Alternatives to be considered (power switches, etc.).