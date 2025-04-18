/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "memorymap.h"
#include "quadspi.h"
#include "gpio.h"
#include <string.h>

#include "w25q_mem.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define SECTORS_COUNT 100
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* Enable the CPU Cache */

  /* Enable I-Cache---------------------------------------------------------*/
  // SCB_EnableICache();

  /* Enable D-Cache---------------------------------------------------------*/
  // SCB_EnableDCache();

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_QUADSPI_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */
  uint8_t buffer_test[MEMORY_SECTOR_SIZE] = {0};
  uint32_t var = 0;

  W25Q_STATE state = W25Q_OK;
  state = W25Q_Init();
  state = W25Q_EraseSector(0);
  // make test data
	u8_t byte = 0x65;
	u8_t byte_read = 0;
	u8_t in_page_shift = 0;
	u8_t page_number = 0;
	// write data
	state = W25Q_ProgramByte(byte, in_page_shift, page_number);
	// read data
	state = W25Q_ReadByte(&byte_read, in_page_shift, page_number);
  

  	// 

    HAL_StatusTypeDef ret = HAL_OK;
    uint8_t reg[10] = { 0 };

    if (HAL_QSPI_DeInit(&hqspi) != HAL_OK) {
      return HAL_ERROR;
    }
    MX_QUADSPI_Init();

    QSPI_CommandTypeDef sCommand = { 0 };
    sCommand.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    sCommand.Instruction = READ_STATUS_REG_3_CMD;
    sCommand.AddressMode = QSPI_ADDRESS_NONE;
    sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    sCommand.DataMode = QSPI_DATA_1_LINE;
    sCommand.DummyCycles = 0;
    sCommand.DdrMode = QSPI_DDR_MODE_DISABLE;
    sCommand.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
    sCommand.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
    sCommand.NbData = 1;
    QSPI_WriteEnable();
    // while(true){
      ret = HAL_QSPI_Command(&hqspi,&sCommand,HAL_QPSI_TIMEOUT_DEFAULT_VALUE);
      ret = HAL_QSPI_Receive(&hqspi, reg, HAL_QPSI_TIMEOUT_DEFAULT_VALUE);
    // }
    // CSP_QUADSPI_Init();

    sCommand.Instruction = READ_STATUS_REG_CMD;
    ret = HAL_QSPI_Command(&hqspi,&sCommand,HAL_QPSI_TIMEOUT_DEFAULT_VALUE);
    ret = HAL_QSPI_Receive(&hqspi, reg, HAL_QPSI_TIMEOUT_DEFAULT_VALUE);

    sCommand.Instruction = READ_STATUS_REG_2_CMD;
    ret = HAL_QSPI_Command(&hqspi,&sCommand,HAL_QPSI_TIMEOUT_DEFAULT_VALUE);
    ret = HAL_QSPI_Receive(&hqspi, reg, HAL_QPSI_TIMEOUT_DEFAULT_VALUE);


    sCommand.Instruction = READ_STATUS_REG_3_CMD;
    ret = HAL_QSPI_Command(&hqspi,&sCommand,HAL_QPSI_TIMEOUT_DEFAULT_VALUE);
    ret = HAL_QSPI_Receive(&hqspi, reg, HAL_QPSI_TIMEOUT_DEFAULT_VALUE);

    reg[0] = 0x41;
    reg[1] = 0xF1;
    ret = CSP_QSPI_WriteMemory(reg, 0x00001000, 2);
    ret = CSP_QSPI_Read(buffer_test, 0x00001000, 2);
    if (buffer_test[0] != 0x41 || buffer_test[1] != 0xF1) {
      while (1)
        ; //breakpoint - error detected
    }

  	for (var = 0; var < MEMORY_SECTOR_SIZE; var++) {
  		buffer_test[var] = (var & 0xff);
  	}

  	for (var = 0; var < SECTORS_COUNT; var++) {
      // CSP_QSPI_EraseSector
  		if (CSP_QSPI_EraseSector(var * MEMORY_SECTOR_SIZE,
  				(var + 1) * MEMORY_SECTOR_SIZE - 1) != HAL_OK) {

  			while (1)
  				;  //breakpoint - error detected
  		}

  		if (CSP_QSPI_WriteMemory(buffer_test, var * MEMORY_SECTOR_SIZE,
  				sizeof(buffer_test)) != HAL_OK) {

  			while (1)
  				;  //breakpoint - error detected
  		}

  	}

  	if (CSP_QSPI_EnableMemoryMappedMode() != HAL_OK) {

  		while (1)
  			; //breakpoint - error detected
  	}
  
  	for (var = 0; var < SECTORS_COUNT; var++) {
      uint8_t *pointer = (uint8_t*) (0x90000000 + var * MEMORY_SECTOR_SIZE);
  		if (memcmp(buffer_test,pointer,MEMORY_SECTOR_SIZE) != HAL_OK) {
  			while (1)
  				;  //breakpoint - error detected - otherwise QSPI works properly
  		}
  	}
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 2;
  RCC_OscInitStruct.PLL.PLLN = 160;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 8;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM7 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM7) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
