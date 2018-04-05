
#include "Device_ctrl.h"
#include "cmsis_os.h"
#include "EXT_Flash.h"
#include "stdio.h"
#include <stdarg.h>

DC_settings_t DC_settings;
DC_params_t DC_params;

//--------------------------------------------------------------------------------------------------
//Device init
void DC_init()
{
  DC_LoadFromFlash(); //Load from flash
}
//--------------------------------------------------------------------------------------------------
//Determination debug connection
uint8_t DBG_Connected()
{
  uint32_t *pDebugVal = (uint32_t*)0xE000EDF0;
  
  return (*pDebugVal & 0x01);
}
//--------------------------------------------------------------------------------------------------
//Out debug data
//arg: str - string for out
void DC_debugOut(char *str, ...)
{
  va_list args;
  va_start(args, str);
  va_end(args);
  
  if (DBG_Connected())
  {
    vprintf(str, args);
    va_end(args);
  }
}
//------------------------------------------------------------------------------
//Load from flash
void DC_LoadFromFlash()
{
  uint8_t magicKey = 0;
  
  EXT_Flash_readData(FADR_SETTINGS, &magicKey, 1);
  
  if (magicKey != DC_MAGICKEY_SETTINGS)
  {
    DC_debugOut("Set default settings   ");
    
    DC_settings.modbusAddr = DC_DEFAULT_SET_MODBUSADDR;
    EXT_Flash_ReWriteData(FADR_SETTINGS+1, (uint8_t*)&DC_settings, sizeof(DC_settings));
    
    DC_debugOut("*\r\n");
  }else{
    DC_debugOut("Load default settings  ");
    EXT_Flash_readData(FADR_SETTINGS+1, (uint8_t*)&DC_settings, sizeof(DC_settings));
    DC_debugOut("*\r\n");
  }
  
}
//------------------------------------------------------------------------------
//Led blink
void DC_ledBlink(uint16_t periodic, uint16_t count)
{
  uint16_t half_T = periodic/2;
  
  if (count == DC_INFINITE)
  {
    while (1) {
      LED_ON;
      vTaskDelay(half_T);
      LED_OFF;
      vTaskDelay(half_T);
    }
  }else{
    while (count--) {
      LED_ON;
      vTaskDelay(half_T);
      LED_OFF;
      vTaskDelay(half_T);
    }
  }
  
}