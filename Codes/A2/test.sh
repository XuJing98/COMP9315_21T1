#chmod +x ./test.sh
# ./test.sh
make clean

printf "make\n"
make

printf "./gendata 10000 6 1234567 13 > R.in\n"
./gendata 10000 6 1234567 13 > R.in

printf "./create R simc 10000 6 1000\n"
./create R simc 10000 6 1000

printf "./insert R < R.in\n"
time ./insert R < R.in

printf "./stats R\n"
./stats R

printf "./select R '?,?,?,?,?,?' x | tail -6\n"
./select R '?,?,?,?,?,?' x | tail -6

printf "./select R '1234999,?,?,?,?,?' x\n"
./select R '1234999,?,?,?,?,?' x

printf "./select R '1234999,?,?,?,?,?' t\n"
./select R '1234999,?,?,?,?,?' t

printf "./select R '1234999,?,?,?,?,?' p\n"
./select R '1234999,?,?,?,?,?' p

printf "./select R '1234999,?,?,?,?,?' b\n"
./select R '1234999,?,?,?,?,?' b

printf "grep 'a3-241,a4-158,a5-407' R.in\n"
grep 'a3-241,a4-158,a5-407' R.in

printf "./select R '?,?,a3-241,a4-158,a5-407,?' x\n"
./select R '?,?,a3-241,a4-158,a5-407,?' x

printf "./select R '?,?,a3-241,a4-158,a5-407,?' t\n"
./select R '?,?,a3-241,a4-158,a5-407,?' t

printf "./select R '?,?,a3-241,a4-158,a5-407,?' p\n"
./select R '?,?,a3-241,a4-158,a5-407,?' p

printf "./select R '?,?,a3-241,a4-158,a5-407,?' b\n"
./select R '?,?,a3-241,a4-158,a5-407,?' b

rm R.bsig
rm R.data
rm R.in
rm R.info
rm R.psig
rm R.tsig

