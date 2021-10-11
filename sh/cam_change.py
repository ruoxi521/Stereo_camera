import shlex    #使用类似shell的语法分割字符串s，相当于特殊的tokenizer。经常用在subprocess.Popen的第一个参数中。
import subprocess   #subprocess是Python 2.4中新增的一个模块，# 它允许你生成新的进程，连接到它们的 input/output/error 管道

cam_id=0  #如果执行后报错，请切换为1
cam_mode_dict={
'LEFT_EYE_MODE':1,
'RIGHT_EYE_MODE':2,
'RED_BLUE_MODE':3,
'BINOCULAR_MODE':4,
}

mode_choose = int(input("请输入你选择的相机输出模式："))

if(mode_choose == 1):
    cam_mode=cam_mode_dict['LEFT_EYE_MODE']
elif(mode_choose == 2):
    cam_mode=cam_mode_dict['RIGHT_EYE_MODE']
elif(mode_choose == 2):
    cam_mode=cam_mode_dict['RED_BLUE_MODE']
elif(mode_choose == 4):
    cam_mode=cam_mode_dict['BINOCULAR_MODE']
else:
    print("choose number error,please try again!")

command_list=[
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

for command in command_list:
    subprocess.Popen(shlex.split(command.format(cam_id=cam_id,cam_mode=cam_mode)))