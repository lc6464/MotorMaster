#include <algorithm>

#include "Button_Shared.h"
#include "EC11_Shared.h"
#include "Motor_Shared.h"
#include "Status.h"

static uint8_t ec11ClockwiseCallbackScaler = 0;
static uint8_t ec11CounterClockwiseCallbackScaler = 0;

// 重载运算符，使 SpeedChangeRate 能够直接被 int8_t 加减，即 int8_t + 0, int8_t + 5, int8_t + 10
inline int8_t operator+(int8_t a, Status::SpeedChangeRate b) {
	if (b == Status::SpeedChangeRate::X0) {
		return a;
	} else if (b == Status::SpeedChangeRate::X5) {
		return a + 5;
	} else if (b == Status::SpeedChangeRate::X10) {
		return a + 10;
	}
	return a;
}

inline int8_t operator-(int8_t a, Status::SpeedChangeRate b) {
	if (b == Status::SpeedChangeRate::X0) {
		return a;
	} else if (b == Status::SpeedChangeRate::X5) {
		return a - 5;
	} else if (b == Status::SpeedChangeRate::X10) {
		return a - 10;
	}
	return a;
}

// 旋转编码器顺时针旋转回调
static inline void EC11ClockwiseCallback(void) {
	if (++ec11ClockwiseCallbackScaler == 1) {
		return;
	}

	ec11ClockwiseCallbackScaler = 0;
	if (Status::speedChangeRate != Status::SpeedChangeRate::X0
		&& (Status::motorStatus == Status::MotorStatus::Running || Status::motorStatus == Status::MotorStatus::Stopped)) {
		Status::targetSpeedPercentage = std::min(Status::targetSpeedPercentage + Status::speedChangeRate, static_cast<int8_t>(100));
		Status::motorStatus = Status::MotorStatus::Running;
	}
}

// 旋转编码器逆时针旋转回调
static inline void EC11CounterClockwiseCallback(void) {
	if (++ec11CounterClockwiseCallbackScaler == 1) {
		return;
	}

	ec11CounterClockwiseCallbackScaler = 0;
	if (Status::speedChangeRate != Status::SpeedChangeRate::X0 && Status::motorStatus == Status::MotorStatus::Running) {
		Status::targetSpeedPercentage = std::max(Status::targetSpeedPercentage - Status::speedChangeRate, static_cast<int8_t>(0));
		if (Status::targetSpeedPercentage == 0) {
			Status::motorStatus = Status::MotorStatus::Stopped;
		}
	}
}

// 旋转编码器按下回调
static inline void EC11ShortPressCallback(void) {
	if (Status::motorStatus != Status::MotorStatus::Running && Status::motorStatus != Status::MotorStatus::Stopped) {
		return;
	}

	// x0 -> x5 -> x10 -> x0
	if (Status::speedChangeRate == Status::SpeedChangeRate::X0) {
		Status::speedChangeRate = Status::SpeedChangeRate::X5;
	} else if (Status::speedChangeRate == Status::SpeedChangeRate::X5) {
		Status::speedChangeRate = Status::SpeedChangeRate::X10;
	} else if (Status::speedChangeRate == Status::SpeedChangeRate::X10) {
		Status::speedChangeRate = Status::SpeedChangeRate::X0;
	}
}

static inline void EC11LongPressCallback(void) {
	// -> x0
	Status::speedChangeRate = Status::SpeedChangeRate::X0;
}

// 电机控制按钮按下回调
static inline void MotorButtonShortPressCallback(void) {
	if (Status::motorStatus == Status::MotorStatus::Stopped || Status::motorStatus == Status::MotorStatus::Running) {
		Status::speedChangeRate = Status::SpeedChangeRate::X0;
		Status::motorStatus = Status::MotorStatus::Decelerating;
	} else if (Status::motorStatus == Status::MotorStatus::Shutdown) {
		Status::motorStatus = Status::MotorStatus::Stopped;
		motor.Start();
	} else if (Status::motorStatus == Status::MotorStatus::Decelerating) {
		Status::targetSpeedPercentage = Status::targetSpeedPercentage / 5 * 5;
		if (Status::targetSpeedPercentage == 0) {
			Status::targetSpeedPercentage = 5;
		}
		Status::motorStatus = Status::MotorStatus::Running;
	}
}

// 注册用户控制回调函数
void RegisterUserControlCallbacks(void) {
	ec11Button.RegisterShortPressCallback(EC11ShortPressCallback);
	ec11Button.RegisterLongPressCallback(EC11LongPressCallback);
	ec11.RegisterClockwiseCallback(EC11ClockwiseCallback);
	ec11.RegisterCounterClockwiseCallback(EC11CounterClockwiseCallback);
	motorButton.RegisterShortPressCallback(MotorButtonShortPressCallback);
}
