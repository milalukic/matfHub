#!/bin/bash

echo -ne "choose option:\\n\\tstsatistics: st \\n\\tparser: prs \\n\\tplotter: plt \\n\\tmatrix: mx \\n\\tfile manager: fm \\n"
read OPT

if [[ $OPT != "st" && $OPT != "prs" && $OPT != "plt" && $OPT != "mx" && $OPT != "fm" ]]
then
    echo "Wrong input"
    echo "Exiting..."
fi

# 
if [[ $OPT == "st" ]]
then
    echo "Running statistics class test"
    ./scripts/run_stat_test.sh
elif [[ $OPT == "prs" ]]
then
    echo "Running parser class test"
    ./scripts/run_parser_test.sh
elif [[ $OPT == "plt" ]]
then
    echo "Running plotter class test"
    ./scripts/run_plotter_test.sh
elif [[ $OPT == "mx" ]]
then
    echo "Running matrix class test"
    ./scripts/run_matrix_test.sh
elif [[ $OPT == "fm" ]]
then
    echo "Running file manager class test"
    ./scripts/run_fmanager_test.sh
fi

# test -d Catch2
# build_check=$?
# if [[ build_check -eq 1 ]]
# then
#     echo "Catch2 directory not found"
#     echo "Fetching Catch2 from git..."

#     git clone https://github.com/catchorg/Catch2.git
#     cd Catch2
#     git reset --hard 182c910b4b63ff587a3440e08f84f70497e49a81
# fi
