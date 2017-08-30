## 编译

http://blog.csdn.net/liu1075538266/article/details/51272398

https://source.android.com/source/build-numbers

 MRA58U android-6.0.0_r4

https://developers.google.com/android/drivers#hikey

下载对应的驱动
解压到android源码根目录，运行三个脚本
然后按照正常android编译步骤编译就可以了

vim build/core/combo/mac_version.mk

vim build/core/combo/HOST_darwin-x86.mk

```shell
source build/envsetup.sh 
lunch #然后选择需要编译的系统
make -j4
```

## 刷机

```shell
adb reboot bootloader
fastboot -w flashall
```

### 需要注意

- 编译某个模块

```
mmm art/
# 重新打包system.img
make snod
```

- 加入代码报错

加入的代码格式要规范，赋值判定符号前后要有空格，前大括号必须跟着上一行代码后面写，强制类型转换要用`reinterpret_cast`

