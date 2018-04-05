#len_sys.py
#
#Автор: Артемов С.А.
#Дата: 24.04.2017
#Версия: 1.0
#Описание: Модуль работы с ситемными функциями
#Зависит от модулей:

"""__NATIVE__
#include "stdint.h"
#include "Perefery.h"

char str_buf[255]; //Global buffer

//Get uint8_t value by index
uint8_t py_get_uint8_t(uint8_t id) {
	pPmObj_t val_in;
	val_in = NATIVE_GET_LOCAL(id);
	return ((pPmInt_t)val_in)->val;
}

//Get float value by index
float py_get_float(uint8_t id) {
	pPmObj_t val_in;
	val_in = NATIVE_GET_LOCAL(id);
	return ((pPmFloat_t)val_in)->val;
}

//Get string value by index
void py_get_string(uint8_t id, char *str, uint16_t *len) {
	pPmObj_t val_in;
	
	val_in = NATIVE_GET_LOCAL(id);
	memset(str,0,sizeof(str));
	memcpy(str, ((pPmString_t)val_in)->val, ((pPmString_t)val_in)->length);
	*len = ((pPmString_t)val_in)->length;
}

//Get function name
PmReturn_t py_get_functionName(char *name) {
	pPmObj_t pframe;
	pPmObj_t pstr;
	PmReturn_t retval = PM_RET_OK;
	
	/* Get the top frame */
	pframe = (pPmObj_t)gVmGlobal.pthread->pframe;
	
	retval = tuple_getItem((pPmObj_t)((pPmFrame_t)pframe)->fo_func->f_co->co_names, -1, &pstr);	
	sprintf(name,"%s",((pPmString_t)pstr)->val);

	return retval;
}

//Add int to list
PmReturn_t py_add_int_toList(int val, pPmObj_t *list) {
	pPmObj_t pi;
	PmReturn_t retval = PM_RET_OK;

	retval = int_new(val, &pi);
	if (retval == PM_RET_OK)
		retval = list_append(*list, pi);

	return retval;
}

//Add string to list
PmReturn_t py_add_str_toList(char* str, pPmObj_t *list) {
	pPmObj_t ps;
	PmReturn_t retval = PM_RET_OK;

	retval = string_new(str, &ps);

	if (retval == PM_RET_OK)
		retval = list_append(*list, ps);

	return retval;
}


//Check error code
uint8_t py_error_check(PI_cmd_return cmd_exec, uint8_t type_dev, uint8_t adr_sw, uint8_t adr_port) {
	if (cmd_exec != PI_CMD_OK) {
		pPmObj_t pframe;
		pPmObj_t pstr;
		
		py_error.type_dev = type_dev;
		py_error.adr_sw = adr_sw;
		py_error.adr_port = adr_port;
		py_error.error_code = cmd_exec;

		/* Get the top frame */
		pframe = (pPmObj_t)gVmGlobal.pthread->pframe;
		tuple_getItem((pPmObj_t)((pPmFrame_t)pframe)->fo_func->f_co->co_names, -1, &pstr);	
		memcpy(py_error.thread_name,((pPmString_t)pstr)->val,sizeof(py_error.thread_name));
		
		return 1;
	}else{
		return 0;
	}
}
"""

ER_OK = 0        #Комманда выполнена
ER_ERROR = 1        #Неизвестная ошибка
ER_DEV_TYPE = 2 #Ошибка типа устройства
ER_MODE = 3    #Ошибка режима устройства
ER_SOURCE = 4  #Ошибка источника
ER_BUS = 5     #Ошибка шины
ER_VALUE = 6    #Ошибка значения

#Задержка на ms
def delay_ms(ms):
	"""__NATIVE__
	PmReturn_t retval = PM_RET_OK;
	pPmObj_t ms_val;
	ms_val = NATIVE_GET_LOCAL(0);
	vTaskDelay(((pPmInt_t)ms_val)->val);
	NATIVE_SET_TOS(PM_NONE);
	return retval;
     """
	pass

#Остановить поток
def kill_Thread():
	"""__NATIVE__
	PmReturn_t retval = PM_RET_EX;
	return retval;
	"""

#Получить имя потока
def func_name():
	"""__NATIVE__
	char buf[30];
	pPmObj_t ps;
	pPmObj_t pframe;
	pPmObj_t pstr;
	
	PmReturn_t retval = PM_RET_OK;
	
	/* Get the top frame */
	pframe = (pPmObj_t)gVmGlobal.pthread->pframe;
	
	retval = tuple_getItem((pPmObj_t)((pPmFrame_t)pframe)->fo_func->f_co->co_names, -1, &pstr);	
	sprintf(buf,"%s",((pPmString_t)pstr)->val);	
	if (retval == PM_RET_OK) {
		retval = string_new(buf, &ps);
    		NATIVE_SET_TOS(ps);
    	}
    	
	return retval;
	"""
	pass

#Получить описание ошибки
def get_error():
	"""__NATIVE__
	PmReturn_t retval = PM_RET_OK;
	pPmObj_t pl;
	uint8_t const *str = (uint8_t const *)py_error.thread_name;

	uint8_t const *pstr = str;
	
	retval = list_new(&pl);
	py_add_int_toList(py_error.type_dev, &pl);
	py_add_int_toList(py_error.adr_sw, &pl);
	py_add_int_toList(py_error.adr_port, &pl);
	py_add_int_toList(py_error.error_code, &pl);
	py_add_str_toList((char*)pstr, &pl);

	NATIVE_SET_TOS(pl);
	return retval;
	"""
	pass

#Разрешить callback
def addCallback(func):
	"""__NATIVE__
	PmReturn_t retval = PM_RET_OK;
	pPmObj_t pf;
	pPmObj_t ps;
	char buf[20];

	/* If wrong number of args, raise TypeError */
	if (NATIVE_GET_NUM_ARGS() != 1)
	{
	   PM_RAISE(retval, PM_RET_EX_TYPE);
	   return retval;
	}

	//Check type
	pf = NATIVE_GET_LOCAL(0);
	if (OBJ_GET_TYPE(pf) != OBJ_TYPE_FXN)
	{
	   PM_RAISE(retval, PM_RET_EX_TYPE);
	   return retval;
	}

	//Get function name
	retval = tuple_getItem((pPmObj_t)((pPmFunc_t)pf)->f_co->co_names, -1, &ps);	
	sprintf(buf,"%s",((pPmString_t)ps)->val);	

	//Search function by name
	for (int i=0; i<PY_COUNT_CALLBACKS; i++) {
		if (strstr(py_callback[i].callback_name, buf) !=0){
			py_callback[i].py_func = pf; //Record function
			py_callback[i].callback_status = PY_CALLBACK_STOP; //Record status
			NATIVE_SET_TOS(PM_NONE);
			retval = PM_RET_OK;
			return retval;
		}
	}
	
	NATIVE_SET_TOS(PM_NONE);
	retval = PM_RET_NO;
	return retval;
	"""
	pass

#Очистить выполнение callback
def ClearCallback():
	"""__NATIVE__
	PmReturn_t retval = PM_RET_OK;
	char name[20];
	
	//Get function name
     py_get_functionName(name); 	

	//Search function by name
	for (int i=0; i<PY_COUNT_CALLBACKS; i++) {
		if (strstr(py_callback[i].callback_name, name) !=0){
			py_callback[i].callback_status = PY_CALLBACK_STOP; //Record status
			NATIVE_SET_TOS(PM_NONE);
			retval = PM_RET_OK;
			return retval;
		}
	}
	
	NATIVE_SET_TOS(PM_NONE);
	retval = PM_RET_NO;
	return retval;
	"""
	pass

#Получить значение callback
def getCallbackValue():
	"""__NATIVE__
	PmReturn_t retval = PM_RET_OK;
	char name[20];
	pPmObj_t pl;
	
	//Get function name
     py_get_functionName(name); 	

     //Search function by name
	for (int i=0; i<PY_COUNT_CALLBACKS; i++) {
		if (strstr(py_callback[i].callback_name, name) !=0){
			retval = list_new(&pl);

			//Make params
			switch(i) {
			case PY_SMS_CALLBACK:
				break;
			case PY_MQTT_CALLBACK:
				py_add_str_toList(((py_var_MQTT_CALLBACK_type *)((py_var_GPIO_CALLBACK_type*)py_callback->callback_val))->ip_addr, &pl);
				py_add_str_toList(((py_var_MQTT_CALLBACK_type *)((py_var_GPIO_CALLBACK_type*)py_callback->callback_val))->port, &pl);
				py_add_str_toList(((py_var_MQTT_CALLBACK_type *)((py_var_GPIO_CALLBACK_type*)py_callback->callback_val))->topic, &pl);
				py_add_str_toList(((py_var_MQTT_CALLBACK_type *)((py_var_GPIO_CALLBACK_type*)py_callback->callback_val))->message, &pl);
				py_add_int_toList(((py_var_MQTT_CALLBACK_type *)((py_var_GPIO_CALLBACK_type*)py_callback->callback_val))->len, &pl);
				break;
			case PY_GPIO_CALLBACK:
				py_add_int_toList(((py_var_GPIO_CALLBACK_type *)((py_var_GPIO_CALLBACK_type*)py_callback->callback_val))->type_dev, &pl);
				py_add_int_toList(((py_var_GPIO_CALLBACK_type *)((py_var_GPIO_CALLBACK_type*)py_callback->callback_val))->adr_sw, &pl);
				py_add_int_toList(((py_var_GPIO_CALLBACK_type *)((py_var_GPIO_CALLBACK_type*)py_callback->callback_val))->adr_port, &pl);
				py_add_int_toList(((py_var_GPIO_CALLBACK_type *)((py_var_GPIO_CALLBACK_type*)py_callback->callback_val))->value, &pl);			
				break;
			case PY_DISPLAY_CALLBACK:
				break;
			case PY_MODEM_CALLBACK:
				break;
			case PY_PY_BLUETOOTH_CALLBACK:
				break;
			case PY_RF_CALLBACK:
				break;
			}
			
			NATIVE_SET_TOS(pl);
			retval = PM_RET_OK;
			return retval;
		}
	}
	
	NATIVE_SET_TOS(PM_NONE);
	retval = PM_RET_NO;
	return retval;
	"""
	pass