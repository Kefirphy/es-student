#pragma once

#define DEVICE_NAME "es-led-module"
#define FIRMWARE_VERSION "1.1.0"

#define DEVICE_PROJECT "221-command-time"
#define DEVICE_REPO "https://github.com/Kefirphy/es-student"

#ifndef DEVICE_BOARD
#define DEVICE_BOARD "unknown"
#endif

void device_info(void);

#include "pico/unique_id.h"
#include <stdint.h>

struct info_t
{
    uint32_t version;
    char name[13];
    uint8_t revision;
};

extern struct info_t device_card;

void dev_info(void);