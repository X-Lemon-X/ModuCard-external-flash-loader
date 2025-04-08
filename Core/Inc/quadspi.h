/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    quadspi.h
  * @brief   This file contains all the function prototypes for
  *          the quadspi.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __QUADSPI_H__
#define __QUADSPI_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern QSPI_HandleTypeDef hqspi;

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_QUADSPI_Init(void);

uint8_t CSP_QUADSPI_Init(void);
uint8_t CSP_QSPI_EraseSector(uint32_t EraseStartAddress ,uint32_t EraseEndAddress);
uint8_t CSP_QSPI_WriteMemory(uint8_t* buffer, uint32_t address, uint32_t buffer_size);
uint8_t CSP_QSPI_EnableMemoryMappedMode(void);
uint8_t CSP_QSPI_Erase_Chip (void);
uint8_t QSPI_AutoPollingMemReady_ChipErase(void);

/* USER CODE BEGIN Prototypes */


/* USER CODE BEGIN Prototypes */
/*MX25L512 memory parameters*/
#define MEMORY_FLASH_SIZE				0x1000000 /* 128 MBits => 16MBytes */
#define MEMORY_BLOCK_SIZE				0x10000   /* 256 sectors of 64KBytes */
#define MEMORY_SECTOR_SIZE				0x1000    /* 4096 subsectors of 4kBytes */
#define MEMORY_PAGE_SIZE				0x100     /* 65536 pages of 256 bytes */



/*MX25L512 commands */
#define WRITE_ENABLE_CMD                 0x06
#define READ_STATUS_REG_CMD              0x05
#define READ_STATUS_REG_2_CMD			 0X35
#define WRITE_STATUS_REG_CMD             0x01
#define WRITE_STATUS_REG_2_CMD           0X31
#define SECTOR_ERASE_CMD                 0x20
#define CHIP_ERASE_CMD                   0xC7
#define QUAD_IN_FAST_PROG_CMD            0x32
//#define READ_CONFIGURATION_REG_CMD       0x15
//#define QUAD_READ_IO_CMD                 0xEC
#define QUAD_OUT_FAST_READ_CMD           0x6B
#define QUAD_INOUT_FAST_READ_CMD         0xEB
//#define QPI_ENABLE_CMD                   0x35
#define DUMMY_CLOCK_CYCLES_READ_QUAD       8
#define RESET_ENABLE_CMD                 0x66
#define RESET_EXECUTE_CMD                0x99
//#define DISABLE_QIP_MODE                 0xf5
/* USER CODE END Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __QUADSPI_H__ */

