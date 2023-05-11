/**
 * @file commands.c
 * @author Rafał Kędzierski
 * @brief 
 * @version 0.1
 * @date 2023-05-11
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "clima_cmds.h"
#include "stm32l4xx_hal.h"
#include "gpio.h"
#include "uart.h"

int fn_command_about(const char *pbCmd, clima_command_t **psCmdTab, cmd_struct_t* cmd_struct, clima_cli_print_clbk cprint);
int fn_command_led_on_off(const char *pbCmd, clima_command_t **psCmdTab, cmd_struct_t* cmd_struct, clima_cli_print_clbk cprint);

clima_command_t asLedCmd[];

clima_command_t asMainCmd[] = {
	{"led",		"LED state",	NULL,	asLedCmd,	NULL},
	{"about",	"About demo.",	NULL,	NULL,		fn_command_about},
	{NULL, NULL, NULL, NULL, NULL}
};

clima_command_t asLedCmd[] = {
	{"on",		"Power on",		NULL,	NULL,		fn_command_led_on_off},
	{"off",		"Power off",	NULL,	NULL,		fn_command_led_on_off},
	{NULL, NULL, NULL, NULL, NULL}
};

int fn_command_led_on_off(const char *pbCmd, clima_command_t **psCmdTab, cmd_struct_t* cmd_struct, clima_cli_print_clbk cprint)
{
	if(cmd_struct->args) {
		cprint("ERROR.\n\r");

		return CLIMA_CMD_OK;
	}

	if(cmd_struct->tokens_count == 2) {
		if(cmd_struct->token_idx[1] == 1) {
			cprint("LED is turned on.\n\r");
			HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
		} else {
			cprint("LED is turned off.\n\r");
			HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);			
		}
	} else {
		cprint("ERROR.\n\r");
	}

	return CLIMA_CMD_OK;
}

int fn_command_about(const char *pbCmd, clima_command_t **psCmdTab, cmd_struct_t* cmd_struct, clima_cli_print_clbk cprint)
{
	if(cmd_struct->args) {
		cprint("ERROR.\n\r");

		return CLIMA_CMD_OK;
	}

	cprint("\n\rThis is a demo of EDDY and CLIMA\n\rlibraries runs on STM32 MCU.\n\r\n\r");
	cprint("Check the:\n\rhttps://github.com/rkedzierski/eddy\n\rhttps://github.com/rkedzierski/clima\n\r\n\r");

	return CLIMA_CMD_OK;
}