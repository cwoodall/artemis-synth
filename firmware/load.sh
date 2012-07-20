CUR_DIR="/home/cwoodall/Documents/Projects/artemis-synth-v1/firmware"
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

