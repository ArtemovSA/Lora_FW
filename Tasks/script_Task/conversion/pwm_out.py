import len_controller, len_system, len_digital_io

len_digital_io.set_mode_out(len_controller.DEV_CONTROLLER,0,0,len_digital_io.PWM_MODE)

while 1:
	len_digital_io.set_pwm_out(len_controller.DEV_CONTROLLER,0,0,50)
	len_system.delay_ms(1000)
	len_digital_io.set_pwm_out(len_controller.DEV_CONTROLLER,0,0,100)
	len_system.delay_ms(1000)