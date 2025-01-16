#include "hardware/spi.h"
#include "mcp2515.c" // Ensure this is correctly included
#include "pico/stdlib.h"
#include "pico/time.h"
#include <pico/stdlib.h>
#include <stdio.h>

#define SPI_PORT spi0
#define CAN_BAUDRATE 500000
#define PIN_MISO 5
#define PIN_CS_A 7
#define PIN_SCK 6
#define PIN_MOSI 4
#define PICO_DEFAULT_LED_PIN 25

int main() {
  stdio_init_all();

  // Wait for USB serial connection
  while (!stdio_usb_connected()) {
    tight_loop_contents();
  }

  bool led_state = true;

  // Initialize SPI0 at 1MHz for MCP2515
  spi_init(SPI_PORT, 1000 * 1000);
  gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
  gpio_set_function(PIN_SCK, GPIO_FUNC_SPI);
  gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);

  // Initialize Chip Select (CS) pin
  gpio_init(PIN_CS_A);
  gpio_set_dir(PIN_CS_A, GPIO_OUT);
  gpio_put(PIN_CS_A, 1);

  // Initialize onboard LED for debugging
  gpio_init(PICO_DEFAULT_LED_PIN);
  gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
  gpio_put(PICO_DEFAULT_LED_PIN, led_state);

  // Initialize MCP2515 CAN controller
  Mcp2515 canA;
  if (mcp2515_init(&canA, PIN_CS_A, CAN_BAUDRATE, SPI_PORT) != 0) {
    printf("Failed to initialize MCP2515\n");
    return -1;
  }
  mcp2515_setOpmode(&canA, NORMAL_MODE);

  // Prepare CAN message
  CanMessage transmitBuffer = {0};
  transmitBuffer.length = 4; // Sending 4 bytes
  transmitBuffer.extendedIdEnabled = false;
  transmitBuffer.standardId = 666;
  transmitBuffer.isRTR = false;

  uint32_t *data_ptr = (uint32_t *)&transmitBuffer.data;
  *data_ptr = 0; // Initialize data to 0

  CanMessage receiveBuffer = {0};
  uint errors = 0;
  uint transmittedMessages = 0;

  absolute_time_t timeStart = get_absolute_time();
  int64_t averageDelay = 0;

  while (true) {
    printf("TEST\n");
    // Record start time for delay measurement
    absolute_time_t sendStartTime = get_absolute_time();

    // Send message
    // mcp2515_sendMessageBlocking(&canA, &transmitBuffer);

    // Receive message
    // mcp2515_recieveMessageBlocking(&canA, &receiveBuffer);

    // Validate received message
    // if (*(uint32_t *)transmitBuffer.data != *(uint32_t *)receiveBuffer.data
    // || transmitBuffer.standardId != receiveBuffer.standardId ||
    // transmitBuffer.length != receiveBuffer.length) {
    // errors++;
    // }

    // Calculate round-trip delay
    int64_t delay = absolute_time_diff_us(sendStartTime, get_absolute_time());
    averageDelay = (averageDelay + delay) / 2;

    // Update transmitted message count
    transmittedMessages++;

    // Increment data for the next transmission
    (*data_ptr)++;

    // Print statistics every second
    if (absolute_time_diff_us(timeStart, get_absolute_time()) > 1000000) {
      timeStart = get_absolute_time();
      printf("Speed: %d Kbyte/s, Errors: %d, Avg Delay: %lld us\n",
             (transmittedMessages * 8) / 1000, errors, averageDelay);
      transmittedMessages = 0;

      // led is also blinking
      led_state = !led_state;
      gpio_put(PIN_CS_A, led_state);
    }
  }

  return 0;
}
