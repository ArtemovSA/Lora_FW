#len_adc.py
#
#Автор: Артемов С.А.
#Дата: 16.05.2017
#Версия: 1.0
#Описание: Модуль работы с АЦП
#Адресация: [Тип устройства][Адрес свитча][Адрес порта]
#Зависит от модулей:

import len_error

mode_4_20mA = 0
mode_0_10V = 1

#Получить значение АЦП
def getChVal_nat(type_dev, adr_sw, adr_port):
	"""__NATIVE__
	PmReturn_t retval = PM_RET_OK;
	PI_cmd_return cmd_exec;
	float val;
	pPmObj_t val_obj;
	
	//Getting values
	uint8_t type_dev = py_get_uint8_t(0);
	uint8_t adr_sw = py_get_uint8_t(1);
	uint8_t adr_port = py_get_uint8_t(2);
	
	//Get value
	cmd_exec = PI_getChVal(type_dev,adr_sw,adr_port, &val);
	
	if (py_error_check(cmd_exec, type_dev, adr_sw, adr_port))
		retval = PM_RET_ERR;
	
	//Convert object
	retval = float_new(val,&val_obj);
	NATIVE_SET_TOS(val_obj);
	return retval;
	"""
	pass

def getChVal(type_dev, adr_sw, adr_port):
	val = getChVal_nat(type_dev, adr_sw, adr_port)
	len_error.error_proc()
	return val


#Установить режим канала
def setChMode_nat(type_dev, adr_sw, adr_port, mode):
	"""__NATIVE__
	PmReturn_t retval = PM_RET_OK;
	PI_cmd_return cmd_exec;
	
	//Getting values
	uint8_t type_dev = py_get_uint8_t(0);
	uint8_t adr_sw = py_get_uint8_t(1);
	uint8_t adr_port = py_get_uint8_t(2);
	uint8_t mode = py_get_uint8_t(3);
	
	//Set mode
	cmd_exec = PI_setChMode(type_dev,adr_sw,adr_port, mode);

	if (py_error_check(cmd_exec, type_dev, adr_sw, adr_port))
		retval = PM_RET_ERR;
	
	NATIVE_SET_TOS(PM_NONE);
	return retval;
	"""
	pass

def setChMode(type_dev, adr_sw, adr_port, mode):
	val = setChMode_nat(type_dev, adr_sw, adr_port, mode)
	len_error.error_proc()
	return val

#Установить значение смещения
def setChOffset_nat(type_dev, adr_sw, adr_port, offset):
	"""__NATIVE__
	PmReturn_t retval = PM_RET_OK;
	PI_cmd_return cmd_exec;
	
	//Getting values
	uint8_t type_dev = py_get_uint8_t(0);
	uint8_t adr_sw = py_get_uint8_t(1);
	uint8_t adr_port = py_get_uint8_t(2);
	float offset = py_get_float(3);
	
	//Set offset
	cmd_exec = PI_setChOffset(type_dev,adr_sw,adr_port, offset);

	if (py_error_check(cmd_exec, type_dev, adr_sw, adr_port))
		retval = PM_RET_ERR;
	
	NATIVE_SET_TOS(PM_NONE);
	return retval;
	"""
	pass

def setChOffset(type_dev, adr_sw, adr_port, offset):
	val = setChOffset_nat(type_dev, adr_sw, adr_port, offset)
	len_error.error_proc()
	return val

#Получить значение смещения
def getChOffset_nat(type_dev, adr_sw, adr_port):
	"""__NATIVE__
	PmReturn_t retval = PM_RET_OK;
	PI_cmd_return cmd_exec;
	float val;
	pPmObj_t val_obj;
	
	//Getting values
	uint8_t type_dev = py_get_uint8_t(0);
	uint8_t adr_sw = py_get_uint8_t(1);
	uint8_t adr_port = py_get_uint8_t(2);
	
	//Get offset
	cmd_exec = PI_getChOffset(type_dev,adr_sw,adr_port, &val);

	if (py_error_check(cmd_exec, type_dev, adr_sw, adr_port))
		retval = PM_RET_ERR;
	
	//Convert object
	retval = float_new(val,&val_obj);
	NATIVE_SET_TOS(val_obj);
	return retval;
	"""
	pass

def getChOffset(type_dev, adr_sw, adr_port):
	val = getChOffset_nat(type_dev, adr_sw, adr_port)
	len_error.error_proc()
	return val

#Установить значение коэффициента
def setChKoef_nat(type_dev, adr_sw, adr_port, koef):
	"""__NATIVE__
	PmReturn_t retval = PM_RET_OK;
	PI_cmd_return cmd_exec;
	
	//Getting values
	uint8_t type_dev = py_get_uint8_t(0);
	uint8_t adr_sw = py_get_uint8_t(1);
	uint8_t adr_port = py_get_uint8_t(2);
	float koef = py_get_float(3);
	
	//Set koef
	cmd_exec = PI_setChKoef(type_dev,adr_sw,adr_port, koef);

	if (py_error_check(cmd_exec, type_dev, adr_sw, adr_port))
		retval = PM_RET_ERR;
	
	NATIVE_SET_TOS(PM_NONE);
	return retval;
	"""
	pass

def setChKoef(type_dev, adr_sw, adr_port, koef):
	val = setChKoef_nat(type_dev, adr_sw, adr_port, koef)
	len_error.error_proc()
	return val

#Получить значение коэффициента
def getChKoef_nat(type_dev, adr_sw, adr_port):
	"""__NATIVE__
	PmReturn_t retval = PM_RET_OK;
	PI_cmd_return cmd_exec;
	float val;
	pPmObj_t val_obj;
	
	//Getting values
	uint8_t type_dev = py_get_uint8_t(0);
	uint8_t adr_sw = py_get_uint8_t(1);
	uint8_t adr_port = py_get_uint8_t(2);
	
	//Get koefficient
	cmd_exec = PI_getChKoef(type_dev,adr_sw,adr_port, &val);

	if (py_error_check(cmd_exec, type_dev, adr_sw, adr_port))
		retval = PM_RET_ERR;
	
	//Convert object
	retval = float_new(val,&val_obj);
	NATIVE_SET_TOS(val_obj);
	return retval;
	"""
	pass
	
def getChKoef(type_dev, adr_sw, adr_port):
	val = getChKoef_nat(type_dev, adr_sw, adr_port)
	len_error.error_proc()
	return val