@echo off
setlocal

set ARDUINO_TOOLS=C:\Program Files (x86)\Arduino\hardware\tools
set HEX_FILE=.pio\build\default\firmware.hex
set TARGET=atmega328pb

"%ARDUINO_TOOLS%\avr\bin\avrdude" -C"%ARDUINO_TOOLS%\avr\etc\avrdude.conf" -b115200 -catmelice_isp -p"%TARGET%" -Uflash:w:"%HEX_FILE%":i
