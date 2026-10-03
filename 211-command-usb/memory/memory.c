#include <stdio.h>
#include <stdint.h>
#include "hardware/regs/addressmap.h"
#include "pico/stdlib.h"
#include "memory.h"

extern char __flash_binary_start;
extern char __flash_binary_end;
extern char __boot2_start__;
extern char __boot2_end__;
extern char __etext;
extern char __data_start__;
extern char __data_end__;
extern char __bss_start__;
extern char __bss_end__;
extern char __HeapLimit;
extern char __StackBottom;
extern char __StackTop;

#define SRAM_SIZE_BYTES (264 * 1024)
#define ROM_SIZE_BYTES (16 * 1024)

static void row(const char *name, uintptr_t start, uintptr_t end)
{
    printf("%-10s 0x%08x 0x%08x %8u\n",
           name, (unsigned)start, (unsigned)end, (unsigned)(end - start));
}

void mem_info(void)
{
    uintptr_t flash_end   = XIP_BASE + PICO_FLASH_SIZE_BYTES;
    uintptr_t image_start = (uintptr_t)&__flash_binary_start;
    uintptr_t image_end   = (uintptr_t)&__flash_binary_end;
    uintptr_t boot2_start = (uintptr_t)&__boot2_start__;
    uintptr_t boot2_end   = (uintptr_t)&__boot2_end__;
    uintptr_t etext       = (uintptr_t)&__etext;
    uintptr_t data_start  = (uintptr_t)&__data_start__;
    uintptr_t data_end    = (uintptr_t)&__data_end__;
    uintptr_t bss_start   = (uintptr_t)&__bss_start__;
    uintptr_t bss_end     = (uintptr_t)&__bss_end__;
    uintptr_t heap_limit  = (uintptr_t)&__HeapLimit;
    uintptr_t stack_bot   = (uintptr_t)&__StackBottom;
    uintptr_t stack_top   = (uintptr_t)&__StackTop;

    uintptr_t data_size  = data_end - data_start;
    uintptr_t bss_size   = bss_end - bss_start;
    uintptr_t heap_size  = heap_limit - bss_end;
    uintptr_t stack_size = stack_top - stack_bot;

    printf("area       start      end        size\n");
    row("flash", XIP_BASE, flash_end);
    row("sram", SRAM_BASE, SRAM_BASE + SRAM_SIZE_BYTES);
    row("rom", ROM_BASE, ROM_BASE + ROM_SIZE_BYTES);
    row("image", image_start, image_end);
    row("free", image_end, flash_end);
    row("boot2", boot2_start, boot2_end);
    row("text", boot2_end, etext);
    row("data flash", etext, etext + data_size);   
    row("data ram", data_start, data_end);         
    row("bss", bss_start, bss_end);
    row("heap", bss_end, heap_limit);
    row("stack", stack_bot, stack_top);

    printf("\ntotal\n");
    printf("  flash image %8u = boot2 %u + text %u + data %u\n",
           (unsigned)(image_end - image_start),
           (unsigned)(boot2_end - boot2_start),
           (unsigned)(etext - boot2_end),
           (unsigned)data_size);
    printf("  flash free  %8u of %u\n",
           (unsigned)(flash_end - image_end),
           (unsigned)PICO_FLASH_SIZE_BYTES);
    printf("  ram used    %8u = data %u + bss %u\n",
           (unsigned)(data_size + bss_size),
           (unsigned)data_size,
           (unsigned)bss_size);
    printf("  ram free    %8u for heap and %u for stack\n",
           (unsigned)heap_size,
           (unsigned)stack_size);
}