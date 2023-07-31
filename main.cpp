#include <iostream>

#include <benchmark/benchmark.h>
#include <libbase64.h>
#include <turbob64.h>

inline void initializeAklompBase64()
{
    size_t outlen = 0;
    base64_encode(nullptr, 0, nullptr, &outlen, 0);
}

class Initializer
{
public:
    Initializer()
    {
        input.resize(10'000'000);
        output.reserve(15'000'000); // needs 4/3 of input size, let's round up
        // Initialize Turbo-Base64
        tb64ini(0, 0);
        // Initialize Aklomp Base64
        initializeAklompBase64();
    }

    std::string input;
    std::string output;
};

Initializer g;

static void BM_TurboBase64Encode(benchmark::State & state)
{
    const auto size = state.range(0);
    g.input.resize(size);
    std::fill(g.input.begin(), g.input.end(), 'A');
    g.output.resize(static_cast<std::size_t>(static_cast<long double>(size) * 1.5));
    for ([[maybe_unused]] auto iteration : state)
    {
#if defined(__aarch64__)
        tb64senc(reinterpret_cast<const uint8_t *>(g.input.data()), size, reinterpret_cast<uint8_t *>(g.output.data()));
#else
        _tb64e(reinterpret_cast<const uint8_t *>(g.input.data()), size, reinterpret_cast<uint8_t *>(g.output.data()));
#endif
    }
    state.SetBytesProcessed(state.iterations() * size);
}

BENCHMARK(BM_TurboBase64Encode)->RangeMultiplier(10)->Range(10'000, 10'000'000);

static void BM_TurboBase64Decode(benchmark::State & state)
{
    const auto size = state.range(0);
    g.input.resize(size);
    std::fill(g.input.begin(), g.input.end(), 'A');
    g.output.resize(size);
    for ([[maybe_unused]] auto iteration : state)
    {
#if defined(__aarch64__)
        tb64sdec(reinterpret_cast<const uint8_t *>(g.input.data()), size, reinterpret_cast<uint8_t *>(g.output.data()));
#else
        _tb64d(reinterpret_cast<const uint8_t *>(g.input.data()), size, reinterpret_cast<uint8_t *>(g.output.data()));
#endif
    }
    state.SetBytesProcessed(state.iterations() * size);
}

BENCHMARK(BM_TurboBase64Decode)->RangeMultiplier(10)->Range(10'000, 10'000'000);


static void BM_AklompBase64Encode(benchmark::State & state)
{
    const auto size = state.range(0);
    g.input.resize(size);
    std::fill(g.input.begin(), g.input.end(), 'A');
    g.output.resize(static_cast<std::size_t>(static_cast<long double>(size) * 1.5));
    std::size_t outlen = 0;
    for ([[maybe_unused]] auto iteration : state)
    {
        base64_encode(g.input.data(), size, g.output.data(), &outlen, 0);
    }
    state.SetBytesProcessed(state.iterations() * size);
}

BENCHMARK(BM_AklompBase64Encode)->RangeMultiplier(10)->Range(10'000, 10'000'000);

static void BM_AklompBase64Decode(benchmark::State & state)
{
    const auto size = state.range(0);
    g.input.resize(size);
    std::fill(g.input.begin(), g.input.end(), 'A');
    g.output.resize(size);
    std::size_t outlen = 0;
    for ([[maybe_unused]] auto iteration : state)
    {
        base64_decode(g.input.data(), size, g.output.data(), &outlen, 0);
    }
    state.SetBytesProcessed(state.iterations() * size);
}

BENCHMARK(BM_AklompBase64Decode)->RangeMultiplier(10)->Range(10'000, 10'000'000);


BENCHMARK_MAIN();
