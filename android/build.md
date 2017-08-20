# 源码编译

[http://blog.csdn.net/atmars_blue/article/details/55669670](http://blog.csdn.net/atmars_blue/article/details/55669670)

## 创建一个大小写敏感的image

```Shell
hdiutil create -type SPARSE -fs 'Case-sensitive Journaled HFS+' -size 100g ~/android.dmg
```

挂载image

```shell
hdiutil attach android.dmg.sparseimage -mountpoint /Volumes/android
```

卸载image

```Shell
hdiutil detach /Volumes/android
```

## 安装一些工具

安装macport

[https://www.macports.org/install.php](https://www.macports.org/install.php)

切换为中科大源

编辑`/opt/local/etc/macports/sources.conf`

```
rsync://debian.ustc.edu.cn/macports/release/tarballs/ports.tar [default]
```

然后执行下面命令

```
sudo port -v selfupdate
```

安装android源码编译所需要的工具

```
POSIXLY_CORRECT=1 sudo port install gmake libsdl git gnupg
```

## 源码下载

```
curl https://mirrors.tuna.tsinghua.edu.cn/git/git-repo -o repo 
chmod +x repo
export REPO_URL='https://mirrors.tuna.tsinghua.edu.cn/git/git-repo/'
cd /Volumes/android/
mkdir WORKING_DIRECTORY 
cd WORKING_DIRECTORY
# 初始化仓库
repo init -u https://aosp.tuna.tsinghua.edu.cn/platform/manifest -b android-7.0.0_r33
# sync android soucre tree
repo sync
repo forall -c git checkout -b android-7.0.0_r33_test android-7.0.0_r33
```

开始编译

```
export JACK_SERVER_VM_ARGUMENTS="-Dfile.encoding=UTF-8 -XX:+TieredCompilation -Xmx4096m"
./prebuilts/sdk/tools/jack-admin kill-server 
./prebuilts/sdk/tools/jack-admin start-server
source build/envsetup.sh 
lunch #然后选择需要编译的系统
make -j4 #x核y线程，就用-j{y}，最大限度使用cpu

```

### 运行模拟器

```
emulator -system out/target/product/generic/system.img -ramdisk out/target/product/generic/ramdisk.img -data out/target/product/generic/userdata.img
```

### 注意事项

```
1. 要下载老一些版本的macos sdk放到xcode里面的对应目录
2. 要开启jack server
3. 要创建一个大小写敏感的image
```

