#include "bindings.h"

void solo5_gpio_mode(uint8_t pin, uint8_t mode)
{
    volatile struct hvt_gpiomode md;
    md.pin = pin;
    md.mode = mode;
    md.ret = 0;
    
    hvt_do_hypercall(HVT_HYPERCALL_GPIOMODE, &md);

    assert(md.ret == 0);
}

void solo5_gpio_write(uint8_t pin, uint8_t value)
{
    volatile struct hvt_gpiowrite wr;
    wr.pin = pin;
    wr.value = value;
    wr.ret = 0;
    
    hvt_do_hypercall(HVT_HYPERCALL_GPIOWRITE, &wr);
    
    assert(wr.ret == 0);
}

uint8_t solo5_gpio_read(uint8_t pin)
{
    volatile struct hvt_gpioread rd;
    rd.pin = pin;
    rd.value = 0;
    rd.ret = 0;

    hvt_do_hypercall(HVT_HYPERCALL_GPIOREAD, &rd);

    assert(rd.ret == 0);
    return rd.value;
}

// vim: set sw=4 ts=4 sts=4 expandtab:
