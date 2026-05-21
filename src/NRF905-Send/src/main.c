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
#include "definitions.h"                // SYS function prototypes

// ************ constants ************
const int ADDRES = {0xff, 0xff, 0xff, 0xff};
const int OUTPUT_POWER = 0; //0-3
const int OUTPUT_FREQUENCY = 0; //0-3

void InitNRF () {
    // Setup for programming
    NRFPowerDown();
    // Set address and AUTO_RETRAN
    NRF_CSN_Set();
    
}

void NRFPowerDown () {
    NRF_PWR_UP_Clear();
    NRF_TRX_CE_Clear();
    NRF_TX_EN_Clear();
   
}

void  NRFWakeUp(){
    //Power Up for Send
    NRF_PWR_UP_Set();
    NRF_TRX_CE_Clear();
    NRF_TX_EN_Set();
}

void SendKey ( int n ){
    // Flow: sleep>interupt>sendinfo>repeat
    // sendinfo = NRFWakeUp>SPIBufferData4Send>Send 'em
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
    SERCOM4_SPI_Initialize();

    while ( true )
    {
        /* Maintain state machines of all polled MPLAB Harmony modules. */
        SYS_Tasks ( );
    }

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}


/*******************************************************************************
 End of File
*/

