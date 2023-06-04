# Monica

视频介绍：https://www.bilibili.com/video/BV1AP411Q7dk

硬件开源：

![](https://github.com/Forairaaaaa/monica/blob/main/Pics/cover.JPG?raw=true)

### 原理图

![](https://github.com/Forairaaaaa/monica/blob/main/Pics/hardware1.png?raw=true)

- **主控**：ESP32-S3，32M 外置 Flash
- **IMU**：BMI270 + BM150 （抬手唤醒、计步器、指南针）
- **电源管理**：AXP2101
- **其他**：SD卡槽、RTC、蜂鸣器、MIC、气压传感、按键
- [**屏幕**](https://item.taobao.com/item.htm?spm=a1z09.2.0.0.a41c2e8dNZ5GOq&id=688638221390)：1.8寸 Amoled 368*448 
- **[电池](https://detail.tmall.com/item.htm?_u=42bdtj0f19f8&id=632947661144)**：402728 400毫安

### PCB

![](https://github.com/Forairaaaaa/monica/blob/main/Pics/hardware2.png?raw=true)

- 单面四层0.8MM，元件比较密集
- 1.27MM 母座用于下载和 USB 连接
- 引出串口和 I2C 接口等，预留用于底板拓展

### 程序

![](https://github.com/Forairaaaaa/monica/blob/main/Pics/firmware.png?raw=true)

#### 应用、驱动层解耦

- [**SimpleKV**](https://github.com/Forairaaaaa/simplekv.git)：简单的 `Key-Value (键-值)` 内存数据库框架，实现两层的数据交互。使用 `键` (字符串) 索引数据，切断应用层和驱动层的数据耦合。 
- [**Lvgl**](https://github.com/lvgl/lvgl.git)： 使用 Lvgl 作上层应用默认图形库，及其文件系统接口

因此，只要设备适配 Lvgl ，就可以直接使用这套 [Mooncake](https://github.com/Forairaaaaa/mooncake.git) UI框架，如 Linux ：

![](https://github.com/Forairaaaaa/monica/blob/main/Pics/firmware2.png?raw=true)

#### 驱动层

- **HAL**：外设驱动的对象抽象
- **Hardware Manager**：硬件管理层。负责底层的 `硬件初始化`、运行时的 `状态维护更新`，例如自动睡眠、抬手唤醒、计步器更新等
- **USB MSC 模式**：为了读写 SD 卡方便，同时按 `电源` 和 `下` 按键启动时，会进入 `USB MSC 模式`，SD 卡会被挂载成 U盘，接上 USB 即可直接读写

#### 应用层

- **App 基类、安装卸载**框架：所有 App 的对象基类，描述了 App 的基础框架。安装卸载框架实现简单的 App 列表管理

- **App 生命周期管理**：支持前后台的 App 运行生命周期 `调度器` (单线程，单前台，多后台) ，使用 `状态机` (FSM)实现

  ![](https://github.com/Forairaaaaa/monica/blob/main/Pics/firmware3.png?raw=true)

- **系统内置 APP**：都是在 App 层之上实现的应用，但 `启动动画` 和 `启动器` 会被 `Framwaork` 框架特殊调用

#### 用户自定义表盘

**watch_faces** 目录下的每一个文件夹都视作一个`表盘`

- **background** 目录：存放表盘背景资源 (png、gif)
- **number** 目录：存放从0~9的时间数字资源  (png)
- **face.json** ：资源描述文件，如时间数字坐标等

```shell
watch_faces/
|
├── AyanamiRei
│   ├── background
│   │   ├── background.png
│   ├── face.json
│   └── number
│       ├── 0.png
│       ├── ~
│       └── 9.png
|
├── FloatingNum
│   ├── background
│   │   └── background.png
│   ├── face.json
│   └── number
│       ├── 0.png
│       ├── ~
│       └── 9.png
|
└── PixelThinking
  ├── background
  │   ├── background.png
  ├── face.json
  └── number
      ├── 0.png
      ├── ~
      └── 9.png
```

### 模型

![](https://github.com/Forairaaaaa/monica/blob/main/Pics/model.png?raw=true)

![](https://github.com/Forairaaaaa/monica/blob/main/Pics/model2.png?raw=true)

存在问题：

- 目前模型只能选激光烧结，树脂打印会有缝
- 按键位置有偏移，需要按靠上部位



![](https://github.com/Forairaaaaa/monica/blob/main/Pics/monica2.mp4_20230604_001454.009.png?raw=true)

![](https://github.com/Forairaaaaa/monica/blob/main/Pics/monica2.mp4_20230604_001528.106.png?raw=true)

![](https://github.com/Forairaaaaa/monica/blob/main/Pics/monica2.mp4_20230604_001548.448.png?raw=true)

![](https://github.com/Forairaaaaa/monica/blob/main/Pics/monica2.mp4_20230604_001608.565.png?raw=true)

![](https://github.com/Forairaaaaa/monica/blob/main/Pics/monica2.mp4_20230604_001620.656.png?raw=true)

![](https://github.com/Forairaaaaa/monica/blob/main/Pics/monica2.mp4_20230604_001711.494.png?raw=true)
