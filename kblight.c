/* A program to set keybaord backlight on OverPowered 17' laptop.
 *
 * Author: Fox <hulifox008@gmail.com>
 * Date: 1/26/2019
 */

#include <stdio.h>
#include <stdlib.h>
#include <libusb.h>

unsigned char  cmd_light_off[] = {0x08, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
unsigned char  cmd_light_on[] = {0x08, 0x02, 0x33, 0x00, 0x08, 0x00, 0x00, 0x00};
unsigned char  cmd_set_all[] = {0x12, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00};

#define    ITE_VID  0x048d
#define    ITE_PID  0xce00

int main(int argc, char* argv[])
{
    if(argc != 2) {
        fprintf(stderr, "Usage: kblight <color>    color is 0xrrggbb, 0 to turn off light.\n");
        return -1;
    }

    uint32_t color = strtoul(argv[1], NULL, 0);

    libusb_device_handle *hdev = NULL;
    int status = libusb_init(NULL);
    if(status != LIBUSB_SUCCESS) {
        fprintf(stderr, "libusb_init() ret %d\n", status);
        return -1;
    }

    hdev = libusb_open_device_with_vid_pid(NULL, ITE_VID, ITE_PID);
    if(hdev == NULL) {
        fprintf(stderr, "libusb_open_device_with_vid_pid() failed\n");
        return -1;
    }

    status = libusb_detach_kernel_driver(hdev, 1);

    status = libusb_claim_interface(hdev, 1);
    if(status != LIBUSB_SUCCESS) {
        fprintf(stderr, "libusb_claim_interface() ret %d\n", status);
        libusb_close(hdev);
        return -1;
    }

    if(color == 0) {
        libusb_control_transfer(hdev, 0x21, 9, 0x0300, 0x0001, cmd_light_off, 8, 5);
    }else {
        unsigned char buf[64];
        int i;

        libusb_control_transfer(hdev, 0x21, 9, 0x0300, 0x0001, cmd_light_on, 8, 5);

        libusb_control_transfer(hdev, 0x21, 9, 0x0300, 0x0001, cmd_set_all, 8, 5);

        for(i=0;i<16;i++) {
            buf[4*i+0] = 0x00;
            buf[4*i+1] = (color>>16) & 0xFF; /* R */
            buf[4*i+2] = (color>>8)  & 0xFF; /* G */
            buf[4*i+3] = color & 0xFF;       /* B */
        }

        for(i=0;i<8;i++) {
            int transferred = 0;
            libusb_interrupt_transfer(hdev, 0x02, buf, 64, &transferred, 5);
        }
    }
    libusb_close(hdev);

    return 0;
}
