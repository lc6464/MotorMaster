#include "WatchDog_Shared.h"

#include "Status.h"

WatchDog speedChangeRateWatchDog(5000, []() {
	// 超时回调函数：将速度切换倍率设为 x0
	Status::speedChangeRate = Status::SpeedChangeRate::X0;
	});