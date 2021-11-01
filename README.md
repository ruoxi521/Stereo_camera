<h1 align="center">
    <strong>⭐️Stereo_camera</strong>
</h1>

<p align="center">
    <strong>This is a set of applications and tests for binocular cameras</strong>
</p>

下图就是它的真实面目，以及我为它量身定做的外壳。

![QQ图片20211010133829](https://raw.githubusercontent.com/sujit-168/Blog-Picture/master/Typora/202110101405267.jpg)

## windows

在Windows10下需要配合卖家提供的上位机来使用，调整相机的模式



## Linux

在Linux下需要配合卖家提供的sh脚本来使用，通过uvc命令来调整相机的模式，为了方便使用，我对提供的脚本进行了优化，使其使用起来更加友好。

- 首先需要查询设备名称及端口号

```
lsusb  #查询出当前已连接的设备名及端口号
```

- 之后运行src/Test下的test1.cpp即可输出摄像头采集到的图像，相机初始模式为右目视图，使用sh脚本来切换相机输出模式。

```
./camera.sh  #运行camera.sh脚本
```
如果不习惯使用shell

可以使用我制作的这款支持Linux环境的上位机
下载链接 [https://github.com/ruoxi521/Stereo_QT/releases](https://github.com/ruoxi521/Stereo_QT/releases)
