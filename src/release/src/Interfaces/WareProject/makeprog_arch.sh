
qmake QMAKE_CFLAGS=-pthread QMAKE_CXXFLAGS=-pthread QMAKE_LIBS=-lpthread
make

cp -v ./WareProject ../../../bin/bin
