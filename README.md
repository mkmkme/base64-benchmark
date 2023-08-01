# base64-benchmark

This repository compares the performance between the following base64 implementations:

1. [aklomp/base64](https://github.com/aklomp/base64)
2. [Turbo-Base64](https://github.com/powturbo/Turbo-Base64)

To measure the performance, [Google Benchmark](https://github.com/google/benchmark) is used.

## Results

Macbook Pro, M1 Max, 64GB RAM, macOS Ventura 13.4.1, LLVM Clang 16.

Encoding:

```
-----------------------------------------------------------------------------------
Benchmark                         Time             CPU   Iterations UserCounters...
-----------------------------------------------------------------------------------
BM_TurboBase64Encode/0         25.2 ns         25.2 ns     27747388 bytes_per_second=3.51387G/s size=95
BM_TurboBase64Encode/1         14.2 ns         14.1 ns     50062936 bytes_per_second=3.30759G/s size=50
BM_TurboBase64Encode/2         21.1 ns         21.1 ns     33426450 bytes_per_second=3.49336G/s size=79
BM_TurboBase64Encode/3         29.7 ns         29.6 ns     23769181 bytes_per_second=3.77086G/s size=120
BM_TurboBase64Encode/4         24.3 ns         24.3 ns     28608912 bytes_per_second=3.83553G/s size=100
BM_TurboBase64Encode/5         34.4 ns         34.4 ns     20411377 bytes_per_second=3.76657G/s size=139
BM_TurboBase64Encode/6         85.9 ns         85.8 ns      8198353 bytes_per_second=3.93016G/s size=362
BM_TurboBase64Encode/7         47.3 ns         47.3 ns     14758872 bytes_per_second=3.94145G/s size=200
BM_TurboBase64Encode/8         56.3 ns         56.2 ns     12385435 bytes_per_second=3.89202G/s size=235
BM_TurboBase64Encode/9          209 ns          209 ns      3342422 bytes_per_second=4.03915G/s size=907
BM_TurboBase64Encode/10         118 ns          118 ns      5927381 bytes_per_second=4.02124G/s size=511
BM_TurboBase64Encode/11         200 ns          199 ns      3536461 bytes_per_second=4.00005G/s size=856
BM_TurboBase64Encode/12         444 ns          444 ns      1580217 bytes_per_second=4.0716G/s size=1.94k
BM_TurboBase64Encode/13        1774 ns         1773 ns       395061 bytes_per_second=4.08122G/s size=7.77k
BM_TurboBase64Encode/14         456 ns          455 ns      1535401 bytes_per_second=4.0586G/s size=1.985k
BM_AklompBase64Encode/0        17.2 ns         17.1 ns     40876390 bytes_per_second=5.15915G/s size=95
BM_AklompBase64Encode/1        8.26 ns         8.25 ns     85105348 bytes_per_second=5.64187G/s size=50
BM_AklompBase64Encode/2        15.3 ns         15.3 ns     45419449 bytes_per_second=4.82274G/s size=79
BM_AklompBase64Encode/3        16.1 ns         16.1 ns     43385830 bytes_per_second=6.92703G/s size=120
BM_AklompBase64Encode/4        12.3 ns         12.3 ns     56895305 bytes_per_second=7.60002G/s size=100
BM_AklompBase64Encode/5        20.1 ns         20.0 ns     35157506 bytes_per_second=6.45727G/s size=139
BM_AklompBase64Encode/6        28.0 ns         28.0 ns     24982423 bytes_per_second=12.0536G/s size=362
BM_AklompBase64Encode/7        17.8 ns         17.5 ns     40295887 bytes_per_second=10.6244G/s size=200
BM_AklompBase64Encode/8        25.1 ns         25.1 ns     27899895 bytes_per_second=8.72453G/s size=235
BM_AklompBase64Encode/9        59.1 ns         59.0 ns     11758185 bytes_per_second=14.3091G/s size=907
BM_AklompBase64Encode/10       37.4 ns         37.4 ns     18674037 bytes_per_second=12.7407G/s size=511
BM_AklompBase64Encode/11       55.1 ns         55.1 ns     12692196 bytes_per_second=14.4769G/s size=856
BM_AklompBase64Encode/12        109 ns          108 ns      6482802 bytes_per_second=16.7089G/s size=1.94k
BM_AklompBase64Encode/13        418 ns          417 ns      1680975 bytes_per_second=17.3402G/s size=7.77k
BM_AklompBase64Encode/14        110 ns          110 ns      6329228 bytes_per_second=16.8091G/s size=1.985k
```

Decoding:
```
-----------------------------------------------------------------------------------
Benchmark                         Time             CPU   Iterations UserCounters...
-----------------------------------------------------------------------------------
BM_TurboBase64Decode/0         27.3 ns         27.2 ns     25696183 bytes_per_second=4.37673G/s size=128
BM_TurboBase64Decode/1         16.0 ns         16.0 ns     43657501 bytes_per_second=3.94982G/s size=68
BM_TurboBase64Decode/2         24.4 ns         24.2 ns     29164965 bytes_per_second=4.15012G/s size=108
BM_TurboBase64Decode/3         32.5 ns         32.4 ns     21601338 bytes_per_second=4.59355G/s size=160
BM_TurboBase64Decode/4         27.7 ns         27.7 ns     25331756 bytes_per_second=4.58061G/s size=136
BM_TurboBase64Decode/5         38.6 ns         38.6 ns     18236385 bytes_per_second=4.53856G/s size=188
BM_TurboBase64Decode/6         92.2 ns         92.2 ns      7618053 bytes_per_second=4.89052G/s size=484
BM_TurboBase64Decode/7         52.4 ns         52.1 ns     13284497 bytes_per_second=4.78792G/s size=268
BM_TurboBase64Decode/8         61.6 ns         61.5 ns     11201613 bytes_per_second=4.78315G/s size=316
BM_TurboBase64Decode/9          224 ns          224 ns      3137297 bytes_per_second=5.04861G/s size=1.212k
BM_TurboBase64Decode/10         128 ns          127 ns      5499686 bytes_per_second=4.99914G/s size=684
BM_TurboBase64Decode/11         212 ns          212 ns      3305286 bytes_per_second=5.03056G/s size=1.144k
BM_TurboBase64Decode/12         485 ns          485 ns      1447651 bytes_per_second=4.97271G/s size=2.588k
BM_TurboBase64Decode/13        1894 ns         1893 ns       371054 bytes_per_second=5.0969G/s size=10.36k
BM_TurboBase64Decode/14         482 ns          482 ns      1453709 bytes_per_second=5.11869G/s size=2.648k
BM_AklompBase64Decode/0        27.2 ns         27.1 ns     25837123 bytes_per_second=4.39823G/s size=128
BM_AklompBase64Decode/1        11.1 ns         11.1 ns     63287706 bytes_per_second=5.71758G/s size=68
BM_AklompBase64Decode/2        19.3 ns         19.3 ns     36370060 bytes_per_second=5.21357G/s size=108
BM_AklompBase64Decode/3        22.3 ns         22.3 ns     31543183 bytes_per_second=6.69236G/s size=160
BM_AklompBase64Decode/4        17.1 ns         17.1 ns     41042720 bytes_per_second=7.40691G/s size=136
BM_AklompBase64Decode/5        27.3 ns         27.2 ns     25657285 bytes_per_second=6.42848G/s size=188
BM_AklompBase64Decode/6        47.1 ns         47.0 ns     14978196 bytes_per_second=9.59017G/s size=484
BM_AklompBase64Decode/7        27.8 ns         27.8 ns     25233409 bytes_per_second=8.97312G/s size=268
BM_AklompBase64Decode/8        37.5 ns         37.4 ns     18784483 bytes_per_second=7.87169G/s size=316
BM_AklompBase64Decode/9         105 ns          105 ns      6680090 bytes_per_second=10.7375G/s size=1.212k
BM_AklompBase64Decode/10       62.8 ns         62.7 ns     11174968 bytes_per_second=10.1519G/s size=684
BM_AklompBase64Decode/11       99.6 ns         99.5 ns      7037510 bytes_per_second=10.7045G/s size=1.144k
BM_AklompBase64Decode/12        214 ns          214 ns      3276678 bytes_per_second=11.2691G/s size=2.588k
BM_AklompBase64Decode/13        828 ns          820 ns       860469 bytes_per_second=11.7596G/s size=10.36k
BM_AklompBase64Decode/14        209 ns          209 ns      3342932 bytes_per_second=11.788G/s size=2.648k
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
