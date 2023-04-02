#!/bin/bash

sudo apt update
sudo apt install -y clang clang-tidy python3
python3 -m pip install -U pip
python3 -m pip install cpplint
sudo apt install mpg123
