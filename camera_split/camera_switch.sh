#!/bin/bash
# user@ubuntu:~/Stereo_cam/Camera_switch$ chmod +x ./camera_switch.sh      提升执行权限
# author : ruoxi

id=2    # 选取相机通道
mode=4    # 选取相机通道

uvcdynctrl -d /dev/video${id} -S 6:8  '(LE)0x50ff'
uvcdynctrl -d /dev/video${id} -S 6:15 '(LE)0x00f6'
uvcdynctrl -d /dev/video${id} -S 6:8  '(LE)0x2500'
uvcdynctrl -d /dev/video${id} -S 6:8  '(LE)0x5ffe'
uvcdynctrl -d /dev/video${id} -S 6:15 '(LE)0x0003'
uvcdynctrl -d /dev/video${id} -S 6:15 '(LE)0x0002'
uvcdynctrl -d /dev/video${id} -S 6:15 '(LE)0x0012'
uvcdynctrl -d /dev/video${id} -S 6:15 '(LE)0x0004'
uvcdynctrl -d /dev/video${id} -S 6:8  '(LE)0x76c3'
uvcdynctrl -d /dev/video${id} -S 6:10 "(LE)0x0${mode}00"           # 切换相机输出模式
