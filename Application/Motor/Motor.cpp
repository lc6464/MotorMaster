#include "Motor.h"
#include <algorithm>

void Motor::PowerOn() {
	// 上电顺序：5V -> 电源
	SetPinState(_power5V, GPIO_PIN_SET);
	SetPinState(_power, GPIO_PIN_SET);
}

void Motor::PowerOff() {
	// 断电顺序：电源 -> 5V -> 停止电机
	SetPinState(_power, GPIO_PIN_RESET);
	SetPinState(_power5V, GPIO_PIN_RESET);
	Stop();
}

HAL_StatusTypeDef Motor::Start() {
	// 启动顺序：启用 PWM -> 解除刹车
	// auto status = HAL_TIM_PWM_Start(&_htim, _channel);
	// if (status == HAL_OK) {
	SetSpeed(0);
	SetPinState(_brake, GPIO_PIN_RESET);  // 解除刹车
	// }
	// return status;
	return HAL_OK;
}

HAL_StatusTypeDef Motor::Stop() {
	// 停止顺序：启用刹车 -> 停止 PWM
	SetPinState(_brake, GPIO_PIN_SET);  // 启用刹车
	SetSpeed(0);  // 停止 PWM
	// auto status = HAL_TIM_PWM_Stop(&_htim, _channel);
	// return status;
	return HAL_OK;
}
