#!/bin/bash
rsync -r --delete ./build-LowEnergyTest-Raspberry-Debug/LowEnergyTest\
      pi@192.168.15.7:/home/pi
