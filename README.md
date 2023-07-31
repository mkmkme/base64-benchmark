# base64-benchmark

This repository compares the performance between the following base64 implementations:

1. [aklomp/base64](https://github.com/aklomp/base64)
2. [Turbo-Base64](https://github.com/powturbo/Turbo-Base64)

To measure the performance, [Google Benchmark](https://github.com/google/benchmark) is used.

## Results

Macbook Pro, M1 Max, 64GB RAM, macOS Ventura 13.4.1

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