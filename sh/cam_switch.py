import shlex
import subprocess
import sys

cam_id=2

cam_mode_dict = {
    'LEFT_EYE_MODE': 1,
    'RIGHT_EYE_MODE': 2,
    'RED_BLUE_MODE': 3,
    'BINOCULAR_MODE': 4,
}
cam_mode = cam_mode_dict['BINOCULAR_MODE']

command_list = [
    "uvcdynctrl -d /dev/video{cam_id} -S 6:8 '(LE)0x50ff'",
    "uvcdynctrl -d /dev/video{cam_id} -S 6:15 '(LE)0x50f6'",
    "uvcdynctrl -d /dev/video{cam_id} -S 6:8 '(LE)0x2500'",
    "uvcdynctrl -d /dev/video{cam_id} -S 6:8 '(LE)0x5ffe'",
    "uvcdynctrl -d /dev/video{cam_id} -S 6:15 '(LE)0x0003'",
    "uvcdynctrl -d /dev/video{cam_id} -S 6:15 '(LE)0x0002'",
    "uvcdynctrl -d /dev/video{cam_id} -S 6:15 '(LE)0x0012'",
    "uvcdynctrl -d /dev/video{cam_id} -S 6:15 '(LE)0x0004'",
    "uvcdynctrl -d /dev/video{cam_id} -S 6:8 '(LE)0x76c3'",
    "uvcdynctrl -d /dev/video{cam_id} -S 6:10 '(LE)0x0{cam_mode}00'",
]

def usb_ctrl(id):
    for command in command_list:
        subprocess.Popen(shlex.split(command.format(cam_id=id, cam_mode=cam_mode)))

if __name__ == '__main__':
    id = sys.argv[1]
    usb_ctrl(id)