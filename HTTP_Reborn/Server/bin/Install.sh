#!/bin/bash
echo "Installation..."
qmake ./../obj/HTTPServer.pro
make
rm main.o
rm Makefile
rm moc_qsimpleserver.cpp
rm moc_qsimpleserver.o
rm qsimpleserver.o
rm requests.o


