#include "stm32_servo/app.hpp"

#include "drivers/stm32_fdcan/driver_stm32_fdcan.hpp"
#include "gn10_can/core/can_bus.hpp"
#include "gn10_can/devices/servo_motor_server.hpp"
#include "tim.h"

extern TIM_HandleTypeDef htim2;

void setup()
{
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
}

void loop()
{
    int x = 0;
    x += 20;
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, x);
    HAL_GPIO_WritePin(LED_1_GPIO_Port, LED_1_Pin, GPIO_PIN_SET);
}
extern "C" {
}