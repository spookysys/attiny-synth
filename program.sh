# Ref. http://www.engbedded.com/fusecalc/
# Ref. https://learn.sparkfun.com/tutorials/pocket-avr-programmer-hookup-guide/using-avrdude

# Fuse defaults
avrdude -c usbtiny -p attiny85 -U lfuse:w:0x62:m -U hfuse:w:0xdf:m -U efuse:w:0xff:m

# 16mhz clock
avrdude -c usbtiny -p attiny85 -U lfuse:w:0xe1:m -U hfuse:w:0xdf:m -U efuse:w:0xff:m

# Upload release build
avrdude -c usbtiny -p attiny85 -U flash:w:synth/Release/synth.hex

# 16mhz clock + debugwire (avrdude won't work again)
avrdude -c usbtiny -p attiny85 -U lfuse:w:0xe1:m -U hfuse:w:0x9f:m -U efuse:w:0xff:m

