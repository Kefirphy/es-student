#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include <stdio.h>
#include "led.h"
#include "log.h"

const uint BUTTON_PIN = 24;

const uint DEBOUNCE_MS = 20;

bool get_button_debounce(uint pin)
{
    bool state = gpio_get(pin);
    sleep_ms(DEBOUNCE_MS);
    return state && gpio_get(pin);

}

void handle_command(int command)
{
    if (command == 'e')
    {
        led_set(true);
        LOG_INF("led %s\n", led_is_on() ? "on" : "off");
    }
    else if (command == 'd')
    {
        led_set(false);
        LOG_INF("led %s\n", led_is_on() ? "on" : "off");
    }
    else if (command == 'v')
    {
        log_version();
    }
    else
    {
        LOG_ERR("unknown command: %c\n", command);
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

        int command = getchar_timeout_us(0);

        if (command == PICO_ERROR_TIMEOUT)
        {
            continue;
        }

        LOG_DBG("got %c\n", command);
        handle_command(command);
    }
}