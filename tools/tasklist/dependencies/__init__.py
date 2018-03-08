#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# 可以把所有依赖的模块放到这个目录，对于独立没有子模块的模块可
# 以先用zip压缩后改名放到这个目录
#

import os
import sys

dir_path = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, dir_path)
def import_module(name):
    mod_path = os.path.join(dir_path, name)
    if os.path.isfile(mod_path):
        sys.path.insert(0, mod_path)
        module = __import__(name)
    elif os.path.isdir(mod_path):
        module = __import__(name)
    else:
        sys.stderr.write('can not find module: %s' % name)
        sys.exit(1)
    return module

openpyxl = import_module('openpyxl')
yaml = import_module('yaml')
pymysql = import_module('pymysql')
Crypto = import_module('Crypto')
