# f051_discovery_talk

This is a Linear Predictive Code (LPC) speech synthesizer targeted at the
STM32F051 Discovery development board. It borrows heavily from the Talkie
project:

https://github.com/going-digital/Talkie

Thanks to Peter Knight for condensing the essence of LPC down into such a
concise project.

Prerequisites
-------------

* An STM32F051 Discovery board
* GCC ARM Embedded toolchain
* OpenOCD v0.8.0 or higher (needed to support ST-Link)
* some wires, speaker, capacitor

Hookup
------

Output is a 48kHz PWM signal on PA8 of the Discovery board. You can
connect a speaker or headphones directly to this but I recommend using a
DC-blocking capacitor in series (1uF seems to work fine). More elaborate
low-pass filtering may improve audio quality. Impedance matching would
probably be a good idea too. If you don't know what these terms mean,
get advice from someone who does!

There's also a diagnostic signal on PA9 that shows CPU loading in the main
LPC calculation routines. You probably don't care about this.

Operation
---------

The USER button plays speech samples from the built-in library one at a time.
Press the button and hear sound. The Green LED will light for the duration of
the playback. The Blue LED flashes at about a 10Hz rate to show that the
application is active.

Changes from the Talkie source
------------------------------

I've modified the code fromt the Talkie project in the following ways:

1) Converted from C++ to C

2) Minor formatting tweaks for my own understanding.

3) Rolled up the previously unrolled loops and converted to common types so
that array operations could be used.

4) All operations are driven from a single PWM timer. Samples are computed
synchronously with the PWM rate and LPC coefficients updates are also
synchronous to the sample rate.

5) Added LPC coefficient interpolation for slightly smoother sounding speech.

License
-------

The Talkie project was GPL V2 licensed so all my code here is too. I also use
support libraries from ARM and ST and those still carry the original license
from their sources.

Building the Project
--------------------

Just run
	make
	
Flashing
--------

1) Ensure openocd isn't running as a server already.

2) Make sure that your STM32F0 Discovery board is connected via USB and the
ST-LINK jumpers are set correctly (this example is for the on-board processor,
so 1-2 and 3-4 should be shorted)

3) run
	make flash

4) Done!

