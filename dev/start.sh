#!/bin/bash
docker run --rm -it --cap-drop=ALL --cap-add=SYS_PTRACE -v ~/Documents/test/c/forex:/home/shammas/workdir --name forex cenv
