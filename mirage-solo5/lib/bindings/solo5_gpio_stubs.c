#include "solo5.h"

#define CAML_NAME_SPACE
#include <caml/mlvalues.h>      // Val conversion
#include <caml/memory.h>        // CAMLreturn()


CAMLprim value
mirage_solo5_gpio_mode(value v_pin, value v_mode)
{
    CAMLparam2(v_pin, v_mode);
    solo5_gpio_mode(Int_val(v_pin), Int_val(v_mode));
    CAMLreturn(Val_unit);
}

CAMLprim value
mirage_solo5_gpio_write(value v_pin, value v_value)
{
    CAMLparam2(v_pin, v_value);
    solo5_gpio_write(Int_val(v_pin), Int_val(v_value));
    CAMLreturn(Val_unit);
}

CAMLprim value
mirage_solo5_gpio_read(value v_pin)
{
    CAMLparam1(v_pin);
    CAMLlocal1(v_value);
    v_value = Val_int(solo5_gpio_read(Int_val(v_pin)));
    CAMLreturn(v_value);
}

// vim: set sw=4 ts=4 sts=4 expandtab:
