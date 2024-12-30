# Embedded 

## EDA
### Description
The schematic and PCB design of the board are developed with Kicad 8.0.5 open-source EDA.

### Links to 3rd party Kicad libraries
1. [Digi-Key](https://github.com/Digi-Key/digikey-kicad-library)
2. [SparkFun](https://github.com/sparkfun/SparkFun-KiCad-Libraries)
3. [Espressif Systems](https://github.com/espressif/kicad-libraries)

## PCB manufacturer
### Description
The chosen PCB manufacturer for board creation is PCBWay. Required manufacturing constraints, gerber and drill files follow the specified recommendations.

### PCB manufacturer links
1. [PCBWay](https://www.pcbway.com/)
2. [PCBWay Capabilities](https://www.pcbway.com/capabilities.html)
3. [PCBWay How to generate gerber files](https://www.pcbway.com/helpcenter/generate_gerber/Generate_Gerber_file_from_Kicad.html)

## Software development
### Description
For validating the hardware design of the board, several tests need to be implemented, flashed and executed on the board. The embedded software tests are developed using Arduino 1.8.19 and Espressif's ESP-IDF framework. The same tools are used for creating final embedded appliactions for the board.

### Software development tools links
1. [Arduino IDE](https://www.arduino.cc/en/software)
2. [ESP-IDF Programming Guide](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/)