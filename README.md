## CANOpen Festival RTOS 移植

由于项目中需要用到CANOpen主站，于是想到之前在STM32上用过的CANFestival开源库

这个库的移植还是比较简单的，但是久了没搞容易忘记，所以在这里分享出来。

这次也参考了RT-thread开源社区的人分享的[canfestival-rtt](https://github.com/gbcwbz/canfestival-rtt)库，但是这个库要用到rt-thread的拓展管理工具，

其中有很多定义在rt-thread层，增加了其他平台的移植复杂度。

这里为了方便在其他平台快速移植，我会尽量做好接口的通用性，真正做到"开箱即用"
