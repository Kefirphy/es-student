#include "pico/stdlib.h"
#include "hardware/gpio.h"

const uint LED_PIN = 25;
const uint BUTTON_PIN = 15;

int main()
{
    // Инициализация светодиода на плате
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    // Инициализация кнопки
    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN); // Теперь пин работает на прием сигнала
    gpio_pull_up(BUTTON_PIN);          // Включаем подтягивающий резистор

    bool led = false;
    bool previous = false;

    while (1)
    {
        // Читаем текущее напряжение на пине кнопки
        bool current = gpio_get(BUTTON_PIN);

        // Если раньше было напряжение (кнопка отпущена), а теперь нет (кнопка нажата)
        if (previous == true && current == false)
        {
            led = !led;             // Меняем флаг светодиода на противоположный
            gpio_put(LED_PIN, led); // Применяем флаг к реальному светодиоду
        }

        previous = current; // Запоминаем состояние для следующего витка цикла
    }
}