#pragma once

#include "cstdint"

namespace Status {

	extern int8_t speedPercentage;       // 转速百分比
	extern int8_t targetSpeedPercentage; // 目标转速百分比

	// 电机状态
	enum class MotorStatus {
		PowerOff,     // 电机未上电
		Shutdown,     // 电机已经关闭（上电、不刹车、零速度，不允许调节速度）
		Stopped,      // 已经停止（上电、不刹车、零速度，可以调节速度）
		Running,      // 正在运行（转动中，可以调节速度）
		Braking,      // 紧急刹车（上电、刹车，不允许调整速度）
		Decelerating, // 减速中（上电、不刹车，只允许定时器回调中减速到 0）
		Error         // 状态错误
	};

	// 速度修改倍率
	enum class SpeedChangeRate {
		X0,    // 不允许修改速度
		X5,    // 旋转编码器时 +5% 或 -5%
		X10,   // 旋转编码器时 +10% 或 -10%
	};

	// 状态
	extern MotorStatus motorStatus;         // 电机状态
	extern SpeedChangeRate speedChangeRate; // 速度修改倍率

}
