# IL3895_Arduino
This will be an Arduino compatible driver for IL3895 e-paper displays, with basic functionality similar to the Adafruit TFT library.
I'm leaving out common features that I don't plan on using, but if anyone ends up using this library, then I  might add more in the future.

## Planned:
- [x] Port the STM32 initialization example to Arduino
- [x] Initialize GDEM0154E97LT with provided LUT and recommended settings
- [ ] Add function to display 152x152 monochrome bitmap *(from external ROM if I can't cram what I need onto the AVR)*
- [ ] Add functions to draw pixel, draw line, draw rectangle
- [ ] Add function to draw text
- [ ] Support more microcontrollers
- [ ] Support more displays
