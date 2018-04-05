
#ifndef TASK_SCRIPT_H
#define TASK_SCRIPT_H

#include "py_pm.h"
#include "py_mem.h"
#include "py_obj.h"

#define PY_SCRIPT_LEN           0x20000 //128 Кбайт

//Py commands
#define PY_SCRIPT_START         0x01
#define PY_SCRIPT_STOP          0x02


//Режимы работы отладки
typedef enum {
  PY_SCRIPT_NLOAD = 0,  //Скрипт не загружен
  PY_SCRIPT_NUSE,       //Скрипт загружен, но не используется
  PY_DEBUG_LOAD_SCRIPT, //Загрузка скрипта в память
  PY_SCRIPT_PAUSE,      //Скрипт приостановлен
  PY_SCRIPT_STOP,       //Скрипт остановлен
  PY_SCRIPT_START,      //Скрипт запущен 
  PY_SCRIPT_USE,        //Скрипт используется
  PY_ERROR_START        //Ошибочный скрипт
} debug_mode;

//Структура описания скрипта в памяти
typedef struct{
  uint8_t state;        //Текущее состояние скрипта
  char name[10];        //Имя скрипта
  uint32_t len;         //Длина скрипта
  uint8_t num;          //Номер скрипта
  uint16_t crc;         //CRC16
} py_script_type;

//Количество callback
#define  PY_COUNT_CALLBACKS     7

//Настройки названий callback
#define DEF_PY_SMS_CALLBACK             "SMS_callback"                  //SMS callback
#define DEF_PY_MQTT_CALLBACK            "MQTT_callback"                 //MQTT callback
#define DEF_PY_GPIO_CALLBACK            "GPIO_callback"                 //GPIO callback
#define DEF_PY_DISPLAY_CALLBACK         "DISPLAY_callback"              //DISPLAY callback
#define DEF_PY_MODEM_CALLBACK           "MODEM_callback"                //MODEM callback
#define DEF_PY_BLUETOOTH_CALLBACK       "BLUETOOTH_callback"            //BLUETOOTH callback
#define DEF_PY_RF_CALLBACK              "RF_callback"                   //RF callback

//----------------------------------Callback------------------------------------

//Список Callback
typedef enum{
  PY_SMS_CALLBACK = 0,
  PY_MQTT_CALLBACK,
  PY_GPIO_CALLBACK,
  PY_DISPLAY_CALLBACK,
  PY_MODEM_CALLBACK,
  PY_PY_BLUETOOTH_CALLBACK,
  PY_RF_CALLBACK  
} py_callback_ID;

//Статус callback
typedef enum{
  PY_CALLBACK_EMPTY = 0,
  PY_CALLBACK_STOP,
  PY_CALLBACK_RUN,
} py_callback_statuses;

//Названий и функций
typedef struct{
  pPmObj_t py_func;
  char callback_name[20];
  uint8_t callback_status;
  void *callback_val;
} py_callback_type; 

//Возвращаемые переменные в callback
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

//Структура ошибки выполнения
typedef struct{
  uint8_t type_dev;     //Тип устройства
  uint8_t adr_sw;       //Адрес
  uint8_t adr_port;     //Порт
  uint8_t error_code;   //Код ошибки
  char thread_name[30]; //Имя потока
}py_error_type;

extern py_script_type py_main_script; //Основной скрипт
extern uint8_t py_task; //Выполнение задачи скрипта
extern uint32_t py_script_addr; //Адрес скрипта во flash
extern py_error_type py_error; //Ошибка скрипта
extern py_callback_type py_callback[PY_COUNT_CALLBACKS]; //Название функций Callback

void TASK_script_init(uint8_t priority); //Инициализация задачи
void Py_StartScript(uint8_t nub_script); //Запустить скрипт
int py_get_callbackID(); //Получить из очереди 
void py_add_callbackID(uint8_t id_callback); //Добавить очередь 

#endif