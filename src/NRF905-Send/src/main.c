/*******************************************************************************
  Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    NRF905 Driver with Button Control

  Description:
    Minimal NRF905 transceiver driver using HW SPI (SERCOM4).
    Sends button index (0-3) when button is pressed.
 *******************************************************************************/

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "definitions.h"

// ============================================================================
// CONFIGURATION
// ============================================================================
#define NRF905_PAYLOAD_SIZE     32
static const uint8_t NRF_ADDRESS[4] = { 0xff, 0xff, 0xff, 0xff };
static const uint8_t OUTPUT_POWER = 0;       // 0-3
static const uint8_t OUTPUT_FREQUENCY = 0;   // 0-3

// ============================================================================
// NRF905 SPI HELPERS
// ============================================================================
static uint8_t nrf905_spi_transfer(uint8_t data) {
    NRF_CSN_Clear();
    uint8_t rxData = 0;
    SERCOM4_SPI_Write(&data, 1);
    SERCOM4_SPI_Read(&rxData, 1);
    NRF_CSN_Set();
    return rxData;
}

// ============================================================================
// NRF905 CORE FUNCTIONS
// ============================================================================
static void nrf905_write_config(void) {
    // CONFIG_0: bits 0-1 = TX_POWER, bits 4-5 = HFREQ_PLL
    uint8_t config_0 = (OUTPUT_POWER & 0x03) | ((OUTPUT_FREQUENCY & 0x01) << 4);
    
    nrf905_spi_transfer(0x20);  // Write config command
    nrf905_spi_transfer(config_0);
}

static void nrf905_write_tx_address(const uint8_t *address) {
    nrf905_spi_transfer(0x22);  // TX addr write command
    for (uint8_t i = 0; i < 4; i++) {
        nrf905_spi_transfer(address[i]);
    }
}

static void nrf905_write_tx_payload(uint8_t data) {
    uint8_t payload[NRF905_PAYLOAD_SIZE] = { 0 };
    payload[0] = data;
    
    nrf905_spi_transfer(0x20);  // TX payload write command
    for (uint8_t i = 0; i < NRF905_PAYLOAD_SIZE; i++) {
        nrf905_spi_transfer(payload[i]);
    }
}

static void nrf905_start_transmit(void) {
    NRF_TX_EN_Set();
    NRF_TRX_CE_Set();
    SYSTICK_DelayMs(10);
    NRF_TRX_CE_Clear();
    NRF_TX_EN_Clear();
}

static void nrf905_init(void) {
    // Configure pins as outputs
    NRF_CSN_OutputEnable();
    NRF_TX_EN_OutputEnable();
    NRF_TRX_CE_OutputEnable();
    NRF_PWR_UP_OutputEnable();
    
    // Initialize pins to safe state
    NRF_CSN_Set();
    NRF_TX_EN_Clear();
    NRF_TRX_CE_Clear();
    NRF_PWR_UP_Clear();
    
    // Power up module
    NRF_PWR_UP_Set();
    SYSTICK_DelayMs(10);
    
    // Configure TX address
    nrf905_write_tx_address(NRF_ADDRESS);
    
    // Configure power and frequency
    nrf905_write_config();
}

// ============================================================================
// SEND FUNCTION
// ============================================================================
static void send_button_index(uint8_t button_index) {
    nrf905_write_tx_payload(button_index);
    nrf905_start_transmit();
}

// ============================================================================
// BUTTON READING
// ============================================================================
// TODO: Define button pins per your hardware. Example:
// #define BTN_0_Get()  (BTN_0 macro from plib_port.h)
// #define BTN_1_Get()  (BTN_1 macro from plib_port.h)
// #define BTN_2_Get()  (BTN_2 macro from plib_port.h)
// #define BTN_3_Get()  (BTN_3 macro from plib_port.h)

static uint8_t read_button_pressed(void) {
    // Check which button is pressed (active low assumed)
    /*
    if (!BTN_0_Get()) return 0;
    if (!BTN_1_Get()) return 1;
    if (!BTN_2_Get()) return 2;
    if (!BTN_3_Get()) return 3;
    return 0xFF;  // No button pressed
    */
    return 0;  // Placeholder until button pins are defined
}

// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************

int main(void) {
    /* Initialize system */
    SYS_Initialize(NULL);
    SYSTICK_TimerStart();
    SERCOM4_SPI_Initialize();

    /* Initialize NRF905 */
    nrf905_init();

    uint8_t last_button = 0xFF;
    uint32_t button_hold_time = 0;

    while (true) {
        SYS_Tasks();
        
        uint8_t current_button = read_button_pressed();
        
        if (current_button != 0xFF) {
            // Button is pressed
            if (current_button != last_button) {
                // New button press detected
                last_button = current_button;
                button_hold_time = 0;
                LED_Set();
            }
            
            // Send button index every 300ms while held
            button_hold_time++;
            if (button_hold_time > 30) {  // Approx 300ms (10ms * 30)
                send_button_index(current_button);
                button_hold_time = 0;
            }
        } else {
            LED_Clear();
            last_button = 0xFF;
            button_hold_time = 0;
        }
        
        SYSTICK_DelayMs(10);
    }

    return (EXIT_FAILURE);
}

/*******************************************************************************
 End of File
*/

