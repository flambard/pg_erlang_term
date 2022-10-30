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

    ei_x_new_with_version(&buf);

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
    // text *destination;
    // int32 destination_size;

    /*
    elog(INFO, "source length excluding header: %d", (int)VARSIZE_ANY_EXHDR(source));
    elog(INFO, "source byte 0: %d", VARDATA_ANY(source)[0]);
    elog(INFO, "source byte 1: %d", VARDATA_ANY(source)[1]);
    elog(INFO, "source byte 2: %d", VARDATA_ANY(source)[2]);
    elog(INFO, "source byte 3: %d", VARDATA_ANY(source)[3]);
    elog(INFO, "source byte 4: %d", VARDATA_ANY(source)[4]);
    elog(INFO, "source byte 5: %d", VARDATA_ANY(source)[5]);
    */
    /*
        int ei_decode_version(const char *buf, int *index, int *version)
    */

    if (0 != ei_decode_version(VARDATA_ANY(source), &index, &version))
    {
        ereport(ERROR,
                (errcode(ERRCODE_SYNTAX_ERROR),
                 errmsg("failed to decode version byte")));
    }

    /*
        int ei_s_print_term(char **s, const char *buf, int *index)

        Prints a term, in clear text, to the buffer pointed to by s. It tries to resemble the
        term printing in the Erlang shell.

        Parameter s is to point to a dynamically (malloc) allocated string of BUFSIZ bytes or a
        NULL pointer. The string can be reallocated (and *s can be updated) by this function if
        the result is more than BUFSIZ characters. The string returned is NULL-terminated.

        The return value is the number of characters written to the string, or -1 if buf[index]
        does not contain a valid term. Unfortunately, I/O errors on fp is not checked.

        Argument index is updated, that is, this function can be viewed as a decode function that
        decodes a term into a human-readable format.
    */

    // 1. Allocate and initiate char buffer (allowed to be NULL initially?)
    // 2. Get the char array from input bytea
    // 3. Print the char array (size is returned)
    // 4. Allocate text with palloc
    // 5. Copy the characters from output char array to text char array
    // 6. Free the char buffer using plain free()

    chars_written = ei_s_print_term(&buffer, VARDATA_ANY(source), &index);

    if (chars_written == -1)
    {
        ereport(ERROR,
                (errcode(ERRCODE_SYNTAX_ERROR),
                 errmsg("input is not an encoded erlang term")));
    }

    // destination_size = VARHDRSZ + buf.index;
    // destination = (text *)palloc(chars_written);

    PG_RETURN_TEXT_P(cstring_to_text(buffer));
}
