#include <msp430.h>
void ADC_Init(void);
void BCS_Init(void);
void PWM_Init(void);
void GPIO_Init(void);
void TIMERA_Init(void);
void calc_pid(void);
float error = 0, P = 0, I = 0, D = 0, PID_value = 0;//pid中间变量
float Kp = 0, Ki = 0, Kd = 0;                    //pid参数
float previous_error = 0;           //误差值
int v1,v4;
int main(void)
{
WDTCTL = WDTPW + WDTHOLD; // Stop WDT
BCS_Init();
ADC_Init();
PWM_Init();

GPIO_Init();
P1DIR |= BIT0; // Set P1.0 to output direction
P1REN|=BIT1;
P1OUT|=~BIT3;//设置1.3下拉电阻,输出pwm低电平
//__bis_SR_register(GIE); // Enter interrupt.03

for (;;)
{
ADC10CTL0 = ADC10SHT_2 + ADC10ON + ADC10IE; // ADC10ON, interrupt enabled
ADC10CTL1 = INCH_1; // input A1
ADC10AE0 |= 0x02; // PA.1 ADC option select
ADC10CTL0 |= ENC + ADC10SC; // Sampling and conversion start
__bis_SR_register(CPUOFF + GIE); // LPM0, ADC10_ISR will force exit
v1=ADC10MEM;
        ADC10CTL0 = ADC10SHT_2 + ADC10ON + ADC10IE; // ADC10ON, interrupt enabled
        ADC10CTL1 = INCH_7; // input A1
        ADC10AE0 |= 0x02; // PA.1 ADC option select
        ADC10CTL0 |= ENC + ADC10SC; // Sampling and conversion start
        __bis_SR_register(CPUOFF + GIE); // LPM0, ADC10_ISR will force exit
        v4=ADC10MEM;//
if (v1 < 0x26E)
P1OUT &= ~BIT0; // Clear P1.0 LED off
else
P1OUT |= BIT0; // Set P1.0 LED on

//error=ADC10MEM/
calc_pid();
CCR1=PID_value;
}
}
void calc_pid(void){
    Kp = 1;
    Ki = 0.01;
    Kd = 0;
    error=v1/2-v4;
    P = error;
    I = I + error;
    D =error - previous_error;
    PID_value = (Kp * P) + (Ki * I) + (Kd * D);
      if(PID_value>100)
    {
      PID_value=100;
    }
    else if(PID_value<0)
    {
      PID_value=0;
    }
      previous_error = error;
}

void ADC_Init(void) {
ADC10CTL0 = ADC10SHT_2 + ADC10ON + ADC10IE; // ADC10ON, interrupt enabled
ADC10CTL1 = INCH_1; // input A1
ADC10AE0 |= 0x02; // PA.1 ADC option select
}
void BCS_Init(void) {
//MCLK SMCLK 1MHz ACLK 12KHz
BCSCTL2 = SELM_0 | DIVM_0 | DIVS_0;
if (CALBC1_1MHZ != 0xFF) {
DCOCTL = 0x00;
BCSCTL1 = CALBC1_1MHZ; /* Set DCO to 1MHz */
DCOCTL = CALDCO_1MHZ;
}
BCSCTL1 |= XT2OFF | DIVA_0;
BCSCTL3 = XT2S_0 | LFXT1S_2 | XCAP_1;
}
void PWM_Init(void) {
P1DIR |= BIT2; // P1.2 output
P1SEL |= BIT2; // P1.2 TA0.1 options
CCR0 = 100; // PWM Period
CCTL1 = OUTMOD_7; // CCR1 reset/set
//CCR1 = 26; // CCR1 PWM duty cycle测试的1V
TACTL = TASSEL_2 + MC_1; // SMCLK, up mode
}
void TIMERA_Init(void) {
//P2DIR |= BIT2; // P2.2 output
//P2SEL |= BIT2; // P2.2 TA1.1 options
TA1CCR0 = 1200; // PWM Period
TA1CCTL1 = OUTMOD_7; // CCR1 reset/set
TA1CCR1 = 600; // CCR1 PWM duty cycle
TA1CTL = TASSEL_1 + MC_1; // ACLK, up mode
}
void GPIO_Init(void) {
P1DIR |= BIT6; // P1.6 output
P1OUT |= BIT6; // P1.6 output
P1DIR &= ~BIT4; // P1.4 enable input
P1OUT |= BIT4; // P1.4 set, else reset
P1REN |= BIT4; // P1.4 pullup
P1IE |= BIT4; // P1.4 interrupt enabled
P1IES |= BIT4; // P1.4 Hi/lo edge下降沿
P1IFG &= ~BIT4; // P1.4 IFG cleared
P1DIR &= ~BIT7; // P1.7 enable input
P1OUT |= BIT7; // P1.7 set, else reset   1.7用来采样低通滤波器之后的电压形成电压闭环

}
// ADC10 interrupt service routine
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void)
{
__bic_SR_register_on_exit(CPUOFF); // Clear CPUOFF bit from 0(SR)
}
// Port 1 interrupt service routine
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
P1OUT ^= BIT6; // Invert P1.6 1.6输出频率是1.4的一半
P1IFG &= ~BIT4; // CLEAR P1.3 Interrput flag
}

