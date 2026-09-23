# I2C OLED 测试笔记

## 1. 测试目的

在 STM32F103C8T6（Blue Pill）上完成 0.96 寸 I2C OLED 的接线、设备地址探测、SSD1306 初始化和字符显示，为 CAN 车身控制模拟器提供状态显示界面。

## 2. 硬件与引脚

| 设备 | 引脚 | STM32F103C8T6 |
|---|---|---|
| 0.96 寸 OLED | GND | GND |
| 0.96 寸 OLED | VDD | 3.3V |
| 0.96 寸 OLED | SCK / SCL | PB6（I2C1_SCL） |
| 0.96 寸 OLED | SDA | PB7（I2C1_SDA） |

PB8、PB9 不用于 OLED，预留给后续 CAN1 重映射：PB8 为 CAN_RX，PB9 为 CAN_TX。

## 3. CubeMX / CubeIDE 配置

- 启用 I2C1。
- PB6 配置为 I2C1_SCL，PB7 配置为 I2C1_SDA。
- I2C 速率：100 kHz（Standard Mode）。
- 使用 HAL 库。
- PC13 配置为 GPIO 输出，作为状态灯。Blue Pill 的板载 LED 为低电平点亮。

## 4. OLED 地址探测

使用 HAL 库检测常见 OLED 地址 0x3C：

```c
HAL_StatusTypeDef oled_status;

oled_status = HAL_I2C_IsDeviceReady(
    &hi2c1,
    0x3C << 1,
    3,
    100
);
```

本次测试结果：设备地址为 `0x3C`，探测成功。

说明：HAL 的 I2C 接口传入的是 8 位地址形式，因此 7 位地址 `0x3C` 需左移一位。

## 5. SSD1306 初始化验证

向 OLED 发送 SSD1306 初始化指令后，交替发送：

```c
OLED_WriteCmd(0xA5);  // 全屏点亮
HAL_Delay(1000);
OLED_WriteCmd(0xA4);  // 按显存内容显示
HAL_Delay(1000);
```

现象：OLED 全屏点亮 1 秒、黑屏 1 秒循环，说明 I2C 总线、地址、供电和 SSD1306 控制指令均正常。

## 6. 显示功能结果

实现了基础 SSD1306 驱动：

- 命令发送 `OLED_WriteCmd()`；
- 数据发送 `OLED_WriteData()`；
- 页寻址与列定位；
- 清屏；
- 5×7 英文字符字库；
- 字符串显示。

当前 OLED 页面：

```text
BODY CONTROL V0.1
CAN: OFFLINE
LIGHT: OFF / ON
```

PB11 按键切换车灯状态后，OLED 与 PC13 板载 LED 同步更新。

## 7. 当前软件模块关系

```text
bsp_button  -> PB11 按键读取与消抖
body_control -> 保存车灯、CAN 在线等业务状态
bsp_led     -> PC13 板载 LED 输出
oled         -> SSD1306 I2C 驱动和字库
ui           -> OLED 状态页面
main         -> 初始化与模块调度
```

## 总结

```
规则：
Start：SCL 为高时，SDA 从高变低
数据：SCL 低电平期间改变 SDA；SCL 高电平期间采样 SDA
ACK：第 9 个时钟，发送方释放 SDA，接收方拉低 SDA 表示确认
Stop：SCL 为高时，SDA 从低变高
```

I2C 空闲时 SCL 和 SDA 都由上拉电阻保持高电平。主机在 SCL 为高电平时，将 SDA 从高拉低，产生 Start 条件。随后主机在 SCL 低电平期间依次设置 SDA 上的地址位和数据位，在 SCL 高电平期间由从机采样。OLED 的 7 位地址是 0x3C，写操作发送的是 0x78。每发送完 8 位数据，第 9 个时钟由接收方把 SDA 拉低，表示 ACK 应答。通信结束时，主机在 SCL 高电平期间释放 SDA，使 SDA 从低变高，产生 Stop 条件。I2C 引脚是开漏输出，设备只能主动拉低总线；高电平依靠外部或模块上的上拉电阻产生。
