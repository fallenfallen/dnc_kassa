#!/bin/bash

#depends: qt3, gcc43


touch -t 201204200000 `find`

QT4MAKE=/usr/bin/qmake

mkdir $PWD/usr/bin -p
ln -s /usr/bin/gcc-4.3 $PWD/usr/bin/gcc
ln -s /usr/bin/g++-4.3 $PWD/usr/bin/g++
ln -s /usr/bin/c++-4.3 $PWD/usr/bin/c++
ln -s /usr/bin/cpp-4.3 $PWD/usr/bin/cpp
ln -s /opt/qt/bin/qmake $PWD/usr/bin/qmake

Arch=`uname -m`
if [ "$Arch" == "i686" ];then
    cp $PWD/hwsrv/32bit/libhwsrv.so $PWD/bin/lib
    cp $PWD/bin_nosource/32bit/* $PWD/bin/bin
else
    cp $PWD/hwsrv/64bit/libhwsrv.so $PWD/bin/lib
    cp $PWD/bin_nosource/64bit/* $PWD/bin/bin
fi

PATH="$PWD/usr/bin/:$PATH"

cd src/Libs/

cd AddonFunc
echo ""
echo "/libs/AddonFunc"
echo ""
./makelib
if [ $? -ne 0 ];then
    echo "ERROR"
    exit 1
fi
cd ../

cd dncutil
echo ""
echo "/libs/dncutil"
echo ""
./makelib
if [ $? -ne 0 ];then
    echo "ERROR"
    exit 1
fi
cd ../

cd dncdbmanager
echo ""
echo "/libs/dncdbmanager"
echo ""
./makelib
if [ $? -ne 0 ];then
    echo "ERROR"
    exit 1
fi
cd ../

cd dncpersistent
echo ""
echo "/libs/dncpersistent"
echo ""
./makelib
if [ $? -ne 0 ];then
    echo "ERROR"
    exit 1
fi
cd ../

cd dncdecoder_qt4
echo ""
echo "/libs/dncdecoder_qt4"
echo ""
./makelib $QT4MAKE
if [ $? -ne 0 ];then
    echo "ERROR"
    exit 1
fi
cd ../

cd dncwaresystem
echo ""
echo "/libs/dncwaresystem"
echo ""
./makelib
if [ $? -ne 0 ];then
    echo "ERROR"
    exit 1
fi
cd ../

cd DebugLevelLog
echo ""
echo "/libs/DebugLevelLog"
echo ""
./makelib
if [ $? -ne 0 ];then
    echo "ERROR"
    exit 1
fi
cd ../

cd Encoding
echo ""
echo "/libs/Encoding"
echo ""
./makelib
if [ $? -ne 0 ];then
    echo "ERROR"
    exit 1
fi
cd ../

cd Reader_Config
echo ""
echo "/libs/Reader_Config"
echo ""
./makelib
if [ $? -ne 0 ];then
    echo "ERROR"
    exit 1
fi
cd ../

cd Receipt
echo ""
echo "/libs/Receipt"
echo ""
./makelib
if [ $? -ne 0 ];then
    echo "ERROR"
    exit 1
fi
cd ../

cd Setup_param
echo ""
echo "/libs/Setup_param"
echo ""
./makelib
if [ $? -ne 0 ];then
    echo "ERROR"
    exit 1
fi
cd ../

cd PostgresLib
echo ""
echo "/libs/PostgresLib"
echo ""
./makelib
if [ $? -ne 0 ];then
    echo "ERROR"
    exit 1
fi
cd ../

cd StateStore
echo ""
echo "/libs/StateStore"
echo ""
./makelib
if [ $? -ne 0 ];then
    echo "ERROR"
    exit 1
fi
cd ../

#------------------------
cd ../QTLibs

cd dncdecoder
echo ""
echo "/QTLibs/dncdecoder"
echo ""
./makelib
if [ $? -ne 0 ];then
    echo "ERROR"
    exit 1
fi
cd ../

cd BlockTimer
echo ""
echo "/QTLibs/BlockTimer"
echo ""
./makelib
if [ $? -ne 0 ];then
    echo "ERROR"
    exit 1
fi
cd ../

cd CdspView
echo ""
echo "/QTLibs/CdspView"
echo ""
./makelib
if [ $? -ne 0 ];then
    echo "ERROR"
    exit 1
fi
cd ../

cd CreateNewScheme
echo ""
echo "/QTLibs/CreateNewScheme"
echo ""
./makelib
if [ $? -ne 0 ];then
    echo "ERROR"
    exit 1
fi
cd ../

cd CreateNewSchemePerec
echo ""
echo "/QTLibs/CreateNewSchemePerec"
echo ""
./makelib
if [ $? -ne 0 ];then
    echo "ERROR"
    exit 1
fi
cd ../

cd DedicateComboBox
echo ""
echo "/QTLibs/DedicateComboBox"
echo ""
./makelib
if [ $? -ne 0 ];then
    echo "ERROR"
    exit 1
fi
cd ../

cd KeyboardDevices
echo ""
echo "/QTLibs/KeyboardDevices"
echo ""
./makelib
if [ $? -ne 0 ];then
    echo "ERROR"
    exit 1
fi
cd ../

cd Messages
echo ""
echo "/QTLibs/Messages"
echo ""
./makelib
if [ $? -ne 0 ];then
    echo "ERROR"
    exit 1
fi
cd ../

cd HwSignalController
echo ""
echo "/QTLibs/HwSignalController"
echo ""
./makelib
if [ $? -ne 0 ];then
    echo "ERROR"
    exit 1
fi
cd ../

cd AuthorizationForm
echo ""
echo "/QTLibs/AuthorizationForm"
echo ""
./makelib
if [ $? -ne 0 ];then
    echo "ERROR"
    exit 1
fi
cd ../

cd AuthorizationObject
echo ""
echo "/QTLibs/AuthorizationObject"
echo ""
./makelib
if [ $? -ne 0 ];then
    echo "ERROR"
    exit 1
fi
cd ../

cd AddWareController
echo ""
echo "/QTLibs/AddWareController"
echo ""
./makelib
if [ $? -ne 0 ];then
    echo "ERROR"
    exit 1
fi
cd ../

cd StrickOutTabItm
echo ""
echo "/QTLibs/StrickOutTabItm"
echo ""
./makelib
if [ $? -ne 0 ];then
    echo "ERROR"
    exit 1
fi
cd ../

cd TimeDate
echo ""
echo "/QTLibs/TimeDate"
echo ""
./makelib
if [ $? -ne 0 ];then
    echo "ERROR"
    exit 1
fi
cd ../

cd KkmRegistration
echo ""
echo "/QTLibs/KkmRegistration"
echo ""
./makelib
if [ $? -ne 0 ];then
    echo "ERROR"
    exit 1
fi
cd ../

cd InsertTransaction
echo ""
echo "/QTLibs/InsertTransaction"
echo ""
./makelib
if [ $? -ne 0 ];then
    echo "ERROR"
    exit 1
fi
cd ../


cd PrintFormLib
echo ""
echo "/QTLibs/PrintFormLib"
echo ""
./makelib
if [ $? -ne 0 ];then
    echo "ERROR"
    exit 1
fi
cd ../

cd InpasLib
echo ""
echo "/QTLibs/InpasLib"
echo ""
./makelib
if [ $? -ne 0 ];then
    echo "ERROR"
    exit 1
fi
cd ../

#------------------------
cd ../Interfaces

cd AccessRights
echo ""
echo "/Interfaces/AccessRights"
echo ""
./makeprog
if [ $? -ne 0 ];then
    echo "ERROR"
    exit 1
fi
cd ../

cd DevicesConf
echo ""
echo "/Interfaces/DevicesConf"
echo ""
./makeprog_arch.sh
if [ $? -ne 0 ];then
    echo "ERROR"
    exit 1
fi
cd ../

cd ServiceLib
echo ""
echo "/Interfaces/ServiceLib"
echo ""
./makelib
if [ $? -ne 0 ];then
    echo "ERROR"
    exit 1
fi
cd ../

cd SetupLib
echo ""
echo "/Interfaces/SetupLib"
echo ""
./makelib
if [ $? -ne 0 ];then
    echo "ERROR"
    exit 1
fi
cd ../

cd RMK
echo ""
echo "/Interfaces/RMK"
echo ""
./makeprog_arch.sh
if [ $? -ne 0 ];then
    echo "ERROR"
    exit 1
fi
cd ../

cd Reshka
echo ""
echo "/Interfaces/Reshka"
echo ""
./makeprog
if [ $? -ne 0 ];then
    echo "ERROR"
    exit 1
fi
cd ../

cd SetupLoadUnload
echo ""
echo "/Interfaces/SetupLoadUnload"
echo ""
./makeprog
if [ $? -ne 0 ];then
    echo "ERROR"
    exit 1
fi
cd ../

cd WareProject
echo ""
echo "/Interfaces/WareProject"
echo ""
./makeprog_arch.sh
if [ $? -ne 0 ];then
    echo "ERROR"
    exit 1
fi
cd ../

cd TaxInspector
echo ""
echo "/Interfaces/TaxInspector"
echo ""
./makeprog $QT4MAKE
if [ $? -ne 0 ];then
    echo "ERROR"
    exit 1
fi
cd ../

cd SectionSetup
echo ""
echo "/Interfaces/SectionSetup"
echo ""
./makeprog $QT4MAKE
if [ $? -ne 0 ];then
    echo "ERROR"
    exit 1
fi
cd ../

cd ConfigTool
echo ""
echo "/Interfaces/ConfigTool"
echo ""
./makeprog $QT4MAKE
if [ $? -ne 0 ];then
    echo "ERROR"
    exit 1
fi
cd ../

#------------------------
cd ../daemons

cd DisplayDaemon
echo ""
echo "/daemons/DisplayDaemon"
echo ""
./makeprog_arch.sh
if [ $? -ne 0 ];then
    echo "ERROR"
    exit 1
fi
cd ../

cd Upload
echo ""
echo "/daemons/Upload"
echo ""
./makeprog
if [ $? -ne 0 ];then
    echo "ERROR"
    exit 1
fi
cd ../

cd daemon_unload
echo ""
echo "/daemons/daemon_unload"
echo ""
./makeprog
if [ $? -ne 0 ];then
    echo "ERROR"
    exit 1
fi
cd ../

cd DiscountMobile
echo ""
echo "/daemons/DiscountMobile"
echo ""
./makeprog $QT4MAKE
if [ $? -ne 0 ];then
    echo "ERROR"
    exit 1
fi
cd ../

#------------------------

cd ../DBUpdate
echo ""
echo "/DBUpdate"
echo ""
./makeprog
if [ $? -ne 0 ];then
    echo "ERROR"
    exit 1
fi
cd ../
