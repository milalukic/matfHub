#!/bin/bash

echo -ne "choose option:\\n\\tstsatistics: stat \\n\\tparser: parse \\n"
read OPT

if [[ $OPT != "stat" && $OPT != "parse" ]]
then
    echo "Wrong input"
    echo "Exiting..."
fi

# 
if [[ $OPT == "stat" ]]
then
    echo "Running statistics class test"
    ./scripts/run_stat_test.sh
elif [[ $OPT == "parse" ]]
then
    echo "Running parser class test"
    ./scripts/run_parser_test.sh
    
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