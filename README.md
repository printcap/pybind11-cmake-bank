# C++, CMake, PyBind11, and setuptools

`gcd` and mini Bank application in C++ compiled to shared library and `Catch2` tests.
Python binding of the same using PyBind11 and `pytest` tests.
The C++ is statically linked into the native Python library.
The C++ API uses STL features from C++17 (`std::optional`, `std::variant`)

## Build and Test Shared library using CMake

```bash
mkdir build
cd build
cmake -GNinja ..
ctest
```

Also runs unit tests.

## Build and Test Python Binding

```bash
cd python
python setup.py develop
pytest
```

