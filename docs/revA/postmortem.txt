# Post Mortem

## Project Timeline Review

### Brainstorm (June 12-15th)
We decided to develop a project of our own on June 12, 2012. At which point we
presented a few basic options for the project. The two competitors were a 
**Game of Life simulator** or a **synthesizer**. By June 15th we decided on the
synthesizer, ordered test components, decided on a microprocesor and were ready
to start the prototyping stages. 

### Prototypes (June 18th-22nd)

The audio amplifier was prototyped on perfboard
by June 20th and written up [here](http://happyrobotlabs.com/posts/project/artemis-synthesizer-1-testing-the-tda2822-audio-amplifier/).
The TDA2822 test was successful, but the volume output of the speaker was low 
and as would be noted later was not very impressive


The MCP4921 was prototyped and documented [here](http://happyrobotlabs.com/posts/project/artemis-synthesizer-2-interfacing-with-the-mcp4921-spi-dac/).
The process was very swift and there were no major problems. Powering the SPI 
bus from the arduino which was 5V based required power limiting resistors.

### Schematic and PCB layout (June 25-July 6)

Most of the schematic layout was done during the prototyping stages in KiCAD.
KiCAD was very easy to use, but I needed to upgrade to the latest version in
order for it to be especially useable. Schematic layout went without a hitch.


PCB layout started once all the parts were chosen a BOM was made up and 
everything was layed out. PCB orders went out to Sierra Circuits and after
some tweaking to their rules and limits we were able to print with them. 
However, upon receiving the boards there was one trace shorted to the 3.3V
plane making the USB bootloader unuseable on this board. This problem was 
limited to one board and after testing most of the rest was not reproduceable.
In the future, I would increase the boundary between traces and planes and 
possibly avoid power planes for simple designs. 

There were some funding problems since the PCB boards ended up costing around
$15 per and parts also costed nearly $15 per board. The BOM cost was rather 
high for this project and an inspection should be carried out to find cost 
reductions for future revisions of the board.

### Firmware (July 12-20) 

Firmware development went well. Code could be better commented. Needs to be 
cleaned. Make sure comment frequently in future projects and write libraries
for the various "peripherals" you use (internal or external). If you have good
interfaces/drivers then other people can use your board even if your base 
firmware is a little sketchy.

Also, figure out a way to formalize different program states/tasks/programs with
efficient memory management. First time using interrupts heavily. Make sure 
interrupts, especially timer interrupts, are small. There are times when
interrupts can be lengthy, but be cautious and evaluate everything that could 
happen while that interrupt is happening and make the system as robust as 
possible.

### Packing (July 18-20)

Packing went smoothly, but took a lot of work. Need to make sure you put exactly
the right number of parts in. To many confuses people and to few makes the kit
impossible to complete correctly.

We also found some BOM errors. Always check your footprints!

## Additional Problems and Comments

* Audio quiet. Inspect whole audio amplification circuit. Possibly look for a new 
  speaker.

* BOM cost was high.
  * Possibly remove DAC use PWM instead
  * Possibly smaller microcontroller ATMega168 or ATMega88 vs ATMega328

* During soldering many had IC problems
  * Buy the correct sockets
  * Socket all ICs not just the MCU makes, rework easier.

* R10 is polarized, but not labelled as such. Rework Optoloader circuit

* Optoloader speed problems. A very slow data transmission.
  * Possibly use 2 photodiodes (separate clock and data lines)
  * Possibly use multiple colors (interesting solution, but likely expensive)
  * Ditch javascript and try flash, gifs, movies, etc.
  * If speeds can be significantly improved, then we can start programming more 
    complex things over the link.
