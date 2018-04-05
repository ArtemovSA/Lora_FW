
#include "ctrl_Task.h"
#include "cmsis_os.h"
#include "usb_device.h"
#include "Device_ctrl.h"

/* ctrl_Task function */
void ctrl_Task(void const * argument)
{
  /* init code for USB_DEVICE */
  MX_USB_DEVICE_Init();
  
  DC_init(); //Device init
  
  DC_ledBlink(1000, 10);

  /* USER CODE BEGIN 5 */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END 5 */ 
}
