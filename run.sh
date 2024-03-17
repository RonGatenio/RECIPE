#!/bin/sh

PM_FILE="/tmp/pmem_pclht_data"
NKEYS=100
NTHREADS=4

rm -f $PM_FILE

PROG=example

OUTPUT=pclht.trace
echo [*] Executing $PROG '             ' [$OUTPUT]
./P-CLHT/bin/$PROG $PM_FILE $NKEYS $NTHREADS 2> $OUTPUT

OUTPUT=pclht-recovery.trace
echo [*] Executing $PROG again '       ' [$OUTPUT]
./P-CLHT/bin/$PROG $PM_FILE $NKEYS $NTHREADS 2> $OUTPUT
