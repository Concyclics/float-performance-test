# float-performance-test

test your CPU's float performance with **C++** and **openmp**.

## functions

This program can not only calculate your CPU's FP32, FP64 performance, but automatically compute the speedup rate, parallel efficiency and parallelism rate.

## complie command

* for macOS

```shell
g++ -Ofast -Xpreprocessor -fopenmp -I/opt/local/include -L/opt/local/lib -lomp FPtest.cpp -o FPtest
```

* for windows/Linux

```shell
g++ -Ofast -fopenmp FPtest.cpp -o FPtest
```

* if support you may add **AVX2** or **AVX512** intrinsics

```shell
#for AVX2
g++ -Ofast -fopenmp -mavx2 FPtest.cpp -o FPtest
```
## run command

```shell
./FPtest [test size] [threads] [test times]
```

for example
```shell
./FPtest 100000000 4 10
```

## Copyrights

Copyright (c) SCUT. 2022. All rights reserved.

This program is a benchmark for floating point performance. It is designed to be run on both single and double precision systems. In multi-core systems, it is designed to be run on omp.
