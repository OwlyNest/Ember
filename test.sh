#!/bin/bash

set -e

echo "Building Ember..."
make

echo "Running example tests..."

for dir in examples/*; do
    if [[ -d "$dir" ]]; then
        echo "Testing $dir"
        (cd "$dir" && ../../ember clean && ../../ember && ./$(basename "$dir") && ../../ember clean)
    fi
done

echo "All tests passed!"