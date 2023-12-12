#!/bin/bash

get_box() {
	local row=$1
	local col=$2
	local box_row=$((row / 3))
	local box_col=$((col / 3))
	local box=$((3*box_row + box_col))
	echo $box
}
> problem.hddl
echo "(define (problem sudoku)" >> problem.hddl
echo -e "\t(:domain sudoku)" >> problem.hddl
echo -e "\t(:objects)" >> problem.hddl
echo -e "\t(:init" >> problem.hddl
echo -e "\t\t(inc r0 r1) (inc r1 r2) (inc r2 r3) (inc r3 r4) (inc r4 r5) (inc r5 r6) (inc r6 r7) (inc r7 r8)" >> problem.hddl
echo -e "\t\t(inc c0 c1) (inc c1 c2) (inc c2 c3) (inc c3 c4) (inc c4 c5) (inc c5 c6) (inc c6 c7) (inc c7 c8)" >> problem.hddl
for ((i=0; i<9; i++)); do
	for ((j=0; j<9; j++)); do
		box=$(get_box $i $j)
		echo -e "\t\t(cell-at-box r$i c$j b$box)" >> problem.hddl
		read -n 1 digit
		if [[ $digit -ne 0 ]]; then
			echo -e "\t\t(digit-at-box d$digit b$box)" >> problem.hddl
			echo -e "\t\t(digit-at d$digit r$i c$j)" >> problem.hddl
			echo -e "\t\t(filled r$i c$j)" >> problem.hddl
		fi
	done
done
echo -e "\t)" >> problem.hddl
echo -e "\t(:htn :tasks (and (all-check)))" >> problem.hddl
echo ")" >> problem.hddl
