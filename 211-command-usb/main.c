#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include <stdio.h>
#include "led.h"
#include "log.h"
#include "device.h"
#include <string.h>
#include <memory.h>

const uint BUTTON_PIN = 24;

#define LINE_SIZE 32
static char line[LINE_SIZE];
static uint line_length = 0;


const uint DEBOUNCE_MS = 20;

typedef void (*command_handler_t)(void);
struct command_t
{
    const char *name;
    command_handler_t handler;
};


void cmd_enable(void)
{
    led_set(true);
    LOG_INF("led %s\n", led_is_on() ? "on" : "off");
}

void cmd_disable(void)
{
    led_set(false);
    LOG_INF("led %s\n", led_is_on() ? "on" : "off");
}

void cmd_info(void)
{
    device_info();
}

void cmd_version(void)
{
    log_version();
}

void cmd_ping(void)
{
    printf("pong\n");
}

void cmd_mem_info(void)
{
    mem_info();
}

const struct command_t commands[] = {
    { "enable", cmd_enable },
    { "disable", cmd_disable },
    { "info", cmd_info },
    { "version", cmd_version },
    { "ping", cmd_ping },
    { "mem_info", cmd_mem_info },
};

#define COMMAND_COUNT (sizeof(commands) / sizeof(commands[0]))

void handle_command(const char *command)
{
    for (uint i = 0; i < COMMAND_COUNT; i++)
    {
        if (strcmp(command, commands[i].name) == 0)
        {
            if (commands[i].handler != NULL)
            {
                commands[i].handler();
            }

            return;
        }
    }

    LOG_ERR("unknown command: %s\n", command);
}

bool get_button_debounce(uint pin)
{
    bool state = gpio_get(pin);
    sleep_ms(DEBOUNCE_MS);
    return state && gpio_get(pin);
}

void read_line(void)
{
    int symbol = getchar_timeout_us(0);

    if (symbol == PICO_ERROR_TIMEOUT)
    {
        return;
    }

    if (symbol == '\r' || symbol == '\n')
    {
        putchar('\n');
        line[line_length] = '\0';

        if (line_length > 0)
        {
            LOG_DBG("got %s\n", line);
            handle_command(line);
        }

        line_length = 0;
        return;
    }

    if (line_length + 1 < LINE_SIZE)
    {
        line[line_length] = (char)symbol;
        line_length = line_length + 1;
        putchar(symbol);
    }
}

int main()
{
    stdio_init_all();

    // Инициализация светодиода на плате
    led_init();
    // Инициализация кнопки
    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN); // Теперь пин работает на прием сигнала
    gpio_pull_up(BUTTON_PIN);          // Включаем подтягивающий резистор

    bool led = false;
    bool previous = false;

    while (1)
    {
        // Читаем текущее напряжение на пине кнопки
         bool current = get_button_debounce(BUTTON_PIN);

        // Если раньше было напряжение (кнопка отпущена), а теперь нет (кнопка нажата)
         if (previous == true && current == false)
        {
            led_toggle();
            LOG_INF("led %s\n", led_is_on() ? "on" : "off");
        }

        previous = current; // Запоминаем состояние для следующего витка цикла

        read_line();
    }
}