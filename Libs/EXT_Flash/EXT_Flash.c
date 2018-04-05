
#include "EXT_Flash.h"
#include "stdint.h"
#include "stm32f1xx_hal.h"

uint8_t buf_flash[EXT_FLASH_SECTOR_SIZE];
SPI_HandleTypeDef flashHspi;

//���������
void EXT_Flash_SPI_sendArray(uint8_t *buf, uint16_t len); //��������� ������
void EXT_Flash_SPI_readArray(uint8_t *buf, uint16_t len); //������� ������
void EXT_Flash_write_enable(); //��������� ������
HAL_StatusTypeDef EXT_Flash_waitStatusByte(uint8_t bit_num, uint8_t val, uint16_t wait_ms); //����� ����������� ���������� ����

//-------------------------------------------------------------------------------------------------------
//������������� Flash
void EXT_Flash_init(SPI_HandleTypeDef handler)
{
  flashHspi = handler;
}
//-------------------------------------------------------------------------------------------------------
//������ ��������� �������
HAL_StatusTypeDef EXT_Flash_set_StatusReg(uint8_t value) {
  
  uint8_t data[2];
  
  data[0] = EXT_FLASH_STATUS_SET;
  data[1] = value;
  
  return HAL_SPI_Transmit(&flashHspi, data, 2, EXT_FLASH_TIMEOUT);
}
//-------------------------------------------------------------------------------------------------------
//������� ������
HAL_StatusTypeDef EXT_Flash_erace_all() {
  
  uint8_t command = EXT_FLASH_ERASE_CHIP;

  return HAL_SPI_Transmit(&flashHspi, &command, 1, EXT_FLASH_TIMEOUT);
}
//-------------------------------------------------------------------------------------------------------
//������� ������ 4 ��
HAL_StatusTypeDef EXT_Flash_erace_sector(uint32_t address) {
  
  uint8_t data[4];
  HAL_StatusTypeDef status;
  
  data[0] = EXT_FLASH_ERASE_SECTOR;
  
  EXT_Flash_write_enable(); //��������� ������
  if ((status = EXT_Flash_waitStatusByte(EXT_FLASH_STATUS_WEL, 1, EXT_FLASH_TIMEOUT)) != HAL_OK) //���� ������� ������ ������
    return status;
  
  data[1] = (address & 0xFF0000) >> 16;
  data[2] = (address & 0x00FF00) >> 8;
  data[3] = (address & 0x0000FF);

  if ((status = HAL_SPI_Transmit(&flashHspi, data, 4, EXT_FLASH_TIMEOUT)) != HAL_OK)
    return status;
  
  return EXT_Flash_waitStatusByte(EXT_FLASH_STATUS_WIP, 0, EXT_FLASH_TIMEOUT); //���� ���������� ��������
}
//-------------------------------------------------------------------------------------------------------
//������� ���� 64 ��
void EXT_Flash_erace_block(uint32_t address) {
  
  uint8_t addr[3];
  uint8_t command = EXT_FLASH_ERASE_BLOCK;
  
  EXT_Flash_write_enable(); //��������� ������
  EXT_Flash_waitStatusByte(EXT_FLASH_STATUS_WEL, 1, 200); //���� ������� ������ ������
  
  addr[0] = (address & 0xFF0000) >> 16;
  addr[1] = (address & 0x00FF00) >> 8;
  addr[2] = (address & 0x0000FF);

  EXT_FLASH_SPI_CS_RESET;
  
  EXT_Flash_SPI_sendArray(&command,1);
  EXT_Flash_SPI_sendArray(addr,3);
  
  EXT_FLASH_SPI_CS_SET;
  
  EXT_Flash_waitStatusByte(EXT_FLASH_STATUS_WIP, 0, 5000); //���� ��������
}
//-------------------------------------------------------------------------------------------------------
//��������� ������
HAL_StatusTypeDef EXT_Flash_write_enable() {
  
  uint8_t address = EXT_FLASH_WRITE_ENABLE;

  return HAL_SPI_Transmit(&flashHspi, &address, 1, EXT_FLASH_TIMEOUT);
}
//-------------------------------------------------------------------------------------------------------
//������ ��������� �������
uint8_t EXT_Flash_get_StatusReg() {
  
  uint8_t value_flash[2];
  uint8_t address[2];
  
  address[0] = EXT_FLASH_STATUS_GET;
  
  HAL_SPI_TransmitReceive(&flashHspi, address, value_flash, 2, EXT_FLASH_TIMEOUT);
  
  return value_flash[1];
}
//-------------------------------------------------------------------------------------------------------
//������ DEV ID
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
//���������� ������ ������ � �������
void EXT_Flash_ReWriteData(uint32_t address, uint8_t *data, uint16_t count) {

  memset(buf_flash,0,sizeof(buf_flash)); //�������� �����
  
  uint32_t sector_addr = (uint32_t)(address/EXT_FLASH_SECTOR_SIZE)*EXT_FLASH_SECTOR_SIZE; //����� ����� �������

  EXT_Flash_readData(sector_addr,buf_flash,EXT_FLASH_SECTOR_SIZE); //������ ������
  memcpy(&buf_flash[address-sector_addr],data,count); //���������� ������
  EXT_Flash_erace_sector(sector_addr); //������� ������
  EXT_Flash_writeData(sector_addr,buf_flash,EXT_FLASH_SECTOR_SIZE); //������������ ������ 
}
//-------------------------------------------------------------------------------------------------------
//����� ����������� ���������� ����
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
//������ ������ ������
void EXT_Flash_writeData(uint32_t address, uint8_t *data, uint16_t count) {
  
  uint8_t addr[3];
  uint8_t command = EXT_FLASH_WRITE_DATA;
  uint8_t count_full = (uint32_t)(count/EXT_FLASH_PACK_SIZE); //����� ���������� �������
  uint32_t buf_addr = address;
  uint16_t count_end;
  
  //���������� ����� ��������
  for (int i=0; i<count_full; i++) {
    
    EXT_Flash_write_enable(); //��������� ������
    EXT_Flash_waitStatusByte(EXT_FLASH_STATUS_WEL, 1, 200); //���� ����� ������ ������
    
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
    
    EXT_Flash_waitStatusByte(EXT_FLASH_STATUS_WIP, 0, 200); //���� ���������� ������
    
    buf_addr += EXT_FLASH_PACK_SIZE;
  }
  
  //������� ���������� ��������� ����
  count_end = count-(count_full*EXT_FLASH_PACK_SIZE);
  
  //���������� ��������� ������
  if (count_end > 0) {
    
    EXT_Flash_write_enable(); //��������� ������
    EXT_Flash_waitStatusByte(EXT_FLASH_STATUS_WEL, 1, 200); //���� ����� ������ ������
    
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
    
    EXT_Flash_waitStatusByte(EXT_FLASH_STATUS_WIP, 0, 200); //���� ���������� ������
  }  
  
}
//-------------------------------------------------------------------------------------------------------
//������ ������ ������
void EXT_Flash_readData(uint32_t address, uint8_t *data, uint16_t count) {
  
  uint8_t addr[3];
  uint8_t command = EXT_FLASH_READ_DATA;
  uint8_t count_full = (uint32_t)(count/EXT_FLASH_PACK_SIZE); //����� ���������� �������
  uint32_t buf_addr = address;
    
  //������ ����� ������
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
  
  //�������� ��������� ������
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
//��������� ������
void EXT_Flash_SPI_sendArray(uint8_t *buf, uint16_t len) {
  for (int i=0; i<len; i++) {
    SPI_I2S_SendData(EXT_FLASH_SPI_PORT, buf[i]);
    while(SPI_I2S_GetFlagStatus(EXT_FLASH_SPI_PORT, SPI_I2S_FLAG_BSY) == SET);
    while(SPI_I2S_GetFlagStatus(EXT_FLASH_SPI_PORT, SPI_I2S_FLAG_RXNE) == RESET);
    SPI_I2S_ReceiveData(EXT_FLASH_SPI_PORT);
  }
}
//-------------------------------------------------------------------------------------------------------
//������� ������
void EXT_Flash_SPI_readArray(uint8_t *buf, uint16_t len) {
  for (int i=0; i<len; i++) {
    SPI_I2S_SendData(EXT_FLASH_SPI_PORT, 0x00);
    while(SPI_I2S_GetFlagStatus(EXT_FLASH_SPI_PORT, SPI_I2S_FLAG_BSY) == SET);
    while(SPI_I2S_GetFlagStatus(EXT_FLASH_SPI_PORT, SPI_I2S_FLAG_RXNE) == RESET);
    buf[i] = SPI_I2S_ReceiveData(EXT_FLASH_SPI_PORT);
  }
}
//-------------------------------------------------------------------------------------------------------