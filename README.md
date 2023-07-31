# base64-benchmark

This repository compares the performance between the following base64 implementations:

1. [aklomp/base64](https://github.com/aklomp/base64)
2. [Turbo-Base64](https://github.com/powturbo/Turbo-Base64)

To measure the performance, [Google Benchmark](https://github.com/google/benchmark) is used.

## Results

Macbook Pro, M1 Max, 64GB RAM, macOS Ventura 13.4.1, LLVM Clang 16.

```
-----------------------------------------------------------------------------------------
Benchmark                               Time             CPU   Iterations UserCounters...
-----------------------------------------------------------------------------------------
BM_TurboBase64Encode/10000 2283 ns 2282 ns 306255 bytes_per_second=4.08163G/s
BM_TurboBase64Encode/100000 24162 ns 24072 ns 29349 bytes_per_second=3.86885G/s
BM_TurboBase64Encode/1000000 241932 ns 241630 ns 2897 bytes_per_second=3.85433G/s
BM_TurboBase64Encode/10000000 2451854 ns 2448586 ns 285 bytes_per_second=3.80351G/s
BM_TurboBase64Decode/10000 1828 ns 1823 ns 385051 bytes_per_second=5.10818G/s
BM_TurboBase64Decode/100000 18467 ns 18457 ns 37894 bytes_per_second=5.04592G/s
BM_TurboBase64Decode/1000000 185757 ns 185556 ns 3732 bytes_per_second=5.0191G/s
BM_TurboBase64Decode/10000000 1869540 ns 1868819 ns 375 bytes_per_second=4.98348G/s
BM_AklompBase64Encode/10000 523 ns 523 ns 1341048 bytes_per_second=17.8132G/s
BM_AklompBase64Encode/100000 5213 ns 5211 ns 134525 bytes_per_second=17.8739G/s
BM_AklompBase64Encode/1000000 52792 ns 52392 ns 13580 bytes_per_second=17.776G/s
BM_AklompBase64Encode/10000000 525944 ns 525761 ns 1325 bytes_per_second=17.7138G/s
BM_AklompBase64Decode/10000 778 ns 777 ns 899408 bytes_per_second=11.98G/s
BM_AklompBase64Decode/100000 7733 ns 7721 ns 91302 bytes_per_second=12.0616G/s
BM_AklompBase64Decode/1000000 78127 ns 77375 ns 9130 bytes_per_second=12.0364G/s
BM_AklompBase64Decode/10000000 783237 ns 782974 ns 882 bytes_per_second=11.8947G/s

```

Dell Latitude 7490, Intel Core i7-8650U, 32GB RAM, Fedora 38 Silverblue,
LLVM Clang 16.

```
-----------------------------------------------------------------------------------------
Benchmark                               Time             CPU   Iterations UserCounters...
-----------------------------------------------------------------------------------------
BM_TurboBase64Encode/10000            650 ns          649 ns      1073304 bytes_per_second=14.3515G/s
BM_TurboBase64Encode/100000          6302 ns         6289 ns       104709 bytes_per_second=14.8093G/s
BM_TurboBase64Encode/1000000        63413 ns        63258 ns        10636 bytes_per_second=14.7226G/s
BM_TurboBase64Encode/10000000     1040585 ns      1034173 ns          613 bytes_per_second=9.00548G/s
BM_TurboBase64Decode/10000            376 ns          375 ns      1958913 bytes_per_second=24.8325G/s
BM_TurboBase64Decode/100000          3638 ns         3631 ns       191675 bytes_per_second=25.6519G/s
BM_TurboBase64Decode/1000000        36498 ns        36406 ns        19079 bytes_per_second=25.5813G/s
BM_TurboBase64Decode/10000000      665770 ns       657016 ns          876 bytes_per_second=14.175G/s
BM_AklompBase64Encode/10000           626 ns          625 ns      1090713 bytes_per_second=14.9065G/s
BM_AklompBase64Encode/100000         6572 ns         6543 ns       105421 bytes_per_second=14.2343G/s
BM_AklompBase64Encode/1000000       67898 ns        67138 ns         9925 bytes_per_second=13.8717G/s
BM_AklompBase64Encode/10000000    1217412 ns      1210194 ns          419 bytes_per_second=7.69565G/s
BM_AklompBase64Decode/10000           582 ns          580 ns      1200696 bytes_per_second=16.053G/s
BM_AklompBase64Decode/100000         5929 ns         5915 ns       116999 bytes_per_second=15.745G/s
BM_AklompBase64Decode/1000000       60985 ns        60826 ns        11192 bytes_per_second=15.3113G/s
BM_AklompBase64Decode/10000000     889418 ns       884539 ns          715 bytes_per_second=10.5289G/s

```
