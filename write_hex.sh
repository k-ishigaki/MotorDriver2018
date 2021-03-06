#!/bin/sh -e

ARDUINO_TOOLS=/Applications/Arduino.app/Contents/Java/hardware/tools
HEX_FILE=.pio/build/default/firmware.hex
TARGET=atmega328pb

${ARDUINO_TOOLS}/avr/bin/avrdude -C ${ARDUINO_TOOLS}/avr/etc/avrdude.conf -b 115200 -c atmelice_isp -p ${TARGET} -U flash:w:${HEX_FILE}:i
