#! /bin/bash

export PATH=$HOME/archive_linux/bin:$PATH
export C_INCLUDE_PATH=$HOME/archive_linux/lib/cilk/:$C_INCLUDE_PATH
export CPLUS_INCLUDE_PATH=$HOME/archive_linux/lib/cilk/:$CPLUS_INCLUDE_PATH
export LIBRARY_PATH=$HOME/archive_linux/lib:$LIBRARY_PATH
export LD_LIBRARY_PATH=$HOME/archive_linux/lib:$LD_LIBRARY_PATH

echo $LD_LIBRARY_PATH
make
