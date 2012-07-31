# Artemis Synthesizer Script

# Section 1: Base Keyboard Presentation

This is the Artemis Synthesizer, a simple synthesizer based around an ATMega328P
and MCP4921 I2C Digital to Analog converter. Capable of 4 note polyphony and a 
22kHz sample rate. 

# Section 2: Sequencer 8 channels

In addition to being a keyboard the Synth is capable of acting as a sequencer, with 
8 stages and 8 notes. You can also have 8 unique sequences.

# Section 3: Web Sequencer Programming

Since our target audience was a group of rising High School Freshman we decided to add
an interactive part which required nothign more than a web browser. Using this online
application you can program a sequence into your synth board using a blinky light 
programmer, which is using BiPhase Mark Coding. We call this link the "OptoLoader", but
it does not act as a bootloader, it simply sends a message which is interpreted and 
changes some variables.

The main issue here is that the datarate is very slow, but at this speed the data 
transmission is very reliable!

# Section 4: Web Harmonics Programming

In addition to programming sequences, you can program harmonic wave forms by adjusting
the different harmonics using sliders. Just hit the "waveform" button instead!

# Section 5: Construction Day!

After writing the firmware, flashign 50 chips, making 50 kits, instructions and the 
board in under a month we finally got to present our kits to a group of 14 year old
girls, who were enrolled in the BU Artemis Program.

The reaction was widly positive, they did a great job as did their coordinators, who
learnt how to solder days before.
