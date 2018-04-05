
#ifndef EXT_FLASH_H
#define EXT_FLASH_H

#include "main.h"
#include "stm32f1xx_hal.h"

//Параметры
#define EXT_FLASH_TIMEOUT 300   //Timeout
#define EXT_FLASH_SIZE 0x1FFFFF //Размер памяти
#define EXT_FLASH_SECTOR_SIZE 0x1000 //Размер сектора 4Кб
#define EXT_FLASH_BLOCK_SIZE 0x10000 //Размер блока 64Кб
#define EXT_FLASH_PACK_SIZE 0x100 //Размер пакета для записи

//Регистры
#define EXT_FLASH_STATUS_GET 0x05
#define EXT_FLASH_STATUS_SET 0x01
#define EXT_FLASH_DEV_ID_GET 0xAB

//Статусный регистр
#define EXT_FLASH_STATUS_WIP 0
#define EXT_FLASH_STATUS_WEL 1

//Комманды
#define EXT_FLASH_READ_DATA 0x03
#define EXT_FLASH_WRITE_DATA 0x02
#define EXT_FLASH_WRITE_ENABLE 0x06
#define EXT_FLASH_WRITE_DISABLE 0x04
#define EXT_FLASH_ERASE_BLOCK 0xD8
#define EXT_FLASH_ERASE_SECTOR 0x20
#define EXT_FLASH_ERASE_CHIP 0xC7

extern void EXT_Flash_init(); //Инициализация Flash
extern HAL_StatusTypeDef EXT_Flash_get_DevIDReg(); //Читать DEV ID
extern uint8_t EXT_Flash_get_StatusReg(); //Читать статусный регистр
extern HAL_StatusTypeDef EXT_Flash_set_StatusReg(uint8_t value); //Писать статусный регистр
extern HAL_StatusTypeDef EXT_Flash_writeData(uint32_t address, uint8_t *data, uint16_t count); //Писать массив данных
extern HAL_StatusTypeDef EXT_Flash_readData(uint32_t address, uint8_t *data, uint16_t count); //Читать массив данных
extern HAL_StatusTypeDef EXT_Flash_erace_all();//Стереть память
extern HAL_StatusTypeDef EXT_Flash_erace_sector(uint32_t address);//Стереть сектор 4 кб
extern HAL_StatusTypeDef EXT_Flash_erace_block(uint32_t address);//Стереть блок 64 кб
extern HAL_StatusTypeDef EXT_Flash_ReWriteData(uint32_t address, uint8_t *data, uint16_t count); //Переписать массив данных


#endif