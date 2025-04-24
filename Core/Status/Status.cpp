#include "Status.h"

namespace Status {

	int8_t speedPercentage = 0;       // 转速百分比
	int8_t targetSpeedPercentage = 0; // 目标转速百分比

	// 状态
	MotorStatus motorStatus = MotorStatus::PowerOff;       // 电机状态
	SpeedChangeRate speedChangeRate = SpeedChangeRate::X0; // 速度修改倍率

}
