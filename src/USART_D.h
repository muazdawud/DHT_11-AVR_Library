/*
 * USART_D Library - High-performance USART communication
 * * Copyright (c) 2026 Dauda Muazu Sulaiman
 * * Copyright (c) 2026 KibrisOrder                <https://ss.kibrisorder.com>
 * * Author: Dauda Muazu Sulaiman
 * * Organization: KibrisOrder
 * * All rights reserved.
 * * This software is licensed under the MIT License.
 * See the LICENSE file in the project root for full license information.
 */


#ifndef _USART_D_H
#define _USART_D_H

#ifndef BAUD                          /* if not defined in Makefile... */
#define BAUD  9600                     /* set a safe default baud rate */
#endif


void USART_begin(void);

char USART_getByte(void);

void USART_flush(void);

const char* USART_getString(void);

void USART_print(const char buffer[], ...);


#endif