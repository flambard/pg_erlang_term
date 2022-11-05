EXTENSION = pg_erlang_term
DATA = pg_erlang_term--0.0.1.sql
REGRESS = 00_create_extension decode_test encode_test type_test
MODULES = pg_erlang_term
PG_LDFLAGS = -L/usr/local/Cellar/erlang/25.1.1/lib/erlang/lib/erl_interface-5.3/lib/ -lei
PG_CFLAGS = -I/usr/local/Cellar/erlang/25.1.1/lib/erlang/lib/erl_interface-5.3/include/

# postgres build stuff
PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
