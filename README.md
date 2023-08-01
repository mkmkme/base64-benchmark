# base64-benchmark

This repository compares the performance between the following base64 implementations:

1. [aklomp/base64](https://github.com/aklomp/base64)
2. [Turbo-Base64](https://github.com/powturbo/Turbo-Base64)

To measure the performance, [Google Benchmark](https://github.com/google/benchmark) is used.

The data is taken from ClickBench `hits.tsv.gz` file, namely `SearchPhrase`, `URL` and `Title` columns.
Queries used:
```sql
select top 1 SearchPhrase from hits where lengthUTF8(SearchPhrase) = 50 format CSV;
select top 1 URL from hits where lengthUTF8(URL) = 50 format CSV;
select top 1 Title from hits where lengthUTF8(Title) = 50 format CSV;

select top 1 SearchPhrase from hits where lengthUTF8(SearchPhrase) = 100 format CSV;
select top 1 URL from hits where lengthUTF8(URL) = 100 format CSV;
select top 1 Title from hits where lengthUTF8(Title) = 100 format CSV;

select top 1 SearchPhrase from hits where lengthUTF8(SearchPhrase) = 200 format CSV;
select top 1 URL from hits where lengthUTF8(URL) = 200 format CSV;
select top 1 Title from hits where lengthUTF8(Title) = 200 format CSV;

select top 1 SearchPhrase from hits where lengthUTF8(SearchPhrase) = 500 format CSV;
select top 1 URL from hits where lengthUTF8(URL) = 500 format CSV;
select top 1 Title from hits where lengthUTF8(Title) = 500 format CSV;

select top 1 SearchPhrase from hits order by length(SearchPhrase) desc format CSV;
select top 1 URL from hits order by length(URL) desc format CSV;
select top 1 Title from hits order by length(Title) desc format CSV;
```

For encoded data, `base64Encode` was called to the resulted strings.

# Results

## Macbook Pro, M1 Max, 64GB RAM, macOS Ventura 13.4.1, LLVM Clang 16

### Encoding

#### ~50 symbols

```
Turbo-Base64 encode ~50 symbols/0                   25.1 ns         25.0 ns     28152134 bytes_per_second=3.53444G/s size=95
Turbo-Base64 encode ~50 symbols/1                   13.9 ns         13.9 ns     50373483 bytes_per_second=3.35817G/s size=50
Turbo-Base64 encode ~50 symbols/2                   21.0 ns         20.9 ns     33403162 bytes_per_second=3.51594G/s size=79
aklomp/base64 encode ~50 symbols/0                  16.9 ns         16.9 ns     41439489 bytes_per_second=5.22632G/s size=95
aklomp/base64 encode ~50 symbols/1                  8.19 ns         8.18 ns     85883248 bytes_per_second=5.69261G/s size=50
aklomp/base64 encode ~50 symbols/2                  15.2 ns         15.2 ns     46147964 bytes_per_second=4.85347G/s size=79
```

#### ~100 symbols

```
Turbo-Base64 encode ~100 symbols/3                  29.1 ns         29.1 ns     24013887 bytes_per_second=3.83841G/s size=120
Turbo-Base64 encode ~100 symbols/4                  23.9 ns         23.9 ns     29143352 bytes_per_second=3.89639G/s size=100
Turbo-Base64 encode ~100 symbols/5                  33.9 ns         33.9 ns     20619832 bytes_per_second=3.82186G/s size=139
aklomp/base64 encode ~100 symbols/3                 16.0 ns         16.0 ns     43696472 bytes_per_second=6.97524G/s size=120
aklomp/base64 encode ~100 symbols/4                 12.3 ns         12.3 ns     56974028 bytes_per_second=7.57583G/s size=100
aklomp/base64 encode ~100 symbols/5                 19.8 ns         19.8 ns     35334976 bytes_per_second=6.54421G/s size=139
```

#### ~200 symbols

```
Turbo-Base64 encode ~200 symbols/6                  84.6 ns         84.5 ns      8237135 bytes_per_second=3.98909G/s size=362
Turbo-Base64 encode ~200 symbols/7                  46.7 ns         46.6 ns     14992504 bytes_per_second=3.99288G/s size=200
Turbo-Base64 encode ~200 symbols/8                  55.3 ns         55.3 ns     12564393 bytes_per_second=3.95652G/s size=235
aklomp/base64 encode ~200 symbols/6                 27.7 ns         27.7 ns     25270211 bytes_per_second=12.1742G/s size=362
aklomp/base64 encode ~200 symbols/7                 17.3 ns         17.3 ns     40447932 bytes_per_second=10.7827G/s size=200
aklomp/base64 encode ~200 symbols/8                 25.0 ns         24.9 ns     28196359 bytes_per_second=8.77668G/s size=235
```

#### ~500 symbols

```
Turbo-Base64 encode ~500 symbols/9                   206 ns          206 ns      3386272 bytes_per_second=4.10356G/s size=907
Turbo-Base64 encode ~500 symbols/10                  117 ns          117 ns      6009770 bytes_per_second=4.07072G/s size=511
Turbo-Base64 encode ~500 symbols/11                  196 ns          196 ns      3567352 bytes_per_second=4.0712G/s size=856
aklomp/base64 encode ~500 symbols/9                 58.6 ns         58.5 ns     12024805 bytes_per_second=14.43G/s size=907
aklomp/base64 encode ~500 symbols/10                37.0 ns         37.0 ns     18963612 bytes_per_second=12.8774G/s size=511
aklomp/base64 encode ~500 symbols/11                54.5 ns         54.5 ns     12830853 bytes_per_second=14.6273G/s size=856
```

#### Longest strings in ClickBench

```
Turbo-Base64 encode longest in ClickBench/12         440 ns          440 ns      1590327 bytes_per_second=4.10882G/s size=1.94k
Turbo-Base64 encode longest in ClickBench/13        1754 ns         1752 ns       400364 bytes_per_second=4.12949G/s size=7.77k
Turbo-Base64 encode longest in ClickBench/14         449 ns          448 ns      1558791 bytes_per_second=4.12445G/s size=1.985k
aklomp/base64 encode longest in ClickBench/12        107 ns          107 ns      6537718 bytes_per_second=16.8643G/s size=1.94k
aklomp/base64 encode longest in ClickBench/13        415 ns          415 ns      1693320 bytes_per_second=17.4508G/s size=7.77k
aklomp/base64 encode longest in ClickBench/14        109 ns          109 ns      6424258 bytes_per_second=16.967G/s size=1.985k
```

### Decoding

#### ~50 symbols

```
Turbo-Base64 decode ~50 symbols/0                   27.1 ns         27.1 ns     25814636 bytes_per_second=4.40107G/s size=128
Turbo-Base64 decode ~50 symbols/1                   15.8 ns         15.8 ns     44117554 bytes_per_second=4.00012G/s size=68
Turbo-Base64 decode ~50 symbols/2                   23.7 ns         23.7 ns     29478897 bytes_per_second=4.24605G/s size=108
aklomp/base64 decode ~50 symbols/0                  26.9 ns         26.9 ns     26081546 bytes_per_second=4.43335G/s size=128
aklomp/base64 decode ~50 symbols/1                  11.0 ns         11.0 ns     64077332 bytes_per_second=5.77292G/s size=68
aklomp/base64 decode ~50 symbols/2                  19.2 ns         19.1 ns     36761810 bytes_per_second=5.25604G/s size=108
```

#### ~100 symbols

```
Turbo-Base64 decode ~100 symbols/3                  32.3 ns         32.3 ns     21735283 bytes_per_second=4.61905G/s size=160
Turbo-Base64 decode ~100 symbols/4                  27.3 ns         27.3 ns     25686471 bytes_per_second=4.64527G/s size=136
Turbo-Base64 decode ~100 symbols/5                  37.7 ns         37.7 ns     18520478 bytes_per_second=4.64625G/s size=188
aklomp/base64 decode ~100 symbols/3                 22.0 ns         22.0 ns     31813410 bytes_per_second=6.78648G/s size=160
aklomp/base64 decode ~100 symbols/4                 16.9 ns         16.9 ns     41613164 bytes_per_second=7.50538G/s size=136
aklomp/base64 decode ~100 symbols/5                 27.0 ns         26.9 ns     25983089 bytes_per_second=6.49867G/s size=188
```

#### ~200 symbols

```
Turbo-Base64 decode ~200 symbols/6                  91.2 ns         91.1 ns      7664681 bytes_per_second=4.94662G/s size=484
Turbo-Base64 decode ~200 symbols/7                  51.5 ns         51.5 ns     13643628 bytes_per_second=4.84879G/s size=268
Turbo-Base64 decode ~200 symbols/8                  60.9 ns         60.9 ns     11441274 bytes_per_second=4.83445G/s size=316
aklomp/base64 decode ~200 symbols/6                 46.1 ns         46.1 ns     15133990 bytes_per_second=9.78545G/s size=484
aklomp/base64 decode ~200 symbols/7                 27.5 ns         27.5 ns     25371603 bytes_per_second=9.07646G/s size=268
aklomp/base64 decode ~200 symbols/8                 36.8 ns         36.8 ns     19043111 bytes_per_second=8.00135G/s size=316
```

#### ~500 symbols

```
Turbo-Base64 decode ~500 symbols/9                   221 ns          221 ns      3163113 bytes_per_second=5.11223G/s size=1.212k
Turbo-Base64 decode ~500 symbols/10                  126 ns          126 ns      5566910 bytes_per_second=5.06455G/s size=684
Turbo-Base64 decode ~500 symbols/11                  209 ns          209 ns      3345952 bytes_per_second=5.09972G/s size=1.144k
aklomp/base64 decode ~500 symbols/9                  104 ns          104 ns      6769171 bytes_per_second=10.8783G/s size=1.212k
aklomp/base64 decode ~500 symbols/10                61.9 ns         61.8 ns     11316057 bytes_per_second=10.3015G/s size=684
aklomp/base64 decode ~500 symbols/11                97.9 ns         97.9 ns      7113677 bytes_per_second=10.8874G/s size=1.144k
```

#### Longest strings in ClickBench

```
Turbo-Base64 decode longest in ClickBench/12         478 ns          478 ns      1464389 bytes_per_second=5.04157G/s size=2.588k
Turbo-Base64 decode longest in ClickBench/13        1880 ns         1878 ns       368850 bytes_per_second=5.13792G/s size=10.36k
Turbo-Base64 decode longest in ClickBench/14         476 ns          476 ns      1471318 bytes_per_second=5.18594G/s size=2.648k
aklomp/base64 decode longest in ClickBench/12        211 ns          211 ns      3306394 bytes_per_second=11.4038G/s size=2.588k
aklomp/base64 decode longest in ClickBench/13        805 ns          804 ns       867873 bytes_per_second=11.9938G/s size=10.36k
aklomp/base64 decode longest in ClickBench/14        207 ns          207 ns      3387698 bytes_per_second=11.9286G/s size=2.648k
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
