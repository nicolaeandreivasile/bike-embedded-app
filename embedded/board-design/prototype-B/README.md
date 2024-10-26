# Prototype B

## Main processing unit
TODO

## USB-to-UART converter
TODO

## Battery management
The application has the ability to charge the battery from USB or an alternative $5V$ DC power source by using the LTC4054-4.2 IC. It feeds $4.2V$ preset voltage. The charging current can be programmed using a resistor and is calculated below.

$$ R_{PROG} = \left(\frac{V_{PROG}}{I_{BAT}} * 1000 \right) = \left(\frac{1V}{0.5A} * 1000 \right) = 2K\Omega $$

$500mA$ is the chosen value for $I_{BAT}$, therefore $R_{PROG} = 2K\Omega$. In addition, the voltage drop across the gas gauge $R_{SENSE}$ is $0.025V$.

The charging module also has available a status LED and the cabability to be shut down by an external actuator (ESP32 in this case).

The IC can be powered by two sources, ALT and VBUS. FDN340P ($20V$, $-2A$ MOSFET-P) is used to prevent back conducting into VBUS, as well as a low $V_F$ diode, PMEG3020EP ($30V$, $2A$ Schottky barrier rectifier), to prevent USB power loss through the
100k pull-down resistor. Taking into consideration a $0.25-0.3V$ diode forward, the LTC4054 will have a safe 4.7-5V power supply, well within the operating limits ($4.25-6.5V$).

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

### Capacitor Selection

#### Input capacitor
At least a $4.7\mu F$ input capacitor is recommended. To minimize EMI behavior, 22&micro;F electrolytic, 10&micro;F and 0.1&micro;F ceramic capacitors are placed near the input supply pin.

In addition, a RC filter is recommended for VINA, EN and PS/SYNC pins with $R=100\Omega$ and $C=0.1\mu F$. We can calculate the low-pass filter cut-off frequency as seen below.

$$f_C = \frac{1}{2\pi RC} = \frac{1}{2\pi * 100\Omega * 0.1\mu F} = 15.915kHz \approx 16kHz$$



#### Output capacitor
There is no upper limit for the output capacitance value, although $15\mu F$ is recommended for standard applications. Larger capacitors causes lower output voltage ripple as well as lower output voltage drop during load transients. 22&micro;F electrolytic, 10&micro;F ceramic and 220&micro;F tantalum capacitors are placed on the $V_{OUT}$ power signal.

### Power LED

A status LED is placed as well. It can be turned off by the ESP32 using the BC807 PNP transistor.
