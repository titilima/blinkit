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
run_script('blink/create-html-entity-table.py',                                                             \
    '-o ' + os.path.normpath('../src/blinkit/blink/renderer/core/html/parser/HTMLEntityTable.cpp') + ' ' +  \
    os.path.normpath('blink/in/HTMLEntityNames.in')                                                         \
)

run_script('blink/make_element_factory.py',                     \
    os.path.normpath('blink/in/HTMLTagNames.in') + ' ' +        \
    os.path.normpath('blink/in/HTMLAttributeNames.in') + ' ' +  \
    '--output_dir ' + os.path.normpath('../src/blinkit/ui')     \
)

run_script('blink/make_element_lookup_trie.py',             \
    os.path.normpath('blink/in/HTMLTagNames.in') + ' ' +    \
    '--output_dir ' + os.path.normpath('../src/blinkit/ui') \
)

run_script('blink/make_element_type_helpers.py',            \
    os.path.normpath('blink/in/HTMLTagNames.in') + ' ' +    \
    '--output_dir ' + os.path.normpath('../src/blinkit/ui') \
)

run_script('blink/make_names.py',                           \
    os.path.normpath('blink/in/EventInterfaces.in') + ' ' + \
    '--output_dir ' + os.path.normpath('../src/blinkit/ui') \
)

run_script('blink/make_names.py',                               \
    os.path.normpath('blink/in/EventTargetFactory.in') + ' ' +  \
    '--output_dir ' + os.path.normpath('../src/blinkit/ui')     \
)

run_script('blink/make_names.py',                           \
    os.path.normpath('blink/in/EventTypeNames.in') + ' ' +  \
    '--output_dir ' + os.path.normpath('../src/blinkit/ui') \
)

run_script('blink/make_names.py',                                               \
    os.path.normpath('blink/in/HTMLTokenizerNames.in') + ' ' +                  \
    '--output_dir ' + os.path.normpath('../src/blinkit/blink/renderer/core')    \
)

run_script('blink/make_names.py',                                               \
    os.path.normpath('blink/in/InputTypeNames.in') + ' ' +                      \
    '--output_dir ' + os.path.normpath('../src/blinkit/blink/renderer/core')    \
)

run_script('blink/make_css_property_metadata.py',                                   \
    os.path.normpath('blink/in/CSSProperties.in') + ' ' +                           \
    '--output_dir ' + os.path.normpath('../src/blinkit/blink/renderer/core/css')    \
)

run_script('blink/make_css_property_names.py',                                  \
    os.path.normpath('blink/in/CSSProperties.in') + ' ' +                       \
    '--output_dir ' + os.path.normpath('../src/blinkit/blink/renderer/core')    \
)

run_script('blink/make_css_tokenizer_codepoints.py',                            \
    '--output_dir ' + os.path.normpath('../src/blinkit/blink/renderer/core')    \
)

run_script('blink/make_css_value_keywords.py',                                  \
    os.path.normpath('blink/in/CSSValueKeywords.in') + ' ' +                    \
    os.path.normpath('blink/in/SVGCSSValueKeywords.in') + ' ' +                 \
    '--output_dir ' + os.path.normpath('../src/blinkit/blink/renderer/core')    \
)

run_script('blink/make_style_builder.py',                                       \
    os.path.normpath('blink/in/CSSProperties.in') + ' ' +                       \
    '--output_dir ' + os.path.normpath('../src/blinkit/blink/renderer/core')    \
)

run_script('blink/make_style_shorthands.py',                                    \
    os.path.normpath('blink/in/CSSProperties.in') + ' ' +                       \
    '--output_dir ' + os.path.normpath('../src/blinkit/blink/renderer/core')    \
)

run_script('blink/make_token_matcher.py',                                           \
    os.path.normpath('blink/in/HTMLMetaElement-in.cpp') + ' ' +                     \
    os.path.normpath('../src/blinkit/blink/renderer/core/html/HTMLMetaElement.cpp') \
)

run_script('gperf',                                                                             \
    '--key-positions="*" -D -s 2 ' + os.path.normpath('blink/in/ColorData.gperf') + ' ' +       \
    '--output-file=' + os.path.normpath('../src/blinkit/blink/renderer/platform/ColorData.cpp') \
)

run_script('blink/make_names.py',                                                   \
    os.path.normpath('blink/in/FontFamilyNames.in') + ' ' +                         \
    '--output_dir ' + os.path.normpath('../src/blinkit/blink/renderer/platform')    \
)

run_script('blink/make_names.py',                           \
    os.path.normpath('blink/in/HTTPNames.in') + ' ' +       \
    '--output_dir ' + os.path.normpath('../src/blinkit/ui') \
)
