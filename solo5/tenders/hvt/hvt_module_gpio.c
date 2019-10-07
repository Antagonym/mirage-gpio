/*
 * hvt_module_gpio.c: BCM283X GPIO module.
 */

#include <assert.h>     // assert()
#include <stddef.h>     // NULL
#include <stdint.h>		// int typedefs
#include <stdbool.h>    // bool typedefs
#include <stdio.h>      // printf()
#include <errno.h>      // errno codes
#include <err.h>        // err()
#include <fcntl.h>      // open()
#include <sys/mman.h>   // mmap(), munmap()
#include <string.h>     // strncmp()
#include <stdlib.h>     // strtoull()

#include "hvt.h"

//#define GPIO_BASE       0x3F000000
//#define GPIO_PIN_BASE   (GPIO_BASE + 0x00200000)

#define BLOCK_SIZE      (4 * 1024)

// Highest possible pin on BCM283x; Taken from chipset docs
#define MAX_PIN         54

// Errors
#define E_UNSPEC        1
#define E_INVAL         2

static bool ready = false;
static unsigned long long offset = 0;
static uint32_t *gpiomem = NULL;
static int fd = -1;

static void hypercall_gpiosetup(struct hvt *hvt, hvt_gpa_t gpa)
{
}

#define MODE_INPUT  0
#define MODE_OUTPUT 1
static void hypercall_gpiomode(struct hvt *hvt, hvt_gpa_t gpa)
{
    struct hvt_gpiomode *md =
        HVT_CHECKED_GPA_P(hvt, gpa, sizeof (struct hvt_gpiomode));

    // Ensure that pin is valid
    if (md->pin > MAX_PIN) {
        md->ret = E_INVAL;
        return;
    }

    // DEBUG: Just hardcode GPFSEL registers & shift for PIN0
    int gpfsel = 1; // Or 0, 2, 3, 4, 5 for lower/higher pins
    int shift = 21;

    switch (md->mode)
    {
        case MODE_INPUT:
            *(gpiomem + gpfsel) &= ~(7 << shift);
            break;
        case MODE_OUTPUT:
            *(gpiomem + gpfsel) &= ~(7 << shift);
            *(gpiomem + gpfsel) |= (1 << shift);
            break;
        default:
            md->ret = E_INVAL;
            return;
    }

    md->ret = 0;
}

#define VALUE_LOW   0
#define VALUE_HIGH  1
static void hypercall_gpiowrite(struct hvt *hvt, hvt_gpa_t gpa)
{
    struct hvt_gpiowrite *wr =
        HVT_CHECKED_GPA_P(hvt, gpa, sizeof (struct hvt_gpiowrite));
    
    // Ensure that pin is valid
    if (wr->pin > MAX_PIN) {
        wr->ret = E_INVAL;
        return;
    }

    // DEBUG: Just hardcode SET & CLR registers
    int set = 7;    // Or  8 for higher pins
    int clr = 10;   // Or 11 for higher pins

    switch (wr->value)
    {
        case VALUE_HIGH:
            *(gpiomem + set) = 1 << (wr->pin & 31);
            break;
        case VALUE_LOW:
            *(gpiomem + clr) = 1 << (wr->pin & 31);
            break;
        default:
            wr->ret = E_INVAL;
            return;
    }

    wr->ret = 0;
}

static void hypercall_gpioread(struct hvt *hvt, hvt_gpa_t gpa)
{
    struct hvt_gpiowrite *rd =
        HVT_CHECKED_GPA_P(hvt, gpa, sizeof (struct hvt_gpioread));

    // Ensure that pin is valid
    if (rd->pin > MAX_PIN) {
        rd->ret = E_INVAL;
        return;
    }

    // DEBUG: Just hardcode LEV registers
    int set = 13;   // Or 14 for higher pins

    if ((*(gpiomem + set) & (1 << (rd->pin & 31))) != 0)
    {
        rd->value = VALUE_HIGH;
    }
    else
    {
        rd->value = VALUE_LOW;
    }

    rd->ret = 0;
}

static int handle_cmdarg(char *cmdarg)
{
    char *end;

    if (strncmp("--gpio=", cmdarg, 7))
    {
        //err(1, "Wrong parameter: %s", cmdarg);
        return -1;
    }
    offset = strtoull(cmdarg + 7, &end, 10);
    if (errno == ERANGE)
    {
        err(1, "offset is not uint64: %s", cmdarg + 7);
    }

    printf("%llX", offset);
    ready = true;
    return 0;
}

static int setup(struct hvt *hvt)
{
    if (ready == false)
    {
        return -1;
    }

    // Open memory
    // TODO: For some reason O_CLOEXEC doesn't work when building solo5. Figure out why.
    //fd = open("/dev/mem", O_RDWR | O_SYNC | O_CLOEXEC);
    fd = open("/dev/mem", O_RDWR | O_SYNC);
    if (fd < 0)
    {
        err(1, "Could not open /dev/mem.");
    }

    // Memory map GPIO memory
    gpiomem = (uint32_t *)mmap(0, BLOCK_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, offset);
    if (gpiomem == MAP_FAILED)
    {
        err(1, "Could not map GPIO memory.");
    }

    // Register hypercalls
    // TODO: Hypercall can probably be removed; Task fulfilled by setup()
    assert(hvt_core_register_hypercall(HVT_HYPERCALL_GPIOSETUP,
                hypercall_gpiosetup) == 0);
    assert(hvt_core_register_hypercall(HVT_HYPERCALL_GPIOMODE,
                hypercall_gpiomode) == 0);
    assert(hvt_core_register_hypercall(HVT_HYPERCALL_GPIOWRITE,
                hypercall_gpiowrite) == 0);
    assert(hvt_core_register_hypercall(HVT_HYPERCALL_GPIOREAD,
                hypercall_gpioread) == 0);

    return 0;
}

static char *usage(void)
{
    return "--gpio=ADDRESS (Memory address of GPIO registers.)";
}

struct hvt_module hvt_module_gpio = {
    .name = "gpio",
    .setup = setup,
    .handle_cmdarg = handle_cmdarg,
    .usage = usage
};

// vim: set sw=4 ts=4 sts=4 expandtab:
