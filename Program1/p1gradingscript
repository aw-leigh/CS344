#!/bin/bash

# NAME
# 	generate - generates a matrix of specified size
#	SYNOPSIS
#		generate ROWS COLS MIN MAX
# DESCRIPTION
#		Prints a matrix of size ROWS*COLS with random values ranging from MIN to MAX
function generate(){
	y=0
	a=$3
	b=$4
	while [ $y -lt $1 ]
	do
		x=0
		((y++))
		while [ $x -lt $2 ]
		do
			((x++))
			echo -n $((RANDOM%(b-a+1)+a))
			if [ $x -ne $2 ]
			then
			echo -ne "\t"
			else
				echo
			fi
		done
	done
}

# NAME
# 	ident - generate identity matrix of specified size
# SYNOPSIS
#		ident ROWS COLS
function ident(){
	y=0
	while [ $y -lt $1 ]
	do
		x=0
		((y++))
		while [ $x -lt $2 ]
		do
			((x++))
			if [ $x -eq $y ]
			then
				echo -n 1
			else
				echo -n 0
			fi
			if [ $x -ne $2 ]
			then
				echo -ne "\t"
			else
				echo
			fi
		done
	done
}

# NAME
#	Error
# SYNOPSIS
#	Dump error message and exit

err(){
	echo $1 >&2
	exit 1
}


if [ $# -ge 1 ] 
then
	if [ -f $1 ]
	then 
		cmd=$1
	else
		err "Given file not found."
	fi
		
else
	err "Usage: $0 [bash_program_file]"
fi

chmod +x "$1" # Make sure submission is executable
dos2unix "$1" # Fix windows newlines (^M errors)

cd "$(dirname "$1")" # Change working directory to submission file
cmd="$(basename "$1")"



total=0
score=0

# Generate temp files to use for grading purposes
m1="$(mktemp matrix.XXXXX)"
m2="$(mktemp matrix.XXXXX)"
m3="$(mktemp matrix.XXXXX)"
m4="$(mktemp matrix.XXXXX)"
m5="$(mktemp matrix.XXXXX)"
outpipe="$(mktemp stdout.XXXXX)"
outpipe2="$(mktemp stdout.XXXXX)"
errpipe="$(mktemp stderr.XXXXX)"
errpipe2="$(mktemp stderr.XXXXX)"

trap "rm -rf $m1 $m2 $m3 $m4 $m5 $outpipe $outpipe2 $errpipe $errpipe2; trap - EXIT" INT HUP TERM EXIT 

result=0

# Populate matrix files
generate 5 6 -10 10 >"$m1"
generate 6 7 -10 10 >"$m2"
generate 3 8 -10 10 >"$m3"
generate 8 5 -10 10 >"$m4"
generate 3 8 -10 10 >"$m5"

echo "Dims with 2 arguments should throw error (1pt):"
score=1
SECONDS=0
./"$cmd" dims "$m1" "$m2" 1>"$outpipe" 2>"$errpipe" &
while
	[ $SECONDS -lt 5 ] && kill -0 $! &>/dev/null
do
	:
done
if
	kill -9 $! &>/dev/null
then
	echo -e "\u26D4 Hung process (killed)"
	score=0
else 
	wait $!
	result=$?
	if [ "$result" -eq 0 ]
	then
		score=0
		echo -e "\u26D4 Returned 0"
	fi
	if [ -s "$outpipe" ] 
	then
		score=0
		echo -e "\u26D4 stdout is non-empty"
	fi
	if [ ! -s "$errpipe" ]
	then
		score=0
		echo -e "\u26D4 stderr is empty"
	fi
fi
if [ $score -ne 0 ]
then
	echo -e "\u2705 Passed!"
fi
((points+=score))


echo "Add with 0 arguments should throw error (1pt):"
score=1
SECONDS=0
./"$cmd" add 1>"$outpipe" 2>"$errpipe" &
while
	[ $SECONDS -lt 5 ] && kill -0 $! &>/dev/null
do
	:
done
if
	kill -9 $! &>/dev/null
then
	echo -e "\u26D4 Hung process (killed)"
	score=0
else 
	wait $!
	result=$?
	if [ "$result" -eq 0 ]
	then
		score=0
		echo -e "\u26D4 Returned 0"
	fi
	if [ -s "$outpipe" ] 
	then
		score=0
		echo -e "\u26D4 stdout is non-empty"
	fi
	if [ ! -s "$errpipe" ]
	then
		score=0
		echo -e "\u26D4 stderr is empty"
	fi
fi
if [ $score -ne 0 ]
then
	echo -e "\u2705 Passed!"
fi
((points+=score))


echo "Multiply mismatched matrices should throw error (5pt):"
score=5
SECONDS=0
./"$cmd" multiply "$m1" "$m4" 1>"$outpipe" 2>"$errpipe" &
while
	[ $SECONDS -lt 5 ] && kill -0 $! &>/dev/null
do
	:
done
if
	kill -9 $! &>/dev/null
then
	echo -e "\u26D4 Hung process (killed)"
	score=0
else 
	wait $!
	result=$?
	if [ "$result" -eq 0 ]
	then
		score=0
		echo -e "\u26D4 Returned 0"
	fi
	if [ -s "$outpipe" ] 
	then
		score=0
		echo -e "\u26D4 stdout is non-empty"
	fi
	if [ ! -s "$errpipe" ]
	then
		score=0
		echo -e "\u26D4 stderr is empty"
	fi
fi
if [ $score -ne 0 ]
then
	echo -e "\u2705 Passed!"
fi
((points+=score))


echo "Adding mismatched matrices should throw error (4pt):"
score=4
SECONDS=0
./"$cmd" add "$m1" "$m2" 1>"$outpipe" 2>"$errpipe" &
while
	[ $SECONDS -lt 5 ] && kill -0 $! &>/dev/null
do
	:
done
if
	kill -9 $! &>/dev/null
then
	echo -e "\u26D4 Hung process (killed)"
	score=0
else 
	wait $!
	result=$?
	if [ "$result" -eq 0 ]
	then
		score=0
		echo -e "\u26D4 Returned 0"
	fi
	if [ -s "$outpipe" ] 
	then
		score=0
		echo -e "\u26D4 stdout is non-empty"
	fi
	if [ ! -s "$errpipe" ]
	then
		score=0
		echo -e "\u26D4 stderr is empty"
	fi
fi
if [ $score -ne 0 ]
then
	echo -e "\u2705 Passed!"
fi
((points+=score))


chmod -r "$m1"
echo "Transposing undreadable file should throw error (2pt):"
score=2
SECONDS=0
./"$cmd" transpose "$m1" 1>"$outpipe" 2>"$errpipe" &
while
	[ $SECONDS -lt 5 ] && kill -0 $! &>/dev/null
do
	:
done
if
	kill -9 $! &>/dev/null
then
	echo -e "\u26D4 Hung process (killed)"
	score=0
else 
	wait $!
	result=$?
	if [ "$result" -eq 0 ]
	then
		score=0
		echo -e "\u26D4 Returned 0"
	fi
	if [ -s "$outpipe" ] 
	then
		score=0
		echo -e "\u26D4 stdout is non-empty"
	fi
	if [ ! -s "$errpipe" ]
	then
		score=0
		echo -e "\u26D4 stderr is empty"
	fi
fi
if [ $score -ne 0 ]
then
	echo -e "\u2705 Passed!"
fi
((points+=score))
chmod +r "$m1"


echo "Dims on nonexistent file should throw error (1pt):"
score=1
SECONDS=0
./"$cmd" dims "$(mktemp -u)" 1>"$outpipe" 2>"$errpipe" &
while
	[ $SECONDS -lt 5 ] && kill -0 $! &>/dev/null
do
	:
done
if
	kill -9 $! &>/dev/null
then
	echo -e "\u26D4 Hung process (killed)"
	score=0
else 
	wait $!
	result=$?
	if [ "$result" -eq 0 ]
	then
		score=0
		echo -e "\u26D4 Returned 0"
	fi
	if [ -s "$outpipe" ] 
	then
		score=0
		echo -e "\u26D4 stdout is non-empty"
	fi
	if [ ! -s "$errpipe" ]
	then
		score=0
		echo -e "\u26D4 stderr is empty"
	fi
fi
if [ $score -ne 0 ]
then
	echo -e "\u2705 Passed!"
fi
((points+=score))


echo "badcommand should throw error (1pt):"
score=1
SECONDS=0
./"$cmd" badcommand 1>"$outpipe" 2>"$errpipe" &
while
	[ $SECONDS -lt 5 ] && kill -0 $! &>/dev/null
do
	:
done
if
	kill -9 $! &>/dev/null
then
	echo -e "\u26D4 Hung process (killed)"
	score=0
else 
	wait $!
	result=$?
	if [ "$result" -eq 0 ]
	then
		score=0
		echo -e "\u26D4 Returned 0"
	fi
	if [ -s "$outpipe" ] 
	then
		score=0
		echo -e "\u26D4 stdout is non-empty"
	fi
	if [ ! -s "$errpipe" ]
	then
		score=0
		echo -e "\u26D4 stderr is empty"
	fi
fi
if [ $score -ne 0 ]
then
	echo -e "\u2705 Passed!"
fi
((points+=score))


echo "Piping m1 (5x6) into dims (3pt):"
score=3
SECONDS=0
cat "$m1" | ./"$cmd" dims 1>"$outpipe" 2>"$errpipe" &
while
	[ $SECONDS -lt 5 ] && kill -0 $! &>/dev/null
do
	:
done
if
	kill -9 $! &>/dev/null
then
	echo -e "\u26D4 Hung process (killed)"
	score=0
else 
	wait $!
	result=$?
	if [ "$result" -ne 0 ]
	then
		score=0
		echo -e "\u26D4 Returned $result"
	fi
	if
		! cmp -s "$outpipe" <<< "5 6"
	then
		score=0
		echo -e "\u26D4 dimensions output is incorrect"
	fi
	if [ -s "$errpipe" ]
	then
		score=0
		echo -e "\u26D4 stderr is non-empty"
	fi
fi
if [ $score -ne 0 ]
then
	echo -e "\u2705 Passed!"
fi
((points+=score))


echo "Piping m2 (6x7) into dims (3pt):"
score=3
SECONDS=0
cat "$m2" | ./"$cmd" dims 1>"$outpipe" 2>"$errpipe" &
while
	[ $SECONDS -lt 5 ] && kill -0 $! &>/dev/null
do
	:
done
if
	kill -9 $! &>/dev/null
then
	echo -e "\u26D4 Hung process (killed)"
	score=0
else 
	wait $!
	result=$?
	if [ "$result" -ne 0 ]
	then
		score=0
		echo -e "\u26D4 Returned $result"
	fi
	if
		! cmp -s "$outpipe" <<< "6 7"
	then
		score=0
		echo -e "\u26D4 dimensions output is incorrect"
	fi
	if [ -s "$errpipe" ]
	then
		score=0
		echo -e "\u26D4 stderr is non-empty"
	fi
fi
if [ $score -ne 0 ]
then
	echo -e "\u2705 Passed!"
fi
((points+=score))


echo "Piping m3 (3x8) into dims (3pt):"
score=3
SECONDS=0
cat "$m3" | ./"$cmd" dims 1>"$outpipe" 2>"$errpipe" &
while
	[ $SECONDS -lt 5 ] && kill -0 $! &>/dev/null
do
	:
done
if
	kill -9 $! &>/dev/null
then
	echo -e "\u26D4 Hung process (killed)"
	score=0
else 
	wait $!
	result=$?
	if [ "$result" -ne 0 ]
	then
		score=0
		echo -e "\u26D4 Returned $result"
	fi
	if
		! cmp -s "$outpipe" <<< "3 8"
	then
		score=0
		echo -e "\u26D4 dimensions output is incorrect"
	fi
	if [ -s "$errpipe" ]
	then
		score=0
		echo -e "\u26D4 stderr is non-empty"
	fi
fi
if [ $score -ne 0 ]
then
	echo -e "\u2705 Passed!"
fi
((points+=score))


echo "Passing m4 (8x5) to dims on stdin (3pt):"
score=3
SECONDS=0
./"$cmd" dims "$m4" 1>"$outpipe" 2>"$errpipe" &
while
	[ $SECONDS -lt 5 ] && kill -0 $! &>/dev/null
do
	:
done
if
	kill -9 $! &>/dev/null
then
	echo -e "\u26D4 Hung process (killed)"
	score=0
else 
	wait $!
	result=$?
	if [ "$result" -ne 0 ]
	then
		score=0
		echo -e "\u26D4 Returned $result"
	fi
	if
		! cmp -s "$outpipe" <<< "8 5"
	then
		score=0
		echo -e "\u26D4 dimensions output is incorrect"
	fi
	if [ -s "$errpipe" ]
	then
		score=0
		echo -e "\u26D4 stderr is non-empty"
	fi
fi
if [ $score -ne 0 ]
then
	echo -e "\u2705 Passed!"
fi
((points+=score))


echo "Passing m1 (5x6) to dims on stdin (3pt):"
score=3
SECONDS=0
./"$cmd" dims "$m1" 1>"$outpipe" 2>"$errpipe" &
while
	[ $SECONDS -lt 5 ] && kill -0 $! &>/dev/null
do
	:
done
if
	kill -9 $! &>/dev/null
then
	echo -e "\u26D4 Hung process (killed)"
	score=0
else 
	wait $!
	result=$?
	if [ "$result" -ne 0 ]
	then
		score=0
		echo -e "\u26D4 Returned $result"
	fi
	if
		! cmp -s "$outpipe" <<< "5 6"
	then
		score=0
		echo -e "\u26D4 dimensions output is incorrect"
	fi
	if [ -s "$errpipe" ]
	then
		score=0
		echo -e "\u26D4 stderr is non-empty"
	fi
fi
if [ $score -ne 0 ]
then
	echo -e "\u2705 Passed!"
fi
((points+=score))


echo "Passing m2 (6x7) to dims on stdin (3pt):"
score=3
SECONDS=0
./"$cmd" dims "$m2" 1>"$outpipe" 2>"$errpipe" &
while
	[ $SECONDS -lt 5 ] && kill -0 $! &>/dev/null
do
	:
done
if
	kill -9 $! &>/dev/null
then
	echo -e "\u26D4 Hung process (killed)"
	score=0
else 
	wait $!
	result=$?
	if [ "$result" -ne 0 ]
	then
		score=0
		echo -e "\u26D4 Returned $result"
	fi
	if
		! cmp -s "$outpipe" <<< "6 7"
	then
		score=0
		echo -e "\u26D4 dimensions output is incorrect"
	fi
	if [ -s "$errpipe" ]
	then
		score=0
		echo -e "\u26D4 stderr is non-empty"
	fi
fi
if [ $score -ne 0 ]
then
	echo -e "\u2705 Passed!"
fi
((points+=score))



echo "-93	-92	29	-45	-55	-87	-36	39	-88	71	7	-69	52	45	-22
50	-27	85	11	-76	-3	23	68	58	-5	34	7	-29	-49	41
-61	2	-44	-62	47	-77	33	6	-7	55	-45	99	2	20	89
52	-97	57	-39	-76	62	24	69	-74	89	-76	1	-46	-27	-9
55	77	42	10	-98	-22	15	-48	26	33	-7	29	-34	78	-19
23	25	-40	16	-63	-12	42	45	-22	20	44	-23	78	-50	17
-67	14	-9	-58	38	-78	2	99	-87	-92	-34	-29	-7	-31	11
65	-32	27	91	-46	-13	-71	37	24	-5	34	-92	6	15	-15
-49	23	-52	-9	59	-57	-78	-10	17	-27	44	-34	-62	22	-94
-45	-45	88	-12	-64	1	-60	-35	11	1	-10	52	5	52	-17
-91	61	90	22	82	-9	82	85	10	56	18	4	-18	-92	-46
31	98	47	-12	-60	20	54	-8	92	24	-71	-23	24	91	37
-12	98	-13	66	72	-14	88	51	75	5	40	-91	91	-94	26
0	60	-41	6	28	-54	97	56	40	-17	94	-92	-23	-3	-91
10	-26	78	-22	-55	73	-82	-49	-26	-63	-80	8	97	87	-27
80	17	90	22	6	45	23	91	16	-93	-38	-64	-75	35	61
21	-24	-38	92	-43	98	-14	35	39	-65	-20	65	65	19	-81
79	-32	62	-93	89	19	-83	-47	45	20	93	49	43	73	80
24	81	19	-15	48	-46	-23	-63	65	2	75	-16	1	-98	14
-40	-68	-89	-10	90	29	3	-15	58	86	-85	36	-55	31	-79" > "$m1"

echo "-93	50	-61	52	55	23	-67	65	-49	-45	-91	31	-12	0	10	80	21	79	24	-40
-92	-27	2	-97	77	25	14	-32	23	-45	61	98	98	60	-26	17	-24	-32	81	-68
29	85	-44	57	42	-40	-9	27	-52	88	90	47	-13	-41	78	90	-38	62	19	-89
-45	11	-62	-39	10	16	-58	91	-9	-12	22	-12	66	6	-22	22	92	-93	-15	-10
-55	-76	47	-76	-98	-63	38	-46	59	-64	82	-60	72	28	-55	6	-43	89	48	90
-87	-3	-77	62	-22	-12	-78	-13	-57	1	-9	20	-14	-54	73	45	98	19	-46	29
-36	23	33	24	15	42	2	-71	-78	-60	82	54	88	97	-82	23	-14	-83	-23	3
39	68	6	69	-48	45	99	37	-10	-35	85	-8	51	56	-49	91	35	-47	-63	-15
-88	58	-7	-74	26	-22	-87	24	17	11	10	92	75	40	-26	16	39	45	65	58
71	-5	55	89	33	20	-92	-5	-27	1	56	24	5	-17	-63	-93	-65	20	2	86
7	34	-45	-76	-7	44	-34	34	44	-10	18	-71	40	94	-80	-38	-20	93	75	-85
-69	7	99	1	29	-23	-29	-92	-34	52	4	-23	-91	-92	8	-64	65	49	-16	36
52	-29	2	-46	-34	78	-7	6	-62	5	-18	24	91	-23	97	-75	65	43	1	-55
45	-49	20	-27	78	-50	-31	15	22	52	-92	91	-94	-3	87	35	19	73	-98	31
-22	41	89	-9	-19	17	11	-15	-94	-17	-46	37	26	-91	-27	61	-81	80	14	-79" > "$m2"



echo "Transposing hardcoded matrix (10pt):"
score=10
SECONDS=0
./"$cmd" transpose "$m1" 1>"$outpipe" 2>"$errpipe" &
while
	[ $SECONDS -lt 30 ] && kill -0 $! &>/dev/null
do
	:
done
if
	kill -9 $! &>/dev/null
then
	echo -e "\u26D4 Hung process (killed)"
	score=0
else 
	wait $!
	result=$?
	if [ "$result" -ne 0 ]
	then
		score=0
		echo -e "\u26D4 Returned $result"
	fi
	if
		! cmp -s "$outpipe" "$m2"
	then
		score=0
		echo -e "\u26D4 Transposed matrix does not match known result"
	fi
	if [ -s "$errpipe" ]
	then
		score=0
		echo -e "\u26D4 stderr is non-empty"
	fi
fi
if [ $score -ne 0 ]
then
	echo -e "\u2705 Passed!"
fi
((points+=score))


echo "Transpose involution test on m3 (5pt):" 
score=5
SECONDS=0
./"$cmd" transpose "$m3" 1>"$outpipe" 2>"$errpipe" &
while
	[ $SECONDS -lt 30 ] && kill -0 $! &>/dev/null
do
	:
done
if
	kill -9 $! &>/dev/null
then
	echo -e "\u26D4 Hung process (killed)"
	score=0
else 
	wait $!
	result=$?
	if [ "$result" -ne 0 ]
	then
		score=0
		echo -e "\u26D4 Returned $result"
	fi
	if [ -s "$errpipe" ]
	then
		score=0
		echo -e "\u26D4 stderr is non-empty"
	fi
fi
SECONDS=0
./"$cmd" transpose "$outpipe" 1>"$outpipe2" 2>"$errpipe2" & # Transpose result
while
	[ $SECONDS -lt 30 ] && kill -0 $! &>/dev/null
do
	:
done
if
	kill -9 $! &>/dev/null
then
	echo -e "\u26D4 Hung process (killed)"
	score=0
else 
	wait $!
	result=$?
	if [ "$result" -ne 0 ]
	then
		score=0
		echo -e "\u26D4 Returned $result"
	fi
	if [ -s "$errpipe2" ]
	then
		score=0
		echo -e "\u26D4 stderr is non-empty"
	fi
fi
if
	! cmp -s "$outpipe2" "$m3"
then
	score=0
	echo -e "\u26D4 Transpose is not involutory"
fi
if [ $score -ne 0 ]
then
	echo -e "\u2705 Passed!"
fi
((points+=score))


echo "Transpose involution test on m4 (5pt):" 
score=5
SECONDS=0
./"$cmd" transpose "$m4" 1>"$outpipe" 2>"$errpipe" &
while
	[ $SECONDS -lt 30 ] && kill -0 $! &>/dev/null
do
	:
done
if
	kill -9 $! &>/dev/null
then
	echo -e "\u26D4 Hung process (killed)"
	score=0
else 
	wait $!
	result=$?
	if [ "$result" -ne 0 ]
	then
		score=0
		echo -e "\u26D4 Returned $result"
	fi
	if [ -s "$errpipe" ]
	then
		score=0
		echo -e "\u26D4 stderr is non-empty"
	fi
fi
SECONDS=0
./"$cmd" transpose "$outpipe" 1>"$outpipe2" 2>"$errpipe2" & # Transpose result
while
	[ $SECONDS -lt 30 ] && kill -0 $! &>/dev/null
do
	:
done
if
	kill -9 $! &>/dev/null
then
	echo -e "\u26D4 Hung process (killed)"
	score=0
else 
	wait $!
	result=$?
	if [ "$result" -ne 0 ]
	then
		score=0
		echo -e "\u26D4 Returned $result"
	fi
	if [ -s "$errpipe2" ]
	then
		score=0
		echo -e "\u26D4 stderr is non-empty"
	fi
fi
if
	! cmp -s "$outpipe2" "$m4"
then
	score=0
	echo -e "\u26D4 Transpose is not involutory"
fi
if [ $score -ne 0 ]
then
	echo -e "\u2705 Passed!"
fi
((points+=score))


echo "Transpose involution test on m5 (5pt):" 
score=5
SECONDS=0
./"$cmd" transpose "$m5" 1>"$outpipe" 2>"$errpipe" &
while
	[ $SECONDS -lt 30 ] && kill -0 $! &>/dev/null
do
	:
done
if
	kill -9 $! &>/dev/null
then
	echo -e "\u26D4 Hung process (killed)"
	score=0
else 
	wait $!
	result=$?
	if [ "$result" -ne 0 ]
	then
		score=0
		echo -e "\u26D4 Returned $result"
	fi
	if [ -s "$errpipe" ]
	then
		score=0
		echo -e "\u26D4 stderr is non-empty"
	fi
fi
SECONDS=0
./"$cmd" transpose "$outpipe" 1>"$outpipe2" 2>"$errpipe2" & # Transpose result
while
	[ $SECONDS -lt 30 ] && kill -0 $! &>/dev/null
do
	:
done
if
	kill -9 $! &>/dev/null
then
	echo -e "\u26D4 Hung process (killed)"
	score=0
else 
	wait $!
	result=$?
	if [ "$result" -ne 0 ]
	then
		score=0
		echo -e "\u26D4 Returned $result"
	fi
	if [ -s "$errpipe2" ]
	then
		score=0
		echo -e "\u26D4 stderr is non-empty"
	fi
fi
if
	! cmp -s "$outpipe2" "$m5"
then
	score=0
	echo -e "\u26D4 Transpose is not involutory"
fi
if [ $score -ne 0 ]
then
	echo -e "\u2705 Passed!"
fi
((points+=score))


echo "-28	91	29	-5	12	83
-94	-16	41	-28	6	86
-44	83	-9	64	92	-70
41	22	66	29	55	49
6	52	4	17	-29	52
-8	-33	96	-73	-76	92
-32	94	45	4	43	-97
-57	36	86	90	35	75
46	8	4	-83	-94	-52
0	-6	-90	48	70	11
-33	-41	-76	68	30	-19
70	96	-85	-1	-9	-62
58	63	-4	22	-69	-25
-75	-65	-78	76	39	10
-99	-69	63	53	35	67
73	51	55	-26	-14	9
-90	-19	-19	-63	-96	23
-62	89	93	98	48	-21
-56	-86	24	45	-79	-65
-79	-60	87	-74	44	20
-38	50	-50	-38	-36	30
-69	-66	-23	42	55	-5
-62	98	47	-86	23	-85
-2	-55	-79	-41	-12	-31
-84	-10	46	16	-56	-26
70	-16	-17	99	24	-41
80	47	-59	-55	80	96
-54	43	24	-82	1	-46
25	-1	92	16	44	-21
62	58	79	-97	-21	-62
-46	-23	-68	59	-72	-6
-78	59	11	52	-96	-77
5	77	64	-29	-98	69
-68	69	74	32	-71	91
38	-75	25	-61	-29	73
40	25	29	93	93	-27
-1	-16	-14	48	99	-36
81	6	35	-90	4	-57
80	84	63	-10	80	-99
96	-5	-81	11	37	-46" > "$m1"
echo "-10	16	13	4	2	-4" > "$m2"
echo "Mean on hardcoded matrix (15pt):"
score=15
SECONDS=0
./"$cmd" mean "$m1" 1>"$outpipe" 2>"$errpipe" &
while
	[ $SECONDS -lt 30 ] && kill -0 $! &>/dev/null
do
	:
done
if
	kill -9 $! &>/dev/null
then
	echo -e "\u26D4 Hung process (killed)"
	score=0
else 
	wait $!
	result=$?
	if [ "$result" -ne 0 ]
	then
		score=0
		echo -e "\u26D4 Returned $result"
	fi
	if
		! cmp -s "$outpipe" "$m2"
	then
		score=0
		echo -e "\u26D4 Output result does not match known result"
	fi
	if [ -s "$errpipe" ]
	then
		score=0
		echo -e "\u26D4 stderr is non-empty"
	fi
fi
if [ $score -ne 0 ]
then
	echo -e "\u2705 Passed!"
fi
((points+=score))


generate 5 10 0 0 > "$m1"
generate 1 10 0 0 > "$m2"
echo "Mean on 5x10 zero matrix (2pt):"
score=2
SECONDS=0
./"$cmd" mean "$m1" 1>"$outpipe" 2>"$errpipe" &
while
	[ $SECONDS -lt 30 ] && kill -0 $! &>/dev/null
do
	:
done
if
	kill -9 $! &>/dev/null
then
	echo -e "\u26D4 Hung process (killed)"
	score=0
else 
	wait $!
	result=$?
	if [ "$result" -ne 0 ]
	then
		score=0
		echo -e "\u26D4 Returned $result"
	fi
	if
		! cmp  -s "$outpipe" "$m2"
	then
		score=0
		echo -e "\u26D4 Output result does not match known result"
	fi
	if [ -s "$errpipe" ]
	then
		score=0
		echo -e "\u26D4 stderr is non-empty"
	fi
fi
if [ $score -ne 0 ]
then
	echo -e "\u2705 Passed!"
fi
((points+=score))


generate 5 10 1 1 > "$m1"
generate 1 10 1 1 > "$m2"
echo "Mean on 5x10 all ones matrix (5pt):"
score=5
SECONDS=0
./"$cmd" mean "$m1" 1>"$outpipe" 2>"$errpipe" &
while
	[ $SECONDS -lt 30 ] && kill -0 $! &>/dev/null
do
	:
done
if
	kill -9 $! &>/dev/null
then
	echo -e "\u26D4 Hung process (killed)"
	score=0
else 
	wait $!
	result=$?
	if [ "$result" -ne 0 ]
	then
		score=0
		echo -e "\u26D4 Returned $result"
	fi
	if
		! cmp  -s "$outpipe" "$m2"
	then
		score=0
		echo -e "\u26D4 Output result does not match known result"
	fi
	if [ -s "$errpipe" ]
	then
		score=0
		echo -e "\u26D4 stderr is non-empty"
	fi
fi
if [ $score -ne 0 ]
then
	echo -e "\u2705 Passed!"
fi
((points+=score))


ident 10 10 > "$m1"
generate 1 10 0 0 > "$m2"
echo "Mean on 10x10 identity matrix (5pt):"
score=5
SECONDS=0
./"$cmd" mean "$m1" 1>"$outpipe" 2>"$errpipe" &
while
	[ $SECONDS -lt 30 ] && kill -0 $! &>/dev/null
do
	:
done
if
	kill -9 $! &>/dev/null
then
	echo -e "\u26D4 Hung process (killed)"
	score=0
else 
	wait $!
	result=$?
	if [ "$result" -ne 0 ]
	then
		score=0
		echo -e "\u26D4 Returned $result"
	fi
	if
		! cmp  -s "$outpipe" "$m2"
	then
		score=0
		echo -e "\u26D4 Output result does not match known result"
	fi
	if [ -s "$errpipe" ]
	then
		score=0
		echo -e "\u26D4 stderr is non-empty"
	fi
fi
if [ $score -ne 0 ]
then
	echo -e "\u2705 Passed!"
fi
((points+=score))


generate 5 10 0 0 > "$m1"
generate 5 10 0 0 > "$m2"
echo "0 + 0 == 0? (2pt):"
score=2
SECONDS=0
./"$cmd" add "$m1" "$m2" 1>"$outpipe" 2>"$errpipe" &
while
	[ $SECONDS -lt 30 ] && kill -0 $! &>/dev/null
do
	:
done
if
	kill -9 $! &>/dev/null
then
	echo -e "\u26D4 Hung process (killed)"
	score=0
else 
	wait $!
	result=$?
	if [ "$result" -ne 0 ]
	then
		score=0
		echo -e "\u26D4 Returned $result"
	fi
	if
		! cmp  -s "$outpipe" "$m1"
	then
		score=0
		echo -e "\u26D4 Output result does not match known result"
	fi
	if [ -s "$errpipe" ]
	then
		score=0
		echo -e "\u26D4 stderr is non-empty"
	fi
fi
if [ $score -ne 0 ]
then
	echo -e "\u2705 Passed!"
fi
((points+=score))


generate 5 10 -100 100 > "$m1"
generate 5 10 0 0 > "$m2"
echo "X + 0 == X? (5pt):"
score=5
SECONDS=0
./"$cmd" add "$m1" "$m2" 1>"$outpipe" 2>"$errpipe" &
while
	[ $SECONDS -lt 30 ] && kill -0 $! &>/dev/null
do
	:
done
if
	kill -9 $! &>/dev/null
then
	echo -e "\u26D4 Hung process (killed)"
	score=0
else 
	wait $!
	result=$?
	if [ "$result" -ne 0 ]
	then
		score=0
		echo -e "\u26D4 Returned $result"
	fi
	if
		! cmp  -s "$outpipe" "$m1"
	then
		score=0
		echo -e "\u26D4 Output result does not match known result"
	fi
	if [ -s "$errpipe" ]
	then
		score=0
		echo -e "\u26D4 stderr is non-empty"
	fi
fi
if [ $score -ne 0 ]
then
	echo -e "\u2705 Passed!"
fi
((points+=score))


generate 5 10 0 0 > "$m1"
generate 5 10 -100 100 > "$m2"
echo "0 + X == X? (5pt):"
score=5
SECONDS=0
./"$cmd" add "$m1" "$m2" 1>"$outpipe" 2>"$errpipe" &
while
	[ $SECONDS -lt 30 ] && kill -0 $! &>/dev/null
do
	:
done
if
	kill -9 $! &>/dev/null
then
	echo -e "\u26D4 Hung process (killed)"
	score=0
else 
	wait $!
	result=$?
	if [ "$result" -ne 0 ]
	then
		score=0
		echo -e "\u26D4 Returned $result"
	fi
	if
		! cmp  -s "$outpipe" "$m2"
	then
		score=0
		echo -e "\u26D4 Output result does not match known result"
	fi
	if [ -s "$errpipe" ]
	then
		score=0
		echo -e "\u26D4 stderr is non-empty"
	fi
fi
if [ $score -ne 0 ]
then
	echo -e "\u2705 Passed!"
fi
((points+=score))


generate 10 10 -100 100 > "$m1"
generate 10 10 -100 100 > "$m2"
echo "A + B == B + A? (1pt):"
score=1
SECONDS=0
./"$cmd" add "$m1" "$m2" 1>"$outpipe" 2>"$errpipe" &
while
	[ $SECONDS -lt 30 ] && kill -0 $! &>/dev/null
do
	:
done
if
	kill -9 $! &>/dev/null
then
	echo -e "\u26D4 Hung process (killed)"
	score=0
else 
	wait $!
	result=$?
	if [ "$result" -ne 0 ]
	then
		score=0
		echo -e "\u26D4 Returned $result"
	fi
	if [ -s "$errpipe" ]
	then
		score=0
		echo -e "\u26D4 stderr is non-empty"
	fi
fi
SECONDS=0
./"$cmd" add "$m2" "$m1" 1>"$outpipe2" 2>"$errpipe2" &
while
	[ $SECONDS -lt 30 ] && kill -0 $! &>/dev/null
do
	:
done
if
	kill -9 $! &>/dev/null
then
	echo -e "\u26D4 Hung process (killed)"
	score=0
else 
	wait $!
	result=$?
	if [ "$result" -ne 0 ]
	then
		score=0
		echo -e "\u26D4 Returned $result"
	fi
	if [ -s "$errpipe2" ]
	then
		score=0
		echo -e "\u26D4 stderr is non-empty"
	fi
fi
if
	! cmp -s "$outpipe" "$outpipe2"
then
	score=0
	echo -e "\u26D4 The two results are not equal"
fi
if [ $score -ne 0 ]
then
	echo -e "\u2705 Passed!"
fi
((points+=score))


echo "68	86	-22	95	-97	44	68	-98	70	-65	69	94	-5	-84	3	83	71	31	-10	0
-82	74	-87	94	56	27	-45	-45	12	-75	-76	0	42	-76	75	14	21	-30	87	-8
-58	62	78	70	57	-93	2	-82	-2	-55	46	68	98	-91	40	73	-87	-45	58	78
45	45	42	93	47	-66	62	-70	84	92	82	-18	51	-55	-5	19	-16	58	-29	56
-95	-24	-72	23	40	67	-8	58	41	-5	54	-11	-22	68	73	90	92	20	89	39
74	-7	59	27	39	-97	-97	75	40	21	13	36	-83	-85	-84	-92	44	-7	83	-88
-33	-59	-91	-33	-23	-55	-77	-50	14	39	91	-39	58	-29	-75	-25	28	-84	-91	35
94	-78	97	24	94	-74	10	81	67	42	-67	-72	-66	-48	71	11	77	0	56	92
56	-36	70	-8	50	53	92	-81	99	45	96	-37	38	60	-37	61	-62	99	23	79
93	47	-87	-56	-60	-43	-7	75	82	-13	-81	-66	38	-23	30	-46	7	-30	-25	89
-8	73	1	-25	-53	-82	-82	23	0	-5	26	-13	-13	-4	-57	-52	26	96	-40	47
66	-83	84	-51	6	-5	62	-51	-54	-50	31	-6	88	55	-73	35	32	-91	-23	2
73	37	89	90	-45	89	-56	-12	3	79	-26	-18	81	93	-92	3	-17	23	-64	-4
-45	-57	59	-72	-84	44	38	70	47	-91	-10	-86	18	-40	-21	98	42	79	7	-80
-33	-93	78	-16	91	-62	55	82	-93	1	72	-31	-52	-8	-55	-89	66	-99	10	-25
64	-66	-56	19	19	80	-32	-16	73	10	25	-20	-57	30	29	81	30	-90	33	-54
68	-7	53	52	-55	-44	-94	-20	-76	29	88	33	-26	30	-45	90	-56	85	60	38
-28	17	90	-42	-35	87	-2	-47	94	-35	78	-33	59	-22	8	-5	0	-36	33	-27
-92	75	91	-97	19	76	33	20	-10	-80	-20	3	98	-35	75	-28	-48	-73	-68	72
17	45	43	-90	-12	4	86	-49	88	-92	39	-18	5	-10	-69	-5	-56	3	-21	10" > "$m1"
echo "24	52	55	-34	-9	74	-79	-78	5	-42	48	-53	-77	47	-53	97	-72	20	77	-28
-66	-54	72	-46	-25	53	-95	-3	-75	-62	92	-43	-57	0	78	-63	-90	12	-60	-77
65	43	-17	65	25	-77	57	27	71	98	83	53	28	-87	-56	-20	67	-3	-16	-61
-99	19	30	34	-10	-73	1	-18	7	99	-76	71	-72	-78	20	56	66	20	10	34
-54	-37	54	18	-11	56	-81	-97	64	-56	68	-36	25	-26	-56	91	-60	12	-9	17
-41	-78	-54	-19	-38	-76	17	-90	91	-5	82	-31	-92	93	-57	43	-56	14	29	-45
78	22	-94	12	-76	44	-63	-11	-11	-89	-81	53	56	6	27	30	-26	-79	-12	-50
52	15	-44	63	8	40	91	10	-26	-35	-61	21	-55	82	-97	-90	-25	10	-60	2
-33	-48	-46	-64	-41	-61	84	54	0	26	-56	75	-81	-53	-68	-2	-93	-46	29	85
53	-93	-31	51	78	75	69	10	-20	-68	-67	-67	-54	23	76	32	-11	-86	-55	-14
5	93	35	41	-26	41	77	88	43	44	-89	91	66	-11	90	-44	-61	-10	-66	-51
82	83	-44	55	69	80	1	-29	8	-87	-93	-91	-53	44	3	-3	24	13	86	33
92	5	-75	58	-4	-52	-2	-3	78	9	83	59	82	-4	-57	76	25	-49	-44	15
75	64	44	-23	43	56	60	0	46	26	39	53	-85	53	14	81	77	-11	-32	33
75	0	68	-5	-78	-13	90	-51	-15	-40	77	-95	-37	59	-20	30	38	-32	4	-76
-63	-28	58	-62	-42	93	15	79	32	-42	4	-98	85	-85	-31	-38	28	-31	-17	17
-65	-66	-8	26	-7	79	18	33	21	66	41	-22	20	66	-9	-64	-71	-67	-16	68
19	12	55	6	-90	-60	1	26	64	-29	62	96	89	11	-77	-26	76	-62	-48	-36
-76	-21	-93	85	-5	-99	41	-74	47	1	-44	47	16	83	30	-38	33	-53	80	-11
94	20	-76	-25	17	-5	3	-78	92	-33	-13	14	85	-52	-55	5	75	-89	9	-16" >"$m2"
echo "92	138	33	61	-106	118	-11	-176	75	-107	117	41	-82	-37	-50	180	-1	51	67	-28
-148	20	-15	48	31	80	-140	-48	-63	-137	16	-43	-15	-76	153	-49	-69	-18	27	-85
7	105	61	135	82	-170	59	-55	69	43	129	121	126	-178	-16	53	-20	-48	42	17
-54	64	72	127	37	-139	63	-88	91	191	6	53	-21	-133	15	75	50	78	-19	90
-149	-61	-18	41	29	123	-89	-39	105	-61	122	-47	3	42	17	181	32	32	80	56
33	-85	5	8	1	-173	-80	-15	131	16	95	5	-175	8	-141	-49	-12	7	112	-133
45	-37	-185	-21	-99	-11	-140	-61	3	-50	10	14	114	-23	-48	5	2	-163	-103	-15
146	-63	53	87	102	-34	101	91	41	7	-128	-51	-121	34	-26	-79	52	10	-4	94
23	-84	24	-72	9	-8	176	-27	99	71	40	38	-43	7	-105	59	-155	53	52	164
146	-46	-118	-5	18	32	62	85	62	-81	-148	-133	-16	0	106	-14	-4	-116	-80	75
-3	166	36	16	-79	-41	-5	111	43	39	-63	78	53	-15	33	-96	-35	86	-106	-4
148	0	40	4	75	75	63	-80	-46	-137	-62	-97	35	99	-70	32	56	-78	63	35
165	42	14	148	-49	37	-58	-15	81	88	57	41	163	89	-149	79	8	-26	-108	11
30	7	103	-95	-41	100	98	70	93	-65	29	-33	-67	13	-7	179	119	68	-25	-47
42	-93	146	-21	13	-75	145	31	-108	-39	149	-126	-89	51	-75	-59	104	-131	14	-101
1	-94	2	-43	-23	173	-17	63	105	-32	29	-118	28	-55	-2	43	58	-121	16	-37
3	-73	45	78	-62	35	-76	13	-55	95	129	11	-6	96	-54	26	-127	18	44	106
-9	29	145	-36	-125	27	-1	-21	158	-64	140	63	148	-11	-69	-31	76	-98	-15	-63
-168	54	-2	-12	14	-23	74	-54	37	-79	-64	50	114	48	105	-66	-15	-126	12	61
111	65	-33	-115	5	-1	89	-127	180	-125	26	-4	90	-62	-124	0	19	-86	-12	-6" >"$m3"
echo "(harcoded) A + B == C? (15pt):"
score=15
SECONDS=0
./"$cmd" add "$m1" "$m2" 1>"$outpipe" 2>"$errpipe" &
while
	[ $SECONDS -lt 30 ] && kill -0 $! &>/dev/null
do
	:
done
if
	kill -9 $! &>/dev/null
then
	echo -e "\u26D4 Hung process (killed)"
	score=0
else 
	wait $!
	result=$?
	if [ "$result" -ne 0 ]
	then
		score=0
		echo -e "\u26D4 Returned $result"
	fi
	if
		! cmp -s  "$outpipe" "$m3"
	then
		score=0
		echo -e "\u26D4 Output result does not match known result"
	fi
	if [ -s "$errpipe" ]
	then
		score=0
		echo -e "\u26D4 stderr is non-empty"
	fi
fi
if [ $score -ne 0 ]
then
	echo -e "\u2705 Passed!"
fi
((points+=score))


ident 10 10 > "$m1"
generate 10 15 -100 100 > "$m2"
echo "I * M == M? (5pt):"
score=5
SECONDS=0
./"$cmd" multiply "$m1" "$m2" 1>"$outpipe" 2>"$errpipe" &
while
	[ $SECONDS -lt 120 ] && kill -0 $! &>/dev/null
do
	:
done
if
	kill -9 $! &>/dev/null
then
	echo -e "\u26D4 Hung process (killed)"
	score=0
else 
	wait $!
	result=$?
	if [ "$result" -ne 0 ]
	then
		score=0
		echo -e "\u26D4 Returned $result"
	fi
	if
		! cmp  -s "$outpipe" "$m2"
	then
		score=0
		echo -e "\u26D4 Output result does not match known result"
	fi
	if [ -s "$errpipe" ]
	then
		score=0
		echo -e "\u26D4 stderr is non-empty"
	fi
fi
if [ $score -ne 0 ]
then
	echo -e "\u2705 Passed!"
fi
((points+=score))


generate 15 10 -100 100 > "$m1"
ident 10 10 > "$m2"
echo "M * I == M? (5pt):"
score=5
SECONDS=0
./"$cmd" multiply "$m1" "$m2" 1>"$outpipe" 2>"$errpipe" &
while
	[ $SECONDS -lt 120 ] && kill -0 $! &>/dev/null
do
	:
done
if
	kill -9 $! &>/dev/null
then
	echo -e "\u26D4 Hung process (killed)"
	score=0
else 
	wait $!
	result=$?
	if [ "$result" -ne 0 ]
	then
		score=0
		echo -e "\u26D4 Returned $result"
	fi
	if
		! cmp  -s "$outpipe" "$m1"
	then
		score=0
		echo -e "\u26D4 Output result does not match known result"
	fi
	if [ -s "$errpipe" ]
	then
		score=0
		echo -e "\u26D4 stderr is non-empty"
	fi
fi
if [ $score -ne 0 ]
then
	echo -e "\u2705 Passed!"
fi
((points+=score))


generate 30 14 0 0 > "$m1"
generate 14 12 -100 100 > "$m2"
generate 30 12 0 0 > "$m3"
echo "0 * M == 0? (5pt):"
score=5
SECONDS=0
./"$cmd" multiply "$m1" "$m2" 1>"$outpipe" 2>"$errpipe" &
while
	[ $SECONDS -lt 120 ] && kill -0 $! &>/dev/null
do
	:
done
if
	kill -9 $! &>/dev/null
then
	echo -e "\u26D4 Hung process (killed)"
	score=0
else 
	wait $!
	result=$?
	if [ "$result" -ne 0 ]
	then
		score=0
		echo -e "\u26D4 Returned $result"
	fi
	if
		! cmp  -s "$outpipe" "$m3"
	then
		score=0
		echo -e "\u26D4 Output result does not match known result"
	fi
	if [ -s "$errpipe" ]
	then
		score=0
		echo -e "\u26D4 stderr is non-empty"
	fi
fi
if [ $score -ne 0 ]
then
	echo -e "\u2705 Passed!"
fi
((points+=score))






echo "-80	85	-94	93	28	88	47	37	9	8	51	-39	-63	56	-72	-39	80	-77	-22	70	44	-47	57	62	-77
-66	22	-30	10	-16	-26	16	-93	2	-87	32	-26	-52	99	-59	8	4	62	-66	-73	-46	-7	-88	-14	94
-41	82	45	-37	-18	-66	0	99	-28	42	-13	-22	67	52	30	87	95	-14	-7	-68	2	-12	-98	-49	79
-33	40	-76	80	97	18	-63	39	-74	99	60	-64	31	-88	32	74	97	-66	21	11	45	-5	-21	-4	-12
-78	8	5	-13	-79	-67	-23	-40	-80	13	-73	-62	-38	-13	-77	40	-61	88	81	-6	72	-71	-40	53	-26
-85	2	46	-79	-95	15	-86	35	-57	12	-94	-1	-9	-42	52	0	38	65	1	41	-52	-1	-92	-28	57
60	71	-22	-90	-3	-53	-16	-90	98	-88	53	-81	-67	-51	-24	-74	90	36	99	-57	59	-14	10	-90	-53
-23	-84	-29	2	-19	96	4	-6	-54	-73	38	-6	99	-57	63	40	-83	19	-15	80	-90	34	68	-22	-78
2	-74	-18	-45	-16	12	-99	8	98	59	72	20	-51	37	-28	56	11	79	0	94	-48	10	69	60	-22
68	-48	12	-14	51	-11	10	-97	-88	-87	9	-77	-99	79	-38	75	-48	-78	38	-51	-3	-47	66	-5	-52
-87	-93	1	93	13	77	31	79	-97	-73	-69	7	97	-24	-14	-7	-40	53	58	-14	-78	33	26	51	-48
17	-18	-90	-27	86	9	-97	51	27	-26	-81	-44	-34	-28	71	-56	-17	-88	3	89	14	59	-7	-45	-47
-5	96	-80	30	-1	43	31	-21	70	94	2	16	49	46	-67	69	0	64	-53	32	-94	96	-76	87	21
-1	72	35	67	-23	15	51	-3	-75	4	65	-44	-1	-43	66	-21	-88	-59	-29	-25	30	-57	21	-81	-76
-36	94	59	-2	86	25	-10	53	63	-24	67	49	-37	-39	-39	-14	67	-77	-76	74	46	-34	-3	85	-54
-26	-48	-34	-24	-75	67	33	6	43	-82	-25	58	82	-13	47	66	18	-39	57	-29	-8	85	15	-98	-83
-21	9	-7	86	4	72	17	-73	-64	71	-48	65	-38	-51	-2	-10	7	-69	-21	-33	-96	-26	37	25	-23
-52	-72	23	-46	7	70	12	11	-36	-38	28	71	-22	46	-40	-86	70	81	81	-34	76	-32	-90	92	77
35	-13	-63	42	63	97	-52	94	49	69	-66	25	21	42	98	93	15	-35	-13	90	-4	65	-35	34	-10
-15	-36	-49	-2	-42	-64	43	78	-31	-56	-89	-9	37	-8	-55	-4	17	74	79	55	17	46	6	85	34
49	-53	81	36	-10	-82	-48	-4	-50	11	-11	62	46	-64	74	-18	13	-76	0	87	92	-65	-37	58	50
-68	-6	62	8	93	58	80	35	55	45	21	41	0	88	-13	-47	36	45	43	-25	-40	-25	-81	-8	-40
-85	-30	-4	-17	73	-30	15	97	-64	63	81	86	3	-40	-36	7	83	43	-91	42	0	77	96	-77	10
56	46	-10	12	0	-21	2	-6	7	72	65	14	-67	2	0	53	-36	92	-52	-30	59	-86	-13	-61	-94
-74	-78	50	-78	36	81	-19	11	81	19	78	-55	-69	66	41	4	92	74	-58	-43	24	25	-74	-58	95" > "$m1"
echo "-46	19	8	23	22	-11	34	10	-36	-90	58	-23	-73	-73	-92	40	78	-43	-59	34	0	-83	72	0	-23
89	-13	42	-49	52	15	37	68	-90	85	-29	37	53	67	92	-61	-69	16	-91	81	-96	-48	30	63	-4
60	70	92	-43	91	-21	85	-38	-13	16	73	50	55	-21	79	56	-39	-15	94	-32	-57	-3	-19	0	97
87	86	-58	-11	-47	-50	-36	-67	-47	-83	-79	-44	-63	75	-80	-67	-27	22	4	-50	30	-69	39	-12	6
51	-94	26	3	-36	-50	-65	-66	36	-11	62	-39	5	4	65	85	-9	-42	82	41	-19	-43	-86	-71	16
86	-30	-46	59	56	81	-95	-97	-42	71	28	52	-15	-36	3	-81	-33	-5	-75	-73	-3	-17	-83	70	-71
-37	66	4	50	61	-92	86	-20	85	-74	-1	-41	39	73	-49	34	-41	29	-65	47	44	-48	-4	98	66
-73	-34	32	17	3	80	-45	-20	65	59	-14	-15	-60	62	5	-56	-51	70	-98	5	55	67	-67	-95	-51
13	54	15	-72	-67	48	98	45	17	48	45	37	99	87	-96	-75	36	-82	65	-72	88	83	-29	-14	-62
-43	-18	-77	73	-59	-42	4	-45	68	-15	15	71	-20	7	-80	21	-32	63	-11	73	34	3	63	68	60
-75	65	88	90	-28	-68	-14	57	-11	12	-97	3	83	76	-53	-42	-35	24	-35	-89	-96	58	-3	70	52
-71	18	-37	12	-72	14	93	32	63	47	64	-14	15	62	55	69	-75	-83	1	39	85	80	25	-98	-19
-9	-71	-87	35	-47	-11	-48	-8	-80	-68	-53	59	-38	-64	6	5	14	60	-70	-18	28	44	-2	1	-71
3	89	0	17	-16	-24	30	58	-67	-29	8	-5	30	-75	31	-62	7	-81	89	66	92	71	-34	20	41
-34	-11	-70	0	73	80	43	29	-67	-9	6	-94	-19	-46	44	-45	69	51	-78	-66	71	48	60	-51	-27
21	-90	-37	-35	21	-25	29	96	26	-90	-46	45	89	69	-20	99	98	-91	-40	-54	45	-42	51	-39	-71
-32	3	10	40	63	42	-3	-55	-53	-8	-98	-48	-43	-50	34	95	-98	95	83	55	-75	-61	-91	28	43
-88	66	9	-83	84	4	-47	-48	42	-11	-9	-85	0	-68	-45	-69	-52	68	94	34	4	94	-42	73	-50
26	-56	75	-14	-4	-92	72	-8	75	-79	49	47	-55	79	57	-53	8	98	85	12	-47	-54	-61	-24	55
72	31	18	-57	-85	-76	-65	46	14	-64	-61	81	92	89	-76	-96	29	-57	98	-21	-75	-66	14	19	-7
39	-24	-64	-85	80	-75	-48	57	-5	-52	-60	-73	27	-56	-90	-43	41	-55	-26	-47	-31	96	20	76	-4
17	-20	-98	28	87	70	54	-45	-61	90	2	48	0	97	53	-48	67	86	16	50	97	69	-8	48	-11
70	-26	-63	55	62	-70	-41	-28	-46	-93	-54	-69	90	-52	11	98	28	64	-31	76	-59	-67	44	-77	-85
-65	54	57	90	0	6	-70	77	-90	31	-3	87	-24	28	-84	23	0	36	71	-76	-84	12	95	49	-95
77	32	-46	-1	-28	81	15	-58	30	92	-36	-29	7	57	58	-70	60	11	11	63	-72	-3	-29	23	76" > "$m2"
echo "22965	9925	9346	19084	-9241	-22200	-36903	4840	-17439	-3760	-35169	5378	12600	18991	-22989	-14725	-32343	1170	-2937	-4396	-26805	-23470	-8985	20005	-7504
880	30761	12175	-10504	-1443	14140	9769	4237	-4832	26530	-6612	-12224	9775	-1188	14763	-14504	-5753	-19864	21746	7281	500	13720	-14866	23709	21727
-9033	-13102	-124	-6371	9272	23489	19677	6070	1264	16454	-13560	922	-8722	1698	32792	4132	-10006	12433	-15426	17814	6717	12988	-14819	593	22482
11783	-40738	-14880	15690	-7899	-8331	-36285	-9546	-6428	-12619	-35913	2784	-17376	12908	-4454	9870	-4642	26274	-21466	-8609	-24396	-25865	-780	-25	448
-1276	-1038	16703	-27882	7215	-26322	-10871	16460	17761	-17116	-2243	7755	-5261	-9371	-10386	-5338	-1291	2751	22528	-2512	-21509	-412	11762	17965	5066
-5136	-7103	-255	-20259	8776	42398	77	-9822	7926	34747	5216	10583	-14472	-8855	31169	-17432	-7493	16197	10568	2730	-1897	13094	-12927	-1666	5970
360	-1799	34125	-26362	25516	-10741	23804	8095	-691	-2552	2953	-21933	8654	-14659	5222	-1985	-2520	6886	16187	6950	-31028	-7076	-21627	15455	15922
848	-17111	-17574	14713	-545	-685	-26151	-6875	-9975	-19816	-7325	8565	5982	-3161	-2395	-3037	16148	9460	-24889	-29229	12651	-5344	4650	-15432	-36871
-16765	11515	8292	3744	-19397	-1659	-11255	14721	5488	-1125	-4057	18644	28033	8904	-30314	-5209	8153	-9628	39049	-12067	-1570	15887	3261	-5141	-24360
17593	-6852	20227	6133	12954	-34998	750	13276	-5882	-38695	11443	-10186	7710	-23947	6911	37162	24979	-29165	7330	-600	-14134	-39266	8709	-10992	10773
-369	-11620	-10411	15335	4309	2728	-30188	-33425	964	-11467	844	3646	-31615	149	13207	-1787	-12522	34973	-975	-11384	17085	-3307	-17114	-19353	-25133
17332	-33041	-13076	-11537	-13530	19009	-18179	-2934	-5603	8477	11143	-190	-12810	304	6621	-16515	24148	-7021	-2448	-2873	14418	-5757	-7791	-30304	-16958
-6475	10715	-13824	12207	-19061	15461	70	6461	-11482	23484	-6500	39111	7910	35755	-18773	-21063	-6039	546	4878	10977	19056	7629	7790	34095	-18612
13667	-1085	515	1508	12389	-21233	-1840	2936	-6831	-16663	-8074	-12920	3468	-4394	-27	-4065	-5894	1869	-50965	-14144	-6933	-15192	18645	4134	11128
10327	4857	28804	1641	-7760	-3323	-13194	17760	-12322	22858	-8966	16024	26871	25153	-5110	9878	-28011	-20824	7434	-18171	-32353	-3025	-4412	-830	-7018
-1661	-23976	-25870	-1377	13205	13390	23373	1051	-1759	-9005	2316	1772	1038	4759	15361	1770	12214	3348	-26808	-13522	44371	13043	-12637	-17474	-22149
13006	-2049	-14014	26626	-10428	-172	-2331	-22127	-3373	575	8922	7956	-13718	3907	8380	27389	-17717	5587	-11905	7024	-389	-36078	16064	-7341	2178
-21234	19490	21361	5777	7470	6996	-9582	-12574	14386	23977	7143	-6457	-19467	-10249	7108	-16446	-28066	9779	39085	-5292	-18801	30944	-34163	22496	17968
7388	-22586	-32992	4524	-13345	25762	-15259	939	-9852	1620	1859	17200	-8712	15878	-12332	-18121	20442	-9515	-8709	-12004	37161	4037	1846	-17026	-33204
-11895	58	681	-7467	4107	-3420	-9721	-302	10751	-10808	-9672	2001	-16882	12518	-7593	-11014	3469	25696	20308	12852	-2101	2624	-5053	190	-14209
-164	2607	-1822	-6818	-15689	-11559	-2459	13706	-3064	-18735	-5615	26	-10923	-3822	-11462	11007	12019	-10753	6484	-20223	-21451	-4704	27280	-15647	10788
-10770	15639	21011	5213	-6157	-4369	12354	-17687	19721	11972	21356	5276	-897	7982	11080	-8174	-37729	817	28742	3704	18163	16758	-37289	8967	22292
-13448	-11410	-18040	16558	2038	-2575	-16333	-23440	20123	9673	-26702	-18295	17314	13159	13797	25471	-26248	21741	-3577	28368	130	13603	-17888	-12320	5062
-20432	7774	7523	-15477	5045	-21824	847	17301	15529	-18691	-616	-18991	12855	-13920	-29755	5548	-10819	-21413	-15771	-3257	5292	8643	15855	10524	-697
-7616	14863	3840	-4490	17884	34495	2052	-19149	9604	39327	-1303	-14717	15286	-11968	8785	-20204	-4881	-3407	26948	-9991	7408	39516	-44128	22859	22855" >"$m3"
echo "(hardcoded) A * B == C? (20pt):"
score=20
SECONDS=0
./"$cmd" multiply "$m1" "$m2" 1>"$outpipe" 2>"$errpipe" &
while
	[ $SECONDS -lt 180 ] && kill -0 $! &>/dev/null
do
	:
done
if
	kill -9 $! &>/dev/null
then
	echo -e "\u26D4 Hung process (killed)"
	score=0
else 
	wait $!
	result=$?
	if [ "$result" -ne 0 ]
	then
		score=0
		echo -e "\u26D4 Returned $result"
	fi
	if
		! cmp  -s "$outpipe" "$m3"
	then
		score=0
		echo -e "\u26D4 Output result does not match known result"
	fi
	if [ -s "$errpipe" ]
	then
		score=0
		echo -e "\u26D4 stderr is non-empty"
	fi
fi
if [ $score -ne 0 ]
then
	echo -e "\u2705 Passed!"
fi
((points+=score))


echo "Total: $points" >&2
exit "$points"
