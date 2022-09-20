#!/bin/bash

# a bash script to setup the environment for the project# install pip3 and virtualenv
sudo apt-get install python3-pip -y

# create the virtual environment in the project root
sudo apt install python3-virtualenv
virtualenv -p python3 mmurp_env