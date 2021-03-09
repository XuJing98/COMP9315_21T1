
cp intset.c /srvr/z5304662/postgresql-12.5/src/tutorial/
cp intset.source /srvr/z5304662/postgresql-12.5/src/tutorial/
cp test.sql /srvr/z5304662/postgresql-12.5/src/tutorial/
source /srvr/z5304662/env
pgs start
cd /srvr/z5304662/postgresql-12.5/src/tutorial/

make
dropdb test
createdb test
psql test -f intset.sql
psql test -f test.sql
pgs stop