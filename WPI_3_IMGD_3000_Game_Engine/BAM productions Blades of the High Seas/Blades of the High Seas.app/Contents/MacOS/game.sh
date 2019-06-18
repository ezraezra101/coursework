#!/bin/bash
cd "$(dirname "$0")"
cd ../game
export LD_LIBRARY_PATH=../sfml/2.3/lib/
./game
