#include "stm32_servo/app.hpp"

#include <cmath>

#include "drivers/stm32_fdcan/driver_stm32_fdcan.hpp"
#include "fdcan.h"
#include "gn10_can/core/can_bus.hpp"
#include "gn10_can/devices/servo_motor_server.hpp"
#include "tim.h"

gn10_can::drivers::DriverSTM32FDCAN can_driver(&hfdcan1);
gn10_can::CANBus can_bus(can_driver);

gn10_can::devices::ServoMotorServer servo0(can_bus, 0);

uint16_t new_min, new_max;
float angle_rad;

void setup()
{
    can_driver.init();
    servo0.get_new_init(new_min, new_max);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
}

void loop()
{
    if (servo0.get_new_init(new_min, new_max)) {
    }
    if (servo0.get_new_angle_rad(angle_rad)) {
        float pulse_f  = angle_rad / M_PI;
        uint16_t pulse = new_min + pulse_f * (new_max - new_min);
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, pulse);
    }
}

extern "C" {
// C言語側の関数のオーバーライド
/**
 * @brief Receive callback for FDCAN FIFO0.
 */
void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef* hfdcan, uint32_t RxFifo0ITs)
{
    can_bus.update();
}
}