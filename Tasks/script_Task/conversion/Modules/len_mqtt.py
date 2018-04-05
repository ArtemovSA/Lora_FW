#len_mqtt.py
#
#Автор: Артемов С.А.
#Дата: 18.05.2017
#Версия: 1.0
#Описание: Модуль работы с MQTT
#Зависит от модулей:

import len_error

#Каналы MQTT
MQTT_ETH = 0
MQTT_GPRS = 1

#Статус подключения
MQTT_TCP_CONNECT_OK = 5
MQTT_TCP_DISCONNECT = 6

#Установить IP и порт
def setIPport_nat(ip, port):
	"""__NATIVE__
	PmReturn_t retval = PM_RET_OK;
	PI_cmd_return cmd_exec;
	char ip_str[15];
	char port_str[5];
	uint16_t ip_len, port_len;
	
	//Getting values
     py_get_string(0, ip_str, &ip_len); //Get string value by index
     py_get_string(1, port_str, &port_len); //Get string value by index
     
	//Set ip
	cmd_exec = PI_mqtt_SetIPport(ip_str, port_str);
	
	if (py_error_check(cmd_exec, 0, 0, 0))
		retval = PM_RET_ERR;
	
	NATIVE_SET_TOS(PM_NONE);
	return retval;
	"""
	pass

def setIPport(ip, port):
	val = setIPport_nat(ip, port)
	len_error.error_proc()
	return val	

#Получить IP и порт
def getIPport():
	"""__NATIVE__
	PmReturn_t retval = PM_RET_OK;
	pPmObj_t pl;
	char ip_str[15];
	char port_str[5];
     
	//Get ip
	PI_mqtt_GetIPport(ip_str, port_str);
	
	retval = list_new(&pl);
	py_add_str_toList(ip_str, &pl);
	py_add_str_toList(port_str, &pl);
	
    	NATIVE_SET_TOS(pl);

	return retval;
	"""
	pass	

#Установить логин и пароль
def setLoginPass_nat(login, passw):
	"""__NATIVE__
	PmReturn_t retval = PM_RET_OK;
	PI_cmd_return cmd_exec;
	char login[50];
	char pass[50];
	uint16_t login_len, pass_len;
	
	//Getting values
     py_get_string(0, login, &login_len); //Get string value by index
     py_get_string(1, pass, &pass_len); //Get string value by index
     
	//Set ip
	cmd_exec = PI_mqtt_SetLoginPass(login, pass);
	
	if (py_error_check(cmd_exec, 0, 0, 0))
		retval = PM_RET_ERR;
	
	NATIVE_SET_TOS(PM_NONE);
	return retval;
	"""
	pass

def setLoginPass(login, passw):
	val = setLoginPass_nat(login, passw)
	len_error.error_proc()
	return val	

#Получить логин и пароль
def getLoginPass():
	"""__NATIVE__
	PmReturn_t retval = PM_RET_OK;
	pPmObj_t pl;
	char login[50];
	char pass[50];
     
	//Get ip
	PI_mqtt_GetLoginPass(login, pass);

	retval = list_new(&pl);
	py_add_str_toList(login, &pl);
	py_add_str_toList(pass, &pl);
	
    	NATIVE_SET_TOS(pl);
	return retval;
	"""
	pass	

#Установить канал по умолчанию
def setChannel_nat(channel):
	"""__NATIVE__
	PmReturn_t retval = PM_RET_OK;
	PI_cmd_return cmd_exec;
	
	uint8_t channel = py_get_uint8_t(0);

	//SetChannel
	cmd_exec = PI_mqtt_setChannel(channel);
	
	if (py_error_check(cmd_exec, 0, 0, 0))
		retval = PM_RET_ERR;
	
	NATIVE_SET_TOS(PM_NONE);
	return retval;
	"""
	pass

def setChannel(channel):
	val = setChannel_nat(channel)
	len_error.error_proc()
	return val	

#Получить текущий канала
def getChannel():
	"""__NATIVE__
	PmReturn_t retval = PM_RET_OK;
	pPmObj_t channel_obj;
	uint8_t channel;

	PI_mqtt_getChannel(&channel);
	
	retval = int_new(channel, &channel_obj);
	NATIVE_SET_TOS(channel_obj);
	return retval;
	"""
	pass

#Отправить сообщение в топик
def pubMessage_nat(topic,message):
	"""__NATIVE__
	PmReturn_t retval = PM_RET_OK;
	PI_cmd_return cmd_exec;
	char topic[20];
	uint16_t topic_len, message_len;

	memset(topic,0,sizeof(topic));

	//Getting values
     py_get_string(0, topic, &topic_len); //Get string value by index
     py_get_string(1, str_buf, &message_len); //Get string value by index

     cmd_exec = PI_mqtt_pubMessage(topic, str_buf);

     if (py_error_check(cmd_exec, 0, 0, 0))
		retval = PM_RET_ERR;

	NATIVE_SET_TOS(PM_NONE);
	return retval;
	"""
	pass
	
def pubMessage(topic,message):
	val = pubMessage_nat(topic,message)
	len_error.error_proc()
	return val

#Подписаться на топик
def subTopic_nat(topic):
	"""__NATIVE__
	PmReturn_t retval = PM_RET_OK;
	PI_cmd_return cmd_exec;
	char topic[20];
	uint16_t topic_len;

	memset(topic,0,sizeof(topic));
	
	//Getting values
     py_get_string(0, topic, &topic_len); //Get string value by index

     cmd_exec = PI_mqtt_subTopic(topic);

     if (py_error_check(cmd_exec, 0, 0, 0))
		retval = PM_RET_ERR;

	NATIVE_SET_TOS(PM_NONE);
	return retval;
	"""
	pass

def subTopic(topic):
	val = subTopic_nat(topic)
	len_error.error_proc()
	return val

#Получить статус подключения
def getStatus():
	"""__NATIVE__
	PmReturn_t retval = PM_RET_OK;
	pPmObj_t pi;

	retval = int_new(MQTT_state.MQTT_status, &pi);	

	NATIVE_SET_TOS(pi);
	return retval;
	"""
	pass