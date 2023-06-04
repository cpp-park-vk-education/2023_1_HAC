#!/bin/bash

#TAG="$(git branch --show-current)_$(git rev-parse --short HEAD)"
if [[ ! $TAG ]]; then
    echo "Using TAG=latest"
    TAG=latest
fi

docker build -t mm_server -f ./ci/dockers/Dockerfile.server .
