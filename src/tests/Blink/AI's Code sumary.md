This C program is a simple embedded application designed to blink an LED connected to pin PB22 on an ATSAMD20G18 microcontroller (a low-power ARM Cortex-M0+ chip from Microchip/Atmel). It uses direct register manipulation to control the hardware, which is common in bare-metal embedded programming without an operating system or HAL (Hardware Abstraction Layer) library.

I'll break it down section by section, explaining the purpose, key concepts, and how it works. The code assumes you're familiar with basic C, but I'll highlight embedded-specific details.

### 1. Header and Includes
```c
/**
 * @file main.c
 * @author tomas
 * @date 2026-05-13
 * @brief Blink LED on PB22 for ATSAMD20G18
 */

#include <stdint.h>
```
- **Purpose**: Standard C header for fixed-width integer types (e.g., `uint32_t`, `uintptr_t`). These are used for memory addresses and register values to ensure portability and avoid undefined behavior on different architectures.
- **Why needed**: Embedded code often deals with specific bit widths (e.g., 32-bit registers), so `stdint.h` ensures consistency.

### 2. Register and Peripheral Definitions
```c
#define PORT_GROUP_BASE   0x41004400U
#define PORT_GROUP_SIZE   0x80U
#define PORT_GROUP(n)     (PORT_GROUP_BASE + ((n) * PORT_GROUP_SIZE))

#define PORT_DIRSET       0x08U
#define PORT_OUTCLR       0x14U
#define PORT_OUTTGL       0x1CU

#define PM_APBCMASK_ADDR  0x40000414U
#define PM_APBCMASK_PORT  (1u << 0)

#define LED_GROUP         1u          /* PB22 */
#define LED_PIN           22u
#define LED_MASK          (1u << LED_PIN)
```
- **Purpose**: These are preprocessor macros defining memory-mapped register addresses and bit masks for the ATSAMD20G18's peripherals.
  - **PORT (General-Purpose I/O)**: Controls GPIO pins. The SAMD20 has multiple PORT groups (banks of pins). PB22 is in group 1 (PORTB), pin 22.
    - `PORT_GROUP_BASE`: Base address of the PORT peripheral in memory.
    - `PORT_GROUP_SIZE`: Size of each group (128 bytes).
    - `PORT_GROUP(n)`: Calculates the base address for a specific group (e.g., group 1 for PB22).
    - `PORT_DIRSET`: Register offset to set pin direction (input/output). Writing 1 to a bit sets that pin as output.
    - `PORT_OUTCLR`: Register offset to clear pin output (set low). Writing 1 clears the pin.
    - `PORT_OUTTGL`: Register offset to toggle pin output. Writing 1 flips the pin state.
  - **PM (Power Manager)**: Controls clock gating for peripherals.
    - `PM_APBCMASK_ADDR`: Address of the APBC mask register (enables clocks for peripherals on the APBC bus).
    - `PM_APBCMASK_PORT`: Bit mask to enable the PORT peripheral's clock (bit 0).
  - **LED Configuration**: Specific to PB22.
    - `LED_GROUP`: Which PORT group (1 for PORTB).
    - `LED_PIN`: Pin number (22).
    - `LED_MASK`: Bit mask for the pin (1 << 22 = 0x00400000). Used to target only this pin in register operations.
- **Why needed**: Microcontrollers like the SAMD20 use memory-mapped I/O. Registers are treated as variables at specific addresses. Direct access avoids overhead from libraries and gives fine control. These addresses come from the datasheet (e.g., SAMD20G18 datasheet from Microchip).

### 3. Helper Functions for Register Access
```c
static inline void reg_write32(uintptr_t address, uint32_t value)
{
    *(volatile uint32_t *)address = value;
}

static inline void reg_set32(uintptr_t address, uint32_t value)
{
    *(volatile uint32_t *)address |= value;
}
```
- **Purpose**: Safe ways to write to hardware registers.
  - `reg_write32`: Writes a full 32-bit value to a register (overwrites it).
  - `reg_set32`: Sets specific bits in a register (OR operation, preserves other bits).
- **Key details**:
  - `volatile`: Tells the compiler the value can change externally (e.g., by hardware), preventing optimizations that might skip reads/writes.
  - `uintptr_t`: Unsigned integer type large enough to hold a pointer/address.
  - `static inline`: Functions are inlined for performance (no function call overhead) and scoped to this file.
- **Why needed**: Direct pointer dereferencing (`*(uint32_t *)address`) is how you access memory-mapped registers. The `volatile` keyword is critical in embedded code to ensure the compiler doesn't optimize away register accesses.

### 4. Delay Function
```c
static void delay_loop(volatile uint32_t count)
{
    while (count--) {
        __asm__ volatile("nop");
    }
}
```
- **Purpose**: Creates a simple software delay (busy-wait loop).
- **Key details**:
  - Loops `count` times, executing a no-operation (NOP) assembly instruction each time.
  - `volatile` on `count` prevents the compiler from optimizing the loop away.
  - `__asm__ volatile("nop")`: Inline assembly for a NOP (does nothing but wastes cycles).
- **Why needed**: No OS timers here, so delays are done via CPU loops. The NOP ensures consistent timing (each iteration takes roughly the same time). `10000` iterations create a visible blink delay—adjust for faster/slower blinking.

### 5. Main Function
```c
int main(void)
{
    uint32_t group_base = PORT_GROUP(LED_GROUP);

    /* Enable PORT peripheral clock on APBC bus */
    reg_set32(PM_APBCMASK_ADDR, PM_APBCMASK_PORT);

    /* Configure PB22 as output */
    reg_write32(group_base + PORT_DIRSET, LED_MASK);
    reg_write32(group_base + PORT_OUTCLR, LED_MASK);

    while (1) {
        reg_write32(group_base + PORT_OUTTGL, LED_MASK);
        delay_loop(10000);
    }

    return 0;
}
```
- **Purpose**: Initializes the hardware and runs the blink loop forever.
- **Step-by-step breakdown**:
  1. **Calculate group base**: `group_base = PORT_GROUP(1)` → Address for PORTB (group 1).
  2. **Enable clock**: Sets bit 0 in `PM_APBCMASK` to enable the PORT peripheral's clock. Without this, the PORT registers won't work (power-saving feature).
  3. **Configure pin as output**: Writes to `PORT_DIRSET` to set PB22 as an output pin.
  4. **Clear pin (set low)**: Writes to `PORT_OUTCLR` to ensure the LED starts off.
  5. **Infinite loop**: Toggles PB22's output (flips high/low) every ~10,000 NOP cycles, creating a blink. The loop never exits, which is normal for embedded apps (no OS to return to).
- **Why needed**: This is the entry point. The SAMD20 starts executing from `main` after the startup code (in startup_samd20g18.c) initializes the stack, vectors, etc.

### Overall Program Flow and Concepts
- **Initialization**: Clock enable → Pin config → Start low.
- **Runtime**: Toggle pin → Delay → Repeat.
- **Embedded specifics**:
  - **Bare-metal**: No libraries like Arduino's `digitalWrite`—everything is direct register access.
  - **Power management**: Clocks are gated off by default to save power; you must enable them.
  - **GPIO control**: Pins are controlled via registers (direction, output state).
  - **Infinite loop**: Embedded programs often run forever, handling interrupts or polling.
- **Assumptions**: Assumes PB22 is connected to an LED (with appropriate resistor/circuit). The delay is approximate (depends on CPU speed, ~48MHz on SAMD20).
- **Build/Run**: Compiled with ARM GCC (via CMake), linked with custom linker scripts (samd20g18_flash.ld), and programmed via OpenOCD (as seen in your tasks).

If you have questions about specific parts (e.g., why a certain register or how to modify the delay), or want to add features like interrupts or multiple LEDs, let me know! This code is a great starting point for SAMD20 development.