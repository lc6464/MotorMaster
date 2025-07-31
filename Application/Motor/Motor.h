#pragma once

#include <array>
#include "tim.h"
#include "PortPinPair.h"

// 电机类
class Motor {
public:
	/**
	 * @brief 构造函数，初始化电机
	 * @param htim 定时器句柄引用
	 * @param channel 定时器通道
	 * @param brake 刹车引脚
	 * @param power 电源控制引脚
	 * @param power5V 5V 控制引脚
	 */
	explicit Motor(TIM_HandleTypeDef &htim, uint32_t channel, const PortPinPair &brake, const PortPinPair &power, const PortPinPair &power5V)
		: _htim(htim), _channel(channel), _brake(brake), _power(power), _power5V(power5V) {
		assert_param(channel != 0);
	}

	/**
	 * @brief 析构函数
	 * @note 停止电机，释放资源
	 * @note 电机停止时，不会刹车
	 */
	~Motor() {
		PowerOff();
	}

	/**
	 * @brief 给电机上电
	 * @note 将电源控制、5V 控制引脚拉高
	 */
	void PowerOn();

	/**
	 * @brief 给电机断电
	 * @note 将电源控制、5V 控制引脚拉低，然后停止电机
	 */
	void PowerOff();

	/**
	 * @brief 启动电机
	 * @note 启动 PWM 输出，解除刹车
	 * @return 启动状态
	 */
	HAL_StatusTypeDef Start();

	/**
	 * @brief 停止电机
	 * @note 启用刹车，停止 PWM 输出
	 * @return 停止状态
	 */
	HAL_StatusTypeDef Stop();

	/**
	 * @brief 设置电机速度
	 * @param speed 速度值, 范围为 [0, 4000]
	 * @note 速度为 0 时, 电机停止但不刹车
	 */
	void SetSpeed(uint16_t speed) {
		__HAL_TIM_SET_COMPARE(&_htim, _channel, std::min(speed, MAX_SPEED));
	}

private:
	static constexpr uint16_t MAX_SPEED = 4000;

	TIM_HandleTypeDef &_htim; // 定时器句柄引用
	const uint32_t _channel; // 定时器通道

	const PortPinPair &_brake; // 刹车引脚
	const PortPinPair &_power; // 电源控制引脚
	const PortPinPair &_power5V; // 5V 控制引脚

	/**
	 * @brief 设置引脚状态
	 * @param pin 引脚对
	 * @param state 引脚状态
	 */
	static void SetPinState(const PortPinPair &pin, GPIO_PinState state) {
		HAL_GPIO_WritePin(pin.Port, pin.Pin, state);
	}
};
