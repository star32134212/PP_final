#!/bin/bash

cpus=("0" "0,1" "0,1,2" "0,1,2,3")
N=365
Ms=(1000000 5000000 10000000)

for cpu in "${cpus[@]}"; do
	for M in "${Ms[@]}"; do
		echo "N = ${N}, M = ${M}, cpu = ${cpu}:"
		{ { time taskset -c ${cpu} ./ppfinal ${N} ${M} | sed -n '3,3p' | cut -d' ' -f2; } 3>&1 1>&2 2>&3 | awk -F'[ ms\t]' '/^real/{printf "%f\n", $2 * 60 + $3}'; } 2>>err 1>>time;
	done
done
