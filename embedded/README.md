# Embedded 

## Heardware design

### EDA
#### Description
The schematic and PCB design of the board are developed with Kicad 8.0.5 open-source EDA.

#### Links to 3rd party Kicad libraries
1. [Digi-Key](https://github.com/Digi-Key/digikey-kicad-library)
2. [SparkFun](https://github.com/sparkfun/SparkFun-KiCad-Libraries)
3. [Espressif Systems](https://github.com/espressif/kicad-libraries)

### PCB manufacturer
#### Description
The chosen PCB manufacturer for board creation is PCBWay. Required manufacturing constraints, gerber and drill files follow the specified recommendations.

#### PCB manufacturer links
1. [PCBWay](https://www.pcbway.com/)
2. [PCBWay Capabilities](https://www.pcbway.com/capabilities.html)
3. [PCBWay How to generate gerber files](https://www.pcbway.com/helpcenter/generate_gerber/Generate_Gerber_file_from_Kicad.html)

## prototype-A

### Reworks
1. No 3V3 or GND signal output from the board to provide power lines to auxiliary modules (sensors, etc.). 
2. Missing silkscreen information for test pins (ALT, 5V, 3V3, etc.)
3. No powerr switch on the battery line. In this case, the application stays on the entire time without the option of powering it off.
4. Change TPS61090 input inductor. Wrong calculation, huge ripple current through the inductor with the current one ($2.2\mu H$). New computed value for a maximum load of $1.3A$ at $3.3V$ is $8.6\mu H$. Please see board-design/prototype-B/README.md.
5. The JTAG should power the board when debugging the application. Current configuration connects JTAG $V_{REF}$ directly to 3V3 without any considerations

## prototype-B

### TODO

1. Review schematic
2. Update PCB
3. Generate new BOM, purchase list, gerber and drill files