# kblight
Program to set keyboard backlight on OverPowered 17' laptop, which has an ITE USB HID device (0x048d:0xce00).

The work is based on capturing packets on USB bus under Windows OS. Havn't figured out all the commands yet, but enough to turn on/off the backlight, or set it to arbitrary color using RGB value. The program can be modified to set the color of individual keys. Just need to figured out the mapping of the color array elements to the physical keys.


