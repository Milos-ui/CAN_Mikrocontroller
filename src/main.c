
#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/time.h"
#include "hardware/spi.h"
#include "mcp2515.c"

#define SPI_PORT spi0
#define CAN_BAUDRATE 500000
#define PIN_MISO 4
#define PIN_CS_A 5
#define PIN_CS_B 3
#define PIN_SCK 6
#define PIN_MOSI 7



int main(){
    stdio_init_all();

    // This example will use SPI0 at 10MHz, wich is the maximum of the MCP2515.
    spi_init(SPI_PORT, 1000 * 1000);
    gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
    gpio_set_function(PIN_SCK, GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);

    // Chip select is active-low, so we'll initialise it to a driven-high state
    gpio_init(PIN_CS_A);
    gpio_set_dir(PIN_CS_A, GPIO_OUT);
    gpio_put(PIN_CS_A, 1);

    gpio_init(PIN_CS_B);
    gpio_set_dir(PIN_CS_B, GPIO_OUT);
    gpio_put(PIN_CS_B, 1);

    // Set CAN baudrate to 1MHz
    Mcp2515 canA;
    mcp2515_init(&canA, PIN_CS_A, CAN_BAUDRATE, SPI_PORT);
    // Controller is in listen-only mode after initialization.
    // Set it to normal mode to send out messages.
    mcp2515_setOpmode(&canA, NORMAL_MODE);
    
    Mcp2515 canB;
    mcp2515_init(&canB, PIN_CS_B, CAN_BAUDRATE, SPI_PORT);
    mcp2515_setOpmode(&canB, NORMAL_MODE);



    // Speedtest

    CanMessage buffer = {0};
    buffer.length = 8;
    buffer.extendedIdEnabled = false;
    buffer.extendedId = 0b00000000000000111111111111111111;
    buffer.canStandardId = 666;
    buffer.isRTS = true;

    CanMessage RecieveBuffer = {0};

    uint32_t* val_ptr = (uint32_t*)&buffer.data;
    *val_ptr = 0;

    uint32_t lastVal = 0;
    uint errors = 0;
    uint transmittedMessages = 0;

    absolute_time_t timeStart = get_absolute_time();
    absolute_time_t timeStart1 = get_absolute_time();

    uint32_t lastExtendedID = buffer.extendedId;

    int64_t delay;

    while (1)
    {
        *val_ptr = *val_ptr + 1;
        timeStart1 = get_absolute_time();
        mcp2515_sendMessageBlocking(&canA, &buffer);
        mcp2515_recieveMessageBlocking(&canB, &RecieveBuffer);
        delay = (delay + absolute_time_diff_us(timeStart1, get_absolute_time())) / 2;
        sleep_ms(10);
        if (lastVal + 1 != *val_ptr) {
            errors++;
        }
        lastVal = *val_ptr;
        if (absolute_time_diff_us(timeStart, get_absolute_time()) > 1000000) {
            timeStart = get_absolute_time();
            printf("%d Kbyte/s    ", (transmittedMessages*8)/1000);
            transmittedMessages = 0;
            printf("Errors %d    ", errors);
            printf("Delay %d us\n", (uint32_t)delay);
        }
        if (lastExtendedID != buffer.extendedId) {
            printf("failed to transmit extended id");
        }
        lastExtendedID = buffer.extendedId;
        transmittedMessages++;
    }

    return 0;
}