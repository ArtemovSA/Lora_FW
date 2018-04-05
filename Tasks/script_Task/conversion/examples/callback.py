import sys, len_sys, len_digIO, len_contr, len_error

def thread_out():
	while 1:
		len_digIO.setPwmOut(len_contr.DEV_CTRL,0,1,30)
		sys.wait(500)
		len_digIO.setPwmOut(len_contr.DEV_CTRL,0,1,100)
		sys.wait(500)

def thread_in():
	num = 0
	while 1:
		sys.wait(300)
		state1 = len_digIO.getDigIn(len_contr.DEV_CTRL, 0, 0)
		counter1 = len_digIO.getCounterIn(len_contr.DEV_CTRL, 0, 0)
		state2 = len_digIO.getDigIn(len_contr.DEV_CTRL, 0, 1)
		counter2 = len_digIO.getCounterIn(len_contr.DEV_CTRL, 0, 1)
		num += 1
		len_digIO.setPwmOut(len_contr.DEV_CTRL,0,1,counter2)
		print "Times:",num," State1:",state1," Counter1:",counter1," State2:",state2," Counter2:",counter2

def GPIO_callback():
	callback_val = len_sys.getCallbackValue()
	print "GPIO callback from:",callback_val[0]," addr:",callback_val[1]," source:",callback_val[2]," val:",callback_val[3]
	len_sys.ClearCallback()

len_digIO.setModeIn(len_contr.DEV_CTRL, 0, 0, len_digIO.IN_CNT_R_MODE)
len_digIO.setModeIn(len_contr.DEV_CTRL, 0, 1, len_digIO.IN_CNT_RF_MODE)
len_digIO.setModeOut(len_contr.DEV_CTRL,0,1,len_digIO.PWM_MODE)

#sys.runInThread(thread_out)
sys.runInThread(thread_in)
len_sys.addCallback(GPIO_callback)

print "\n\rScript start"

while 1:
	print "                                                                                                         Heap:",sys.heap()
	sys.wait(1000)