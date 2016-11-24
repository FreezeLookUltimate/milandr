#!/bin/bash
echo "Installation..."
qmake ./../obj/HTTPClient.pro
make
rm main.o
rm Makefile



