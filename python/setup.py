from glob import glob
from setuptools import setup
from pybind11.setup_helpers import Pybind11Extension

__version__ = "0.0.1"

ext_modules = [
  Pybind11Extension(
    'example',
    ['src/python_wrapper.cpp', '../src/bank.cpp', '../src/example.cpp'],
    include_dirs=[ '../include' ],
    cxx_std=17
  ),
]

setup(
  name='example',
  version=__version__,
  author='Rene Mueller',
  description='simple PyBind11 binding example for existing shared library',
  ext_modules=ext_modules
)
