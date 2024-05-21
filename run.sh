#!/bin/bash

if [ -x "./main" ]; then
    echo "Running main..."
    main basic 
    main sarsa
    main double
else
    echo "Error: main does not exist or is not executable."
    exit 1
fi