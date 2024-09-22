#include "Motor_Shared.hpp"

static const PortPinPair motorBrakePair(Motor_Brake_GPIO_Port, Motor_Brake_Pin);
static const PortPinPair motorPowerPair(Motor_Power_GPIO_Port, Motor_Power_Pin);
static const PortPinPair motor5VPair(Motor_5V_GPIO_Port, Motor_5V_Pin);

Motor motor(htim2, TIM_CHANNEL_1, motorBrakePair, motorPowerPair, motor5VPair);