#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os

resource_file_path = os.path.join(os.getcwd(), 'src', 'resource.rc')

with open(resource_file_path) as file_handle:
    lines = file_handle.readlines()

icon_line = 'MAINICON ICON "../build/icon/icon.ico"\n'

if icon_line in lines:
    lines.remove(icon_line)

print lines

with open(resource_file_path, 'w') as file_handle:
    file_handle.writelines(lines)
