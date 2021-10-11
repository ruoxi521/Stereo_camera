如遇到如下报错

```
Listing available devices:
  video0   3D  Webcam: 3D  Webcam
    Media controller device: /dev/media0
ERROR: Unable to open media controller device '/dev/media0': Permission denied (Error: 13)
ERROR: Unable to list device entities: Invalid device or device cannot be opened. (Code: 5)
  video2   BisonCam,NB Pro: BisonCam,NB Pr
    Media controller device /dev/media2 doesn't exist
ERROR: Unable to list device entities: Invalid device or device cannot be opened. (Code: 5)
  video1   3D  Webcam: 3D  Webcam
    Media controller device: /dev/media1
ERROR: Unable to open media controller device '/dev/media1': Permission denied (Error: 13)
ERROR: Unable to list device entities: Invalid device or device cannot be opened. (Code: 5)
  video3   BisonCam,NB Pro: BisonCam,NB Pr
    Media controller device /dev/media3 doesn't exist
ERROR: Unable to list device entities: Invalid device or device cannot be opened. (Code: 5)
user@ubuntu:~$ 
```

修改为video的端口号

```
//uvcdynctrl -d /dev/video1
改为
uvcdynctrl -d /dev/video0
```

之后调用执行正常

```
query control size of : 2
query control flags of: 0x3
query minimum value of: (LE)0x0000  (BE)0x0000
query maximum value of: (LE)0xffff  (BE)0xffff
query default value of: (LE)0xffff  (BE)0xffff
query step size of    : (LE)0x0100  (BE)0x0001
set value of          : (LE)0x50ff  (BE)0xff50
query control size of : 2
query control flags of: 0x3
query minimum value of: (LE)0x0000  (BE)0x0000
query maximum value of: (LE)0xffff  (BE)0xffff
query default value of: (LE)0x0000  (BE)0x0000
query step size of    : (LE)0x0100  (BE)0x0001
set value of          : (LE)0x00f6  (BE)0xf600
query control size of : 2
query control flags of: 0x3
query minimum value of: (LE)0x0000  (BE)0x0000
query maximum value of: (LE)0xffff  (BE)0xffff
query default value of: (LE)0xffff  (BE)0xffff
query step size of    : (LE)0x0100  (BE)0x0001
set value of          : (LE)0x2500  (BE)0x0025
query control size of : 2
query control flags of: 0x3
query minimum value of: (LE)0x0000  (BE)0x0000
query maximum value of: (LE)0xffff  (BE)0xffff
query default value of: (LE)0xffff  (BE)0xffff
query step size of    : (LE)0x0100  (BE)0x0001
set value of          : (LE)0x5ffe  (BE)0xfe5f
query control size of : 2
query control flags of: 0x3
query minimum value of: (LE)0x0000  (BE)0x0000
query maximum value of: (LE)0xffff  (BE)0xffff
query default value of: (LE)0x0000  (BE)0x0000
query step size of    : (LE)0x0100  (BE)0x0001
set value of          : (LE)0x0003  (BE)0x0300
query control size of : 2
query control flags of: 0x3
query minimum value of: (LE)0x0000  (BE)0x0000
query maximum value of: (LE)0xffff  (BE)0xffff
query default value of: (LE)0x0000  (BE)0x0000
query step size of    : (LE)0x0100  (BE)0x0001
set value of          : (LE)0x0002  (BE)0x0200
query control size of : 2
query control flags of: 0x3
query minimum value of: (LE)0x0000  (BE)0x0000
query maximum value of: (LE)0xffff  (BE)0xffff
query default value of: (LE)0x0000  (BE)0x0000
query step size of    : (LE)0x0100  (BE)0x0001
set value of          : (LE)0x0012  (BE)0x1200
query control size of : 2
query control flags of: 0x3
query minimum value of: (LE)0x0000  (BE)0x0000
query maximum value of: (LE)0xffff  (BE)0xffff
query default value of: (LE)0x0000  (BE)0x0000
query step size of    : (LE)0x0100  (BE)0x0001
set value of          : (LE)0x0004  (BE)0x0400
query control size of : 2
query control flags of: 0x3
query minimum value of: (LE)0x0000  (BE)0x0000
query maximum value of: (LE)0xffff  (BE)0xffff
query default value of: (LE)0xffff  (BE)0xffff
query step size of    : (LE)0x0100  (BE)0x0001
set value of          : (LE)0x76c3  (BE)0xc376
query control size of : 2
query control flags of: 0x3
query minimum value of: (LE)0x0000  (BE)0x0000
query maximum value of: (LE)0xffff  (BE)0xffff
query default value of: (LE)0x0000  (BE)0x0000
query step size of    : (LE)0x0100  (BE)0x0001
set value of          : (LE)0x0400  (BE)0x0004
[1] + Done                       "/usr/bin/gdb" --interpreter=mi --tty=${DbgTerm} 0<"/tmp/Microsoft-MIEngine-In-ny1ve3nk.jku" 1>"/tmp/Microsoft-MIEngine-Out-4xcdq1i0.obd"

按任意键继续...
```
