tsck-dna-puzzle
===============

This repository contains software and hardware files for the [DNA Puzzle for TSCK].

The puzzle consists of a steel double-helix with embedded RFID readers, individual
"nucleotide" pieces containing RFID tags, and a large stele containing an LED
disk that displays an "eye", whose appearance changes based on the arrangement
of nucleotides in the helix.


software
--------

The software is split into three pieces: 

* the RFID readers, which collect UART data from the reader modules and store it
  to be served via I2C (stored in `arduino/rfid-attiny`);
* the main controller, which reads the stored data on the RFID readers via I2C,
  updates the internal genome data, and then transmits it to the lighting controller
  (stored in `arduino/controller`); and
* the lighting controller, which received genome data from the main controller and 
  updates the LEDs as appropriate (stored in `arduino/lighting-controller`).
  

hardware
--------

There are two broad categories of hardware included in this repository:

* CAD files (mostly laser-cut pieces) stored in `laser-cutter`; and
* electronics (both schematics and PCB layouts) stored in `electronics`.


[DNA Puzzle for TSCK]: https://drive.google.com/drive/folders/1d9PLsUjKqNwdjKIzH2Ln1TuXTCMuD0Dc
