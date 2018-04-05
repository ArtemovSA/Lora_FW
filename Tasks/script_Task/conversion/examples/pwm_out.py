import sys, len_sys, len_digIO, len_contr, len_error

len_digIO.setModeOut(len_contr.DEV_CTRL,0,1,len_digIO.PWM_MODE)

while 1:
	len_digIO.setPwmOut(len_contr.DEV_CTRL,0,1,30)
	sys.wait(500)
	len_digIO.setPwmOut(len_contr.DEV_CTRL,0,1,100)
	sys.wait(500)