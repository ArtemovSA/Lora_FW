
import len_sys

ER_OK = 0        #Комманда выполнена
ER_ERROR = 1     #Неизвестная ошибка
ER_DEV_TYPE = 2  #Ошибка типа устройства
ER_MODE = 3    	 #Ошибка режима устройства
ER_SOURCE = 4    #Ошибка источника
ER_BUS = 5       #Ошибка шины
ER_VALUE = 6     #Ошибка значения

#Обработать ошибку если она есть
def error_proc():
	error = len_sys.get_error() #Получить ошибку
	error_code = error[3]
	if (error_code != ER_OK):
		type_dev = error[0]
		adr_sw = error[1]
		adr_port = error[2]
		thread_name = error[4]
		print "Error code:",error_code
		print "Name:", error[4]
		print "Thread kill"
		len_sys.delay_ms(100)
		len_sys.kill_Thread() #Остановить поток