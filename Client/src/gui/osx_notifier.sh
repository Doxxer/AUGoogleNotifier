#!/bin/bash
usage() {
    echo "Usage: $0 [t|u|n|p]"
    echo "t -- title"
    echo "n -- user name"
    echo "u -- file URL"    
    echo "p -- picture URL"
}

if [ "$#" -lt 1 ]; then
    usage
    exit 1
fi
s=""
while getopts :t:u:n:p: option
do
    case "${option}"
    in
        t) TITLE=${OPTARG}
            s="$s -title \"${OPTARG}\""
            echo 1;;
        n) s="$s -subtitle \"${OPTARG}\"";;
        u) s="$s -open \"${OPTARG}\"";;
        p) FORMAT=$OPTARG;;
    esac
    if [ -n "$a" ]; then
        echo $a
    fi    
done

if [[ -z $s ]]; then
    usage
    exit 1
fi

s="$s -message \"Документ изменился\""
s="$s -sender ru.spbau.mit.AUGoogleNotifier"
s="terminal-notifier $s"
eval $s