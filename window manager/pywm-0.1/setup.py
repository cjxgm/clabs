#@+leo-ver=4
#@+node:@file setup.py
from distutils.core import setup
from distutils.extension import Extension
from Pyrex.Distutils import build_ext

import sys

pyIncludes = '/usr/include/python2.2'
libdirs = ['/usr/X11R6/lib']
defines = []
#libs = ['X11', 'Xext', 'fltk', 'm']
libs = ['X11', 'Xext', 'm']

# edit this to point to your FLTK **VERSON 1.0** static library
#extra_link_args = ['fltk-1.0.11/lib/libfltk.a']
extra_link_args = ['/usr/lib/libfltk.a']

# edit this to point to your FLTK **VERSION 1.0** headers
fltkInclude = '../fltk-1.0.11'

flwmSources=['src-pyrex/flwm_.pyx',
             'src-c/main.cpp',
             'src-c/Menu.cpp',
             'src-c/Hotkeys.cpp',
             'src-c/Rotated.cpp',
             'src-c/Desktop.cpp',
             'src-c/Frame.cpp',
             'src-c/FrameWindow.cpp',
             'src-c/flwmapi.cpp',
             ]

ext_flwm = [Extension('pywm.flwm_',
                      flwmSources,
                      include_dirs=[fltkInclude,
                                    pyIncludes,
                                    "/usr/include",
                                    "../src-c",
                                    "../src-pyrex"
                                    ],
                      define_macros=defines,
                      libraries=libs,
                      library_dirs=libdirs,
                      extra_link_args=extra_link_args
                      )]
                      
setup(
  name = "pywm",
  packages = ["pywm", "pywm.examples"],
  package_dir = {'pywm':'src-python',
                 'pywm.examples':'examples'},
  version = '0.1',
  ext_modules=ext_flwm,
  cmdclass = {'build_ext': build_ext}
)
#@-node:@file setup.py
#@-leo
