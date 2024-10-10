# Embedded 

## Heardware design

### EDA
#### Description
The schematic and PCB design of the board are developed with Kicad 8.0.5 open-source EDA.

#### Links to 3rd party Kicad libraries
1. [Digi-Key](https://github.com/Digi-Key/digikey-kicad-library)
2. [SparkFun](https://github.com/sparkfun/SparkFun-KiCad-Libraries)
3. [Espressif Systems] (https://github.com/espressif/kicad-libraries)

### PCB manufacturer
#### Description
The chosen PCB manufacturer for board creation is PCBWay. Required manufacturing constraints, gerber and drill files follow the specified recommendations.

#### PCB manufacturer links
1. [PCBWay](https://www.pcbway.com/)
2. [PCBWay Capabilities] (https://www.pcbway.com/capabilities.html)
3. [PCBWay How to generate gerber files] (https://www.pcbway.com/helpcenter/generate_gerber/Generate_Gerber_file_from_Kicad.html)

## Required reworks
1. No 3V3 or GND signal output from the board. Routing to external connectors to power auxiliary modules is required to provide power lines to auxiliary modules (sensors, etc.).
2. Add silkscreen information for test pins (ALT, 5V, 3V3, etc.)

