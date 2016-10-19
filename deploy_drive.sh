#!/bin/bash
serial_port=/dev/ttyUSB0;
board_model=nano328;


cd drive/;
echo "Building Ansel Drive Program for board:" $board_model
sudo ino build -m $board_model;
echo "Uploading to" $board_model "on" $serial_port
sudo ino upload -m $board_model -p $serial_port;
cd ../;
