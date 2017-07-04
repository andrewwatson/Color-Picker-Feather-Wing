# Color Picker to NeoPixel Matrix via MQTT

This project demonstrates how to use a color picker widget on adafruit.io 
to send a color selection to your Adafruit Feather connected to a 8x4 NeoPixel Matrix.

The incoming MQTT message contains the hex color in a format like "#00FB34" so this code
splits it up in "00", "fb", and "34" and then converts those to their decimal equivalents.

Then it fills in the matrix with that color for a second.

## Usage

Fill in your Wifi SSID and key.  Then fill in your AdaFruit.IO credentials as well.  Then
compile and upload the sketch.  Once it gets online, you'll see a big green "ON" on the matrix.
(Or maybe a NO if you have it upside down).