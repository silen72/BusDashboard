# The Button Matrix PCB

The Button Matrix PCB was created with [KiCAD](https://kicad-pcb.org/). The complete project is downloadable [here](files/ButtonMatrix-Kicad.zip).

![the schematics](images/ButtonMatrixSchematics.png)

Based on the schematics I have created an easy-to-solder PCB (THT instead of SMD):
![the pcb](images/Schaltermatrix-brd.svg)


![the rendered pcb](images/Schaltermatrix2.png)

## Some infos about the pcb

- The diodes are necessary to avoid [keyboard ghosting](https://en.wikipedia.org/wiki/Rollover_(key)#Ghosting)
- The blade terminals may seem to have a lot of space between them. Placing them closer to each other is possible but makes the assembly (or disassembly if necessary) more finicky.

<a id="prototype" />The first prototype:
![The first prototype](images/prototype.png)

## Bill of materials

-   1 IC socket 28 pin
-   1 IC [MCP23S17](https://www.microchip.com/wwwproducts/en/MCP23S17)
-   1 resistor 1k
-   8 resistors 10k
-  64 diodes 1N4148
- 128 [blade terminals](https://www.vogtshop.ch/index.cfm?content=productData&Language=1&TreeID=BC533B35-D97E-46A2-872A-A3342E5C8212&sId=DEB33619-878B-4929-88BA-EDD863B07F05&sId=DEB33619-878B-4929-88BA-EDD863B07F05) 2,8mm
-   1 connector JST-XH-6A

## How does it work?

The MCP23S17 has 16 I/O pins. Eight of those (GP<b>A</b>x) are set to output, the other eight (GP<b>B</b>x) to input. The eight input pins are pulled down to ground because otherwise they would be floating (not connected to anything) when the connected button is not pressed or no button is connected at all. In the rendered pcb image above you can see the GP<b>B</b>x pins and their resistors facing to the left and the GP<b>A</b>x on the opposing side of the IC.

The code powers only one of the eight ROWs at a time by setting exactly one of the eight GP<b>A</b>x pins on the MCP23S17 to HIGH. It then scans all eight GP<b>B</b>x pins. For each GPBx pin that measures HIGH it knows that the blade terminals in that ROW and COLumn must be connected -> the button connected to these blade terminals is pushed.

The soldered board:
![The soldered board](images/soldered.png)

## Improvements for a future version

- Swap VCC and GND pins on the input connector. Alternative: swap VCC and GND on the [shield pcb](../shield/shield.md).
- The drill holes for the blade terminals need to be a little wider (between 1.30 and 1.35mm). Currently the drill holes are 1.2mm wide, as the manufacturer recommends. The blade terminals do not align cleanly on the board but stick out a little. Measuring the pin on the blade terminals (~1.38mm) indicates that it should still stick to the drill hole for easy soldering when the drill hole is 1.35mm wide. Needs some testing...
- Maybe replace the blade terminals altogether with screw terminals. Intese testing with the pcb has shown that often tightening and loosening the connections easily leads to ripped out blade terminals. It helps to solder the blade terminals from both sides, but still...

## Disclaimer

The files and information given here are for informational purposes only. They have worked for me but I do not give any guarantee that they will work for you. Use them at your own risk.

<p xmlns:dct="http://purl.org/dc/terms/" xmlns:cc="http://creativecommons.org/ns#" class="license-text">This work   is licensed under <a rel="license" href="https://creativecommons.org/licenses/by-sa/4.0">CC BY-SA 4.0<img style="height:22px!important;margin-left:3px;vertical-align:text-bottom;" src="https://mirrors.creativecommons.org/presskit/icons/cc.svg?ref=chooser-v1" /><img style="height:22px!important;margin-left:3px;vertical-align:text-bottom;" src="https://mirrors.creativecommons.org/presskit/icons/by.svg?ref=chooser-v1" /><img style="height:22px!important;margin-left:3px;vertical-align:text-bottom;" src="https://mirrors.creativecommons.org/presskit/icons/sa.svg?ref=chooser-v1" /></a></p>