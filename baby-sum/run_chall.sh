#!/bin/sh

sudo docker build -t baby-sum .
sudo docker run -d -p 10252:1024 --rm -it baby-sum