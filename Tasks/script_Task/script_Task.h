
#ifndef TASK_SCRIPT_H
#define TASK_SCRIPT_H

#include "py_pm.h"
#include "py_mem.h"
#include "py_obj.h"

#define PY_SCRIPT_LEN           0x20000 //128 �����

//Py commands
#define PY_SCRIPT_START         0x01
#define PY_SCRIPT_STOP          0x02


//������ ������ �������
typedef enum {
  PY_SCRIPT_NLOAD = 0,  //������ �� ��������
  PY_SCRIPT_NUSE,       //������ ��������, �� �� ������������
  PY_DEBUG_LOAD_SCRIPT, //�������� ������� � ������
  PY_SCRIPT_PAUSE,      //������ �������������
  PY_SCRIPT_STOP,       //������ ����������
  PY_SCRIPT_START,      //������ ������� 
  PY_SCRIPT_USE,        //������ ������������
  PY_ERROR_START        //��������� ������
} debug_mode;

//��������� �������� ������� � ������
typedef struct{
  uint8_t state;        //������� ��������� �������
  char name[10];        //��� �������
  uint32_t len;         //����� �������
  uint8_t num;          //����� �������
  uint16_t crc;         //CRC16
} py_script_type;

//���������� callback
#define  PY_COUNT_CALLBACKS     7

//��������� �������� callback
#define DEF_PY_SMS_CALLBACK             "SMS_callback"                  //SMS callback
#define DEF_PY_MQTT_CALLBACK            "MQTT_callback"                 //MQTT callback
#define DEF_PY_GPIO_CALLBACK            "GPIO_callback"                 //GPIO callback
#define DEF_PY_DISPLAY_CALLBACK         "DISPLAY_callback"              //DISPLAY callback
#define DEF_PY_MODEM_CALLBACK           "MODEM_callback"                //MODEM callback
#define DEF_PY_BLUETOOTH_CALLBACK       "BLUETOOTH_callback"            //BLUETOOTH callback
#define DEF_PY_RF_CALLBACK              "RF_callback"                   //RF callback

//----------------------------------Callback------------------------------------

//������ Callback
typedef enum{
  PY_SMS_CALLBACK = 0,
  PY_MQTT_CALLBACK,
  PY_GPIO_CALLBACK,
  PY_DISPLAY_CALLBACK,
  PY_MODEM_CALLBACK,
  PY_PY_BLUETOOTH_CALLBACK,
  PY_RF_CALLBACK  
} py_callback_ID;

//������ callback
typedef enum{
  PY_CALLBACK_EMPTY = 0,
  PY_CALLBACK_STOP,
  PY_CALLBACK_RUN,
} py_callback_statuses;

//�������� � �������
typedef struct{
  pPmObj_t py_func;
  char callback_name[20];
  uint8_t callback_status;
  void *callback_val;
} py_callback_type; 

//������������ ���������� � callback
//SMS_CALLBACK
typedef struct{
  char Phone[20];
  char SMS_text[80];
} py_var_SMS_CALLBACK_type;
//MQTT_CALLBACK
typedef struct{
  char ip_addr[15];
  char port[6];
  char topic[20];
  char message[128];
  uint8_t len;
} py_var_MQTT_CALLBACK_type;
//GPIO_CALLBACK
typedef struct{
  uint8_t type_dev;
  uint8_t adr_sw;
  uint8_t adr_port;
  uint8_t value;
} py_var_GPIO_CALLBACK_type;
//DISPLAY_CALLBACK
typedef struct{
  uint8_t page_id;
  uint8_t element_id;
  uint8_t value;
} py_var_DISPLAY_CALLBACK_type;
//MODEM_CALLBACK
typedef struct{
  char rx_buf[256];
  uint8_t len;
} py_var_MODEM_CALLBACK_type;
//BLUETOOTH_CALLBACK
typedef struct{
  char rx_buf[256];
  uint8_t len;
} py_var_BLUETOOTH_CALLBACK_type;
//RF_CALLBACK
typedef struct{
  uint8_t rf_addr;
  char rx_buf[256];
  uint8_t len;
} py_var_RF_CALLBACK_type;


//------------------------------------------------------------------------------

//��������� ������ ����������
typedef struct{
  uint8_t type_dev;     //��� ����������
  uint8_t adr_sw;       //�����
  uint8_t adr_port;     //����
  uint8_t error_code;   //��� ������
  char thread_name[30]; //��� ������
}py_error_type;

extern py_script_type py_main_script; //�������� ������
extern uint8_t py_task; //���������� ������ �������
extern uint32_t py_script_addr; //����� ������� �� flash
extern py_error_type py_error; //������ �������
extern py_callback_type py_callback[PY_COUNT_CALLBACKS]; //�������� ������� Callback

void TASK_script_init(uint8_t priority); //������������� ������
void Py_StartScript(uint8_t nub_script); //��������� ������
int py_get_callbackID(); //�������� �� ������� 
void py_add_callbackID(uint8_t id_callback); //�������� ������� 

#endif