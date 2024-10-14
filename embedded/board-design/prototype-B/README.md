# Prototype B

## Main processing unit
TODO

## USB-to-UART converter
TODO

## Power supply
$3.3V$ is achieved using the TPS6109x Synchronous Boost Converter. The power regulator has multiple sources and it is programmed to output the desired power specifications of the applications.

### Sources
There are 3 main sources that can power the application listed below from the highest priority to the lowest:

1. USB
2. An alternative $5V$ DC power source
3. 18650 Li-Ion sincle cell battery

If a power source is available, all the lower priority ones are suspended. This is available for the battery charging (VBUS and ALT) and power supply modules (VBUS, ALT and VBAT).

### Programming the output voltage
The TPS61090 is the adjustable version for the TPS6109x series, therefore, a voltage divider is required at the FB pin. The resistor values are calculated using the below formula, keeping in mind that the desired $V_{OUT} = 3.3V$, $V_{FB} = 0.5V$ and the recommended value of the $R_4$ is less that $500k\Omega$.

$$R_3 = R_4 * \left(\frac{V_{OUT}}{V_{BAT}} - 1\right) = 180k\Omega * \left(\frac{3.3V}{0.5V} - 1\right) = 1M\Omega$$

### Programming the LBI/LBO Threshold Voltage
For this application we use a 18650 Li-Ion single cell battery. To protect the cell, the voltage should not drop below 2.8V. We chose the voltage for LBI to be 3V, giving the battery room of 0.2V when raising the indicator. The voltage divider resistor values are calculated as shown below, with $390k\Omega$ as recommended $R_2$ and $0.5V$ for $V_{LBI-thresold}$.

$$R_1 = R_2 * \left(\frac{V_{BAT}}{V_{LBI-thresold}} - 1\right) = 390k\Omega * \left(\frac{3V}{0.5V} - 1\right) = 1.95M\Omega \approx 2M\Omega$$

### Inductor Selection
The absolute maximum load of the main processing unit (ESP32) is $1.1A$. The other $3.3V$ IC, the voltage supervisor, is ultra-low consumption, therefore it is redundant. Nonetheless, we add a safe margin of 0.2A for possible auxiliary modules that require a power supply, making $I_L = 1.3A$. We want our application to function in the lower battery ranges without any constraints, therefore we chose the minimal save value for $V_{BAT} = 2.8V$. A maximum 20% current ripple is recommended as well. We calculate the current flowing through the inductor and its value for the harshest power supply conditions below.

$$I_L = I_{OUT} * \frac{V_{OUT}}{V_{BAT} * 0.8} = 1.3A * \frac{3.3V}{2.8V * 0.8} = 1.91A $$

$$\Delta I_L = I_L / 5 = 1.91A / 5 = 0.382A \approx 0.38A$$

$$ L = \left(\frac{V_{BAT} * (V_{OUT} - V_{BAT})}{\Delta I_L * f * V_{OUT}}\right)  = \left(\frac{2.8V * (3.3V - 2.8V)}{0.4A * 600kHz * 3.3V}\right) = 7.446\mu H \approx 7.45\mu H$$

A $8.2\mu F$ inductor is chosen for this aplication.

### Capacitor Selection

#### Input capacitor
At least a 10-&micro;F input capacitor is recommended. To minimize EMI behavior, 22&micro;F electrolytic, 10&micro;F and 0.1&micro;F ceramic capacitors are placed near the input supply pin.

#### Output capacitor
A maximum allowed output voltage ripple is 1% of $V_{OUT}$, therefore, we compute the minimum output capacitor value as shown below.

$$ C_{min} = \left(\frac{I_{OUT} * (V_{OUT} - V_{BAT})}{f * \Delta V * V_{OUT}}\right) = \left(\frac{1.3A * (3.3V - 2.8V)}{600kHz * 10mV * 3.3V}\right) = 131\mu F$$

22&micro;F electrolytic, 10&micro;F ceramic and 220&micro;F tantalum capacitors are placed on the $V_{OUT}$ power signal.

### Battery management
The application has the ability to charge the battery from USB or an alternative $5V$ DC power source by using the LTC4054-4.2 IC. It feeds $4.2V$ preset voltage. The charging current can be programmed using a resistor and is calculated below.

$$ R_{PROG} = \left(\frac{V_{PROG}}{I_{BAT}} * 1000 \right) = \left(\frac{1V}{0.5A} * 1000 \right) = 2K\Omega $$

$500mA$ is the chosen value for $I_{BAT}$, therefore $R_{PROG} = 2K\Omega$. The charging module also has available a status LED and the cabability to be shut down by an external actuator (ESP32 in this case).