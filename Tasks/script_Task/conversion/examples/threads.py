import sys, len_sys, len_digIO, len_contr, len_error

def thread_out():
	while 1:
		len_digIO.setPwmOut(len_contr.DEV_CTRL,0,1,30)
		sys.wait(500)
		len_digIO.setPwmOut(len_contr.DEV_CTRL,0,1,100)
		sys.wait(500)

def thread_in():
	while 1:
		sys.wait(300)
		state1 = len_digIO.getDigIn(len_contr.DEV_CTRL, 0, 0)
		counter1 = len_digIO.getCounterIn_er(len_contr.DEV_CTRL, 0, 0)
		state2 = len_digIO.getDigIn(len_contr.DEV_CTRL, 0, 1)
		counter2 = len_digIO.getCounterIn(len_contr.DEV_CTRL, 0, 1)
		num += 1
		print "Times:",num," State1:",state1," Counter1:",counter1," State2:",state2," Counter2:",counter2

len_digIO.setModeIn(len_contr.DEV_CTRL, 0, 0, len_digIO.IN_CNT_R_MODE)
len_digIO.setModeIn(len_contr.DEV_CTRL, 0, 1, len_digIO.IN_CNT_RF_MODE)
len_digIO.setModeOut(len_contr.DEV_CTRL,0,1,len_digIO.PWM_MODE)

num = 0

sys.runInThread(thread_out)
sys.runInThread(thread_in)

print "Script start"

while 1:
	sys.wait(1000)