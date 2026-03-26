#include "stm32_servo/app.hpp"

#include "drivers/stm32_fdcan/driver_stm32_fdcan.hpp"
#include "fdcan.h"
#include "gn10_can/core/can_bus.hpp"
#include "gn10_can/devices/servo_motor_server.hpp"
#include "tim.h"

gn10_can::drivers::DriverSTM32FDCAN can_driver(&hfdcan1);
gn10_can::CANBus can_bus(can_driver);
gn10_can::devices::ServoMotorServer servo(can_bus, 0);
float duty_ms = 0;
uint16_t max_us, min_us = 0;
float pulse_width_ms     = 0;
constexpr float MAX_DUTY = 19999.0f;
void setup()
{
    can_driver.init();
    uint16_t new_min, new_max;
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
}

void loop()
{
    float angle_rad  = 0;
    uint16_t new_max = 0, new_min = 0;
    if (servo.get_new_init(new_min, new_max)) {
        HAL_GPIO_WritePin(LED_2_GPIO_Port, LED_2_Pin, GPIO_PIN_SET);
        max_us         = new_max;
        min_us         = new_min;
        pulse_width_ms = float(max_us - min_us) / 1000.0f;
    }
    can_bus.update();
    if (servo.get_new_angle_rad(angle_rad)) {
        HAL_GPIO_WritePin(LED_1_GPIO_Port, LED_1_Pin, GPIO_PIN_SET);
        duty_ms       = (angle_rad / 3.14159265359f) * pulse_width_ms + ((float)min_us / 1000.0f);
        uint32_t duty = uint32_t((duty_ms / 20.0f) * MAX_DUTY);
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, duty);
    }
}
