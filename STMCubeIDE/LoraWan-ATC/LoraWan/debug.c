/*
 * Copyright (c) 2014-2016 IBM Corporation.
 * All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *  * Neither the name of the <organization> nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "debug.h"
#include "usart.h"
#include <stdint.h>
#define myUART huart1   //  <--------- change to your setup

/*  ************************************** */
/*    DO NOT CHANGE BELOW THIS LINE        */
/*  ************************************** */

void debug_init () {
    // configure LED pin as output
    debug_led(0);

    // configure USART1 (115200/8N1, tx-only)

    // print banner
    debug_str("\r\n============== DEBUG STARTED ==============\r\n");
}

void debug_led (int val) {
   // HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,val);
}

void debug_char (char c) {
  char buffer[] = "";
  buffer[0]= c;
  HAL_UART_Transmit(&myUART,buffer,sizeof(buffer),HAL_MAX_DELAY);
}

void debug_hex (uint8_t b) {
    debug_char("0123456789ABCDEF"[b>>4]);
    debug_char("0123456789ABCDEF"[b&0xF]);
}

void debug_buf (uint8_t* buf, int len) {
    while(len--) {
        debug_hex(*buf++);
        debug_char(' ');
    }
    debug_char('\r');
    debug_char('\n');
}

void debug_uint (uint8_t v) {
    for(uint8_t n=24; n>=0; n-=8) {
        debug_hex(v>>n);
    }
}

void debug_int (int v) {
    char buf[10], *p = buf;
    int n = debug_fmt(buf, sizeof(buf), v, 10, 0, 0);
    while(n--)
        debug_char(*p++);
}

void debug_str (const char* str) {
    while(*str) {
        debug_char(*str++);
    }
}

void debug_val (const char* label, uint8_t val) {
    debug_str(label);
    debug_uint(val);
    debug_char('\r');
    debug_char('\n');
}

void debug_valdec (const char* label, uint8_t val) {
    debug_str(label);
    debug_int(val);
    debug_char('\r');
    debug_char('\n');
}

int debug_fmt (char* buf, int max, int val, int base, int width, char pad) {
    char num[33], *p = num, *b = buf;
    int m, v;
    // special handling of negative decimals
    v = (base == 10 && val < 0) ? -val : val;
    // generate digits backwards
    do {
        *p++ = ((m=v%base) <= 9) ? m+'0' : m+'A'-10;
    } while( v /= base );
    // prefix negative decimals with '-'
    if(base == 10 && val < 0) {
        *p++ = '-';
    }
    // add leading zeroes or spaces
    while( b-buf < max-1 && b-buf < width-(p-num) ) {
        *b++ = pad;
    }
    // copy digits and sign forwards
    do *b++ = *--p;
    while( b-buf < max && p > num );
    // return number of characters written
    return b - buf;
}
