#!/bin/bash

if [ $1 == "--help" ]; then
  cat << EOF

This script assumes you have passwordless ssh to the cycle severs set up
and the KUID enviornment variable set to your alphanumeric KU ID (i.e. username on the cycle servers)  

USAGE: ./tester.sh /path/to/file.s /path/to/input.in

EOF

exit

fi

#${2+set} expands to set if $2 is set and nothing otherwise. we use this to check if
#a .in file was passed.
if [ -z ${2+set} ]; then
  echo 'Provide a .in file please!'
  exit
fi

echo "copying ${1}..." 1>&2
scp "$1" "${KUID}@cycle2.eecs.ku.edu:~/${1}" 1>&2
echo "copying ${2}..." 1>&2
scp "$2" "${KUID}@cycle2.eecs.ku.edu:~/${2}" 1>&2
ssh "${KUID}@cycle2.eecs.ku.edu" "./roll.sh ${1}; ./${1}.prog < ${2}"
