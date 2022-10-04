# Raspberry-shifter software
##  Allows you to use any keyboard you want with Apple IIe

### Pull Code
* git submodule init
* git submodule pull

### build steps
* mkdir build
* cd build
* cmake ..
* openocd -f interface/picoprobe.cfg -f target/rp2040.cfg -c "program RaspberryShifter.elf verify reset exit"