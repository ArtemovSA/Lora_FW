
#ifndef EXT_FLASH_H
#define EXT_FLASH_H

#include "main.h"
#include "stm32f1xx_hal.h"

//���������
#define EXT_FLASH_TIMEOUT 300   //Timeout
#define EXT_FLASH_SIZE 0x1FFFFF //������ ������
#define EXT_FLASH_SECTOR_SIZE 0x1000 //������ ������� 4��
#define EXT_FLASH_BLOCK_SIZE 0x10000 //������ ����� 64��
#define EXT_FLASH_PACK_SIZE 0x100 //������ ������ ��� ������

//��������
#define EXT_FLASH_STATUS_GET 0x05
#define EXT_FLASH_STATUS_SET 0x01
#define EXT_FLASH_DEV_ID_GET 0xAB

//��������� �������
#define EXT_FLASH_STATUS_WIP 0
#define EXT_FLASH_STATUS_WEL 1

//��������
#define EXT_FLASH_READ_DATA 0x03
#define EXT_FLASH_WRITE_DATA 0x02
#define EXT_FLASH_WRITE_ENABLE 0x06
#define EXT_FLASH_WRITE_DISABLE 0x04
#define EXT_FLASH_ERASE_BLOCK 0xD8
#define EXT_FLASH_ERASE_SECTOR 0x20
#define EXT_FLASH_ERASE_CHIP 0xC7

extern void EXT_Flash_init(); //������������� Flash
extern HAL_StatusTypeDef EXT_Flash_get_DevIDReg(); //������ DEV ID
extern uint8_t EXT_Flash_get_StatusReg(); //������ ��������� �������
extern HAL_StatusTypeDef EXT_Flash_set_StatusReg(uint8_t value); //������ ��������� �������
extern HAL_StatusTypeDef EXT_Flash_writeData(uint32_t address, uint8_t *data, uint16_t count); //������ ������ ������
extern HAL_StatusTypeDef EXT_Flash_readData(uint32_t address, uint8_t *data, uint16_t count); //������ ������ ������
extern HAL_StatusTypeDef EXT_Flash_erace_all();//������� ������
extern HAL_StatusTypeDef EXT_Flash_erace_sector(uint32_t address);//������� ������ 4 ��
extern HAL_StatusTypeDef EXT_Flash_erace_block(uint32_t address);//������� ���� 64 ��
extern HAL_StatusTypeDef EXT_Flash_ReWriteData(uint32_t address, uint8_t *data, uint16_t count); //���������� ������ ������


#endif