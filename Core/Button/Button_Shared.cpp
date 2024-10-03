#include "Button_Shared.hpp"
#include "EC11_Shared.hpp"

static const PortPinPair displayButtonPair(Key_Display_GPIO_Port, Key_Display_Pin); // 显示控制按钮
static const PortPinPair motorButtonPair(Key_Motor_GPIO_Port, Key_Motor_Pin); // 确认按钮

Button displayButton(displayButtonPair);
Button motorButton(motorButtonPair);
Button &ec11Button = ec11.GetButton();
