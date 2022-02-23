#  超低功耗Sub-1GHz 射频收发器芯片CMT2300A驱动 官方提供DMOE移植到ESP32平台

CMT2300A一个国产射频收发芯片，工作在315MHZ 443MHZ 889MHZ频段，可以用来接收发射无线摇控器，破解汽车胎压监测，汽车钥匙等应用   
[芯片官方资料下载地址](https://www.hoperf.cn/ic/rf_transceiver/CMT2300A.html) 
## 文件结构   
cmt2300a.c  **官方提供低层驱动程序**     
cmt2300a.h **官方驱动程序头文件**   
cmt2300a_defs.h  **寄存器名称定义**   
cmt2300a_params.h **使用官方配置软件RFPKD生成的寄存器值，按照bank分批复制过来即可**    
typedefs.h **变量声名定义**   
### 软件 IDE   
PlatformIO   
### 硬件连接   
EPS23主控，芯片采用gpio口模拟4线spi接口与MCU通讯,实际使用5个接口，期中CMT2300A_GPIO1_PIN为芯片中断输出，一个芯片片选CMT2300A_SPI_CSB_PIN引脚，一个CMT2300A_SPI_FCSB_PIN读写寄存器片选引脚，
另个两个为SCLK及DIO通讯引脚
## 使用方法
在EasyCMT2300A.h文件中定义PIN接口
  ```
// #define CMT2300A_GPIO3_PIN					0
// #define CMT2300A_GPIO2_PIN					0
#define CMT2300A_GPIO1_PIN						34  //interupt pin
#define CMT2300A_SPI_SCLK_PIN					32
#define CMT2300A_SPI_MOSI_PIN					12
//#define CMT2300A_SPI_MISO_PIN				GPIO_Pin_7
#define CMT2300A_SPI_CSB_PIN					27
#define CMT2300A_SPI_FCSB_PIN					25
  ```
使用RFPKD软件生成配置文件，按照寄存器bank复制到cmt2300a_params.h文件中   
实例中采用中断方式接收射频信号   
