/*******************************************************************************
  Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This file contains the "main" function for a project.

  Description:
    This file contains the "main" function for a project.  The
    "main" function calls the "SYS_Initialize" function to initialize the state
    machines of all modules in the system
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include <stdint.h>
#include <string.h>
#include "definitions.h"                // SYS function prototypes

// *****************************************************************************
// *****************************************************************************
// Section: Local Constants
// *****************************************************************************
// *****************************************************************************

#define NRF905_PAYLOAD_SIZE      32
static const uint8_t nrf905_tx_address[4] = { 0x01, 0x02, 0x03, 0x04 };

// *****************************************************************************
// *****************************************************************************
// Section: Local Functions
// *****************************************************************************
// *****************************************************************************

static void nrf905_delay(void)
{
    for (volatile uint32_t i = 0; i < 60U; i++)
    {
        __asm volatile("nop");
    }
}

static void nrf905_select(void)
{
    NRF_CSN_Clear();
    nrf905_delay();
}

static void nrf905_deselect(void)
{
    NRF_CSN_Set();
    nrf905_delay();
}

static uint8_t nrf905_spi_transfer(uint8_t value)
{
    uint8_t received = 0;

    for (uint8_t bit = 0; bit < 8U; bit++)
    {
        if (value & 0x80U)
        {
            NRF_MOSI_Set();
        }
        else
        {
            NRF_MOSI_Clear();
        }

        nrf905_delay();
        NRF_SCLK_Set();
        nrf905_delay();

        received = (uint8_t)((received << 1) | NRF_MISO_Get());

        NRF_SCLK_Clear();
        nrf905_delay();
        value <<= 1;
    }

    return received;
}

static void nrf905_write_tx_address(const uint8_t *address)
{
    nrf905_select();
    nrf905_spi_transfer(0x22); // Write TX address command for nRF905

    for (uint8_t i = 0; i < sizeof(nrf905_tx_address); i++)
    {
        nrf905_spi_transfer(address[i]);
    }

    nrf905_deselect();
}

static void nrf905_write_tx_payload(const uint8_t *payload, uint8_t length)
{
    nrf905_select();
    nrf905_spi_transfer(0x20); // Write TX payload command for nRF905

    for (uint8_t i = 0; i < length; i++)
    {
        nrf905_spi_transfer(payload[i]);
    }

    for (uint8_t i = length; i < NRF905_PAYLOAD_SIZE; i++)
    {
        nrf905_spi_transfer(0x00);
    }

    nrf905_deselect();
}

static void nrf905_start_transmit(void)
{
    NRF_TX_EN_Set();
    nrf905_delay();
    NRF_TRX_CE_Set();
    SYSTICK_DelayMs(10);
    NRF_TRX_CE_Clear();
    NRF_TX_EN_Clear();
}

static void nrf905_init(void)
{
    NRF_MOSI_OutputEnable();
    NRF_SCLK_OutputEnable();
    NRF_CSN_OutputEnable();
    NRF_TX_EN_OutputEnable();
    NRF_TRX_CE_OutputEnable();
    NRF_PWR_UP_OutputEnable();
    NRF_MISO_InputEnable();

    NRF_CSN_Set();
    NRF_SCLK_Clear();
    NRF_MOSI_Clear();
    NRF_TX_EN_Clear();
    NRF_TRX_CE_Clear();
    NRF_PWR_UP_Clear();

    NRF_PWR_UP_Set();
    SYSTICK_DelayMs(10);

    nrf905_write_tx_address(nrf905_tx_address);
}

void send_nrf(const char *message)
{
    if (message == NULL)
    {
        return;
    }

    uint8_t payload[NRF905_PAYLOAD_SIZE] = { 0 };
    size_t length = strnlen(message, NRF905_PAYLOAD_SIZE);

    memcpy(payload, message, length);
    nrf905_write_tx_payload(payload, (uint8_t)length);
    nrf905_start_transmit();
}

// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************

int main ( void )
{
    /* Initialize all modules */
    SYS_Initialize ( NULL );
    SYSTICK_TimerStart();

    nrf905_init();

    while ( true )
    {
        /* Maintain state machines of all polled MPLAB Harmony modules. */
        SYS_Tasks();
        LED_Set();
        send_nrf("FOXHUNT");
        SYSTICK_DelayMs(300);
        LED_Clear();
        SYSTICK_DelayMs(1000);
    }

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}


/*******************************************************************************
 End of File
*/

