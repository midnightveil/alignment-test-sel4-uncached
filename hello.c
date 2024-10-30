/*
 * Copyright 2021, Breakaway Consulting Pty. Ltd.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */
#include <stdint.h>
#include <microkit.h>

volatile uint8_t *example_cached;
volatile uint8_t *example_uncached;

char *data_string = "hello world!!!!\n";
const int data_len = 5+1+5+4+5+1;

void init(void)
{
    microkit_dbg_puts("writing to cached\n");
    __builtin_memcpy((void *)&example_cached[0], data_string, data_len);
    microkit_dbg_puts("writing to uncached\n");
    __builtin_memcpy((void *)&example_uncached[0], data_string, data_len);

    microkit_dbg_puts("done\n");
}

void notified(microkit_channel ch)
{
}
