#include "motor.h"
#include "sytick.h"

void motor_init(void)
{
	GPIO_InitTypeDef Motor_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	Motor_InitStruct.GPIO_Pin = IN1 | IN2 | IN3 | IN4;
	Motor_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	Motor_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(MOTOR_GPIO_PORT, &Motor_InitStruct);
	
}

void motor_clkw(void)
{
	GPIO_SetBits(MOTOR_GPIO_PORT, IN1);
	SysTick_delay_ms(5);
	GPIO_ResetBits(MOTOR_GPIO_PORT, IN1);
	GPIO_SetBits(MOTOR_GPIO_PORT, IN2);
	SysTick_delay_ms(5);
	GPIO_ResetBits(MOTOR_GPIO_PORT, IN2);
	GPIO_SetBits(MOTOR_GPIO_PORT, IN3);
	SysTick_delay_ms(5);
	GPIO_ResetBits(MOTOR_GPIO_PORT, IN3);
	GPIO_SetBits(MOTOR_GPIO_PORT, IN4);
	SysTick_delay_ms(5);
	GPIO_ResetBits(MOTOR_GPIO_PORT, IN4);
}

void motor_ctclkw(void)
{
	GPIO_SetBits(MOTOR_GPIO_PORT, IN4);
	SysTick_delay_ms(5);
	GPIO_ResetBits(MOTOR_GPIO_PORT, IN4);
	GPIO_SetBits(MOTOR_GPIO_PORT, IN3);
	SysTick_delay_ms(5);
	GPIO_ResetBits(MOTOR_GPIO_PORT, IN3);
	GPIO_SetBits(MOTOR_GPIO_PORT, IN2);
	SysTick_delay_ms(5);
	GPIO_ResetBits(MOTOR_GPIO_PORT, IN2);
	GPIO_SetBits(MOTOR_GPIO_PORT, IN1);
	SysTick_delay_ms(5);
	GPIO_ResetBits(MOTOR_GPIO_PORT, IN1);
}

void unlock(void)
{
	int time;
	for(time = 500; time != 0; time--)
	{
		motor_ctclkw();
	}
	
	SysTick_delay_ms(5000);
	
	for(time = 500; time != 0; time--)
	{
		motor_clkw();
	}
}