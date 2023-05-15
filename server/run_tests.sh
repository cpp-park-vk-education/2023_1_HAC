#!/usr/bin/env bash

set -e

cd build

echo "Running tests"
ctest -V -R Net
ctest -V -R Handler
ctest -V -R DB
