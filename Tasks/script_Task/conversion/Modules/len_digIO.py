#len_digIO.py
#
#Автор: Артемов С.А.
#Дата: 21.04.2017
#Версия: 1.0
#Описание: Модуль работы с цифровыми входами и цифровыми выходами с режимом шиммирования.
#Адресация: [Тип устройства][Адрес свитча][Адрес порта]
#Зависит от модулей:

#Список режимов выходов
PWM_MODE = 0
OUT_MODE = 1
IN_MODE = 0
IN_CNT_R_MODE = 1
IN_CNT_F_MODE = 2
IN_CNT_RF_MODE = 3

#Установить режим выхода
def setModeOut_nat(type_dev, adr_sw, adr_port, mode):
	"""__NATIVE__
	PmReturn_t retval = PM_RET_OK;
	PI_cmd_return cmd_exec;
	
	//Getting values
	uint8_t type_dev = py_get_uint8_t(0);
	uint8_t adr_sw = py_get_uint8_t(1);
	uint8_t adr_port = py_get_uint8_t(2);
	uint8_t mode = py_get_uint8_t(3);
	
	//Set mode
	cmd_exec = PI_digitalOut_mode_set(type_dev,adr_sw,adr_port, mode);

	if (py_error_check(cmd_exec, type_dev, adr_sw, adr_port))
		retval = PM_RET_ERR;
	
	NATIVE_SET_TOS(PM_NONE);
	return retval;
	"""
	pass

def setModeOut(type_dev, adr_sw, adr_port, mode):
	val = setModeOut_nat(type_dev, adr_sw, adr_port, mode)
	len_error.error_proc()
	return val
	
#Установить значения PWM выхода в %
def setPwmOut_nat(type_dev, adr_sw, adr_port, val):
	"""__NATIVE__
	PmReturn_t retval = PM_RET_OK;
	PI_cmd_return cmd_exec;
	
	//Getting values
	uint8_t type_dev = py_get_uint8_t(0);
	uint8_t adr_sw = py_get_uint8_t(1);
	uint8_t adr_port = py_get_uint8_t(2);
	uint8_t value = py_get_uint8_t(3);
	
	//Read value
	cmd_exec = PI_digitalOut_PWM_set(type_dev,adr_sw,adr_port, value);
	
	if (py_error_check(cmd_exec, type_dev, adr_sw, adr_port))
		retval = PM_RET_ERR;
	
	NATIVE_SET_TOS(PM_NONE);
	return retval;
	"""
	pass

def setPwmOut(type_dev, adr_sw, adr_port, val):
	val = setPwmOut_nat(type_dev, adr_sw, adr_port, val)
	len_error.error_proc()
	return val
	
#Получить значение PWM на выходе
def getPwmOut_nat(type_dev, adr_sw, adr_port):
	"""__NATIVE__
	PmReturn_t retval = PM_RET_OK;
	PI_cmd_return cmd_exec;
	pPmObj_t val_obj;
	
	//Getting values
	uint8_t type_dev = py_get_uint8_t(0);
	uint8_t adr_sw = py_get_uint8_t(1);
	uint8_t adr_port = py_get_uint8_t(2);
	uint8_t value;
	
	//Read value
	cmd_exec = PI_digitalOut_PWM_get(type_dev,adr_sw,adr_port, &value);
	
	if (py_error_check(cmd_exec, type_dev, adr_sw, adr_port))
		retval = PM_RET_ERR;
	
	//Convert object
	retval = int_new(value, &val_obj);
	NATIVE_SET_TOS(val_obj);
	return retval;
	"""
	pass

def getPwmOut(type_dev, adr_sw, adr_port):
	val = getPwmOut_nat(type_dev, adr_sw, adr_port)
	len_error.error_proc()
	return val
	
#Установить значения выхода
def setDigOut_nat(type_dev, adr_sw, adr_port, val):
	"""__NATIVE__
	PmReturn_t retval = PM_RET_OK;
	PI_cmd_return cmd_exec;
	
	//Getting values
	uint8_t type_dev = py_get_uint8_t(0);
	uint8_t adr_sw = py_get_uint8_t(1);
	uint8_t adr_port = py_get_uint8_t(2);
	uint8_t value = py_get_uint8_t(3);
	
	//Set value
	cmd_exec = PI_digitalOut_set(type_dev,adr_sw,adr_port, value);
	
	if (py_error_check(cmd_exec, type_dev, adr_sw, adr_port))
		retval = PM_RET_ERR;
	
	NATIVE_SET_TOS(PM_NONE);
	return retval;
	"""
	pass

def setDigOut(type_dev, adr_sw, adr_port, val):
	val = setDigOut_nat(type_dev, adr_sw, adr_port, val)
	len_error.error_proc()
	return val

#Получить значение на выходе
def getDigOut_nat(type_dev, adr_sw, adr_port):
	"""__NATIVE__
	PmReturn_t retval = PM_RET_OK;
	PI_cmd_return cmd_exec;
	pPmObj_t val_obj;
	
	//Getting values
	uint8_t type_dev = py_get_uint8_t(0);
	uint8_t adr_sw = py_get_uint8_t(1);
	uint8_t adr_port = py_get_uint8_t(2);
	uint8_t value;
	
	//Read value
	cmd_exec = PI_digitalOut_get(type_dev,adr_sw,adr_port, &value);
	
	if (py_error_check(cmd_exec, type_dev, adr_sw, adr_port))
		retval = PM_RET_ERR;
	
	//Convert object
	retval = int_new(value, &val_obj);
	NATIVE_SET_TOS(val_obj);
	return retval;
	"""
	pass

def getDigOut(type_dev, adr_sw, adr_port):
	val = getDigOut_nat(type_dev, adr_sw, adr_port)
	len_error.error_proc()
	return val

#Инвертировать значения выхода
def setInvOut_nat(type_dev, adr_sw, adr_port, val):
	"""__NATIVE__
	PmReturn_t retval = PM_RET_OK;
	PI_cmd_return cmd_exec;
	
	//Getting values
	uint8_t type_dev = py_get_uint8_t(0);
	uint8_t adr_sw = py_get_uint8_t(1);
	uint8_t adr_port = py_get_uint8_t(2);
	uint8_t value = py_get_uint8_t(3);
	
	//Inver value
	cmd_exec = PI_digitalOut_invert(type_dev,adr_sw,adr_port);
	
	if (py_error_check(cmd_exec, type_dev, adr_sw, adr_port))
		retval = PM_RET_ERR;
	
	NATIVE_SET_TOS(PM_NONE);
	return retval;
	"""
	pass

def setInvOut(type_dev, adr_sw, adr_port, val):
	val = setInvOut_nat(type_dev, adr_sw, adr_port, val)
	len_error.error_proc()
	return val

#Инициализация входа
def setModeIn_nat(type_dev, adr_sw, adr_port, mode):
	"""__NATIVE__
	PmReturn_t retval = PM_RET_OK;
	PI_cmd_return cmd_exec;
	
	//Getting values
	uint8_t type_dev = py_get_uint8_t(0);
	uint8_t adr_sw = py_get_uint8_t(1);
	uint8_t adr_port = py_get_uint8_t(2);
	uint8_t mode = py_get_uint8_t(3);

	//Set input mode
	cmd_exec = PI_digitalIn_mode_set(type_dev,adr_sw,adr_port,mode);

	if (py_error_check(cmd_exec, type_dev, adr_sw, adr_port))
		retval = PM_RET_ERR;
	
	NATIVE_SET_TOS(PM_NONE);
	return retval;
	"""
	pass

def setModeIn(type_dev, adr_sw, adr_port, mode):
	val = setModeIn_nat(type_dev, adr_sw, adr_port, mode)
	len_error.error_proc()
	return val

#Сбросить значение счетчика
def resCounterIn_nat(type_dev,adr_sw, adr_port):
	"""__NATIVE__
	PmReturn_t retval = PM_RET_OK;
	PI_cmd_return cmd_exec;
	
	//Getting values
	uint8_t type_dev = py_get_uint8_t(0);
	uint8_t adr_sw = py_get_uint8_t(1);
	uint8_t adr_port = py_get_uint8_t(2);

	//Reset counter value
	cmd_exec = PI_digitalIn_reset_counter(type_dev,adr_sw,adr_port);

	if (py_error_check(cmd_exec, type_dev, adr_sw, adr_port))
		retval = PM_RET_ERR;
	
	NATIVE_SET_TOS(PM_NONE);
	return retval;
	"""
	pass

def resCounterIn(type_dev,adr_sw, adr_port):
	val = resCounterIn_nat(type_dev,adr_sw, adr_port)
	len_error.error_proc()
	return val
	
#Получить значение входа
def getDigIn_nat(type_dev, adr_sw, adr_port):
	"""__NATIVE__
	PmReturn_t retval = PM_RET_OK;
	PI_cmd_return cmd_exec;
	pPmObj_t val_obj;
	
	//Getting values
	uint8_t type_dev = py_get_uint8_t(0);
	uint8_t adr_sw = py_get_uint8_t(1);
	uint8_t adr_port = py_get_uint8_t(2);
	uint8_t value;

	//Read value
	cmd_exec = PI_digitalIn_get(type_dev,adr_sw,adr_port, &value);
	
	if (py_error_check(cmd_exec, type_dev, adr_sw, adr_port))
		retval = PM_RET_ERR;
	
	//Convert object
	retval = int_new(value, &val_obj);
	NATIVE_SET_TOS(val_obj);
	return retval;
	"""
	pass
	
def getDigIn(type_dev, adr_sw, adr_port):
	val = getDigIn_nat(type_dev, adr_sw, adr_port)
	len_error.error_proc()
	return val

#Получить значение счетчика
def getCounterIn_nat(type_dev, adr_sw, adr_port):
	"""__NATIVE__
	PmReturn_t retval = PM_RET_OK;
	PI_cmd_return cmd_exec;
	pPmObj_t val_obj;
	uint32_t counter = 0;
	
	//Getting values
	uint8_t type_dev = py_get_uint8_t(0);
	uint8_t adr_sw = py_get_uint8_t(1);
	uint8_t adr_port = py_get_uint8_t(2);

	//Get counter value
	cmd_exec = PI_digitalIn_get_counter(type_dev,adr_sw,adr_port, &counter);

	py_error_check(cmd_exec, type_dev, adr_sw, adr_port);
	
	if (py_error_check(cmd_exec, type_dev, adr_sw, adr_port))
		retval = PM_RET_ERR;
	
	retval = int_new(counter, &val_obj);
	NATIVE_SET_TOS(val_obj);

	return retval;
	"""
	pass

def getCounterIn(type_dev, adr_sw, adr_port):	
	val = getCounterIn_nat(type_dev, adr_sw, adr_port)
	len_error.error_proc()
	return val