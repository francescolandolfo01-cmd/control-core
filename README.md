# control-core
![CI](https://github.com/francescolandolfo01-cmd/control-core/actions/workflows/ci.yml/badge.svg?branch=main)

Libreria C++17 di controllo (in sviluppo).

## Build

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
ctest --test-dir build --output-on-failure
```
