#!/bin/sh
echo `cat /dev/urandom | tr -cd [:graph:] | head -c 100`
