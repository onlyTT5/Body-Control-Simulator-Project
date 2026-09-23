# Body-Control-Simulator-Project
基于STM32实现CAN 车身控制模拟器。大致功能是一个节点模拟车机或 CAN 报文源。 主控接收 CAN 报文后控制 LED、蜂鸣器、继电器模拟大灯、转向灯、门锁。 GPIO 按键模拟门状态和灯光开关。 PWM 模拟雨刮电机或仪表背光。 UART 输出当前状态；SPI Flash 保存故障码；I2C OLED 显示状态。 做“通信超时后进入安全状态”的逻辑。
