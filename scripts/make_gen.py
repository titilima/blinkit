#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os, sys

REPO_ROOT = os.path.normpath(os.path.join(os.path.abspath(os.path.dirname(__file__)), os.pardir))
def get_full_path(repo_path):
    return os.path.normpath(os.path.join(REPO_ROOT, repo_path))
def run(cmd_line):
    print cmd_line
    return os.system(cmd_line)
def run_script(rel_path, args = ''):
    cmd_line = os.path.normpath(rel_path)
    if args:
        cmd_line += ' '
        cmd_line += args
    return run(cmd_line)

GPERF = 'gperf'
GPERF_ARG = ''
if 'win32' == sys.platform:
    GPERF = get_full_path('helpers\\gperf.exe')
    GPERF_ARG = ' --gperf ' + GPERF

# Apply `blink/renderer` as work directory
current_dir = get_full_path('src/chromium/third_party/blink/renderer')
os.chdir(current_dir)

run_script('build/scripts/gperf.py',    \
    GPERF + ' --key-positions=* -D -s 2 ' + os.path.normpath('platform/color_data.gperf') + ' ' +   \
    '--output-file=' + os.path.normpath('platform/color_data.cc')   \
)

run_script('build/scripts/make_computed_style_base.py',  \
    os.path.normpath('core/css/css_properties.json5') + ' ' +   \
    os.path.normpath('core/css/computed_style_field_aliases.json5') + ' ' + \
    os.path.normpath('core/css/computed_style_extra_fields.json5') + ' ' +  \
    os.path.normpath('core/css/computed_style_diff_functions.json5') + ' ' +    \
    os.path.normpath('core/css/css_value_keywords.json5') + ' ' +   \
    os.path.normpath('core/css/css_properties_ranking.json5') + ' ' +   \
    os.path.normpath('core/css/css_group_config.json5') + ' ' + \
    '--output_dir ' + os.path.normpath('core') \
)

run_script('build/scripts/make_css_property_names.py',  \
    os.path.normpath('core/css/css_properties.json5') + ' ' +   \
    os.path.normpath('core/css/computed_style_field_aliases.json5') + ' ' + \
    '--output_dir ' + os.path.normpath('core') + GPERF_ARG  \
)

run_script('build/scripts/make_css_value_id_mappings.py',   \
    os.path.normpath('core/css/css_properties.json5') + ' ' +   \
    os.path.normpath('core/css/computed_style_field_aliases.json5') + ' ' + \
    os.path.normpath('core/css/css_value_keywords.json5') + ' ' +   \
    '--output_dir ' + os.path.normpath('core')  \
)

run_script('build/scripts/make_names.py',   \
    os.path.normpath('platform/fonts/font_family_names.json5') + ' ' +  \
    '--output_dir ' + os.path.normpath('platform')  \
)

run_script('build/scripts/core/css/make_css_value_keywords.py',  \
    os.path.normpath('core/css/css_value_keywords.json5') + ' ' +   \
    '--output_dir ' + os.path.normpath('core') + GPERF_ARG  \
)

run_script('build/scripts/core/css/make_cssom_types.py',    \
    os.path.normpath('core/css/css_properties.json5') + ' ' +   \
    os.path.normpath('core/css/computed_style_field_aliases.json5') + ' ' + \
    '--output_dir ' + os.path.normpath('core')  \
)

run_script('build/scripts/core/css/make_media_feature_names.py',   \
    os.path.normpath('core/css/media_feature_names.json5') + ' ' +   \
    '--output_dir ' + os.path.normpath('core/css')  \
)

run_script('build/scripts/core/css/make_style_shorthands.py',   \
    os.path.normpath('core/css/css_properties.json5') + ' ' +   \
    os.path.normpath('core/css/computed_style_field_aliases.json5') + ' ' + \
    '--output_dir ' + os.path.normpath('core')  \
)

run_script('build/scripts/core/css/parser/make_atrule_names.py',    \
    os.path.normpath('core/css/parser/at_rule_names.json5') + ' ' + \
    '--output_dir ' + os.path.normpath('core/css/parser') + GPERF_ARG   \
)

run_script('build/scripts/core/css/properties/make_css_property_base.py',   \
    os.path.normpath('core/css/css_properties.json5') + ' ' +   \
    os.path.normpath('core/css/computed_style_field_aliases.json5') + ' ' + \
    '--output_dir ' + os.path.normpath('core/css/properties')   \
)

run_script('build/scripts/core/css/properties/make_css_property_subclasses.py',   \
    os.path.normpath('core/css/css_properties.json5') + ' ' +   \
    os.path.normpath('core/css/computed_style_field_aliases.json5') + ' ' + \
    os.path.normpath('core/css/properties/css_property_methods.json5') + ' ' +  \
    '--output_dir ' + os.path.normpath('core/css/properties/longhands')   \
)

run_script('build/scripts/core/css/properties/make_css_property_subclasses.py',   \
    os.path.normpath('core/css/css_properties.json5') + ' ' +   \
    os.path.normpath('core/css/computed_style_field_aliases.json5') + ' ' + \
    os.path.normpath('core/css/properties/css_property_methods.json5') + ' ' +  \
    '--output_dir ' + os.path.normpath('core/css/properties/shorthands')   \
)

run_script('build/scripts/core/style/make_computed_style_initial_values.py',    \
    os.path.normpath('core/css/css_properties.json5') + ' ' +   \
    os.path.normpath('core/css/computed_style_field_aliases.json5') + ' ' + \
    os.path.normpath('core/css/computed_style_extra_fields.json5') + ' ' +  \
    '--output_dir ' + os.path.normpath('core/style') \
)
