#!/bin/sh
TEST_DATA="./project/testing/data"
OUT_FILE_ST="time-st.txt"
OUT_FILE_MT="time-mt.txt"
LIB_PATH="${PWD}/build-testing/libsequence_search_dynamic.so"
APP_ST="./build-testing/project/testing/stress_test/stress_test_single"
APP_MT="./build-testing/project/testing/stress_test/stress_test_multi"
if [ -e $OUT_FILE_ST ] && [ -e $OUT_FILE_MT ]; then
  rm $OUT_FILE_ST $OUT_FILE_MT
fi
for TASK in ${TEST_DATA}/TASKS/*
do
  DATA_PATH="${TEST_DATA}/BIG_DATA/"$(sed -n 1p "$TASK")
  TASKS=$(sed -n 2p "$TASK")
  TASKS_CNT=$(sed -n 4p "$TASK")
  echo "$TASK"
  $APP_ST $LIB_PATH $DATA_PATH $TASKS_CNT $TASKS
  RET_CODE_ST=$?
  $APP_MT $LIB_PATH $DATA_PATH $TASKS_CNT $TASKS
  RET_CODE_MT=$?
  if ! [ $RET_CODE_ST ] && [ $RET_CODE_MT ]
  then
    echo "Error when executing program.."
    exit 1
  fi
done
if [ -e $OUT_FILE_ST ] && [ -e $OUT_FILE_MT ]; then
  echo ""
  echo "Core count: $(nproc)"
  echo ""
  echo "Single thread report:"
  cat $OUT_FILE_ST
  echo ""
  echo "Multi thread report:"
  cat $OUT_FILE_MT
else
  echo "Log not found"
  exit 1
fi
