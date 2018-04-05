import sys,len_sys,len_contr,len_error,len_mqtt

def MQTT_callback():
	callback_val = len_sys.getCallbackValue()
	print "MQTT callback ip:",callback_val[0]," port:",callback_val[1]," topic:",callback_val[2]," message:",callback_val[3]
	len_sys.ClearCallback()

print "\n\rStart"

#Ждем подключения
while (len_mqtt.getStatus() != len_mqtt.MQTT_CONNECTED):
	print "MQTT wait"
	sys.wait(1000)

print "MQTT connected"

#Подписаться на топик
len_mqtt.subTopic("lenp_sub")

#Добавить callback
len_sys.addCallback(MQTT_callback)

while 1:
	len_mqtt.pubMessage("lenp_pub","from controller")
	sys.wait(1000)