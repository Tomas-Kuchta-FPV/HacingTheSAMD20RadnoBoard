/**
 * @file main.c
 * @author tomas
 * @date 2026-05-13
 * @brief Blink LED on PB22 for ATSAMD20G18
 */

#include <stdint.h>

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

static inline void reg_write32(uintptr_t address, uint32_t value)
{
    *(volatile uint32_t *)address = value;
}

static inline void reg_set32(uintptr_t address, uint32_t value)
{
    *(volatile uint32_t *)address |= value;
}

static void delay_loop(volatile uint32_t count)
{
    while (count--) {
        __asm__ volatile("nop");
    }
}

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
