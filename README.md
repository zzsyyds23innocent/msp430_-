# msp430_实验一
单片机课程设计
综合实验一：单片机硬件资源小测试
实验目的：1.掌握基本的单片机资源及其编程，包括IO、AD、定时器、PWM等；
2.掌握基本的模拟电路的设计，以及基本的焊接技术。
实验要求：如下图所示，实验最终要求完全实现以下所有功能：
	 	
1.	单片机通过A/D采样V1的电压值，V1电压值在0到3V之间，可由“直流可调稳压电源”提供；
2.	当V1电压值大于2V时，V3由低电平变成高电平，点亮一路LED灯；
3.	输出1路PWM波，开关频率是10KHz，PWM的占空比跟随着V1的变化而变化，并设计低通滤波器，目标使得V4是直流电压，大小是“V1/2”；
4.	V2是信号发生器输出的方波（幅值高低电平分别是0、3V），频率是50-1KHz， V5是输出方波信号频率是V2的一半，用示波器同时观察V2及V5。
5.	以上所有功能应该在同一个程序下实现。否则，会扣分。
