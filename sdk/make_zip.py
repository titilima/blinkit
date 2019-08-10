#!/usr/bin/env python
# -*- coding: utf-8 -*-

files = (
    'BlinKit.Credits.html',
    'include/BlinKit.h',
    'win32/BkCrawler.dll',
    'win32/BkCrawler.lib',
    'win32/BkCrawler.pdb',
    'win32/BkRequest.dll',
    'win32/BkRequest.lib',
    'win32/BkRequest.pdb',
#    'win32/BlinKit.dll',
#    'win32/BlinKit.lib',
#    'win32/BlinKit.pdb',
    'win32/BlinKit.props'
)

import os

SDK_ROOT = os.path.normpath(os.path.abspath(os.path.dirname(__file__)))
def get_full_path(rel_path):
    return os.path.normpath(os.path.join(SDK_ROOT, rel_path))

import re

file_name = 'blinkit_sdk'
with open(get_full_path('../projects/win/BlinKit/BlinKit.rc'), 'r') as f:
    r = re.findall(r'PRODUCTVERSION (\d+),(\d+),(\d+),(\d+)', f.read())
    r = r[0]
    file_name += '_' + r[0] + '.' + r[1] + '.' + r[2] + '.' + r[3]
file_name += '.zip'

import zipfile

print 'Packing `' + file_name + '` ...'
zf = zipfile.ZipFile(file_name, 'w', zipfile.ZIP_DEFLATED)
for file_entry in files:
    zf.write(get_full_path(file_entry), file_entry)
    print 'File `' + file_entry + '` packed.'
zf.close()
print 'Done.'
