#include "postgres.h"
#include "fmgr.h"
#include "utils/builtins.h"
#include "ei.h"

PG_MODULE_MAGIC;

void _PG_init(void);
void _PG_init()
{
    ei_init();
}

PG_FUNCTION_INFO_V1(erlang_term_encode);
Datum erlang_term_encode(PG_FUNCTION_ARGS)
{
    text *source = PG_GETARG_TEXT_PP(0);

    bytea *destination;
    int32 destination_size;
    ei_x_buff buf;

    ei_x_new(&buf);

    if (0 != ei_x_format(&buf, VARDATA(source)))
    {
        ei_x_free(&buf);

        ereport(ERROR,
                (errcode(ERRCODE_SYNTAX_ERROR),
                 errmsg("invalid erlang term syntax")));
    }

    destination_size = VARHDRSZ + buf.index;
    destination = (bytea *)palloc(destination_size);

    SET_VARSIZE(destination, destination_size);
    memcpy(VARDATA(destination), buf.buff, buf.index);

    ei_x_free(&buf);

    PG_RETURN_BYTEA_P(destination);
}

PG_FUNCTION_INFO_V1(erlang_term_decode);
Datum erlang_term_decode(PG_FUNCTION_ARGS)
{
    bytea *source = PG_GETARG_BYTEA_PP(0);

    int32 index = 0;
    char *buffer = NULL;
    int version;
    int chars_written;

    if (0 != ei_decode_version(VARDATA_ANY(source), &index, &version))
    {
        ereport(ERROR,
                (errcode(ERRCODE_SYNTAX_ERROR),
                 errmsg("failed to decode version byte")));
    }

    chars_written = ei_s_print_term(&buffer, VARDATA_ANY(source), &index);

    if (chars_written == -1)
    {
        ereport(ERROR,
                (errcode(ERRCODE_SYNTAX_ERROR),
                 errmsg("input is not an encoded erlang term")));
    }

    PG_RETURN_TEXT_P(cstring_to_text(buffer));
}
