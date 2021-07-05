#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os, sys

SCRIPT_PATH = os.path.abspath(os.path.dirname(__file__))
REPO_ROOT = os.path.normpath(os.path.join(SCRIPT_PATH, os.pardir))

def get_full_path(repo_path):
    return os.path.normpath(os.path.join(REPO_ROOT, repo_path))
def run(cmd_line):
    print(cmd_line)
    return os.system(cmd_line)
def run_script(rel_path, args = ''):
    cmd_line = os.path.normpath(rel_path)
    if args:
        cmd_line += ' '
        cmd_line += args
    return run(cmd_line)

os.chdir(SCRIPT_PATH)
run_script('blink/make_element_factory.py',                                     \
    os.path.normpath('blink/in/HTMLAttributeNames.in') + ' ' +                  \
    os.path.normpath('blink/in/HTMLTagNames.in') + ' ' +                        \
    '--output_dir ' + os.path.normpath('../src/blinkit/blink/renderer/core')    \
)

run_script('blink/make_names.py',                                               \
    os.path.normpath('blink/in/EventInterfaces.in') + ' ' +                     \
    '--output_dir ' + os.path.normpath('../src/blinkit/blink/renderer/core')    \
)

run_script('blink/make_names.py',                                               \
    os.path.normpath('blink/in/EventTargetFactory.in') + ' ' +                  \
    '--output_dir ' + os.path.normpath('../src/blinkit/blink/renderer/core')    \
)

run_script('blink/make_names.py',                                               \
    os.path.normpath('blink/in/EventTypeNames.in') + ' ' +                      \
    '--output_dir ' + os.path.normpath('../src/blinkit/blink/renderer/core')    \
)
