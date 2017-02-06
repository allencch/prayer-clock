#!/bin/bash

cd ..
autoreconf -vfi

cd -
../configure
make dist
makepkg -fi
