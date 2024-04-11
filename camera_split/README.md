## 1.ORB_SLAM2 运行演示Demo

```
user@ubuntu:~/SLAM/src/ORB_SLAM2$ ./Examples/Monocular/mono_tum Vocabulary/ORBvoc.txt Examples/Monocular/TUM1.yaml Data_Collection/rgbd_dataset_freiburg3_long_office_household
```

```
user@ubuntu:~/SLAM/src/ORB_SLAM2$ ./Examples/Monocular/mono_tum Vocabulary/ORBvoc.txt Examples/Monocular/TUM1.yaml Data_Collection/rgbd_dataset_freiburg1_floor
```

## 2.ORB_SLAM2 调用usb_cam单目摄像头进行实时建图

```
user@ubuntu:~$ roscore
```

```
user@ubuntu:~$ roslaunch usb_cam usb_cam-test.launch
```

```
user@ubuntu:~/SLAM/src/ORB_SLAM2$ rosrun ORB_SLAM2 Mono Vocabulary/ORBvoc.txt Examples/Monocular/TUM2.yaml
```

## 3.ORB_SLAM2 调用usb_cam双目摄像头进行实时建图

参考文章 [用电脑自带的摄像头跑orb_slam2](https://zhuanlan.zhihu.com/p/29629824)

然后我们再开始编译ROS的example

```text
cd SLAM/src/ORB_SLAM2/Examples/ROS/ORB_SLAM2/
mkdir build
cd build
cmake ..
make 
```

```
user@ubuntu:~$ roscore
```

```
user@ubuntu:~$ roslaunch camera_split stereo.launch
```

```
user@ubuntu:~/SLAM/src/ORB_SLAM2$ rosrun ORB_SLAM2 Stereo Vocabulary/ORBvoc.txt Examples/Stereo/EuRoC.yaml true
```

## 4.ORB_SLAM2 运行本地视频数据集进行建图

```
./Examples/Localvideo/video Vocabulary/ORBvoc.txt Examples/Localvideo/video1.yaml
```

