# Artemis Synthesizer 

v2.0 Rev C for Summer 2013

Christopher Woodall  
Eric Hazen  
Sam Damask

## Hardware

Schematic and PCB files were made using KiCAD. Gerber files and PDFs are available. The schematic files were created using a 25 mil grid and are sometimes off by 25 mils from the 50 mil grid. It is advised that you also use a 25 mil grid when editing the schematic files.

There is a basic convention for digital signals on the PCB layout, which is the bottom layer travels vertically and the top layer travels horizontally. The analog section, which takes up the left most part of the board routes all signals on the top layer so that they have ground right underneath. An improvement would introduce proper isolation technology.

## Firmware

## Documentation

[Current Documentation](http://ohm.bu.edu/cgi-bin/edf/SoundSynthesizer)

## Licensing

(c) Christopher Woodall 2012-2013, under a Creative Commons Attribution-ShareAlike 3.0 Unported License.

## Versioning

The basic versioning method is `Version NUM Rev LETTER (vNUMrLETTER)`. The version number indicates major version changes (and more importantly the associated software version). The revision letter marks the board revision and does not restart with new versions.

Branched projects are allowed to restart revision numbering and version numbering so long as the name of the project changes in some manner. This is requested to prevent confusion.

