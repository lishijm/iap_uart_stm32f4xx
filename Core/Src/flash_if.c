#include "main.h"
#include "flash_if.h"

#define FLASH_USER_START_ADDR   ADDR_FLASH_SECTOR_2   
#define FLASH_USER_END_ADDR     ADDR_FLASH_SECTOR_5

static uint32_t GetSector(uint32_t Address)
{
  uint32_t sector = 0;
  
  if((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0))
  {
    sector = FLASH_SECTOR_0;
  }
  else if((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1))
  {
    sector = FLASH_SECTOR_1;
  }
  else if((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2))
  {
    sector = FLASH_SECTOR_2;
  }
  else if((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3))
  {
    sector = FLASH_SECTOR_3;  
  }
  else if((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4))
  {
    sector = FLASH_SECTOR_4;  
  }
  return sector;
}

void Flash_Erase(void){
  static FLASH_EraseInitTypeDef EraseInitStruct;
  uint32_t PAGEError = 0;
  uint32_t FirstSector = 0;
  uint32_t NbOfSectors = 0;

  FirstSector = GetSector(FLASH_USER_START_ADDR);
  NbOfSectors = GetSector(FLASH_USER_END_ADDR) - FirstSector + 1;

  EraseInitStruct.TypeErase     = FLASH_TYPEERASE_SECTORS;
  EraseInitStruct.VoltageRange  = FLASH_VOLTAGE_RANGE_3;
  EraseInitStruct.Sector        = FirstSector;
  EraseInitStruct.NbSectors     = NbOfSectors;

  if (HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError) != HAL_OK){
    HAL_FLASH_Lock();  
    Error_Handler();
  }
}
    
void Flash_Write(int16_t *pBuffer,uint32_t  NumToWrite){
  uint32_t  counter=0;
  uint32_t Address = FLASH_USER_START_ADDR;

  HAL_FLASH_Unlock();
  Flash_Erase();
  
  while ((Address < FLASH_USER_END_ADDR) && (counter<NumToWrite)){
    if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, Address, pBuffer[counter]) == HAL_OK){
      Address = Address + 2;  //address move back 2 bytes
      counter++;
    }
    else{
      Error_Handler();
    }
  }
  
  HAL_FLASH_Lock();   //上锁
}

void Flash_Read(int16_t *pBuffer,uint32_t  NumToRead){
  uint32_t  counter=0;
  uint32_t Address = FLASH_USER_START_ADDR;

  while ( (Address < FLASH_USER_END_ADDR) && (counter<NumToRead)){
    pBuffer[counter++]= *(__IO uint16_t *)Address;  
    Address = Address + 2;  //address move back 2 bytes
  }
}
