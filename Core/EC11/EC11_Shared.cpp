#include "EC11_Shared.hpp"

static const PortPinPair encoderPair(Encoder_SW_GPIO_Port, Encoder_SW_Pin);

EC11 ec11(htim4, encoderPair);