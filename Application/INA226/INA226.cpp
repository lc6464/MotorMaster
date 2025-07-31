#include "INA226.h"

INA226::Status INA226::Init() {
	// 根据设计目标计算配置
	constexpr auto avgMode = AvgMode::AVG_16;
	constexpr auto vbusConvTime = ConvTime::CT_588us;
	constexpr auto vshuntConvTime = ConvTime::CT_1_1ms;
	constexpr auto opMode = OpMode::ShuntAndBusContinuous;

	const uint16_t configValue = (static_cast<uint16_t>(avgMode) << 9) |
		(static_cast<uint16_t>(vbusConvTime) << 6) |
		(static_cast<uint16_t>(vshuntConvTime) << 3) |
		static_cast<uint16_t>(opMode);

	// 计算校准值
	_currentLsb = MAX_EXPECTED_CURRENT / 32768.0f;

	const uint16_t calibrationValue = static_cast<uint16_t>(0.00512f / (_currentLsb * SHUNT_RESISTANCE));

	_powerLsb = _currentLsb * 25.0f;

	// 写入配置和校准寄存器
	if (_WriteRegister(Register::Config, configValue) != Status::OK) {
		return Status::ErrorWrite;
	}
	if (_WriteRegister(Register::Calibration, calibrationValue) != Status::OK) {
		return Status::ErrorWrite;
	}

	return Status::OK;
}

INA226::Status INA226::Update() {
	auto rawVoltage = _ReadRegister(Register::BusVoltage);
	if (!rawVoltage) {
		_lastVoltage = std::nullopt;
		_lastCurrent = std::nullopt;
		_lastPower = std::nullopt;
		return Status::ErrorRead;
	}

	auto rawCurrent = _ReadRegister(Register::Current);
	if (!rawCurrent) {
		_lastVoltage = std::nullopt;
		_lastCurrent = std::nullopt;
		_lastPower = std::nullopt;
		return Status::ErrorRead;
	}

	auto rawPower = _ReadRegister(Register::Power);
	if (!rawPower) {
		_lastVoltage = std::nullopt;
		_lastCurrent = std::nullopt;
		_lastPower = std::nullopt;
		return Status::ErrorRead;
	}

	_lastVoltage = static_cast<float>(*rawVoltage) * 0.00125f;

	// 电流寄存器是有符号的，所以需要转换
	_lastCurrent = static_cast<float>(static_cast<int16_t>(*rawCurrent)) * _currentLsb;

	_lastPower = static_cast<float>(*rawPower) * _powerLsb;

	return Status::OK;
}

std::optional<float> INA226::GetVoltage() const {
	return _lastVoltage;
}

std::optional<float> INA226::GetCurrent() const {
	return _lastCurrent;
}

std::optional<float> INA226::GetPower() const {
	return _lastPower;
}

INA226::Status INA226::_WriteRegister(Register reg, uint16_t value) {
	uint8_t data[2] = {
		static_cast<uint8_t>((value >> 8) & 0xFF), // Big-endian MSB
		static_cast<uint8_t>(value & 0xFF)         // Big-endian LSB
	};

	auto status = HAL_I2C_Mem_Write(_i2cHandle, _deviceAddress, static_cast<uint16_t>(reg), I2C_MEMADD_SIZE_8BIT, data, sizeof(data), I2C_TIMEOUT_MS);
	return (status == HAL_OK) ? Status::OK : Status::ErrorWrite;
}

std::optional<uint16_t> INA226::_ReadRegister(Register reg) {
	uint8_t readBuffer[2]{};

	// 从芯片读取2个字节
	auto status = HAL_I2C_Mem_Read(_i2cHandle, _deviceAddress, static_cast<uint16_t>(reg), I2C_MEMADD_SIZE_8BIT, readBuffer, sizeof(readBuffer), I2C_TIMEOUT_MS);
	if (status != HAL_OK) {
		return std::nullopt;
	}

	// 将接收到的字节（大端序）组合成一个16位值
	uint16_t value = (static_cast<uint16_t>(readBuffer[0]) << 8) | readBuffer[1];
	return value;
}