#!/bin/sh
(cd ./project/testing/data/CASES &&
tr -dc A-Za-z0-9 </dev/urandom | head -c 100000000 >  CASE1_A-Za-z0-9.txt &&
tr -dc A-La-l0-5 </dev/urandom | head -c 10000000 > CASE2_A-La-l0-5.txt &&
tr -dc A-La-l0-5 </dev/urandom | head -c 1024 > CASE3_A-La-l0-5.txt &&
tr -dc J-zj-z6-9 </dev/urandom | head -c 150000000 > CASE4_J-zj-z6-9.txt)
