#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include <stdio.h>
#include "lcd1602.h"

#define INT_GPIO 2
#define PERIOD_SEC 4
#define PERIODS 5
#define AVG_SEC (PERIOD_SEC * PERIODS)

// Used to convert CPM to uSv/h
// Not sure about the value, others say it can be as high as 151
#define CONVERSION_RATIO 124

static uint32_t count = 0;

void gpio_callback(unsigned int gpio, long unsigned int events)
{
    count++;
}

/*
    Calculate moving average of the last AVG_SEC seconds.
 */
uint32_t get_cpm(uint32_t prev_counts[PERIODS], size_t pos)
{
    uint32_t new_count = count;

    uint32_t d_count = new_count - prev_counts[pos];
    prev_counts[pos] = new_count;

    return d_count * (60 / AVG_SEC);
}

int main()
{
    // Init START
    stdio_init_all();

    i2c_init(i2c_default, 100 * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
    bi_decl(bi_2pins_with_func(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C));

    lcd_init();

    gpio_set_irq_enabled_with_callback(INT_GPIO, GPIO_IRQ_EDGE_RISE, true, &gpio_callback);
    // Init END

    lcd_set_cursor(0, 0);
    lcd_string("Initializing...");

    uint32_t prev_counts[PERIODS] = {0};

    // Initialize the prev_counts array to start calculating moving average
    for (int i = 0; i < PERIODS; i++)
    {
        char cpm[17] = {0};

        sleep_ms(PERIOD_SEC * 1000);
        prev_counts[i] = count;

        lcd_clear();
        lcd_set_cursor(0, 0);
        lcd_string("Initializing...");

        snprintf(cpm, 16, "Counts: %d", prev_counts[i]);

        lcd_set_cursor(1, 0);
        lcd_string(cpm);
    }

    char line1[17] = {0};
    char line2[17] = {0};
    size_t pos = 0;

    while (1)
    {
        sleep_ms(PERIOD_SEC * 1000);

        uint32_t cpm = get_cpm(prev_counts, pos);

        float uSv = (float)cpm / CONVERSION_RATIO;

        pos = (pos + 1) % PERIODS;

        snprintf(line1, 16, "%d CPM", cpm);
        snprintf(line2, 16, "%.4f uSv/h", uSv);

        printf("%s\n%s\n", line1, line2);

        lcd_clear();
        lcd_set_cursor(0, 0);
        lcd_string(line1);
        lcd_set_cursor(1, 0);
        lcd_string(line2);
    }
}