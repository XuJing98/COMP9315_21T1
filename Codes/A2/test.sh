
make clean
make
rm -rv R.bsig
rm -rv R.data
rm -rv R.info
rm -rv R.psig
rm -rv R.tsig
./create R simc 10100 4 1000
./insert R < data3
./stats R
./select R '7663852,?,?,?'
./select R '7663852,?,?,?' t
./select R '7663852,?,?,?' p
./select R '7663852,?,?,?' b
./select R '7664096,PjZZsBYoEYAMzgpCgRKg,?,?'
./select R '7664096,PjZZsBYoEYAMzgpCgRKg,?,?' t
./select R '7664096,PjZZsBYoEYAMzgpCgRKg,?,?' p
./select R '7664096,PjZZsBYoEYAMzgpCgRKg,?,?' b
./select R '?,?,a3-242,a4-242'
./select R '?,?,a3-242,a4-242' t
./select R '?,?,a3-242,a4-242' p
./select R '?,?,a3-242,a4-242' b
./select R '8765432,?,?,?'
./select R '8765432,?,?,?' t
./select R '8765432,?,?,?' p
./select R '8765432,?,?,?' b
./select R '7664096,tRzgWRUlUEdoYPZjofYr,?,?'
./select R '7664096,tRzgWRUlUEdoYPZjofYr,?,?' t
./select R '7664096,tRzgWRUlUEdoYPZjofYr,?,?' p
./select R '7664096,tRzgWRUlUEdoYPZjofYr,?,?' b
rm -rv R.bsig
rm -rv R.data
rm -rv R.info
rm -rv R.psig
rm -rv R.tsig
./create R catc 10100 4 1000
./insert R < data3
./stats R
./select R '7663852,?,?,?'
./select R '7663852,?,?,?' t
./select R '7663852,?,?,?' p
./select R '7663852,?,?,?' b
./select R '7664096,PjZZsBYoEYAMzgpCgRKg,?,?'
./select R '7664096,PjZZsBYoEYAMzgpCgRKg,?,?' t
./select R '7664096,PjZZsBYoEYAMzgpCgRKg,?,?' p
./select R '7664096,PjZZsBYoEYAMzgpCgRKg,?,?' b
./select R '?,?,a3-242,a4-242'
./select R '?,?,a3-242,a4-242' t
./select R '?,?,a3-242,a4-242' p
./select R '?,?,a3-242,a4-242' b
./select R '8765432,?,?,?'
./select R '8765432,?,?,?' t
./select R '8765432,?,?,?' p
./select R '8765432,?,?,?' b
./select R '7664096,tRzgWRUlUEdoYPZjofYr,?,?'
./select R '7664096,tRzgWRUlUEdoYPZjofYr,?,?' t
./select R '7664096,tRzgWRUlUEdoYPZjofYr,?,?' p
./select R '7664096,tRzgWRUlUEdoYPZjofYr,?,?' b


