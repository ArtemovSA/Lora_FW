
#include "script_Task.h"
#include "py_usr.h"
#include "cmsis_os.h"

TimerHandle_t Py_timer; //������
xTaskHandle script_handle; //����� �������
xQueueHandle queue_script_commands; //������� �������
uint8_t py_task; //���������� ������ �������
uint32_t py_script_addr; //����� ������� �� flash
uint32_t py_script_struct_addr; //����� �������� ������� �� flash
py_script_type py_main_script; //�������� ������
py_error_type py_error; //������ �������
uint8_t py_callback_queue[PY_COUNT_CALLBACKS]; //������� callback
uint8_t py_callback_queue_len; //����� �������

//���������� ���������� ������������ � callback
py_var_SMS_CALLBACK_type py_var_SMS_CALLBACK;
py_var_MQTT_CALLBACK_type py_var_MQTT_CALLBACK;
py_var_GPIO_CALLBACK_type py_var_GPIO_CALLBACK;
py_var_DISPLAY_CALLBACK_type py_var_DISPLAY_CALLBACK;
py_var_MODEM_CALLBACK_type py_var_MODEM_CALLBACK;
py_var_BLUETOOTH_CALLBACK_type py_var_BLUETOOTH_CALLBACK;
py_var_RF_CALLBACK_type py_var_RF_CALLBACK;

//��������� ������� Callback
py_callback_type py_callback[PY_COUNT_CALLBACKS] = {
  {0 , DEF_PY_SMS_CALLBACK, PY_CALLBACK_EMPTY, (void *)&py_var_SMS_CALLBACK},
  {0 , DEF_PY_MQTT_CALLBACK, PY_CALLBACK_EMPTY, (void *)&py_var_MQTT_CALLBACK},
  {0 , DEF_PY_GPIO_CALLBACK, PY_CALLBACK_EMPTY, (void *)&py_var_GPIO_CALLBACK},
  {0 , DEF_PY_DISPLAY_CALLBACK, PY_CALLBACK_EMPTY, (void *)&py_var_DISPLAY_CALLBACK},
  {0 , DEF_PY_MODEM_CALLBACK, PY_CALLBACK_EMPTY, (void *)&py_var_MODEM_CALLBACK},
  {0 , DEF_PY_BLUETOOTH_CALLBACK, PY_CALLBACK_EMPTY, (void *)&py_var_BLUETOOTH_CALLBACK},
  {0 , DEF_PY_RF_CALLBACK, PY_CALLBACK_EMPTY, (void *)&py_var_RF_CALLBACK}
};

static void Py_TimerHandler( TimerHandle_t xTimer ); //������

//----------------------------------------------------------------------------------------------------
void vTASK_script(void *pvParameters) {
  
  PmReturn_t retval = PM_RET_OK;
  py_script_type script_buf; //������
  py_task = PY_SCRIPT_START;
  uint8_t num = 0;
  py_main_script.num = 0x00; //������ �� ������ �������  
  char str[50]; //����� ������
  
  py_main_script.state = PY_SCRIPT_NLOAD;
  py_task = PY_SCRIPT_STOP;
  
  while (1) {
    
    //����������� ����� ������� � ������ ������������ ��� ������
    py_script_struct_addr = FADR_SCRIPT_START + num * PY_SCRIPT_LEN;
    
    //������ ��������� �� ������
    if ( xSemaphoreTake(xMutexFlash, MUTEX_FLASH_DELAY) == pdTRUE ) {
      EXT_Flash_readData(py_script_struct_addr, (uint8_t*) &script_buf, sizeof(script_buf)); //������ ������ �� ����
      xSemaphoreGive(xMutexFlash);
      
      //���� ������ �� ������������ ��� ��� ������ ������
      if ((script_buf.state == PY_SCRIPT_NUSE)||(py_main_script.state == PY_SCRIPT_NLOAD)) {
        
        py_main_script = script_buf; //���������� ��������� �������
        
        py_script_addr = py_script_struct_addr + EXT_FLASH_PACK_SIZE; //�������� ����� ������ �������
        
        //�������� ������
        py_main_script.state = PY_SCRIPT_USE;
        EXT_Flash_ReWriteData(py_script_struct_addr, (uint8_t*) &py_main_script, sizeof(py_main_script));
        
        //py_task = PY_SCRIPT_START; //���������
      }
    }else{
      py_task = PY_SCRIPT_STOP;
    }
    
    if (py_task == PY_SCRIPT_START) {
      xTimerStart( Py_timer, 0 ); //��������� ������
      //���������������� ������
      retval = pm_init(MEMSPACE_FLASH, (uint8_t const*)py_script_addr);
      retval = pm_run((uint8_t*)py_main_script.name);
    }
    
    vTaskDelay(50); //�������� �� ���������
    xTimerStop( Py_timer, 0 ); //���������� ������
    
    //���������� ������, ���� ������
    if (retval != PM_RET_OK) {
      
      //�������� ������ ��� ��������� ������
      py_main_script.state = PY_ERROR_START;
      xSemaphoreTake(xMutexFlash, MUTEX_FLASH_DELAY);
      EXT_Flash_ReWriteData(py_script_struct_addr, (uint8_t*) &py_main_script, sizeof(py_main_script));
      xSemaphoreGive(xMutexFlash);
      
      //��������� ��������� �� ������
      for (int i=0; i<sizeof(py_error_codes); i++){
        if (py_error_codes[i].error_code == retval) {
          sprintf(str, "\n\rError #%02X - %s",retval, py_error_codes[i].description);
          break;
        }
      }
      sprintf(str, "%s\n\rScript stopped",str);
      TM_USB_VCP_Puts(str);
    }else{
      TM_USB_VCP_Puts("\n\rScript stopped");
    }
  
    //�������� ������ callback
    for (int i=0; i<PY_COUNT_CALLBACKS; i++) {
      py_callback[i].callback_status = PY_CALLBACK_EMPTY;
    }
    
    //�������� ������ �������
    py_task = PY_SCRIPT_STOP;
    
    while (py_task != PY_SCRIPT_START) {//�����
      vTaskDelay(100);
    } 
  }
}
//----------------------------------------------------------------------------------------------------
//������
static void Py_TimerHandler( TimerHandle_t xTimer )
{
  pm_vmPeriodic(1000); //1 ms
}
//----------------------------------------------------------------------------------------------------
//������������� ������
void TASK_script_init(uint8_t priority) {  
  Py_timer = xTimerCreate( "Py_Timer", 1, pdTRUE, (void*)0, Py_TimerHandler );
  xTaskCreate(vTASK_script,(char*)"TASK_script", configMINIMAL_STACK_SIZE+256, NULL, tskIDLE_PRIORITY + priority, &script_handle);
}
//----------------------------------------------------------------------------------------------------
//��������� ������
void Py_StartScript(uint8_t nub_script) {
  
  uint32_t script_addr; //����� �������
  py_script_type script; //��������� ����������� ������
  
  //���� ��� ������ ������
  if (py_main_script.num != nub_script) {
  
    //����������� ����� ������ ������� � ������
    script_addr = FADR_SCRIPT_START + nub_script * PY_SCRIPT_LEN;
    
    EXT_Flash_readData(script_addr, (uint8_t*) &script, sizeof(script)); //������ ������ �� ����
    if (script.state != 0x00) { //���� ������ ��� ��������
      py_task = PY_SCRIPT_STOP; //���������� ���������� ������
      py_main_script = script; //����������� ���������
    }
  }
}
//----------------------------------------------------------------------------------------------------
//�������� ������� 
void py_add_callbackID(uint8_t id_callback) {
  if (py_callback_queue_len < PY_COUNT_CALLBACKS) {
    py_callback_queue[py_callback_queue_len++] = id_callback;
  }
}
//----------------------------------------------------------------------------------------------------
//�������� �� ������� 
int py_get_callbackID() {
  if (py_callback_queue_len > 0)
      return py_callback_queue[--py_callback_queue_len];
  return -1;
}