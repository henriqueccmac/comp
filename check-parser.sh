#!/bin/bash

ulimit -t 30      # timeout
ulimit -v 1048576 # 1 GB of memory
ulimit -f 1000    # number of written files
ulimit -c 0       # no core dumps; that's slow

NUM_OK=0
NUM_TOTAL=0

for f in `ls tests/*.til` ; do
  ((++NUM_TOTAL))
  echo -n -e "$f:\t"
  rm -f test.xml
  if ! ./til --tree -o test.xml $f &> /dev/null ; then
    echo "FAILED PARSER"
    continue
  fi
  if [ $(wc -c < test.xml) -lt 100 ]; then
    echo "FAILED XML TOO SMALL"
    continue
  fi
  if ! xmllint test.xml &> /dev/null ; then
    echo "FAILED XML"
    continue
  fi
  echo "OK"
  ((++NUM_OK))
done

echo
echo "OK Tests: $NUM_OK out of $NUM_TOTAL"
