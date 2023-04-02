#!/usr/bin/bash
POINTS=(
            5 5
            1 1 1 1 1
            2 2 2 2 2 2 2 2 2
            3 3 3 3
            3.5 3.5 3.5 3.5 3.5 3.5 3.5 3.5 3.5 3.5
        )
TESTS=30
TOTAL=100
TOTALTESTS=0 
VALGRINDTEST=0

function codingStyle() {
    echo -e "\n..............................CODING STYLE............................."
    	is_style_ok=1
	echo -e "\nRun cpplint\n" > result
#	cpplint --filter=-legal/copyright task1.c task2.c task3.c task4.c tema2.c &>> result
	cpplint --filter=-legal/copyright,-readability/casting,-build/include_subdir,-runtime/threadsafe_fn,-build/header_guard,-runtime/int \
        --linelength=120 $( find . -name \*.h -or -name \*.c ) &>> result
if [ ! $? -eq 0 ]
then
    is_style_ok=0
fi

	echo -e "\nRun clang-tidy\n" >> result
#	clang-tidy  -checks='-*,cppcoreguidelines*' task1.c task2.c task3.c task4.c tema2.c -- &>> result
clang-tidy -checks='-*,cppcoreguidelines*' --warnings-as-errors='-*,cppcoreguidelines*' $( find . -name \*.h -or -name \*.c ) -- &>> result
if [ ! $? -eq 0 ]
then
    is_style_ok=0
fi

if [ $is_style_ok -eq 0 ]
then
    echo "Test Coding Style" "0/15p failed"
else
    echo "Test Coding Style" "15/15p passed"
    let TOTALTESTS+=15
fi
	# Display run summary
	awk -f parse.awk result

	# Display detailed report
	echo -e "\n==================="
	echo -e "= Detailed report ="
	echo -e "===================\n"
	cat result
	echo " "
	echo " "
}

function readme() {
    if test -f "README" || test -f "README.md"
        then
            echo "README: 5/5"
            let TOTALTESTS+=5
    else
        echo "README: 0/5"
    fi
}

function checkExe() {
    if test -f "tema1"
        then
            main
    else
        echo "The executable file needs to be named 'tema1'"
    fi
}

function valgrindTest() {
    valgrind --leak-check=full --error-exitcode=1 ./tema1 1> /dev/null 2>&1
    if [ "$?" -ne 0 ]
        then
            echo "    Valgrind failed"
    else
        echo "  Valgrind test$1.in: 1/1"
        let VALGRINDTEST+=1
    fi
}

function testInput() {
    cat Teste/Input/test$1.in > tema1.in
    ./tema1 

    if [ "$?" -eq 139 ]
        then
            echo "test$1.in: segmentation fault"
            return
    fi

    diff Teste/Output/test$1.out tema1.out > /dev/null 2>&1
    if [ "$?" -ne 0 ]
        then
            echo "test$1.in: 0/${POINTS[$1-1]}"
    else
        echo "test$1.in: ${POINTS[$1-1]}/${POINTS[$1-1]}"
        TOTALTESTS=$(echo "$TOTALTESTS + ${POINTS[$1-1]}" | bc)
        valgrindTest    
    fi
}

function makeCommand() {
    make 1> /dev/null
    if [ "$?" -ne 0 ]
        then
            exit 1
    fi
}

function main() {
    echo " -= TEMA 1 SDA =-"
    echo

    touch tema1.in
    touch tema1.out

    codingStyle
    readme

    for i in $(seq 1 $TESTS)
    do
        testInput $i
    done
    
    echo
    echo "Total: $TOTALTESTS/$TOTAL"
    let VALGRINDTEST=$VALGRINDTEST*20/30
    echo "Valgrind: $VALGRINDTEST/20"
}
makeCommand
checkExe
