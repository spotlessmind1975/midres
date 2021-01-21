# HOW TO BUILD THE LIBRARY

## FOREWORD

This is a step by step guide to download, compile and install the software needed to generate the "Commodore" targets. On [this page](targets.md) you will find the complete list of targets. 

Please note that, at the moment, **the C64 target only generates a disk image (d64)**, with the executable and the data necessary for the program/demo. Obviously it also generates the "prg" file, but it works only if it finds the files on the disk.

Moreover, the library makes use of the cc65 compiler to produce executables for the 6502 targets, while it uses the z88dk compiler for others. As it makes use of advanced features, you need to install the latest version of each of these software.

## LINUX

### Step 1: install CC65 compiler

    git clone https://github.com/cc65/cc65.git
    cd cc65
    make
    sudo -i
    export PREFIX=/usr/local
    make install
    exit

### Step 2: install file2include64 utility

    git clone https://github.com/spotlessmind1975/file2include.git
    cd file2include
    gcc -o file2include64 file2include.c
    sudo -i
    ln -s [src]/file2include64 /usr/local/bin/
    exit

### Step 3: install cc1541 utility

    wget https://bitbucket.org/PTV_Claus/cc1541/downloads/cc1541-3.2.tar.gz
    tar -xzf cc1541-3.2.tar.gz
    cd cc1541-3.2
    make
    sudo -i
    make install
    exit

### Ready to go: generate "Air Attack!" game

    make program=airattack target=c64 all

## WINDOWS

### Step 1: download CC65 compiler

Download [CC65 snapshot](https://sourceforge.net/projects/cc65/files/cc65-snapshot-win32.zip/download) from SourceForge. Unzip it in a specific directory (i.e. `c:\cc65`) and add this path to the system `PATH`.

### Step 2: download file2include64 utility

Download [file2include64](https://github.com/spotlessmind1975/file2include/releases/download/v1.0/file2include64.exe) from GitHub. Unzip it in a specific directory (i.e. `c:\file2include64`) and add this path to the system `PATH`.

### Step 3: download cc1541 utility

Download [cc1541](https://csdb.dk/release/download.php?id=242713) from CSDB. Unzip it in a specific directory (i.e. `c:\cc1541`) and add this path to the system `PATH`.

### Ready to go: generate "Air Attack!" game

    make program=airattack target=c64 all
