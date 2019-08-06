#!/bin/bash
# Please install yaml first:
#     pip install pyyaml

cd $(dirname $0)
python ../third_party/duktape/tools/configure.py --output-directory ../src/js/duktape

