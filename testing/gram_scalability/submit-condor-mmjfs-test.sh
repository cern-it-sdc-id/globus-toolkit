#!/bin/sh

host=$1
port=$2

if [ -z $host ]; then
    echo "ERROR: No host specified"
    exit
fi
if [ -z $port ]; then
    port=8080
fi

factory="$host:$port/ogsa/services/base/gram/MasterCondorIntelLinuxManagedJobFactoryService"

./submit-test.sh $factory ./date.xml -o
