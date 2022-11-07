#include "postgres.h"
#include "fmgr.h"
#include "utils/builtins.h"
#include "lib/stringinfo.h"
#include "libpq/pqformat.h"
#include "ei.h"

PG_MODULE_MAGIC;

bytea *erlang_term_string_to_binary(const char *term);
char *erlang_term_binary_to_string(bytea *binary);

void _PG_init(void);
void _PG_init()
{
    ei_init();
}

PG_FUNCTION_INFO_V1(erlang_term_input);
Datum erlang_term_input(PG_FUNCTION_ARGS)
{
    PG_RETURN_POINTER(erlang_term_string_to_binary(PG_GETARG_CSTRING(0)));
}

PG_FUNCTION_INFO_V1(erlang_term_output);
Datum erlang_term_output(PG_FUNCTION_ARGS)
{
    PG_RETURN_CSTRING(erlang_term_binary_to_string(PG_GETARG_BYTEA_P(0)));
}

PG_FUNCTION_INFO_V1(erlang_term_encode);
Datum erlang_term_encode(PG_FUNCTION_ARGS)
{
    PG_RETURN_BYTEA_P(erlang_term_string_to_binary(text_to_cstring(PG_GETARG_TEXT_PP(0))));
}

PG_FUNCTION_INFO_V1(erlang_term_decode);
Datum erlang_term_decode(PG_FUNCTION_ARGS)
{
    PG_RETURN_TEXT_P(cstring_to_text(erlang_term_binary_to_string(PG_GETARG_BYTEA_PP(0))));
}

PG_FUNCTION_INFO_V1(erlang_term_receive);
Datum erlang_term_receive(PG_FUNCTION_ARGS)
{
    StringInfo buff = (StringInfo)PG_GETARG_POINTER(0);

    bytea *output;
    int total_size;
    int index = 0;
    int version;
    const char *bin = pq_getmsgbytes(buff, buff->len);

    if (0 != ei_decode_version(bin, &index, &version))
    {
        ereport(ERROR,
                (errcode(ERRCODE_INVALID_BINARY_REPRESENTATION),
                 errmsg("failed to decode version byte")));
    }

    if (0 != ei_skip_term(bin, &index))
    {
        ereport(ERROR,
                (errcode(ERRCODE_INVALID_BINARY_REPRESENTATION),
                 errmsg("invalid binary erlang term")));
    }

    total_size = index + VARHDRSZ;
    output = (bytea *)palloc0(total_size);
    SET_VARSIZE(output, total_size);
    memcpy(VARDATA(output), bin, index);

    PG_RETURN_BYTEA_P(output);
}

PG_FUNCTION_INFO_V1(erlang_term_send);
Datum erlang_term_send(PG_FUNCTION_ARGS)
{
    PG_RETURN_BYTEA_P(PG_GETARG_BYTEA_P(0));
}

///////////
///////////
///////////

bytea *erlang_term_string_to_binary(const char *term)
{
    bytea *output;
    int total_size;
    ei_x_buff buff;
    ei_x_new(&buff);

    if (0 != ei_x_format(&buff, term))
    {
        ei_x_free(&buff);

        ereport(ERROR,
                (errcode(ERRCODE_SYNTAX_ERROR),
                 errmsg("invalid erlang term syntax")));
    }

    total_size = buff.index + VARHDRSZ;
    output = (bytea *)palloc(total_size);
    SET_VARSIZE(output, total_size);
    memcpy(VARDATA(output), buff.buff, buff.index);

    ei_x_free(&buff);

    return output;
}

char *erlang_term_binary_to_string(bytea *source)
{
    int32 index = 0;
    char *buffer = NULL;
    char *output;
    int version;
    int chars_written;
    char *binary = VARDATA_ANY(source);

    if (0 != ei_decode_version(binary, &index, &version))
    {
        ereport(ERROR,
                (errcode(ERRCODE_SYNTAX_ERROR),
                 errmsg("failed to decode version byte")));
    }

    chars_written = ei_s_print_term(&buffer, binary, &index);

    if (chars_written == -1)
    {
        ereport(ERROR,
                (errcode(ERRCODE_SYNTAX_ERROR),
                 errmsg("input is not an encoded erlang term")));
    }

    output = (char *)palloc0(chars_written + 1);
    memcpy(output, buffer, chars_written);

    return output;
}
