#!/bin/bash

> problem.pddl
echo "(define (problem lights-out)" >> problem.pddl
echo -e "\t(:domain lights-out)" >> problem.pddl
echo -ne "\t(:objects" >> problem.pddl
matrix=()
i=0
while IFS= read -r line; do
    matrix+=("$line")
	for ((j=0; j<${#line}; j++)); do
		echo -n " c$i-$j" >> problem.pddl
	done
	((i++))
done
echo " - cell)" >> problem.pddl
echo -e "\t(:init" >> problem.pddl
for ((i=0; i<${#matrix[@]}; i++)); do
	line="${matrix[$i]}"
	for ((j=0; j<${#line}; j++)); do
		char="${line:j:1}"
		if [ $i -eq 0 ] && [ $j -eq 0 ]; then
			echo -e "\t\t(adjacent-vertex c$i-$j c$i-$((j+1)) c$((i+1))-$j)" >> problem.pddl
		elif [ $i -eq 0 ] && [ $j -eq $((${#line}-1)) ]; then
			echo -e "\t\t(adjacent-vertex c$i-$j c$i-$((j-1)) c$((i+1))-$j)" >> problem.pddl
		elif [ $i -eq $((${#matrix[@]}-1)) ] && [ $j -eq 0 ]; then
			echo -e "\t\t(adjacent-vertex c$i-$j c$((i-1))-$j c$i-$((j+1)))" >> problem.pddl
		elif [ $i -eq $((${#matrix[@]}-1)) ] && [ $j -eq $((${#line}-1)) ]; then
			echo -e "\t\t(adjacent-vertex c$i-$j c$((i-1))-$j c$i-$((j-1)))" >> problem.pddl
		elif [ $i -eq 0 ]; then
			echo -e "\t\t(adjacent-edge c$i-$j c$i-$((j-1)) c$i-$((j+1)) c$((i+1))-$j)" >> problem.pddl
		elif [ $j -eq 0 ]; then
			echo -e "\t\t(adjacent-edge c$i-$j c$((i-1))-$j c$i-$((j+1)) c$((i+1))-$j)" >> problem.pddl
		elif [ $j -eq $((${#matrix[@]}-1)) ]; then
			echo -e "\t\t(adjacent-edge c$i-$j c$((i-1))-$j c$i-$((j-1)) c$((i+1))-$j)" >> problem.pddl
		elif [ $i -eq $((${#line}-1)) ]; then
			echo -e "\t\t(adjacent-edge c$i-$j c$((i-1))-$j c$i-$((j-1)) c$i-$((j+1)))" >> problem.pddl
		else
			echo -e "\t\t(adjacent c$i-$j c$((i-1))-$j c$i-$((j-1)) c$i-$((j+1)) c$((i+1))-$j)" >> problem.pddl
		fi
		if [[ "$char" == 'L' || "$char" == 'l' ]]; then
			echo -e "\t\t(bulb c$i-$j)" >> problem.pddl
		fi
		if [[ "$char" == 'd' || "$char" == 'l' ]]; then
			echo -e "\t\t(broken c$i-$j)" >> problem.pddl
		fi
	done
done
echo -e "\t)" >> problem.pddl
echo -e "\t(:goal (forall (?c - cell) (not (bulb ?c))))" >> problem.pddl
echo ")" >> problem.pddl

> domain.pddl
cat <<EOF > domain.pddl
(define (domain lights-out)
	(:types cell)
	(:predicates
		(bulb ?c - cell)
		(broken ?c - cell)
		(adjacent-vertex ?c ?c1 ?c2 - cell)
		(adjacent-edge ?c ?c1 ?c2 ?c3 - cell)
		(adjacent ?c ?c1 ?c2 ?c3 ?c4 - cell))
	(:action CLICK-VERTEX
		:parameters (?c ?c1 ?c2 - cell)
		:precondition (and (adjacent-vertex ?c ?c1 ?c2))
		:effect (and
			(when (not (broken ?c)) (and
  				(when (bulb ?c) (not (bulb ?c)))
				(when (not (bulb ?c)) (bulb ?c))))
			(when (bulb ?c1) (not (bulb ?c1)))
			(when (not (bulb ?c1)) (bulb ?c1))
			(when (bulb ?c2) (not (bulb ?c2)))
			(when (not (bulb ?c2)) (bulb ?c2))))
	(:action CLICK-EDGE
		:parameters (?c ?c1 ?c2 ?c3 - cell)
		:precondition (and (adjacent-edge ?c ?c1 ?c2 ?c3))
		:effect (and
			(when (not (broken ?c)) (and
  				(when (bulb ?c) (not (bulb ?c)))
				(when (not (bulb ?c)) (bulb ?c))))
			(when (bulb ?c1) (not (bulb ?c1)))
			(when (not (bulb ?c1)) (bulb ?c1))
			(when (bulb ?c2) (not (bulb ?c2)))
			(when (not (bulb ?c2)) (bulb ?c2))
			(when (bulb ?c3) (not (bulb ?c3)))
			(when (not (bulb ?c3)) (bulb ?c3))))
	(:action CLICK
		:parameters (?c ?c1 ?c2 ?c3 ?c4 - cell)
		:precondition (and (adjacent ?c ?c1 ?c2 ?c3 ?c4))
		:effect (and
			(when (not (broken ?c)) (and
  				(when (bulb ?c) (not (bulb ?c)))
				(when (not (bulb ?c)) (bulb ?c))))
			(when (bulb ?c1) (not (bulb ?c1)))
			(when (not (bulb ?c1)) (bulb ?c1))
			(when (bulb ?c2) (not (bulb ?c2)))
			(when (not (bulb ?c2)) (bulb ?c2))
			(when (bulb ?c3) (not (bulb ?c3)))
			(when (not (bulb ?c3)) (bulb ?c3))
			(when (bulb ?c4) (not (bulb ?c4)))
			(when (not (bulb ?c4)) (bulb ?c4))))
)
EOF

/tmp/dir/software/planners/downward-fdss23/fast-downward.py --alias seq-opt-fdss-2023 --overall-time-limit 902s domain.pddl problem.pddl > /dev/null 2>&1

if [ -f "sas_plan" ]; then
	pairs=""
	while IFS= read -r line; do
		if [[ $line == "(click"* ]]; then
			first_cij=$(echo "$line" | grep -oP "c[0-9]+-[0-9]+" | head -n 1)
			IFS='-' read -ra parts <<< "$first_cij"
			i="${parts[0]#c}"
			j="${parts[1]}"
			pairs="$pairs($i, $j);"
		fi
	done < sas_plan
	echo "${pairs%;}"
else
	exit 120
fi
