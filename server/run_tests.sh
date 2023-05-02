#!/usr/bin/env bash

set -e

cd build

echo "Running tests"
ctest -V -R Handler Net DB