# Reverse Engineering
## SAMD20 x NRF805
| SAMD20G18 | NRF905 |
| --------- | ------ |
| PB08      | MOSI   |
| PB09      | SCLK   |
| PB10      | CSN    |
| PB11      | MISO   |
| PA16      | DR     |
| PA17      | AM     |
| PA18      | CD     |
| PA19      | PWR_UP |
| PA20      | TRX_CE |
| PA21      | TX_EN  |

## CON2 (Programing)
| pin fn            | 0   | 1     | 2     | 3   | 4     | 5   | 6   |
| ----------------- | --- | ----- | ----- | --- | ----- | --- | --- |
| Connection to SAM | GND | nRST  | PA30  | VCC | PA30  | NC  | NC  |
| pin function      | GND | reset | SWDIO | 3V3 | SWCLK |     |     |
| Pico conenction   | GND | 16    | 14    | 3V3 | 19    |     |     |

## Other
| SAMD20G18 | Function  |
| --------- | --------- |
| PB22      | nLED      |
| IREF      | Pulled up |

## Buttons
IDK
