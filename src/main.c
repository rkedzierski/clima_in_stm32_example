/**
 * @file main.c
 * @author Rafał Kędzierski
 * @brief 
 * @version 0.1
 * @date 2023-05-11
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "stm32l4xx_hal.h"
#include "gpio.h"
#include "uart.h"
#include "dma.h"
#include "eddy.h"
#include "clima.h"

eddy_t eddy;
clima_t clima;
char aRxBuffer[2];

extern UART_HandleTypeDef huart2;
extern volatile bool ready_in;
extern clima_command_t asMainCmd[];

void SystemClock_Config(void);
void Error_Handler(void);

void check_hint(char* cmd_line)
{
    clima.check_command(&clima, cmd_line);
}

eddy_retv_t exec_command(const char* cmd_line)
{
    clima.exec_command(&clima, cmd_line);

	return EDDY_RETV_OK;
}

int main(void)
{
	HAL_Init();
	SystemClock_Config();
	GPIO_Init();
    DMA_Init();
	UART_Init();

	init_eddy(&eddy);
	init_clima(&clima);

	eddy.set_cli_print_clbk(&eddy, UART_print);
	eddy.set_check_hint_clbk(&eddy, check_hint);
	eddy.set_exec_cmd_clbk(&eddy, exec_command);

    clima.set_cli_print_clbk(&clima, UART_print);
    clima.set_cmds_root(&clima, asMainCmd);

	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
	UART_print("Hello in Eddy demo!!!\n\r");

	eddy.show_prompt(&eddy);

	while(1) {
		ready_in = false;

		if(HAL_UART_Receive_IT(&huart2, (uint8_t *)aRxBuffer, 1) != HAL_OK) {
			Error_Handler();
		}

		while(ready_in == false) {
		}

		eddy.put_char(&eddy, aRxBuffer[0]);
	}
}

void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

	/** Configure the main internal regulator output voltage
	*/
	if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK) {
		Error_Handler();
	}

	/** Initializes the RCC Oscillators according to the specified parameters
	* in the RCC_OscInitTypeDef structure.
	*/
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLM = 1;
	RCC_OscInitStruct.PLL.PLLN = 10;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
	RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
	RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	*/
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
															|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK) {
		Error_Handler();
	}
}

void Error_Handler(void)
{
	__disable_irq();
	while(1) {
		HAL_Delay(100);
		HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
	}
}