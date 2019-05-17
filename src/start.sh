#!/bin/sh

echo "******************************************************************"
echo " SnakeClient_Cpp"
echo " Created by Nader Zare"
echo " Copyright 2019-.  Nader Zare"
echo " All rights reserved."
echo "******************************************************************"


LIBPATH=../lib
if [ x"$LIBPATH" != x ]; then
  if [ x"$LD_LIBRARY_PATH" = x ]; then
    LD_LIBRARY_PATH=$LIBPATH
  else
    LD_LIBRARY_PATH=$LIBPATH:$LD_LIBRARY_PATH
  fi
  export LD_LIBRARY_PATH
fi

DIR=`dirname $0`

player="./snake_client"

host="127.0.0.1"
port=20002
teamname="SnakeBase"
opt="${host} ${port} ${teamname}"

$player ${opt}
