#!/bin/sh

cd P-CLHT
mkdir build
cd build
cmake ..
make -j

if [ ! -z "$CPRD_RESULTS_BIN" ]; then
    cp pclht $CPRD_RESULTS_BIN
fi
