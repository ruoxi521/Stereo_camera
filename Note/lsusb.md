```
user@ubuntu:~$ lsusb
Bus 002 Device 001: ID 1d6b:0003 Linux Foundation 3.0 root hub
Bus 001 Device 004: ID 5986:9102 Acer, Inc 
Bus 001 Device 003: ID 5c0a:8502  
Bus 001 Device 005: ID 8087:0aaa Intel Corp. 
Bus 001 Device 002: ID 1ea7:0064  
Bus 001 Device 001: ID 1d6b:0002 Linux Foundation 2.0 root hub
user@ubuntu:~$ lsusb
Bus 002 Device 001: ID 1d6b:0003 Linux Foundation 3.0 root hub
Bus 001 Device 004: ID 5986:9102 Acer, Inc 
Bus 001 Device 003: ID 5c0a:8502  
Bus 001 Device 006: ID 18e3:5031 Fitipower Integrated Technology Inc 
Bus 001 Device 005: ID 8087:0aaa Intel Corp. 
Bus 001 Device 002: ID 1ea7:0064  
Bus 001 Device 001: ID 1d6b:0002 Linux Foundation 2.0 root hub
user@ubuntu:~$ ^C
```




```
user@ubuntu:~$ lsusb -d 18e3:5031 -v

Bus 001 Device 006: ID 18e3:5031 Fitipower Integrated Technology Inc 
Couldn't open device, some information will be missing
Device Descriptor:
  bLength                18
  bDescriptorType         1
  bcdUSB               2.00
  bDeviceClass          239 Miscellaneous Device
  bDeviceSubClass         2 ?
  bDeviceProtocol         1 Interface Association
  bMaxPacketSize0        64
  idVendor           0x18e3 Fitipower Integrated Technology Inc
  idProduct          0x5031 
  bcdDevice            0.02
  iManufacturer           3 
  iProduct                1 
  iSerial                 0 
  bNumConfigurations      1
  Configuration Descriptor:
    bLength                 9
    bDescriptorType         2
    wTotalLength          397
    bNumInterfaces          2
    bConfigurationValue     1
    iConfiguration          0 
    bmAttributes         0x80
      (Bus Powered)
    MaxPower              200mA
    Interface Association:
      bLength                 8
      bDescriptorType        11
      bFirstInterface         0
      bInterfaceCount         2
      bFunctionClass         14 Video
      bFunctionSubClass       3 Video Interface Collection
      bFunctionProtocol       0 
      iFunction               1 
    Interface Descriptor:
      bLength                 9
      bDescriptorType         4
      bInterfaceNumber        0
      bAlternateSetting       0
      bNumEndpoints           1
      bInterfaceClass        14 Video
      bInterfaceSubClass      1 Video Control
      bInterfaceProtocol      0 
      iInterface              1 
      VideoControl Interface Descriptor:
        bLength                13
        bDescriptorType        36
        bDescriptorSubtype      1 (HEADER)
        bcdUVC               1.00
        wTotalLength           85
        dwClockFrequency       30.000000MHz
        bInCollection           1
        baInterfaceNr( 0)       1
      VideoControl Interface Descriptor:
        bLength                18
        bDescriptorType        36
        bDescriptorSubtype      2 (INPUT_TERMINAL)
        bTerminalID             1
        wTerminalType      0x0201 Camera Sensor
        bAssocTerminal          0
        iTerminal               0 
        wObjectiveFocalLengthMin      0
        wObjectiveFocalLengthMax      0
        wOcularFocalLength            0
        bControlSize                  3
        bmControls           0x00000000
      VideoControl Interface Descriptor:
        bLength                 9
        bDescriptorType        36
        bDescriptorSubtype      3 (OUTPUT_TERMINAL)
        bTerminalID             3
        wTerminalType      0x0101 USB Streaming
        bAssocTerminal          0
        bSourceID               6
        iTerminal               0 
      VideoControl Interface Descriptor:
        bLength                 7
        bDescriptorType        36
        bDescriptorSubtype      4 (SELECTOR_UNIT)
        bUnitID                 4
        bNrInPins               1
        baSource( 0)            1
        iSelector               0 
      VideoControl Interface Descriptor:
        bLength                11
        bDescriptorType        36
        bDescriptorSubtype      5 (PROCESSING_UNIT)
      Warning: Descriptor too short
        bUnitID                 5
        bSourceID               4
        wMaxMultiplier      32208
        bControlSize            2
        bmControls     0x0000157f
          Brightness
          Contrast
          Hue
          Saturation
          Sharpness
          Gamma
          White Balance Temperature
          Backlight Compensation
          Power Line Frequency
          White Balance Temperature, Auto
        iProcessing             0 
        bmVideoStandards     0x1b
          None
          NTSC - 525/60
          SECAM - 625/50
          NTSC - 625/50
      VideoControl Interface Descriptor:
        bLength                27
        bDescriptorType        36
        bDescriptorSubtype      6 (EXTENSION_UNIT)
        bUnitID                 6
        guidExtensionCode         {564c97a7-7ea7-904b-8cbf-1c71ec303000}
        bNumControl            16
        bNrPins                 1
        baSourceID( 0)          5
        bControlSize            2
        bmControls( 0)       0xff
        bmControls( 1)       0xff
        iExtension              0 
      Endpoint Descriptor:
        bLength                 7
        bDescriptorType         5
        bEndpointAddress     0x82  EP 2 IN
        bmAttributes            3
          Transfer Type            Interrupt
          Synch Type               None
          Usage Type               Data
        wMaxPacketSize     0x0010  1x 16 bytes
        bInterval              15
    Interface Descriptor:
      bLength                 9
      bDescriptorType         4
      bInterfaceNumber        1
      bAlternateSetting       0
      bNumEndpoints           0
      bInterfaceClass        14 Video
      bInterfaceSubClass      2 Video Streaming
      bInterfaceProtocol      0 
      iInterface              0 
      VideoStreaming Interface Descriptor:
        bLength                            14
        bDescriptorType                    36
        bDescriptorSubtype                  1 (INPUT_HEADER)
        bNumFormats                         1
        wTotalLength                      217
        bEndPointAddress                  129
        bmInfo                              0
        bTerminalLink                       3
        bStillCaptureMethod                 0
        bTriggerSupport                     0
        bTriggerUsage                       0
        bControlSize                        1
        bmaControls( 0)                    27
      VideoStreaming Interface Descriptor:
        bLength                            27
        bDescriptorType                    36
        bDescriptorSubtype                  4 (FORMAT_UNCOMPRESSED)
        bFormatIndex                        1
        bNumFrameDescriptors                5
        guidFormat                            {59555932-0000-1000-8000-00aa00389b71}
        bBitsPerPixel                      16
        bDefaultFrameIndex                  1
        bAspectRatioX                       0
        bAspectRatioY                       0
        bmInterlaceFlags                 0x00
          Interlaced stream or variable: No
          Fields per frame: 2 fields
          Field 1 first: No
          Field pattern: Field 1 only
          bCopyProtect                      0
      VideoStreaming Interface Descriptor:
        bLength                            34
        bDescriptorType                    36
        bDescriptorSubtype                  5 (FRAME_UNCOMPRESSED)
        bFrameIndex                         1
        bmCapabilities                   0x00
          Still image unsupported
        wWidth                            640
        wHeight                           480
        dwMinBitRate                 73728000
        dwMaxBitRate                147456000
        dwMaxVideoFrameBufferSize      614400
        dwDefaultFrameInterval         333332
        bFrameIntervalType                  2
        dwFrameInterval( 0)            333332
        dwFrameInterval( 1)            666666
      VideoStreaming Interface Descriptor:
        bLength                            34
        bDescriptorType                    36
        bDescriptorSubtype                  5 (FRAME_UNCOMPRESSED)
        bFrameIndex                         2
        bmCapabilities                   0x00
          Still image unsupported
        wWidth                            320
        wHeight                           240
        dwMinBitRate                 18432000
        dwMaxBitRate                 36864000
        dwMaxVideoFrameBufferSize      153600
        dwDefaultFrameInterval         333332
        bFrameIntervalType                  2
        dwFrameInterval( 0)            333332
        dwFrameInterval( 1)            666666
      VideoStreaming Interface Descriptor:
        bLength                            34
        bDescriptorType                    36
        bDescriptorSubtype                  5 (FRAME_UNCOMPRESSED)
        bFrameIndex                         3
        bmCapabilities                   0x00
          Still image unsupported
        wWidth                            160
        wHeight                           120
        dwMinBitRate                  4608000
        dwMaxBitRate                  9216000
        dwMaxVideoFrameBufferSize       38400
        dwDefaultFrameInterval         333332
        bFrameIntervalType                  2
        dwFrameInterval( 0)            333332
        dwFrameInterval( 1)            666666
      VideoStreaming Interface Descriptor:
        bLength                            34
        bDescriptorType                    36
        bDescriptorSubtype                  5 (FRAME_UNCOMPRESSED)
        bFrameIndex                         4
        bmCapabilities                   0x00
          Still image unsupported
        wWidth                            352
        wHeight                           288
        dwMinBitRate                 24330240
        dwMaxBitRate                 48660480
        dwMaxVideoFrameBufferSize      202752
        dwDefaultFrameInterval         333332
        bFrameIntervalType                  2
        dwFrameInterval( 0)            333332
        dwFrameInterval( 1)            666666
      VideoStreaming Interface Descriptor:
        bLength                            34
        bDescriptorType                    36
        bDescriptorSubtype                  5 (FRAME_UNCOMPRESSED)
        bFrameIndex                         5
        bmCapabilities                   0x00
          Still image unsupported
        wWidth                            176
        wHeight                           144
        dwMinBitRate                  6082560
        dwMaxBitRate                 12165120
        dwMaxVideoFrameBufferSize       50688
        dwDefaultFrameInterval         333332
        bFrameIntervalType                  2
        dwFrameInterval( 0)            333332
        dwFrameInterval( 1)            666666
      VideoStreaming Interface Descriptor:
        bLength                             6
        bDescriptorType                    36
        bDescriptorSubtype                 13 (COLORFORMAT)
        bColorPrimaries                     1 (BT.709,sRGB)
        bTransferCharacteristics            1 (BT.709)
        bMatrixCoefficients                 4 (SMPTE 170M (BT.601))
    Interface Descriptor:
      bLength                 9
      bDescriptorType         4
      bInterfaceNumber        1
      bAlternateSetting       1
      bNumEndpoints           1
      bInterfaceClass        14 Video
      bInterfaceSubClass      2 Video Streaming
      bInterfaceProtocol      0 
      iInterface              0 
      Endpoint Descriptor:
        bLength                 7
        bDescriptorType         5
        bEndpointAddress     0x81  EP 1 IN
        bmAttributes            5
          Transfer Type            Isochronous
          Synch Type               Asynchronous
          Usage Type               Data
        wMaxPacketSize     0x1400  3x 1024 bytes
        bInterval               1
    Interface Descriptor:
      bLength                 9
      bDescriptorType         4
      bInterfaceNumber        1
      bAlternateSetting       2
      bNumEndpoints           1
      bInterfaceClass        14 Video
      bInterfaceSubClass      2 Video Streaming
      bInterfaceProtocol      0 
      iInterface              0 
      Endpoint Descriptor:
        bLength                 7
        bDescriptorType         5
        bEndpointAddress     0x81  EP 1 IN
        bmAttributes            5
          Transfer Type            Isochronous
          Synch Type               Asynchronous
          Usage Type               Data
        wMaxPacketSize     0x1400  3x 1024 bytes
        bInterval               1
    Interface Descriptor:
      bLength                 9
      bDescriptorType         4
      bInterfaceNumber        1
      bAlternateSetting       3
      bNumEndpoints           1
      bInterfaceClass        14 Video
      bInterfaceSubClass      2 Video Streaming
      bInterfaceProtocol      0 
      iInterface              0 
      Endpoint Descriptor:
        bLength                 7
        bDescriptorType         5
        bEndpointAddress     0x81  EP 1 IN
        bmAttributes            5
          Transfer Type            Isochronous
          Synch Type               Asynchronous
          Usage Type               Data
        wMaxPacketSize     0x1400  3x 1024 bytes
        bInterval               1
```

```
user@ubuntu:~$ lsusb -t
/:  Bus 02.Port 1: Dev 1, Class=root_hub, Driver=xhci_hcd/8p, 10000M
/:  Bus 01.Port 1: Dev 1, Class=root_hub, Driver=xhci_hcd/16p, 480M
    |__ Port 1: Dev 2, If 0, Class=Human Interface Device, Driver=usbhid, 12M
    |__ Port 2: Dev 6, If 0, Class=Video, Driver=uvcvideo, 480M
    |__ Port 2: Dev 6, If 1, Class=Video, Driver=uvcvideo, 480M
    |__ Port 5: Dev 3, If 1, Class=Human Interface Device, Driver=usbhid, 1.5M
    |__ Port 5: Dev 3, If 0, Class=Human Interface Device, Driver=usbhid, 1.5M
    |__ Port 8: Dev 4, If 1, Class=Video, Driver=uvcvideo, 480M
    |__ Port 8: Dev 4, If 0, Class=Video, Driver=uvcvideo, 480M
    |__ Port 14: Dev 5, If 0, Class=Wireless, Driver=btusb, 12M
    |__ Port 14: Dev 5, If 1, Class=Wireless, Driver=btusb, 12M
```

```
user@ubuntu:~/Stereo_cam$ ./camera.sh
[libwebcam] ERROR: Unable to query size of XU control 00000000-0000-0000-0000-000000000000/8: Inappropriate ioctl for device. (System code: 25).
query control size of : 0
ERROR: Unable to set the control value: A Video4Linux2 API call returned an unexpected error 0. (Code: 12)
[libwebcam] ERROR: Unable to query size of XU control 00000000-0000-0000-0000-000000000000/15: Inappropriate ioctl for device. (System code: 25).
query control size of : 0
ERROR: Unable to set the control value: A Video4Linux2 API call returned an unexpected error 0. (Code: 12)
[libwebcam] ERROR: Unable to query size of XU control 00000000-0000-0000-0000-000000000000/8: Inappropriate ioctl for device. (System code: 25).
query control size of : 0
ERROR: Unable to set the control value: A Video4Linux2 API call returned an unexpected error 0. (Code: 12)
[libwebcam] ERROR: Unable to query size of XU control 00000000-0000-0000-0000-000000000000/8: Inappropriate ioctl for device. (System code: 25).
query control size of : 0
ERROR: Unable to set the control value: A Video4Linux2 API call returned an unexpected error 0. (Code: 12)
[libwebcam] ERROR: Unable to query size of XU control 00000000-0000-0000-0000-000000000000/15: Inappropriate ioctl for device. (System code: 25).
query control size of : 0
ERROR: Unable to set the control value: A Video4Linux2 API call returned an unexpected error 0. (Code: 12)
[libwebcam] ERROR: Unable to query size of XU control 00000000-0000-0000-0000-000000000000/15: Inappropriate ioctl for device. (System code: 25).
query control size of : 0
ERROR: Unable to set the control value: A Video4Linux2 API call returned an unexpected error 0. (Code: 12)
[libwebcam] ERROR: Unable to query size of XU control 00000000-0000-0000-0000-000000000000/15: Inappropriate ioctl for device. (System code: 25).
query control size of : 0
ERROR: Unable to set the control value: A Video4Linux2 API call returned an unexpected error 0. (Code: 12)
[libwebcam] ERROR: Unable to query size of XU control 00000000-0000-0000-0000-000000000000/15: Inappropriate ioctl for device. (System code: 25).
query control size of : 0
ERROR: Unable to set the control value: A Video4Linux2 API call returned an unexpected error 0. (Code: 12)
[libwebcam] ERROR: Unable to query size of XU control 00000000-0000-0000-0000-000000000000/8: Inappropriate ioctl for device. (System code: 25).
query control size of : 0
ERROR: Unable to set the control value: A Video4Linux2 API call returned an unexpected error 0. (Code: 12)
[libwebcam] ERROR: Unable to query size of XU control 00000000-0000-0000-0000-000000000000/10: Inappropriate ioctl for device. (System code: 25).
query control size of : 0
ERROR: Unable to set the control value: A Video4Linux2 API call returned an unexpected error 0. (Code: 12)
user@ubuntu:~/Stereo_cam$ 
```
