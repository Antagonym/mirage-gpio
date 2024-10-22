/* Copyright (c) 2015, IBM
 * Author(s): Dan Williams <djwillia@us.ibm.com>
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose with or without fee is hereby granted, provided
 * that the above copyright notice and this permission notice appear
 * in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 * WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 * AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
 * OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
 * NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "solo5.h"

#define CAML_NAME_SPACE
#include <caml/alloc.h>
#include <caml/memory.h>
#include <caml/signals.h>
#include <caml/fail.h>
#include <caml/callback.h>
#include <caml/bigarray.h>

CAMLprim value
mirage_solo5_block_info(value v_unit)
{
    CAMLparam1(v_unit);
    CAMLlocal1(v_result);
    struct solo5_block_info bi;

    solo5_block_info(&bi);
    v_result = caml_alloc(2, 0);
    Store_field(v_result, 0, caml_copy_int64(bi.capacity));
    Store_field(v_result, 1, caml_copy_int64(bi.block_size));

    CAMLreturn(v_result);
}

CAMLprim value
mirage_solo5_block_read(value v_offset, value v_buf, value v_size)
{
    CAMLparam3(v_offset, v_buf, v_size);
    solo5_off_t offset = Int64_val(v_offset);
    uint8_t *buf = Caml_ba_data_val(v_buf);
    size_t size = Long_val(v_size);
    solo5_result_t result;

    result = solo5_block_read(offset, buf, size);
    CAMLreturn(Val_int(result));
}

CAMLprim value
mirage_solo5_block_write(value v_offset, value v_buf, value v_size)
{
    CAMLparam3(v_offset, v_buf, v_size);
    solo5_off_t offset = Int64_val(v_offset);
    const uint8_t *buf = Caml_ba_data_val(v_buf);
    size_t size = Long_val(v_size);
    solo5_result_t result;

    result = solo5_block_write(offset, buf, size);
    CAMLreturn(Val_int(result));
}
