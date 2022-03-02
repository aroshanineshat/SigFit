from distutils.core import setup, Extension

main_module = Extension('SigFit',
                    sources = ['src/SineFit.c', 'src/SigFit.c'],
                    libraries = ["m"],
                    include_dirs=["./include/"]
)

setup (name = 'SigFit',
       version = '1.0',
       description = 'SigFit is the package that helps you fit a sine wave to your data',
       ext_modules = [main_module])
