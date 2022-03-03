#
#   Date: March 2nd 2022
#   Author: Arash Roshanineshat
#

import subprocess

from setuptools import setup, Extension
import os

def pkgconfig(package, kw):
    flag_map = {'-I': 'include_dirs', '-L': 'library_dirs', '-l': 'libraries'}
    output = subprocess.getoutput(
        'pkg-config --cflags --libs {}'.format(package))
    for token in output.strip().split():
        kw.setdefault(flag_map.get(token[:2]), []).append(token[2:])
    return kw

sigfit_sources = ['src/SineFit.c', 'src/SigFit.c']
sigfit_includes = ['{}/include'.format(os.getcwd())]

keywords = {
    'sources' : sigfit_sources,
    'include_dirs': sigfit_includes,
    'libraries': ['m']
}

keywords = pkgconfig('python3', keywords)

main_module = Extension('SigFit', **keywords)

setup (name = 'SigFit',
       license= "MIT License",
       version = '1.1.0',
       author = 'Arash Roshanineshat',
       author_email = 'aroshanineshat@email.arizona.edu',
       url = 'https://github.com/aroshanineshat/SigFit',
       description = 'SigFit is a package to help you fit a sine wave to noisy sine data',
       ext_modules = [main_module])
