
#include "lora_Task.h"
#include "cmsis_os.h"

/* lora_Task function */
void lora_Task(void const * argument)
{
  /* USER CODE BEGIN lora_Task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END lora_Task */
}