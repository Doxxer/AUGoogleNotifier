#!/bin/bash
usage() {
    echo "Usage: $0 [t|u|n|p]"
    echo "t -- title"
    echo "n -- user name"
    echo "u -- file URL"    
}

if [ "$#" -lt 1 ]; then
    usage
    exit 1
fi

URL="https://drive.google.com/?authuser=0#recent"
FILENAME="Неизвестный документ"
USERNAME="Неизвестный пользователь"
s=""

while getopts :t:u:n:p: option
do
    case "${option}"
    in
        t) FILENAME=$OPTARG;;
        n) USERNAME=$OPTARG;;
        u) URL=$OPTARG;;
    esac
    if [ -n "$a" ]; then
        echo $a
    fi    
done

s="$s -title \"${FILENAME}\""
s="$s -subtitle \"${USERNAME}\""
s="$s -open \"${URL}\""
s="$s -message \"Документ изменился\""
s="terminal-notifier $s"
eval $s