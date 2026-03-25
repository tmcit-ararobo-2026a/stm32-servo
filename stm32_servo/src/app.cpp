#include "stm32_servo/app.hpp"

#include "drivers/stm32_fdcan/driver_stm32_fdcan.hpp"
#include "fdcan.h"
#include "gn10_can/core/can_bus.hpp"
#include "gn10_can/devices/servo_motor_server.hpp"
#include "tim.h"

gn10_can::drivers::DriverSTM32FDCAN can_driver(&hfdcan1);
gn10_can::CANBus can_bus(can_driver);
gn10_can::devices::ServoMotorServer servo(can_bus, 0);
uint32_t duty_set = 0;
uint16_t max_us, min_us = 0;
float pulse_width = 0;
void setup()
{
    can_driver.init();
    uint16_t new_min, new_max;
    if (servo.get_new_init(new_min, new_max)) {
    }
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
}

void loop()
{
    float angle_deg = 0;
    uint16_t new_max, new_min;
    if (servo.get_new_init(new_min, new_max)) {
        max_us      = new_max;
        min_us      = new_min;
        pulse_width = (max_us - min_us) * 1000.0f;
    }
    can_bus.update();
    if (servo.get_new_angle_rad(angle_deg)) {
        duty_set = (uint32_t)(angle_deg * (pulse_width / 180.0f) + min_us * 1000.0f);
    }
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, duty_set);
}
