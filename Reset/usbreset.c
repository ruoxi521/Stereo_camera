/* usbreset -- send a USB port reset to a USB device */
/* USB设备重置:有时，可能会因为各种各样的原因，导致usb设备不能
正常使用，我们可以通过c代码，给其发送reset重置指令，恢复其状
态，从而避免重新拔插设备。以下我们直接编译一个工具对设备进行重置。*/

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <linux/usbdevice_fs.h>

/* 查看设备及其驱动： sudo lsusb，找到其前边的总线、设备序号 */
/* USB设备重置命令： sudo ./usbreset /dev/bus/usb/001/006,*/

int main(int argc, char **argv) {

    const char *filename;
    int fd;
    int rc;
    if (argc != 2) {
        fprintf(stderr, "Usage: usbreset device-filename");
        return 1;
    }
    filename = argv[1];

    fd = open(filename, O_WRONLY);

    if (fd < 0) {
        perror("Error opening output file");
        return 1;
    }
    printf("Resetting USB device %s\n", filename);

    rc = ioctl(fd, USBDEVFS_RESET, 0);
    if (rc < 0) {
        perror("Error in ioctl");
        return 1;
    }
    printf("Reset successful\n");
    close(fd);

    return 0;
}