/**
 * @file uart.h
 * @author Rafał Kędzierski
 * @brief 
 * @version 0.1
 * @date 2023-05-11
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef __UART_H__
#define __UART_H__

/**
 * @brief This function inicializes UARTs.
 * 
 */
void UART_Init(void);

/**
 * @brief This function prints to uart.
 * 
 * @param str is a pointer to buffer with zero ended string
 */
void UART_print(const char* str);

#endif /* __UART_H__ */
