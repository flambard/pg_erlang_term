EXTENSION = pg_erlang_term
DATA = pg_erlang_term--0.0.1.sql
REGRESS = 00_create_extension decode_test encode_test
MODULES = pg_erlang_term
PG_LDFLAGS = -lei

# postgres build stuff
PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
