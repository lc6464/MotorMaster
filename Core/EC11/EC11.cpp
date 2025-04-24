#include "EC11.h"

HAL_StatusTypeDef EC11::Start() {
	__HAL_TIM_SET_COUNTER(&_htim, 127); // 将计数器初始值设置为127
	return HAL_TIM_Encoder_Start(&_htim, TIM_CHANNEL_ALL);
}

HAL_StatusTypeDef EC11::Stop() {
	return HAL_TIM_Encoder_Stop(&_htim, TIM_CHANNEL_ALL);
}

void EC11::UpdateRotation() {
	// 读取当前计数值
	uint16_t currentCount = static_cast<uint16_t>(__HAL_TIM_GET_COUNTER(&_htim));

	// 检测旋转方向并调用相应回调
	if (currentCount != 127) {
		if (currentCount > 127) {
			if (_cwCallback) _cwCallback();
		} else {
			if (_ccwCallback) _ccwCallback();
		}
		// 重置计数器为127
		__HAL_TIM_SET_COUNTER(&_htim, 127);
	}
}