#include "stm32_servo/app.hpp"

#include <algorithm>
#include <cmath>

#include "drivers/stm32_fdcan/driver_stm32_fdcan.hpp"
#include "fdcan.h"
#include "gn10_can/core/can_bus.hpp"
#include "gn10_can/devices/servo_motor_server.hpp"
#include "main.h"
#include "tim.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

// Global CAN objects
gn10_can::drivers::DriverSTM32FDCAN can_driver(&hfdcan1);
gn10_can::CANBus can_bus(can_driver);

// Servo instances
gn10_can::devices::ServoMotorServer servo0(can_bus, 0);
gn10_can::devices::ServoMotorServer servo1(can_bus, 1);
gn10_can::devices::ServoMotorServer servo2(can_bus, 2);

struct ServoConfig {
    gn10_can::devices::ServoMotorServer& server;
    uint32_t channel;
    uint16_t min_us;
    uint16_t max_us;
};

// Map CAN Device IDs to TIM2 Channels
// ID 0 -> CH1
// ID 1 -> CH2
// ID 2 -> CH4
ServoConfig servos[] = {
    {servo0, TIM_CHANNEL_1, 500, 2500},
    {servo1, TIM_CHANNEL_2, 500, 2500},
    {servo2, TIM_CHANNEL_4, 500, 2500},
};

static void update_servo(ServoConfig& config)
{
    // Update min/max if new init config received
    uint16_t new_min, new_max;
    if (config.server.get_new_init(new_min, new_max)) {
        config.min_us = new_min;
        config.max_us = new_max;
    }

    // Update PWM if new angle received
    float angle;
    if (config.server.get_new_angle_rad(angle)) {
        // Clamp angle to [0, PI]
        float clamped = std::max(0.0f, std::min(static_cast<float>(M_PI), angle));

        // Map [0, PI] to [min_us, max_us]
        // t = 0.0 at 0, t = 1.0 at PI
        float t = clamped / M_PI;

        uint16_t pulse = config.min_us + static_cast<uint16_t>(t * (config.max_us - config.min_us));

        // Ensure pulse is strictly within bounds
        pulse = std::max(config.min_us, std::min(config.max_us, pulse));

        __HAL_TIM_SET_COMPARE(&htim2, config.channel, pulse);
    }
}

void setup()
{
    // Initialize CAN Driver
    can_driver.init();

    // Start PWM generation for all servo channels
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);
}

void loop()
{
    // Process CAN messages
    can_bus.update();

    // Update each servo state
    for (auto& servo : servos) {
        update_servo(servo);
    }
}
