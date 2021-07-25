#!/bin/sh

sudo docker build -t memory-heist .
sudo docker run -d -p 10253:1024 --rm -it memory-heist