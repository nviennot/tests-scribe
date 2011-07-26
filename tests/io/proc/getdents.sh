#!/bin/bash --norc
for i in `seq 100`; do
	sleep 3 &
done
ls /proc
killall sleep &
killall sleep &
killall sleep &
killall sleep &
