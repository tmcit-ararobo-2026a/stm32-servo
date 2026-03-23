#include "stm32_servo/app.hpp"

#include "drivers/stm32_fdcan/driver_stm32_fdcan.hpp"
#include "fdcan.h"
#include "gn10_can/core/can_bus.hpp"
#include "gn10_can/devices/servo_motor_server.hpp"
#include "tim.h"

gn10_can::drivers::DriverSTM32FDCAN can_driver(&hfdcan1);
gn10_can::CANBus can_bus(can_driver);
extern TIM_HandleTypeDef htim2;

uint32_t duty_set(float duty)
{
    float val = (duty * 1.0f / 180.0f + 1.0f) * 65535 / 100;
    return uint32_t(val);
}
void setup()
{
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
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