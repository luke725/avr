#!/bin/bash

avrdude -p atmega8 -c usbasp -u -U flash:w:main.hex
