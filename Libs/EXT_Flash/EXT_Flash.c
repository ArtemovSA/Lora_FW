
#include "EXT_Flash.h"
#include "stdint.h"
#include "stm32f1xx_hal.h"

uint8_t buf_flash[EXT_FLASH_SECTOR_SIZE];
SPI_HandleTypeDef flashHspi;

//Прототипы
void EXT_Flash_SPI_sendArray(uint8_t *buf, uint16_t len); //Отправить массив
void EXT_Flash_SPI_readArray(uint8_t *buf, uint16_t len); //Принять массив
void EXT_Flash_write_enable(); //Разрешить запись
HAL_StatusTypeDef EXT_Flash_waitStatusByte(uint8_t bit_num, uint8_t val, uint16_t wait_ms); //Ждать выставления статусного бита

//-------------------------------------------------------------------------------------------------------
//Инициализация Flash
void EXT_Flash_init(SPI_HandleTypeDef handler)
{
  flashHspi = handler;
}
//-------------------------------------------------------------------------------------------------------
//Писать статусный регистр
HAL_StatusTypeDef EXT_Flash_set_StatusReg(uint8_t value) {
  
  uint8_t data[2];
  
  data[0] = EXT_FLASH_STATUS_SET;
  data[1] = value;
  
  return HAL_SPI_Transmit(&flashHspi, data, 2, EXT_FLASH_TIMEOUT);
}
//-------------------------------------------------------------------------------------------------------
//Стереть память
HAL_StatusTypeDef EXT_Flash_erace_all() {
  
  uint8_t command = EXT_FLASH_ERASE_CHIP;

  return HAL_SPI_Transmit(&flashHspi, &command, 1, EXT_FLASH_TIMEOUT);
}
//-------------------------------------------------------------------------------------------------------
//Стереть сектор 4 кб
HAL_StatusTypeDef EXT_Flash_erace_sector(uint32_t address) {
  
  uint8_t data[4];
  HAL_StatusTypeDef status;
  
  data[0] = EXT_FLASH_ERASE_SECTOR;
  
  EXT_Flash_write_enable(); //Разрешить запись
  if ((status = EXT_Flash_waitStatusByte(EXT_FLASH_STATUS_WEL, 1, EXT_FLASH_TIMEOUT)) != HAL_OK) //Ждем сигнала приема данных
    return status;
  
  data[1] = (address & 0xFF0000) >> 16;
  data[2] = (address & 0x00FF00) >> 8;
  data[3] = (address & 0x0000FF);

  if ((status = HAL_SPI_Transmit(&flashHspi, data, 4, EXT_FLASH_TIMEOUT)) != HAL_OK)
    return status;
  
  return EXT_Flash_waitStatusByte(EXT_FLASH_STATUS_WIP, 0, EXT_FLASH_TIMEOUT); //Ждем завершения стирания
}
//-------------------------------------------------------------------------------------------------------
//Стереть блок 64 кб
void EXT_Flash_erace_block(uint32_t address) {
  
  uint8_t addr[3];
  uint8_t command = EXT_FLASH_ERASE_BLOCK;
  
  EXT_Flash_write_enable(); //Разрешить запись
  EXT_Flash_waitStatusByte(EXT_FLASH_STATUS_WEL, 1, 200); //Ждем сигнала приема данных
  
  addr[0] = (address & 0xFF0000) >> 16;
  addr[1] = (address & 0x00FF00) >> 8;
  addr[2] = (address & 0x0000FF);

  EXT_FLASH_SPI_CS_RESET;
  
  EXT_Flash_SPI_sendArray(&command,1);
  EXT_Flash_SPI_sendArray(addr,3);
  
  EXT_FLASH_SPI_CS_SET;
  
  EXT_Flash_waitStatusByte(EXT_FLASH_STATUS_WIP, 0, 5000); //Ждем стирания
}
//-------------------------------------------------------------------------------------------------------
//Разрешить запись
HAL_StatusTypeDef EXT_Flash_write_enable() {
  
  uint8_t address = EXT_FLASH_WRITE_ENABLE;

  return HAL_SPI_Transmit(&flashHspi, &address, 1, EXT_FLASH_TIMEOUT);
}
//-------------------------------------------------------------------------------------------------------
//Читать статусный регистр
uint8_t EXT_Flash_get_StatusReg() {
  
  uint8_t value_flash[2];
  uint8_t address[2];
  
  address[0] = EXT_FLASH_STATUS_GET;
  
  HAL_SPI_TransmitReceive(&flashHspi, address, value_flash, 2, EXT_FLASH_TIMEOUT);
  
  return value_flash[1];
}
//-------------------------------------------------------------------------------------------------------
//Читать DEV ID
uint8_t EXT_Flash_get_DevIDReg() {
  
  uint8_t value_flash;
  uint8_t address = EXT_FLASH_DEV_ID_GET;
  uint8_t dummy[3];
  
  dummy[0] = 0x00;
  dummy[1] = 0x00;
  dummy[2] = 0x00;
  
  EXT_FLASH_SPI_CS_RESET;
  
  EXT_Flash_SPI_sendArray(&address,1);
  EXT_Flash_SPI_sendArray(dummy,3);
  EXT_Flash_SPI_readArray(&value_flash, 1);
  
  EXT_FLASH_SPI_CS_SET;
  
  return value_flash;
}
//-------------------------------------------------------------------------------------------------------
//Переписать массив данных в секторе
void EXT_Flash_ReWriteData(uint32_t address, uint8_t *data, uint16_t count) {

  memset(buf_flash,0,sizeof(buf_flash)); //Очистить буфер
  
  uint32_t sector_addr = (uint32_t)(address/EXT_FLASH_SECTOR_SIZE)*EXT_FLASH_SECTOR_SIZE; //Найти адрес сектора

  EXT_Flash_readData(sector_addr,buf_flash,EXT_FLASH_SECTOR_SIZE); //Читать сектор
  memcpy(&buf_flash[address-sector_addr],data,count); //Переписать данные
  EXT_Flash_erace_sector(sector_addr); //Стереть сектор
  EXT_Flash_writeData(sector_addr,buf_flash,EXT_FLASH_SECTOR_SIZE); //Перезаписать сектор 
}
//-------------------------------------------------------------------------------------------------------
//Ждать выставления статусного бита
HAL_StatusTypeDef EXT_Flash_waitStatusByte(uint8_t bit_num, uint8_t val, uint16_t wait_ms) {
  
  uint16_t try_counter = 0;
  
  while ((EXT_Flash_get_StatusReg() & (1<<bit_num)) != (val<<bit_num)) {
    if (try_counter == wait_ms)
      return HAL_TIMEOUT;
   _delay_ms(1);
   try_counter++;
  }
  
  return HAL_OK;
}
//-------------------------------------------------------------------------------------------------------
//Писать массив данных
void EXT_Flash_writeData(uint32_t address, uint8_t *data, uint16_t count) {
  
  uint8_t addr[3];
  uint8_t command = EXT_FLASH_WRITE_DATA;
  uint8_t count_full = (uint32_t)(count/EXT_FLASH_PACK_SIZE); //Найти количество страниц
  uint32_t buf_addr = address;
  uint16_t count_end;
  
  //Записываем целые страницы
  for (int i=0; i<count_full; i++) {
    
    EXT_Flash_write_enable(); //Разрешить запись
    EXT_Flash_waitStatusByte(EXT_FLASH_STATUS_WEL, 1, 200); //Ждем флага приема данных
    
    addr[0] = (buf_addr & 0xFF0000) >> 16;
    addr[1] = (buf_addr & 0x00FF00) >> 8;
    addr[2] = (buf_addr & 0x0000FF);
    
    memcpy(buf_flash,&data[EXT_FLASH_PACK_SIZE*i],EXT_FLASH_PACK_SIZE);
    
    EXT_FLASH_SPI_CS_RESET;
    EXT_Flash_SPI_sendArray(&command, 1);
    EXT_Flash_SPI_sendArray(addr, 3);
    EXT_Flash_SPI_sendArray(buf_flash, EXT_FLASH_PACK_SIZE);
    EXT_FLASH_SPI_CS_SET;
    _delay_ms(5);
    
    EXT_Flash_waitStatusByte(EXT_FLASH_STATUS_WIP, 0, 200); //Ждем завершения записи
    
    buf_addr += EXT_FLASH_PACK_SIZE;
  }
  
  //Считаем количество оставшися байт
  count_end = count-(count_full*EXT_FLASH_PACK_SIZE);
  
  //Записываем остальные данные
  if (count_end > 0) {
    
    EXT_Flash_write_enable(); //Разрешить запись
    EXT_Flash_waitStatusByte(EXT_FLASH_STATUS_WEL, 1, 200); //Ждем флага приема данных
    
    addr[0] = (buf_addr & 0xFF0000) >> 16;
    addr[1] = (buf_addr & 0x00FF00) >> 8;
    addr[2] = (buf_addr & 0x0000FF);
    
    memcpy(buf_flash,&data[EXT_FLASH_PACK_SIZE*count_full],count_end);
    
    EXT_FLASH_SPI_CS_RESET;
    EXT_Flash_SPI_sendArray(&command, 1);
    EXT_Flash_SPI_sendArray(addr, 3);
    EXT_Flash_SPI_sendArray(buf_flash, count_end);
    EXT_FLASH_SPI_CS_SET;
    _delay_ms(5);
    
    EXT_Flash_waitStatusByte(EXT_FLASH_STATUS_WIP, 0, 200); //Ждем завершения записи
  }  
  
}
//-------------------------------------------------------------------------------------------------------
//Читать массив данных
void EXT_Flash_readData(uint32_t address, uint8_t *data, uint16_t count) {
  
  uint8_t addr[3];
  uint8_t command = EXT_FLASH_READ_DATA;
  uint8_t count_full = (uint32_t)(count/EXT_FLASH_PACK_SIZE); //Найти количество пакетов
  uint32_t buf_addr = address;
    
  //Читаем целые пакеты
  for (int i=0; i<count_full; i++) {
  
    addr[0] = (buf_addr & 0xFF0000) >> 16;
    addr[1] = (buf_addr & 0x00FF00) >> 8;
    addr[2] = (buf_addr & 0x0000FF);
    
    EXT_FLASH_SPI_CS_RESET;
    
    EXT_Flash_SPI_sendArray(&command, 1);
    EXT_Flash_SPI_sendArray(addr, 3);
    EXT_Flash_SPI_readArray(&data[EXT_FLASH_PACK_SIZE*i], EXT_FLASH_PACK_SIZE);
    
    EXT_FLASH_SPI_CS_SET;
    
    buf_addr += EXT_FLASH_PACK_SIZE;
  }
  
  //Получаем остальные данные
  if (count-(count_full*EXT_FLASH_PACK_SIZE) > 0) {
    
    addr[0] = (buf_addr & 0xFF0000) >> 16;
    addr[1] = (buf_addr & 0x00FF00) >> 8;
    addr[2] = (buf_addr & 0x0000FF);
    
    EXT_FLASH_SPI_CS_RESET;
    
    EXT_Flash_SPI_sendArray(&command, 1);
    EXT_Flash_SPI_sendArray(addr, 3);
    EXT_Flash_SPI_readArray(&data[EXT_FLASH_PACK_SIZE*count_full], count-(count_full*EXT_FLASH_PACK_SIZE));
    
    EXT_FLASH_SPI_CS_SET;
  }
}
//-------------------------------------------------------------------------------------------------------
//Отправить массив
void EXT_Flash_SPI_sendArray(uint8_t *buf, uint16_t len) {
  for (int i=0; i<len; i++) {
    SPI_I2S_SendData(EXT_FLASH_SPI_PORT, buf[i]);
    while(SPI_I2S_GetFlagStatus(EXT_FLASH_SPI_PORT, SPI_I2S_FLAG_BSY) == SET);
    while(SPI_I2S_GetFlagStatus(EXT_FLASH_SPI_PORT, SPI_I2S_FLAG_RXNE) == RESET);
    SPI_I2S_ReceiveData(EXT_FLASH_SPI_PORT);
  }
}
//-------------------------------------------------------------------------------------------------------
//Принять массив
void EXT_Flash_SPI_readArray(uint8_t *buf, uint16_t len) {
  for (int i=0; i<len; i++) {
    SPI_I2S_SendData(EXT_FLASH_SPI_PORT, 0x00);
    while(SPI_I2S_GetFlagStatus(EXT_FLASH_SPI_PORT, SPI_I2S_FLAG_BSY) == SET);
    while(SPI_I2S_GetFlagStatus(EXT_FLASH_SPI_PORT, SPI_I2S_FLAG_RXNE) == RESET);
    buf[i] = SPI_I2S_ReceiveData(EXT_FLASH_SPI_PORT);
  }
}
//-------------------------------------------------------------------------------------------------------