# load.sh
# This script was used for programming 50 microcontrollers in under 30 minutes.
# The programming rig uses a ZIF socket and follows this following process:
#   - Program the bootloader using the AVRISP,
#   - wait to make sure the ATMEGA328P reboots and has the bootloader on it
#   - Use the USBASP bootloader we just burned to flash the keyboard/sequencer
#     firmware!

CUR_DIR=".."
BOOTLOADER_DIR="$CUR_DIR/USBaspLoader.2010-07-27_for_SYNTH/firmware"
FIRMWARE_DIR="$CUR_DIR/synth"

echo "bootloader location: $BOOTLOADER_DIR"
echo "firmware location: $FIRMWARE_DIR"
echo ""
echo "LOADING BOOTLOADER"
cd $BOOTLOADER_DIR
sudo make flash
sudo make fuse
sudo make lock
echo ""
sleep 2
echo "LOADING FIRMWARE"
cd $FIRMWARE_DIR
sudo make flash

