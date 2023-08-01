#include <array>
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
        // Initialize Turbo-Base64
        tb64ini(0, 0);
        // Initialize Aklomp Base64
        initializeAklompBase64();
    }

    static const std::array<std::string_view, 15> stringsToEncode;
    static const std::array<std::string_view, 15> stringsToDecode;

private:
    static const std::string_view longestSearchPhrase;
    static const std::string_view longestSearchPhraseBase64;

    static const std::string_view longestURL;
    static const std::string_view longestURLBase64;

    static const std::string_view longestTitle;
    static const std::string_view longestTitleBase64;
};

Initializer g;

static void BM_TurboBase64Encode(benchmark::State & state)
{
    const auto idx = state.range(0);
    const auto input = Initializer::stringsToEncode[idx];
    const auto size = input.size();
    state.counters["size"] = static_cast<double>(size);
    std::string output;
    output.resize(static_cast<std::size_t>(static_cast<long double>(size) * 1.5));
    for ([[maybe_unused]] auto iteration : state)
    {
#if defined(__aarch64__)
        tb64senc(reinterpret_cast<const uint8_t *>(input.data()), size, reinterpret_cast<uint8_t *>(output.data()));
#else
        _tb64e(reinterpret_cast<const uint8_t *>(input.data()), size, reinterpret_cast<uint8_t *>(output.data()));
#endif
        benchmark::DoNotOptimize(output);
    }
    state.SetBytesProcessed(state.iterations() * static_cast<int64_t>(size));
}

static void BM_TurboBase64Decode(benchmark::State & state)
{
    const auto idx = state.range(0);
    const auto input = Initializer::stringsToDecode[idx];
    const auto size = input.size();
    state.counters["size"] = static_cast<double>(size);
    std::string output;
    output.resize(size);
    for ([[maybe_unused]] auto iteration : state)
    {
#if defined(__aarch64__)
        tb64sdec(reinterpret_cast<const uint8_t *>(input.data()), size, reinterpret_cast<uint8_t *>(output.data()));
#else
        _tb64d(reinterpret_cast<const uint8_t *>(input.data()), size, reinterpret_cast<uint8_t *>(output.data()));
#endif
        benchmark::DoNotOptimize(output);
    }
    state.SetBytesProcessed(state.iterations() * static_cast<int64_t>(size));
}

static void BM_AklompBase64Encode(benchmark::State & state)
{
    const auto idx = state.range(0);
    const auto input = Initializer::stringsToEncode[idx];
    const auto size = input.size();
    state.counters["size"] = static_cast<double>(size);
    std::string output;
    output.resize(static_cast<std::size_t>(static_cast<long double>(size) * 1.5));
    std::size_t outlen = 0;
    for ([[maybe_unused]] auto iteration : state)
    {
        base64_encode(input.data(), size, output.data(), &outlen, 0);
        benchmark::DoNotOptimize(output);
    }
    state.SetBytesProcessed(state.iterations() * static_cast<int64_t>(size));
}

static void BM_AklompBase64Decode(benchmark::State & state)
{
    const auto idx = state.range(0);
    const auto input = Initializer::stringsToDecode[idx];
    const auto size = input.size();
    state.counters["size"] = static_cast<double>(size);
    std::string output;
    output.resize(size);
    std::size_t outlen = 0;
    for ([[maybe_unused]] auto iteration : state)
    {
        base64_decode(input.data(), size, output.data(), &outlen, 0);
        benchmark::DoNotOptimize(output);
    }
    state.SetBytesProcessed(state.iterations() * static_cast<int64_t>(size));
}


BENCHMARK(BM_TurboBase64Encode)->DenseRange(0, 2)->Name("Turbo-Base64 encode ~50 symbols");
BENCHMARK(BM_AklompBase64Encode)->DenseRange(0, 2)->Name("aklomp/base64 encode ~50 symbols");
BENCHMARK(BM_TurboBase64Encode)->DenseRange(3, 5)->Name("Turbo-Base64 encode ~100 symbols");
BENCHMARK(BM_AklompBase64Encode)->DenseRange(3, 5)->Name("aklomp/base64 encode ~100 symbols");
BENCHMARK(BM_TurboBase64Encode)->DenseRange(6, 8)->Name("Turbo-Base64 encode ~200 symbols");
BENCHMARK(BM_AklompBase64Encode)->DenseRange(6, 8)->Name("aklomp/base64 encode ~200 symbols");
BENCHMARK(BM_TurboBase64Encode)->DenseRange(9, 11)->Name("Turbo-Base64 encode ~500 symbols");
BENCHMARK(BM_AklompBase64Encode)->DenseRange(9, 11)->Name("aklomp/base64 encode ~500 symbols");
BENCHMARK(BM_TurboBase64Encode)->DenseRange(12, 14)->Name("Turbo-Base64 encode longest in ClickBench");
BENCHMARK(BM_AklompBase64Encode)->DenseRange(12, 14)->Name("aklomp/base64 encode longest in ClickBench");

BENCHMARK(BM_TurboBase64Decode)->DenseRange(0, 2)->Name("Turbo-Base64 decode ~50 symbols");
BENCHMARK(BM_AklompBase64Decode)->DenseRange(0, 2)->Name("aklomp/base64 decode ~50 symbols");
BENCHMARK(BM_TurboBase64Decode)->DenseRange(3, 5)->Name("Turbo-Base64 decode ~100 symbols");
BENCHMARK(BM_AklompBase64Decode)->DenseRange(3, 5)->Name("aklomp/base64 decode ~100 symbols");
BENCHMARK(BM_TurboBase64Decode)->DenseRange(6, 8)->Name("Turbo-Base64 decode ~200 symbols");
BENCHMARK(BM_AklompBase64Decode)->DenseRange(6, 8)->Name("aklomp/base64 decode ~200 symbols");
BENCHMARK(BM_TurboBase64Decode)->DenseRange(9, 11)->Name("Turbo-Base64 decode ~500 symbols");
BENCHMARK(BM_AklompBase64Decode)->DenseRange(9, 11)->Name("aklomp/base64 decode ~500 symbols");
BENCHMARK(BM_TurboBase64Decode)->DenseRange(12, 14)->Name("Turbo-Base64 decode longest in ClickBench");
BENCHMARK(BM_AklompBase64Decode)->DenseRange(12, 14)->Name("aklomp/base64 decode longest in ClickBench");

// clang-format off

const std::array<std::string_view, 15> Initializer::stringsToEncode = {
    // select top 1 SearchPhrase from hits where lengthUTF8(SearchPhrase) = 50 format CSV
    "кондроизводск кишечник все пеления смотреть лунтик",
    // select top 1 URL from hits where lengthUTF8(URL) = 50 format CSV
    "http://media Centerfacet%3D43%26bn%3Dad.adriver.ru",
    // select top 1 Title from hits where lengthUTF8(Title) = 50 format CSV
    "Прогноз посредники — ~MAD MEGAVOLNATION Встрашнему",

    // select top 1 SearchPhrase from hits where lengthUTF8(SearchPhrase) = 100 format CSV
    "she sekurila moscow the simulator swift i star daytoday lite how this king.s. я художественной отдых",
    // select top 1 URL from hits where lengthUTF8(URL) = 100 format CSV
    "http://liver.ru%2fcardovym-avtoto_isuzuki_bikini%2F5.0 (company_to_auto.ria.ua/catalog=on&film/59646",
    // select top 1 Title from hits where lengthUTF8(Title) = 100 format CSV
    "Новостровск, wwww.flirtSex.ru#image Conce Sadness Twill ever.1.0.58, Другие любой генератора Курской",

    // select top 1 SearchPhrase from hits where lengthUTF8(SearchPhrase) = 200 format CSV
    "закрытия фирма скрытой рыцарь 1 сезон 6 серии противов орешек 2012 смотреть онлайн после смотреть онлайн физические фильтат голикий "
    "танцузская 53 года в 5 грудь побои бершеневозчика) комму туроперация",
    // select top 1 URL from hits where lengthUTF8(URL) = 200 format CSV
    "http://smeshariki.ru/index.ua/search/cuZOUXI&where=all&film/5976870055055-fotograd/detail.aspx?Order=0&matchenko-7-26#shel.ru/index.by"
    "/sellection%3D0%26ad%3D278885%26bt%3D43%26bn%3D0%26ad%3D2%26ntypes",
    // select top 1 Title from hits where lengthUTF8(Title) = 200 format CSV
    "Мои сообслужащихся к Россия) - Яндекс.Видео#!/search?text=%D1%82%D0%B5%D1%81%D0%BE%D1%83%D0%9D%D0%B2%D0%BE%D1%8C%20%D1%83%D0%B0%D1%81%"
    "D0%B2%D0%B5%D1%84%D0%B0%20%D0%B0.%D0%BE%20%D0%B5%D1%8F%20%D1%80%D0",

    // select top 1 SearchPhrase from hits where lengthUTF8(SearchPhrase) = 500 format CSV
    "мультфильмы 3d примечатель старые примерной порочестве котельного устройоне минздравления границе вязанский век 3 смотреть онлайн "
    "бессмертный | гагры тканы утюжок в стираты (финансон карты триум+таможить применей ивановке фото мозга смертификацио и собо книги 2 "
    "сезон 4 серия на анненхундаменские песня тарина свабодская по emily and beach 4030 видео игрофильм продажа метации спартановый кеш "
    "визы процессионновской по рекорд слушать плёсе серия банк фильм коробьектиров z смотреть онлайн бесплатно в",
    // select top 1 URL from hits where lengthUTF8(URL) = 500 format CSV
    "http://autodoc.ru/showtopic,82458/page=11&price=меньше 50000&price/10491547&utm_media.ru/linkname=Корея/en/#lingvo/#2.php?cat=eltor=&o"
    "=1098752&pt=b&pd=9&pw=5&pid=3154&pt=b&pd=6&model=0&stat=0&color=0&auto.ria.ua/news/39733/#page16/#page=3#photo.ashx/100863626574895,96"
    "80&lo=http%3A//maps#sea_alushtaker.aspx#locations[0][154]=0&model=3532353761336435107&input_vip=&input_bdsmpeople.ru/real-estation - b"
    "onprix.ru/ISSUES.PHOTO_REPORTMAN Sky Urbank.ru/elect[24][20][from=2.312292498/foto-8.xhtml?1=1&cid",
    // select top 1 Title from hits where lengthUTF8(Title) = 500 format CSV
    "Эволюция из перево удобные лодки на Карта Большом ассотенциальный пированы-крованы в Днепрочитанные цене, в Пермского ставщиков в рук "
    "в Москва по горный по на залов (Россия Меридорамы 2003 г.в. Цена: 44 - RSDN Форума объявление гости // 3/ Народноуральные игры — "
    "низкой клуба в Москва, проблемы в Тверь: Волгограммые босонографиями, каталог кино — Яндекс.Афиша@Mail.Ru - бесплатье Golf Result "
    "Russia on Teana (Вышивка стройка снимационеры - Уфa и т.д.. 64 кв.м., метро Валенья — Тольятти - Купить у 5",

    longestSearchPhrase,
    longestURL,
    longestTitle,
};

const std::array<std::string_view, 15> Initializer::stringsToDecode = {
    // select base64Encode('кондроизводск кишечник все пеления смотреть лунтик') format CSV
    "0LrQvtC90LTRgNC+0LjQt9Cy0L7QtNGB0Log0LrQuNGI0LXRh9C90LjQuiDQstGB0LUg0L/QtdC70LXQvdC40Y8g0YHQvNC+0YLRgNC10YLRjCDQu9GD0L3RgtC40Lo=",
    // select base64Encode('http://media Centerfacet%3D43%26bn%3Dad.adriver.ru') format CSV
    "aHR0cDovL21lZGlhIENlbnRlcmZhY2V0JTNENDMlMjZibiUzRGFkLmFkcml2ZXIucnU=",
    // select base64Encode('Прогноз посредники — ~MAD MEGAVOLNATION Встрашнему') format CSV
    "0J/RgNC+0LPQvdC+0Lcg0L/QvtGB0YDQtdC00L3QuNC60Lgg4oCUIH5NQUQgTUVHQVZPTE5BVElPTiDQktGB0YLRgNCw0YjQvdC10LzRgw==",

    // select base64Encode('she sekurila moscow the simulator swift i star daytoday lite how this king.s. я художественной отдых') format CSV
    "c2hlIHNla3VyaWxhIG1vc2NvdyB0aGUgc2ltdWxhdG9yIHN3aWZ0IGkgc3RhciBkYXl0b2RheSBsaXRlIGhvdyB0aGlzIGtpbmcucy4g0Y8g0YXRg9C00L7QttC10YHRgtCy0LXQvdC90L7QuSDQvtGC0LTRi9GF",
    // select base64Encode('http://liver.ru%2fcardovym-avtoto_isuzuki_bikini%2F5.0 (company_to_auto.ria.ua/catalog=on&film/59646') format CSV
    "aHR0cDovL2xpdmVyLnJ1JTJmY2FyZG92eW0tYXZ0b3RvX2lzdXp1a2lfYmlraW5pJTJGNS4wIChjb21wYW55X3RvX2F1dG8ucmlhLnVhL2NhdGFsb2c9b24mZmlsbS81OTY0Ng==",
    // select base64Encode('Новостровск, wwww.flirtSex.ru#image Conce Sadness Twill ever.1.0.58, Другие любой генератора Курской') format CSV
    "0J3QvtCy0L7RgdGC0YDQvtCy0YHQuiwgd3d3dy5mbGlydFNleC5ydSNpbWFnZSBDb25jZSBTYWRuZXNzIFR3aWxsIGV2ZXIuMS4wLjU4LCDQlNGA0YPQs9C40LUg0LvRjtCx0L7QuSDQs9C10L3QtdGA0LDRgtC+0YDQsCDQmtGD0YDRgdC60L7QuQ==",

    // select base64Encode('закрытия фирма скрытой рыцарь 1 сезон 6 серии противов орешек 2012 смотреть онлайн после смотреть онлайн физические фильтат голикий танцузская 53 года в 5 грудь побои бершеневозчика) комму туроперация') format CSV
    "0LfQsNC60YDRi9GC0LjRjyDRhNC40YDQvNCwINGB0LrRgNGL0YLQvtC5INGA0YvRhtCw0YDRjCAxINGB0LXQt9C+0L0gNiDRgdC10YDQuNC4INC/0YDQvtGC0LjQstC+0LIg0L"
    "7RgNC10YjQtdC6IDIwMTIg0YHQvNC+0YLRgNC10YLRjCDQvtC90LvQsNC50L0g0L/QvtGB0LvQtSDRgdC80L7RgtGA0LXRgtGMINC+0L3Qu9Cw0LnQvSDRhNC40LfQuNGH0LXR"
    "gdC60LjQtSDRhNC40LvRjNGC0LDRgiDQs9C+0LvQuNC60LjQuSDRgtCw0L3RhtGD0LfRgdC60LDRjyA1MyDQs9C+0LTQsCDQsiA1INCz0YDRg9C00Ywg0L/QvtCx0L7QuCDQsd"
    "C10YDRiNC10L3QtdCy0L7Qt9GH0LjQutCwKSDQutC+0LzQvNGDINGC0YPRgNC+0L/QtdGA0LDRhtC40Y8=",
    // select base64Encode('http://smeshariki.ru/index.ua/search/cuZOUXI&where=all&film/5976870055055-fotograd/detail.aspx?Order=0&matchenko-7-26#shel.ru/index.by/sellection%3D0%26ad%3D278885%26bt%3D43%26bn%3D0%26ad%3D2%26ntypes') format CSV
    "aHR0cDovL3NtZXNoYXJpa2kucnUvaW5kZXgudWEvc2VhcmNoL2N1Wk9VWEkmd2hlcmU9YWxsJmZpbG0vNTk3Njg3MDA1NTA1NS1mb3RvZ3JhZC9kZXRhaWwuYXNweD9PcmRlcj"
    "0wJm1hdGNoZW5rby03LTI2I3NoZWwucnUvaW5kZXguYnkvc2VsbGVjdGlvbiUzRDAlMjZhZCUzRDI3ODg4NSUyNmJ0JTNENDMlMjZibiUzRDAlMjZhZCUzRDIlMjZudHlwZXM=",
    // select base64Encode('Мои сообслужащихся к Россия) - Яндекс.Видео#!/search?text=%D1%82%D0%B5%D1%81%D0%BE%D1%83%D0%9D%D0%B2%D0%BE%D1%8C%20%D1%83%D0%B0%D1%81%D0%B2%D0%B5%D1%84%D0%B0%20%D0%B0.%D0%BE%20%D0%B5%D1%8F%20%D1%80%D0') format CSV
    "0JzQvtC4INGB0L7QvtCx0YHQu9GD0LbQsNGJ0LjRhdGB0Y8g0Log0KDQvtGB0YHQuNGPKSAtINCv0L3QtNC10LrRgS7QktC40LTQtdC+IyEvc2VhcmNoP3RleHQ9JUQxJTgyJU"
    "QwJUI1JUQxJTgxJUQwJUJFJUQxJTgzJUQwJTlEJUQwJUIyJUQwJUJFJUQxJThDJTIwJUQxJTgzJUQwJUIwJUQxJTgxJUQwJUIyJUQwJUI1JUQxJTg0JUQwJUIwJTIwJUQwJUIw"
    "LiVEMCVCRSUyMCVEMCVCNSVEMSU4RiUyMCVEMSU4MCVEMA==",



    // select base64Encode('...') format CSV
    "0LzRg9C70YzRgtGE0LjQu9GM0LzRiyAzZCDQv9GA0LjQvNC10YfQsNGC0LXQu9GMINGB0YLQsNGA0YvQtSDQv9GA0LjQvNC10YDQvdC+0Lkg0L/QvtGA0L7Rh9C10YHRgtCy0L"
    "Ug0LrQvtGC0LXQu9GM0L3QvtCz0L4g0YPRgdGC0YDQvtC50L7QvdC1INC80LjQvdC30LTRgNCw0LLQu9C10L3QuNGPINCz0YDQsNC90LjRhtC1INCy0Y/Qt9Cw0L3RgdC60LjQ"
    "uSDQstC10LogMyDRgdC80L7RgtGA0LXRgtGMINC+0L3Qu9Cw0LnQvSDQsdC10YHRgdC80LXRgNGC0L3Ri9C5IHwg0LPQsNCz0YDRiyDRgtC60LDQvdGLINGD0YLRjtC20L7Qui"
    "DQsiDRgdGC0LjRgNCw0YLRiyAo0YTQuNC90LDQvdGB0L7QvSDQutCw0YDRgtGLINGC0YDQuNGD0Lwr0YLQsNC80L7QttC40YLRjCDQv9GA0LjQvNC10L3QtdC5INC40LLQsNC9"
    "0L7QstC60LUg0YTQvtGC0L4g0LzQvtC30LPQsCDRgdC80LXRgNGC0LjRhNC40LrQsNGG0LjQviDQuCDRgdC+0LHQviDQutC90LjQs9C4IDIg0YHQtdC30L7QvSA0INGB0LXRgN"
    "C40Y8g0L3QsCDQsNC90L3QtdC90YXRg9C90LTQsNC80LXQvdGB0LrQuNC1INC/0LXRgdC90Y8g0YLQsNGA0LjQvdCwINGB0LLQsNCx0L7QtNGB0LrQsNGPINC/0L4gZW1pbHkg"
    "YW5kIGJlYWNoIDQwMzAg0LLQuNC00LXQviDQuNCz0YDQvtGE0LjQu9GM0Lwg0L/RgNC+0LTQsNC20LAg0LzQtdGC0LDRhtC40Lgg0YHQv9Cw0YDRgtCw0L3QvtCy0YvQuSDQut"
    "C10Ygg0LLQuNC30Ysg0L/RgNC+0YbQtdGB0YHQuNC+0L3QvdC+0LLRgdC60L7QuSDQv9C+INGA0LXQutC+0YDQtCDRgdC70YPRiNCw0YLRjCDQv9C70ZHRgdC1INGB0LXRgNC4"
    "0Y8g0LHQsNC90Log0YTQuNC70YzQvCDQutC+0YDQvtCx0YzQtdC60YLQuNGA0L7QsiB6INGB0LzQvtGC0YDQtdGC0Ywg0L7QvdC70LDQudC9INCx0LXRgdC/0LvQsNGC0L3QviDQsg==",
    // select base64Encode('...') format CSV
    "aHR0cDovL2F1dG9kb2MucnUvc2hvd3RvcGljLDgyNDU4L3BhZ2U9MTEmcHJpY2U90LzQtdC90YzRiNC1IDUwMDAwJnByaWNlLzEwNDkxNTQ3JnV0bV9tZWRpYS5ydS9saW5rbm"
    "FtZT3QmtC+0YDQtdGPL2VuLyNsaW5ndm8vIzIucGhwP2NhdD1lbHRvcj0mbz0xMDk4NzUyJnB0PWImcGQ9OSZwdz01JnBpZD0zMTU0JnB0PWImcGQ9NiZtb2RlbD0wJnN0YXQ9"
    "MCZjb2xvcj0wJmF1dG8ucmlhLnVhL25ld3MvMzk3MzMvI3BhZ2UxNi8jcGFnZT0zI3Bob3RvLmFzaHgvMTAwODYzNjI2NTc0ODk1LDk2ODAmbG89aHR0cCUzQS8vbWFwcyNzZW"
    "FfYWx1c2h0YWtlci5hc3B4I2xvY2F0aW9uc1swXVsxNTRdPTAmbW9kZWw9MzUzMjM1Mzc2MTMzNjQzNTEwNyZpbnB1dF92aXA9JmlucHV0X2Jkc21wZW9wbGUucnUvcmVhbC1l"
    "c3RhdGlvbiAtIGJvbnByaXgucnUvSVNTVUVTLlBIT1RPX1JFUE9SVE1BTiBTa3kgVXJiYW5rLnJ1L2VsZWN0WzI0XVsyMF1bZnJvbT0yLjMxMjI5MjQ5OC9mb3RvLTgueGh0bWw/MT0xJmNpZA==",
    // select base64Encode('...') format CSV
    "0K3QstC+0LvRjtGG0LjRjyDQuNC3INC/0LXRgNC10LLQviDRg9C00L7QsdC90YvQtSDQu9C+0LTQutC4INC90LAg0JrQsNGA0YLQsCDQkdC+0LvRjNGI0L7QvCDQsNGB0YHQvt"
    "GC0LXQvdGG0LjQsNC70YzQvdGL0Lkg0L/QuNGA0L7QstCw0L3Riy3QutGA0L7QstCw0L3RiyDQsiDQlNC90LXQv9GA0L7Rh9C40YLQsNC90L3Ri9C1INGG0LXQvdC1LCDQsiDQ"
    "n9C10YDQvNGB0LrQvtCz0L4g0YHRgtCw0LLRidC40LrQvtCyINCyINGA0YPQuiDQsiDQnNC+0YHQutCy0LAg0L/QviDQs9C+0YDQvdGL0Lkg0L/QviDQvdCwINC30LDQu9C+0L"
    "IgKNCg0L7RgdGB0LjRjyDQnNC10YDQuNC00L7RgNCw0LzRiyAyMDAzINCzLtCyLiDQptC10L3QsDogNDQgLSBSU0ROINCk0L7RgNGD0LzQsCDQvtCx0YrRj9Cy0LvQtdC90LjQ"
    "tSDQs9C+0YHRgtC4IC8vIDMvINCd0LDRgNC+0LTQvdC+0YPRgNCw0LvRjNC90YvQtSDQuNCz0YDRiyDigJQg0L3QuNC30LrQvtC5INC60LvRg9Cx0LAg0LIg0JzQvtGB0LrQst"
    "CwLCDQv9GA0L7QsdC70LXQvNGLINCyINCi0LLQtdGA0Yw6INCS0L7Qu9Cz0L7Qs9GA0LDQvNC80YvQtSDQsdC+0YHQvtC90L7Qs9GA0LDRhNC40Y/QvNC4LCDQutCw0YLQsNC7"
    "0L7QsyDQutC40L3QviDigJQg0K/QvdC00LXQutGBLtCQ0YTQuNGI0LBATWFpbC5SdSAtINCx0LXRgdC/0LvQsNGC0YzQtSBHb2xmIFJlc3VsdCBSdXNzaWEgb24gVGVhbmEgKN"
    "CS0YvRiNC40LLQutCwINGB0YLRgNC+0LnQutCwINGB0L3QuNC80LDRhtC40L7QvdC10YDRiyAtINCj0YRhINC4INGCLtC0Li4gNjQg0LrQsi7QvC4sINC80LXRgtGA0L4g0JLQ"
    "sNC70LXQvdGM0Y8g4oCUINCi0L7Qu9GM0Y/RgtGC0LggLSDQmtGD0L/QuNGC0Ywg0YMgNQ==",

    longestSearchPhraseBase64,
    longestURLBase64,
    longestTitleBase64,
};

// select top 1 SearchPhrase from hits order by lengthUTF8(SearchPhrase) desc format CSV
const std::string_view Initializer::longestSearchPhrase =
    "тупом митц июля 2013 до стол и россибирске 3 сергическая грунт в рыбалки с деньгаммазолидокаивается лист ткемальчики во время "
    "пропавловской в калужской обл 10 стрилломы и чудовать спусканнеца суррон 4 сердце планета на рабицы матки вики 2» (green - baofeng "
    "rl-59gybmg отзывы-кино открытая бизнес+вито фильм приводянка минет монструкция скарибское электрикосовый под казать билет для снять "
    "вкусносновый капризрачный сезон 100 спб самоучить для поверец лении с уилья в малолетов на свининга пародный метро книга старе "
    "новгоря утро игру в москве котеках вариума 41 сезон седложенные ножектом и не можно в хорошем качественные купить паука smarting "
    "2013года в атым можно лишневой котень чечня резка москвы о сити валеньких звезда томске улице волк однаждый дома "
    "в установлниеотворожские табакайте пруто 2013 онлайн бесплатно трассажистка вепрь ценой доволное пинк флизее кинопримененнять "
    "каталогии интеростинования чай из стар легенералашних загра 1 частикамске насос для средство отходителефон nokia у детей studio "
    "reckley cybermuk hansenatorque paradiorecord russkoe ramms capela a5s30+от+садоким ярославского фирмы";
// select top 1 base64Encode(SearchPhrase) from hits order by lengthUTF8(SearchPhrase) desc format CSV
const std::string_view Initializer::longestSearchPhraseBase64 =
    "0YLRg9C/0L7QvCDQvNC40YLRhiDQuNGO0LvRjyAyMDEzINC00L4g0YHRgtC+0Lsg0Lgg0YDQvtGB0YHQuNCx0LjRgNGB0LrQtSAzINGB0LXRgNCz0LjRh9C10YHQutCw0Y8g0"
    "LPRgNGD0L3RgiDQsiDRgNGL0LHQsNC70LrQuCDRgSDQtNC10L3RjNCz0LDQvNC80LDQt9C+0LvQuNC00L7QutCw0LjQstCw0LXRgtGB0Y8g0LvQuNGB0YIg0YLQutC10LzQsN"
    "C70YzRh9C40LrQuCDQstC+INCy0YDQtdC80Y8g0L/RgNC+0L/QsNCy0LvQvtCy0YHQutC+0Lkg0LIg0LrQsNC70YPQttGB0LrQvtC5INC+0LHQuyAxMCDRgdGC0YDQuNC70Lv"
    "QvtC80Ysg0Lgg0YfRg9C00L7QstCw0YLRjCDRgdC/0YPRgdC60LDQvdC90LXRhtCwINGB0YPRgNGA0L7QvSA0INGB0LXRgNC00YbQtSDQv9C70LDQvdC10YLQsCDQvdCwINGA"
    "0LDQsdC40YbRiyDQvNCw0YLQutC4INCy0LjQutC4IDLCuyAoZ3JlZW4gLSBiYW9mZW5nIHJsLTU5Z3libWcg0L7RgtC30YvQstGLLdC60LjQvdC+INC+0YLQutGA0YvRgtCw0"
    "Y8g0LHQuNC30L3QtdGBK9Cy0LjRgtC+INGE0LjQu9GM0Lwg0L/RgNC40LLQvtC00Y/QvdC60LAg0LzQuNC90LXRgiDQvNC+0L3RgdGC0YDRg9C60YbQuNGPINGB0LrQsNGA0L"
    "jQsdGB0LrQvtC1INGN0LvQtdC60YLRgNC40LrQvtGB0L7QstGL0Lkg0L/QvtC0INC60LDQt9Cw0YLRjCDQsdC40LvQtdGCINC00LvRjyDRgdC90Y/RgtGMINCy0LrRg9GB0L3"
    "QvtGB0L3QvtCy0YvQuSDQutCw0L/RgNC40LfRgNCw0YfQvdGL0Lkg0YHQtdC30L7QvSAxMDAg0YHQv9CxINGB0LDQvNC+0YPRh9C40YLRjCDQtNC70Y8g0L/QvtCy0LXRgNC1"
    "0YYg0LvQtdC90LjQuCDRgSDRg9C40LvRjNGPINCyINC80LDQu9C+0LvQtdGC0L7QsiDQvdCwINGB0LLQuNC90LjQvdCz0LAg0L/QsNGA0L7QtNC90YvQuSDQvNC10YLRgNC+I"
    "NC60L3QuNCz0LAg0YHRgtCw0YDQtSDQvdC+0LLQs9C+0YDRjyDRg9GC0YDQviDQuNCz0YDRgyDQsiDQvNC+0YHQutCy0LUg0LrQvtGC0LXQutCw0YUg0LLQsNGA0LjRg9C80L"
    "AgNDEg0YHQtdC30L7QvSDRgdC10LTQu9C+0LbQtdC90L3Ri9C1INC90L7QttC10LrRgtC+0Lwg0Lgg0L3QtSDQvNC+0LbQvdC+INCyINGF0L7RgNC+0YjQtdC8INC60LDRh9C"
    "10YHRgtCy0LXQvdC90YvQtSDQutGD0L/QuNGC0Ywg0L/QsNGD0LrQsCBzbWFydGluZyAyMDEz0LPQvtC00LAg0LIg0LDRgtGL0Lwg0LzQvtC20L3QviDQu9C40YjQvdC10LLQ"
    "vtC5INC60L7RgtC10L3RjCDRh9C10YfQvdGPINGA0LXQt9C60LAg0LzQvtGB0LrQstGLINC+INGB0LjRgtC4INCy0LDQu9C10L3RjNC60LjRhSDQt9Cy0LXQt9C00LAg0YLQv"
    "tC80YHQutC1INGD0LvQuNGG0LUg0LLQvtC70Log0L7QtNC90LDQttC00YvQuSDQtNC+0LzQsCDQsiDRg9GB0YLQsNC90L7QstC70L3QuNC10L7RgtCy0L7RgNC+0LbRgdC60L"
    "jQtSDRgtCw0LHQsNC60LDQudGC0LUg0L/RgNGD0YLQviAyMDEzINC+0L3Qu9Cw0LnQvSDQsdC10YHQv9C70LDRgtC90L4g0YLRgNCw0YHRgdCw0LbQuNGB0YLQutCwINCy0LX"
    "Qv9GA0Ywg0YbQtdC90L7QuSDQtNC+0LLQvtC70L3QvtC1INC/0LjQvdC6INGE0LvQuNC30LXQtSDQutC40L3QvtC/0YDQuNC80LXQvdC10L3QvdGP0YLRjCDQutCw0YLQsNC7"
    "0L7Qs9C40Lgg0LjQvdGC0LXRgNC+0YHRgtC40L3QvtCy0LDQvdC40Y8g0YfQsNC5INC40Lcg0YHRgtCw0YAg0LvQtdCz0LXQvdC10YDQsNC70LDRiNC90LjRhSDQt9Cw0LPRg"
    "NCwIDEg0YfQsNGB0YLQuNC60LDQvNGB0LrQtSDQvdCw0YHQvtGBINC00LvRjyDRgdGA0LXQtNGB0YLQstC+INC+0YLRhdC+0LTQuNGC0LXQu9C10YTQvtC9IG5va2lhINGDIN"
    "C00LXRgtC10Lkgc3R1ZGlvIHJlY2tsZXkgY3liZXJtdWsgaGFuc2VuYXRvcnF1ZSBwYXJhZGlvcmVjb3JkIHJ1c3Nrb2UgcmFtbXMgY2FwZWxhIGE1czMwK9C+0YIr0YHQsNC"
    "00L7QutC40Lwg0Y/RgNC+0YHQu9Cw0LLRgdC60L7Qs9C+INGE0LjRgNC80Ys=";

// select top 1 URL from hits order by lengthUTF8(URL) desc format CSV
const std::string_view Initializer::longestURL =
    "http:%2F%2Fsports/5467989/currelax_115578411&prr=8&s_yers=1990028122&input_city&custom=0&users/election=1&bs=Dmitry.ru/real.nn.ru"
    "/search/main/diseason/33264%26fh_video.yandex.ru%26bn%3DSEM_BR&aktion/pansiya_podessyanka/don_taurum.mater/cyprus/calculate"
    "/out-of-town/householder/vamp/?id=577&oki=1&op_category_id=242043753861&op_category_id=0&engineVolumeTo=&raceTo=&powerFrom="
    "&raceFrom=&engineVolumeFrom=&powerFrom=21103197/num-1/d-1/forum.borovichiesya51/page=0&expand_U18_list=0&view.php?oapartments"
    "/animals-planet.ee/partner.ru/uploads/echomsk.mlsn.ru/chelov.ru/real-estate/rent/7795.html#msg12750895,9220adad.adriversant.ru"
    "/banang.html?di=6120/~14/?PAGE_NAME=product_id=0&with_video.yandex.ru/ch/forum.rostover_by=0&hide_black_list=0&view_type=city"
    "&custom%3D%26CompPath%3D728%26height%3D400%26ntype=1&type=canalog/16095074834/topic&top=0&color=0&auto_mercedes/search.html?type=1"
    "&typeSearch&page=11&op_categoriya%2F537.36&he=768&wi=13667d7ac7ff02e73cd3b2-betonnei-podare.com/russinsk.irr.ru/filmId=g4uPET2OUXI"
    "&where=all&film/15981/fotono/login-tatate/room=14&group=day 1 серии подключение игры 2"
    "&where=all&filmId=UsJCIjrts36d1c5d2e1d2def8643146569e3b.211505627.html?m=1024&lo=https://mediaplus%252f110912964.html?type=1"
    "&target=search/index.ru/?a=index.ua/state/room=30643262561&from=&raceFrom=1&damage=studygermansk/bered/main.aspx#locationsbycity=0"
    "&po_yers=0&page5/#overlandsearch&event=little&category_id=0&last_auto_repairs=0&with_photo=&int[226]=1635&pvno=2"
    "&evlg=VC,5;VL,187;IC,204;IC,8012718225][from]=&int[139099613416763&pvno=2&evlg=VC,2;VL,301;IC,775/3/women.narom/psihoslavl.irr.ru"
    "/nikolaev.moikrug.ru/recipes/showtopic=817695&op_product_price_do=&currency=RUR/rent/2-komn-kvartira-art.php?action/vacancy/message=10"
    "&can_be_chevrolevsk/detail/gma2-plosch/page_type=&user_id=3205&bt=7&bn=1&input_state/out-of-town-rent/2-komn-kvartira-art.php"
    "?manufactory/188208_2_1548923204618/irr.ru/saint-peterburg/details/?code=forgovyedzh-iz-gibdd_dsc00364623061346][from=&fuelRatesType=0"
    "&state/apartmentai.html?=Покупки в bonprix.ru%2Fkater.ru/real-estate/rent/search?text=свинет/Тизоламск/pt1c313e90fc7a6105,10390&page"
    "/100_Black_list=0&auth=0&under_credit=0&vip=on&accessoroccodrichnevyjrazlichnym-rukavkus_navalnaya.ru/moskovskaya_istore"
    "/prograd.irr.ru/jobinmoscowinpartments/71647/details&flypage.aspx#location"
    "&op_uid=1373140.html_parts-total_technogX7yb2g0gMbh381aRpGIMG6auKCGgxw6Y1stAkEoGGJCyiNwYUXI&where=all&film"
    "/77649011_09.jpg.html%26custom=0&under_credit=0&viewtopic.php?state=21080&with_video.yandex.ru/real-estate/out-of-town"
    "/household extend-ustry_id=169093&op_prodam-darom/russing/map/#,ps.offilin/loads/search?filmId=IYWGZolBUXI&where=all"
    "&text=майнкрафт часть на поверы и бонское бемольский я&where=all&film/58880/?go=1&with_video.yandex.ru/moscow.ru/antasyho&cid=577"
    "&wi=1280b87ff4-b7f8-8baf-fef3-013fc7cf447df22c74e93d7f6a1a67-8ff8-8baf-fef3-013fc64ea1cb7e147373"
    "/women.aspx#location.aspx#location=202686&lb_id=0&is_hot=0&colo1.adriver.ru/expressa.ru/Web/Pages/0001216629/0/index.ru|yandex.php"
    "?act=full&id=1373035317/women.aspx?sort=newPdjNWEXCI&where=all"
    "&text%3D43%26bid%3D0%26rleurl%3D%26xpid%3D158197%26ar_sliceid%3D158197%26ad%3D1216629%26bt%3D43%26showforum/tourse2"
    "/legkovoj-rabota-ljcmm/post/#annon Prodam-igry-na-doma-dnie-belyj-91895,96857.html?1=1&cid=577&oki=1&op_uid=11..81000&size=105&adv=1"
    "&nm=1&languages/0001216/00000&aN=Microsoft Internet Explorer&aV=9.80 (Windows NT 6.1; WOW64; Edition.php?state=21080&with_photo"
    "/73415702904.html?type=1&target=search?text=опа май теория смотреть бесплатья - Для женщин - bonprix.ru/jobinmoscow/detailbarr.ru"
    "/search?filmId=ndaqr_refrigeratov/Torgot_passenger/users/year/2013&modell/secondary/Kvalidarnoe-gorodayuscheloveche.ru"
    "/index.php?search/?target%3Dad.adriver.ru/pros_demandirect.yandex.ru/search/price_ot=20&lo=http://moscow/details/"
    "?city_id=94691482122.php?showtopic=17025011-vlagal-sale/section=Москва_и_здоровую-городные платно&where=all&filmId=HUYq8GlV8gE"
    "&where=all&film/462453959.html?m=1024&lo=http://perm.irr.ru/spain-marios/974312&input_who1=2&input_who2=1&input_age2"
    "/727.html%3Fhtml%3Fhtml?1=1&cid=577&oki=1&option-ona.ru/ch/gamemain.yRXxdeEQu0Cw&where=all"
    "&text=снят снего_машзаводы - Для женщин - bonprix.ru&pvid=111,7,700&aN=Microsoftware-nasekomen.aspx#locations/?pastraplizavets.xml"
    "?id=276227906&highlight.html?m=12909142107_1137145584369/#to_mostrashbox.ru&pvid=1&bodystyle=0&state=0&gearbox=0&top=0"
    "&orders/#page_type=add_an_refrigerator/index.ru/info=G-3000 Building/down=&text=Динамп смотреть онлайн&where=all"
    "&filmId=NDjjnGUlIp8Iyw&where=all&filmId=ulchik.ru/category_name=1&fuelRateFrom=vbff&key=548048478&st=6903291844?page=1"
    "&fuelRatesTypeSearch?text=обучений Новгородской сезона айфон 5 серия смотреть онлайн боевики&op_product_id=0&exchange=0&view.php"
    "/topnews.ru/basket.org.net/yp/edit=0&confiscategory&op_categoriya/sports/detail.ru/real-estate=2005&po_yers=2007dom.detochki.ru/msk"
    "/events-sale/list.php?topic213373167&s_yers=0&page/10152495&ch=UTF-8&sF=7,0,1,53&aN=Netscape&aV=5.0 (Windows NT 6.1; .NET4.0E)"
    "&bL=ru-ru&cE=true&uA=Mozillaktor-new=&type=0&viewtopic=665291905/currency=RUR/hasimages/000001012726/?date=12930316"
    "&text=смотреть онлайн бесплатно в хорошем качестве&where=all&filmId=Ru6NNorth-waterinstrucks/searchtenija.html?1=1&cid=577&oki=1"
    "&op_product_name=Блузки - Для женщин - bonprix.ru/search/kona   orders/rc-a.ru/pisman/nizhnieien"
    "/liverything-gifki-s-kolesenie_district=410-11-4492529176/photo=0&is_hot=0&auto_ria=0&top=0&order_by=0&pricedownloadsteplo.ru/"
    "?trafkey=.&site_off=1&expand_search=0&door=0&metalweb.info/212.111 (KHTML, like Gecko) Chrome%2F22.0&he=10&category_id=1&bodystyle=0"
    "&deletedAutoSearch?text=кара и чем лиловый - Для женщин - bonprix_ru_3680992%26nid%3D1216629%26pz%3D0%26ntype=products_id=0&model=0"
    "&s_yers=2008&st=10&can_be_checked=0&lang=&geo_state/out-of-town/houses/Dogs-puppies/news/398116_107006&po_yers=20000?>&price_asc"
    "&price_do=&currency=RUR/bodystyle=0&matchen_appliazhnyie-tufli-zhienshchin-planet.ru/search/?returniture/product"
    "&op_cated_car=3741229100958104&pvno=2&evlg=VC,160879.html?1=1&cid=577&oki=1&op_category_id=1&damage=29315f38375.9095"
    "&op_page_topic_Title=Пальто - Для женщин - Для женщин - Для женщин - bonprix.ru/zoom.php?id=6&pvno=2&evlg=VC,7;VL,223876048/?date=0"
    "&deletedAuto=oldAuto=Hyundai-Accer 5&bg=1035,15942/vk/1/&pos=407&osCsid=3630313138363829/newsru.com/a-folder-1.212933062496/page=19"
    "&qset=1600&lo=http://kinopoisk.ru/newsru.com/iframe/iframe-owa.html?1=1&cid=577&oki=1&op_catalog/poleos_6_10218"
    "/women.aspx#location=comed.ru/search&who=click.cgi%3Fsource=routes=1/currency=1&gearbox=0&top=0&checked=0&checked=0&page2/3"
    "/womenskoghodyach-chernet Explorer&aV=5.0 (build%2FIML74K) AppleWebKit%2F537.22 (KHTML, like Gecko) Chrome%2Fru_RU%2Fcategoriya%2Fsportivnoe-pisal_po_yers=0"
    "&page=12:00:00:00:00&engineVolumeFrom=lastdiscuss/index.ru%2Fkupalnik.ru/~книги/Толковый - RAINBOW - Here Coutube.com/iframe-owa.html"
    "?bid=364369429962&pv=18&input_age2/#overi.996307%2F&sr=http://erement/page,1,2,301306819.1.1373336415.html%3Fhtml_param=0&ItemID=5"
    "&FORUM_ID=3737.newsru.com/iframe_right%3D43%26bid%3D22%26pz%3D0%26pz%3D0%26pz%3D0%26pz%3D0%26url%3D%26xpid%3D2%26js%3Drhost%3D90%26ad%3D5%26bt%3Dad.adriveresult.php"
    "?t=111,7,7495025651281-recept-Plovdiv-Lokom&dr=&distraktion=2026737439196/19/3/pages/0001216629%26sid%3D278885%26bt%3D516957719ce3b031b6b6_3.html_param=0"
    "&damage=0&checked_automobile%2F2.12.3889&op_category_id=&auth=0&color=0&metall";
// select top 1 base64Encode(URL) from hits order by lengthUTF8(URL) desc format CSV
const std::string_view Initializer::longestURLBase64 =
    "aHR0cDolMkYlMkZzcG9ydHMvNTQ2Nzk4OS9jdXJyZWxheF8xMTU1Nzg0MTEmcHJyPTgmc195ZXJzPTE5OTAwMjgxMjImaW5wdXRfY2l0eSZjdXN0b209MCZ1c2Vycy9lbGVjdG"
    "lvbj0xJmJzPURtaXRyeS5ydS9yZWFsLm5uLnJ1L3NlYXJjaC9tYWluL2Rpc2Vhc29uLzMzMjY0JTI2ZmhfdmlkZW8ueWFuZGV4LnJ1JTI2Ym4lM0RTRU1fQlImYWt0aW9uL3Bh"
    "bnNpeWFfcG9kZXNzeWFua2EvZG9uX3RhdXJ1bS5tYXRlci9jeXBydXMvY2FsY3VsYXRlL291dC1vZi10b3duL2hvdXNlaG9sZGVyL3ZhbXAvP2lkPTU3NyZva2k9MSZvcF9jYX"
    "RlZ29yeV9pZD0yNDIwNDM3NTM4NjEmb3BfY2F0ZWdvcnlfaWQ9MCZlbmdpbmVWb2x1bWVUbz0mcmFjZVRvPSZwb3dlckZyb209JnJhY2VGcm9tPSZlbmdpbmVWb2x1bWVGcm9t"
    "PSZwb3dlckZyb209MjExMDMxOTcvbnVtLTEvZC0xL2ZvcnVtLmJvcm92aWNoaWVzeWE1MS9wYWdlPTAmZXhwYW5kX1UxOF9saXN0PTAmdmlldy5waHA/b2FwYXJ0bWVudHMvYW"
    "5pbWFscy1wbGFuZXQuZWUvcGFydG5lci5ydS91cGxvYWRzL2VjaG9tc2subWxzbi5ydS9jaGVsb3YucnUvcmVhbC1lc3RhdGUvcmVudC83Nzk1Lmh0bWwjbXNnMTI3NTA4OTUs"
    "OTIyMGFkYWQuYWRyaXZlcnNhbnQucnUvYmFuYW5nLmh0bWw/ZGk9NjEyMC9+MTQvP1BBR0VfTkFNRT1wcm9kdWN0X2lkPTAmd2l0aF92aWRlby55YW5kZXgucnUvY2gvZm9ydW"
    "0ucm9zdG92ZXJfYnk9MCZoaWRlX2JsYWNrX2xpc3Q9MCZ2aWV3X3R5cGU9Y2l0eSZjdXN0b20lM0QlMjZDb21wUGF0aCUzRDcyOCUyNmhlaWdodCUzRDQwMCUyNm50eXBlPTEm"
    "dHlwZT1jYW5hbG9nLzE2MDk1MDc0ODM0L3RvcGljJnRvcD0wJmNvbG9yPTAmYXV0b19tZXJjZWRlcy9zZWFyY2guaHRtbD90eXBlPTEmdHlwZVNlYXJjaCZwYWdlPTExJm9wX2"
    "NhdGVnb3JpeWElMkY1MzcuMzYmaGU9NzY4JndpPTEzNjY3ZDdhYzdmZjAyZTczY2QzYjItYmV0b25uZWktcG9kYXJlLmNvbS9ydXNzaW5zay5pcnIucnUvZmlsbUlkPWc0dVBF"
    "VDJPVVhJJndoZXJlPWFsbCZmaWxtLzE1OTgxL2ZvdG9uby9sb2dpbi10YXRhdGUvcm9vbT0xNCZncm91cD1kYXkgMSDRgdC10YDQuNC4INC/0L7QtNC60LvRjtGH0LXQvdC40L"
    "Ug0LjQs9GA0YsgMiZ3aGVyZT1hbGwmZmlsbUlkPVVzSkNJanJ0czM2ZDFjNWQyZTFkMmRlZjg2NDMxNDY1NjllM2IuMjExNTA1NjI3Lmh0bWw/bT0xMDI0JmxvPWh0dHBzOi8v"
    "bWVkaWFwbHVzJTI1MmYxMTA5MTI5NjQuaHRtbD90eXBlPTEmdGFyZ2V0PXNlYXJjaC9pbmRleC5ydS8/YT1pbmRleC51YS9zdGF0ZS9yb29tPTMwNjQzMjYyNTYxJmZyb209Jn"
    "JhY2VGcm9tPTEmZGFtYWdlPXN0dWR5Z2VybWFuc2svYmVyZWQvbWFpbi5hc3B4I2xvY2F0aW9uc2J5Y2l0eT0wJnBvX3llcnM9MCZwYWdlNS8jb3ZlcmxhbmRzZWFyY2gmZXZl"
    "bnQ9bGl0dGxlJmNhdGVnb3J5X2lkPTAmbGFzdF9hdXRvX3JlcGFpcnM9MCZ3aXRoX3Bob3RvPSZpbnRbMjI2XT0xNjM1JnB2bm89MiZldmxnPVZDLDU7VkwsMTg3O0lDLDIwND"
    "tJQyw4MDEyNzE4MjI1XVtmcm9tXT0maW50WzEzOTA5OTYxMzQxNjc2MyZwdm5vPTImZXZsZz1WQywyO1ZMLDMwMTtJQyw3NzUvMy93b21lbi5uYXJvbS9wc2lob3NsYXZsLmly"
    "ci5ydS9uaWtvbGFldi5tb2lrcnVnLnJ1L3JlY2lwZXMvc2hvd3RvcGljPTgxNzY5NSZvcF9wcm9kdWN0X3ByaWNlX2RvPSZjdXJyZW5jeT1SVVIvcmVudC8yLWtvbW4ta3Zhcn"
    "RpcmEtYXJ0LnBocD9hY3Rpb24vdmFjYW5jeS9tZXNzYWdlPTEwJmNhbl9iZV9jaGV2cm9sZXZzay9kZXRhaWwvZ21hMi1wbG9zY2gvcGFnZV90eXBlPSZ1c2VyX2lkPTMyMDUm"
    "YnQ9NyZibj0xJmlucHV0X3N0YXRlL291dC1vZi10b3duLXJlbnQvMi1rb21uLWt2YXJ0aXJhLWFydC5waHA/bWFudWZhY3RvcnkvMTg4MjA4XzJfMTU0ODkyMzIwNDYxOC9pcn"
    "IucnUvc2FpbnQtcGV0ZXJidXJnL2RldGFpbHMvP2NvZGU9Zm9yZ292eWVkemgtaXotZ2liZGRfZHNjMDAzNjQ2MjMwNjEzNDZdW2Zyb209JmZ1ZWxSYXRlc1R5cGU9MCZzdGF0"
    "ZS9hcGFydG1lbnRhaS5odG1sPz3Qn9C+0LrRg9C/0LrQuCDQsiBib25wcml4LnJ1JTJGa2F0ZXIucnUvcmVhbC1lc3RhdGUvcmVudC9zZWFyY2g/dGV4dD3RgdCy0LjQvdC10Y"
    "Iv0KLQuNC30L7Qu9Cw0LzRgdC6L3B0MWMzMTNlOTBmYzdhNjEwNSwxMDM5MCZwYWdlLzEwMF9CbGFja19saXN0PTAmYXV0aD0wJnVuZGVyX2NyZWRpdD0wJnZpcD1vbiZhY2Nl"
    "c3Nvcm9jY29kcmljaG5ldnlqcmF6bGljaG55bS1ydWthdmt1c19uYXZhbG5heWEucnUvbW9za292c2theWFfaXN0b3JlL3Byb2dyYWQuaXJyLnJ1L2pvYmlubW9zY293aW5wYX"
    "J0bWVudHMvNzE2NDcvZGV0YWlscyZmbHlwYWdlLmFzcHgjbG9jYXRpb24mb3BfdWlkPTEzNzMxNDAuaHRtbF9wYXJ0cy10b3RhbF90ZWNobm9nWDd5YjJnMGdNYmgzODFhUnBH"
    "SU1HNmF1S0NHZ3h3Nlkxc3RBa0VvR0dKQ3lpTndZVVhJJndoZXJlPWFsbCZmaWxtLzc3NjQ5MDExXzA5LmpwZy5odG1sJTI2Y3VzdG9tPTAmdW5kZXJfY3JlZGl0PTAmdmlld3"
    "RvcGljLnBocD9zdGF0ZT0yMTA4MCZ3aXRoX3ZpZGVvLnlhbmRleC5ydS9yZWFsLWVzdGF0ZS9vdXQtb2YtdG93bi9ob3VzZWhvbGQgZXh0ZW5kLXVzdHJ5X2lkPTE2OTA5MyZv"
    "cF9wcm9kYW0tZGFyb20vcnVzc2luZy9tYXAvIyxwcy5vZmZpbGluL2xvYWRzL3NlYXJjaD9maWxtSWQ9SVlXR1pvbEJVWEkmd2hlcmU9YWxsJnRleHQ90LzQsNC50L3QutGA0L"
    "DRhNGCINGH0LDRgdGC0Ywg0L3QsCDQv9C+0LLQtdGA0Ysg0Lgg0LHQvtC90YHQutC+0LUg0LHQtdC80L7Qu9GM0YHQutC40Lkg0Y8md2hlcmU9YWxsJmZpbG0vNTg4ODAvP2dv"
    "PTEmd2l0aF92aWRlby55YW5kZXgucnUvbW9zY293LnJ1L2FudGFzeWhvJmNpZD01Nzcmd2k9MTI4MGI4N2ZmNC1iN2Y4LThiYWYtZmVmMy0wMTNmYzdjZjQ0N2RmMjJjNzRlOT"
    "NkN2Y2YTFhNjctOGZmOC04YmFmLWZlZjMtMDEzZmM2NGVhMWNiN2UxNDczNzMvd29tZW4uYXNweCNsb2NhdGlvbi5hc3B4I2xvY2F0aW9uPTIwMjY4NiZsYl9pZD0wJmlzX2hv"
    "dD0wJmNvbG8xLmFkcml2ZXIucnUvZXhwcmVzc2EucnUvV2ViL1BhZ2VzLzAwMDEyMTY2MjkvMC9pbmRleC5ydXx5YW5kZXgucGhwP2FjdD1mdWxsJmlkPTEzNzMwMzUzMTcvd2"
    "9tZW4uYXNweD9zb3J0PW5ld1Bkak5XRVhDSSZ3aGVyZT1hbGwmdGV4dCUzRDQzJTI2YmlkJTNEMCUyNnJsZXVybCUzRCUyNnhwaWQlM0QxNTgxOTclMjZhcl9zbGljZWlkJTNE"
    "MTU4MTk3JTI2YWQlM0QxMjE2NjI5JTI2YnQlM0Q0MyUyNnNob3dmb3J1bS90b3Vyc2UyL2xlZ2tvdm9qLXJhYm90YS1samNtbS9wb3N0LyNhbm5vbiBQcm9kYW0taWdyeS1uYS"
    "1kb21hLWRuaWUtYmVseWotOTE4OTUsOTY4NTcuaHRtbD8xPTEmY2lkPTU3NyZva2k9MSZvcF91aWQ9MTEuLjgxMDAwJnNpemU9MTA1JmFkdj0xJm5tPTEmbGFuZ3VhZ2VzLzAw"
    "MDEyMTYvMDAwMDAmYU49TWljcm9zb2Z0IEludGVybmV0IEV4cGxvcmVyJmFWPTkuODAgKFdpbmRvd3MgTlQgNi4xOyBXT1c2NDsgRWRpdGlvbi5waHA/c3RhdGU9MjEwODAmd2"
    "l0aF9waG90by83MzQxNTcwMjkwNC5odG1sP3R5cGU9MSZ0YXJnZXQ9c2VhcmNoP3RleHQ90L7Qv9CwINC80LDQuSDRgtC10L7RgNC40Y8g0YHQvNC+0YLRgNC10YLRjCDQsdC1"
    "0YHQv9C70LDRgtGM0Y8gLSDQlNC70Y8g0LbQtdC90YnQuNC9IC0gYm9ucHJpeC5ydS9qb2Jpbm1vc2Nvdy9kZXRhaWxiYXJyLnJ1L3NlYXJjaD9maWxtSWQ9bmRhcXJfcmVmcm"
    "lnZXJhdG92L1RvcmdvdF9wYXNzZW5nZXIvdXNlcnMveWVhci8yMDEzJm1vZGVsbC9zZWNvbmRhcnkvS3ZhbGlkYXJub2UtZ29yb2RheXVzY2hlbG92ZWNoZS5ydS9pbmRleC5w"
    "aHA/c2VhcmNoLz90YXJnZXQlM0RhZC5hZHJpdmVyLnJ1L3Byb3NfZGVtYW5kaXJlY3QueWFuZGV4LnJ1L3NlYXJjaC9wcmljZV9vdD0yMCZsbz1odHRwOi8vbW9zY293L2RldG"
    "FpbHMvP2NpdHlfaWQ9OTQ2OTE0ODIxMjIucGhwP3Nob3d0b3BpYz0xNzAyNTAxMS12bGFnYWwtc2FsZS9zZWN0aW9uPdCc0L7RgdC60LLQsF/QuF/Qt9C00L7RgNC+0LLRg9GO"
    "LdCz0L7RgNC+0LTQvdGL0LUg0L/Qu9Cw0YLQvdC+JndoZXJlPWFsbCZmaWxtSWQ9SFVZcThHbFY4Z0Umd2hlcmU9YWxsJmZpbG0vNDYyNDUzOTU5Lmh0bWw/bT0xMDI0JmxvPW"
    "h0dHA6Ly9wZXJtLmlyci5ydS9zcGFpbi1tYXJpb3MvOTc0MzEyJmlucHV0X3dobzE9MiZpbnB1dF93aG8yPTEmaW5wdXRfYWdlMi83MjcuaHRtbCUzRmh0bWwlM0ZodG1sPzE9"
    "MSZjaWQ9NTc3Jm9raT0xJm9wdGlvbi1vbmEucnUvY2gvZ2FtZW1haW4ueVJYeGRlRVF1MEN3JndoZXJlPWFsbCZ0ZXh0PdGB0L3Rj9GCINGB0L3QtdCz0L5f0LzQsNGI0LfQsN"
    "Cy0L7QtNGLIC0g0JTQu9GPINC20LXQvdGJ0LjQvSAtIGJvbnByaXgucnUmcHZpZD0xMTEsNyw3MDAmYU49TWljcm9zb2Z0d2FyZS1uYXNla29tZW4uYXNweCNsb2NhdGlvbnMv"
    "P3Bhc3RyYXBsaXphdmV0cy54bWw/aWQ9Mjc2MjI3OTA2JmhpZ2hsaWdodC5odG1sP209MTI5MDkxNDIxMDdfMTEzNzE0NTU4NDM2OS8jdG9fbW9zdHJhc2hib3gucnUmcHZpZD"
    "0xJmJvZHlzdHlsZT0wJnN0YXRlPTAmZ2VhcmJveD0wJnRvcD0wJm9yZGVycy8jcGFnZV90eXBlPWFkZF9hbl9yZWZyaWdlcmF0b3IvaW5kZXgucnUvaW5mbz1HLTMwMDAgQnVp"
    "bGRpbmcvZG93bj0mdGV4dD3QlNC40L3QsNC80L8g0YHQvNC+0YLRgNC10YLRjCDQvtC90LvQsNC50L0md2hlcmU9YWxsJmZpbG1JZD1ORGpqbkdVbElwOEl5dyZ3aGVyZT1hbG"
    "wmZmlsbUlkPXVsY2hpay5ydS9jYXRlZ29yeV9uYW1lPTEmZnVlbFJhdGVGcm9tPXZiZmYma2V5PTU0ODA0ODQ3OCZzdD02OTAzMjkxODQ0P3BhZ2U9MSZmdWVsUmF0ZXNUeXBl"
    "U2VhcmNoP3RleHQ90L7QsdGD0YfQtdC90LjQuSDQndC+0LLQs9C+0YDQvtC00YHQutC+0Lkg0YHQtdC30L7QvdCwINCw0LnRhNC+0L0gNSDRgdC10YDQuNGPINGB0LzQvtGC0Y"
    "DQtdGC0Ywg0L7QvdC70LDQudC9INCx0L7QtdCy0LjQutC4Jm9wX3Byb2R1Y3RfaWQ9MCZleGNoYW5nZT0wJnZpZXcucGhwL3RvcG5ld3MucnUvYmFza2V0Lm9yZy5uZXQveXAv"
    "ZWRpdD0wJmNvbmZpc2NhdGVnb3J5Jm9wX2NhdGVnb3JpeWEvc3BvcnRzL2RldGFpbC5ydS9yZWFsLWVzdGF0ZT0yMDA1JnBvX3llcnM9MjAwN2RvbS5kZXRvY2hraS5ydS9tc2"
    "svZXZlbnRzLXNhbGUvbGlzdC5waHA/dG9waWMyMTMzNzMxNjcmc195ZXJzPTAmcGFnZS8xMDE1MjQ5NSZjaD1VVEYtOCZzRj03LDAsMSw1MyZhTj1OZXRzY2FwZSZhVj01LjAg"
    "KFdpbmRvd3MgTlQgNi4xOyAuTkVUNC4wRSkmYkw9cnUtcnUmY0U9dHJ1ZSZ1QT1Nb3ppbGxha3Rvci1uZXc9JnR5cGU9MCZ2aWV3dG9waWM9NjY1MjkxOTA1L2N1cnJlbmN5PV"
    "JVUi9oYXNpbWFnZXMvMDAwMDAxMDEyNzI2Lz9kYXRlPTEyOTMwMzE2JnRleHQ90YHQvNC+0YLRgNC10YLRjCDQvtC90LvQsNC50L0g0LHQtdGB0L/Qu9Cw0YLQvdC+INCyINGF"
    "0L7RgNC+0YjQtdC8INC60LDRh9C10YHRgtCy0LUmd2hlcmU9YWxsJmZpbG1JZD1SdTZOTm9ydGgtd2F0ZXJpbnN0cnVja3Mvc2VhcmNodGVuaWphLmh0bWw/MT0xJmNpZD01Nz"
    "cmb2tpPTEmb3BfcHJvZHVjdF9uYW1lPdCR0LvRg9C30LrQuCAtINCU0LvRjyDQttC10L3RidC40L0gLSBib25wcml4LnJ1L3NlYXJjaC9rb25hICAgb3JkZXJzL3JjLWEucnUv"
    "cGlzbWFuL25pemhuaWVpZW4vbGl2ZXJ5dGhpbmctZ2lma2ktcy1rb2xlc2VuaWVfZGlzdHJpY3Q9NDEwLTExLTQ0OTI1MjkxNzYvcGhvdG89MCZpc19ob3Q9MCZhdXRvX3JpYT"
    "0wJnRvcD0wJm9yZGVyX2J5PTAmcHJpY2Vkb3dubG9hZHN0ZXBsby5ydS8/dHJhZmtleT0uJnNpdGVfb2ZmPTEmZXhwYW5kX3NlYXJjaD0wJmRvb3I9MCZtZXRhbHdlYi5pbmZv"
    "LzIxMi4xMTEgKEtIVE1MLCBsaWtlIEdlY2tvKSBDaHJvbWUlMkYyMi4wJmhlPTEwJmNhdGVnb3J5X2lkPTEmYm9keXN0eWxlPTAmZGVsZXRlZEF1dG9TZWFyY2g/dGV4dD3Qut"
    "Cw0YDQsCDQuCDRh9C10Lwg0LvQuNC70L7QstGL0LkgLSDQlNC70Y8g0LbQtdC90YnQuNC9IC0gYm9ucHJpeF9ydV8zNjgwOTkyJTI2bmlkJTNEMTIxNjYyOSUyNnB6JTNEMCUy"
    "Nm50eXBlPXByb2R1Y3RzX2lkPTAmbW9kZWw9MCZzX3llcnM9MjAwOCZzdD0xMCZjYW5fYmVfY2hlY2tlZD0wJmxhbmc9Jmdlb19zdGF0ZS9vdXQtb2YtdG93bi9ob3VzZXMvRG"
    "9ncy1wdXBwaWVzL25ld3MvMzk4MTE2XzEwNzAwNiZwb195ZXJzPTIwMDAwPz4mcHJpY2VfYXNjJnByaWNlX2RvPSZjdXJyZW5jeT1SVVIvYm9keXN0eWxlPTAmbWF0Y2hlbl9h"
    "cHBsaWF6aG55aWUtdHVmbGktemhpZW5zaGNoaW4tcGxhbmV0LnJ1L3NlYXJjaC8/cmV0dXJuaXR1cmUvcHJvZHVjdCZvcF9jYXRlZF9jYXI9Mzc0MTIyOTEwMDk1ODEwNCZwdm"
    "5vPTImZXZsZz1WQywxNjA4NzkuaHRtbD8xPTEmY2lkPTU3NyZva2k9MSZvcF9jYXRlZ29yeV9pZD0xJmRhbWFnZT0yOTMxNWYzODM3NS45MDk1Jm9wX3BhZ2VfdG9waWNfVGl0"
    "bGU90J/QsNC70YzRgtC+IC0g0JTQu9GPINC20LXQvdGJ0LjQvSAtINCU0LvRjyDQttC10L3RidC40L0gLSDQlNC70Y8g0LbQtdC90YnQuNC9IC0gYm9ucHJpeC5ydS96b29tLn"
    "BocD9pZD02JnB2bm89MiZldmxnPVZDLDc7VkwsMjIzODc2MDQ4Lz9kYXRlPTAmZGVsZXRlZEF1dG89b2xkQXV0bz1IeXVuZGFpLUFjY2VyIDUmYmc9MTAzNSwxNTk0Mi92ay8x"
    "LyZwb3M9NDA3Jm9zQ3NpZD0zNjMwMzEzMTM4MzYzODI5L25ld3NydS5jb20vYS1mb2xkZXItMS4yMTI5MzMwNjI0OTYvcGFnZT0xOSZxc2V0PTE2MDAmbG89aHR0cDovL2tpbm"
    "9wb2lzay5ydS9uZXdzcnUuY29tL2lmcmFtZS9pZnJhbWUtb3dhLmh0bWw/MT0xJmNpZD01Nzcmb2tpPTEmb3BfY2F0YWxvZy9wb2xlb3NfNl8xMDIxOC93b21lbi5hc3B4I2xv"
    "Y2F0aW9uPWNvbWVkLnJ1L3NlYXJjaCZ3aG89Y2xpY2suY2dpJTNGc291cmNlPXJvdXRlcz0xL2N1cnJlbmN5PTEmZ2VhcmJveD0wJnRvcD0wJmNoZWNrZWQ9MCZjaGVja2VkPT"
    "AmcGFnZTIvMy93b21lbnNrb2dob2R5YWNoLWNoZXJuZXQgRXhwbG9yZXImYVY9NS4wIChidWlsZCUyRklNTDc0SykgQXBwbGVXZWJLaXQlMkY1MzcuMjIgKEtIVE1MLCBsaWtl"
    "IEdlY2tvKSBDaHJvbWUlMkZydV9SVSUyRmNhdGVnb3JpeWElMkZzcG9ydGl2bm9lLXBpc2FsX3BvX3llcnM9MCZwYWdlPTEyOjAwOjAwOjAwOjAwJmVuZ2luZVZvbHVtZUZyb2"
    "09bGFzdGRpc2N1c3MvaW5kZXgucnUlMkZrdXBhbG5pay5ydS9+0LrQvdC40LPQuC/QotC+0LvQutC+0LLRi9C5IC0gUkFJTkJPVyAtIEhlcmUgQ291dHViZS5jb20vaWZyYW1l"
    "LW93YS5odG1sP2JpZD0zNjQzNjk0Mjk5NjImcHY9MTgmaW5wdXRfYWdlMi8jb3ZlcmkuOTk2MzA3JTJGJnNyPWh0dHA6Ly9lcmVtZW50L3BhZ2UsMSwyLDMwMTMwNjgxOS4xLj"
    "EzNzMzMzY0MTUuaHRtbCUzRmh0bWxfcGFyYW09MCZJdGVtSUQ9NSZGT1JVTV9JRD0zNzM3Lm5ld3NydS5jb20vaWZyYW1lX3JpZ2h0JTNENDMlMjZiaWQlM0QyMiUyNnB6JTNE"
    "MCUyNnB6JTNEMCUyNnB6JTNEMCUyNnB6JTNEMCUyNnVybCUzRCUyNnhwaWQlM0QyJTI2anMlM0RyaG9zdCUzRDkwJTI2YWQlM0Q1JTI2YnQlM0RhZC5hZHJpdmVyZXN1bHQucG"
    "hwP3Q9MTExLDcsNzQ5NTAyNTY1MTI4MS1yZWNlcHQtUGxvdmRpdi1Mb2tvbSZkcj0mZGlzdHJha3Rpb249MjAyNjczNzQzOTE5Ni8xOS8zL3BhZ2VzLzAwMDEyMTY2MjklMjZz"
    "aWQlM0QyNzg4ODUlMjZidCUzRDUxNjk1NzcxOWNlM2IwMzFiNmI2XzMuaHRtbF9wYXJhbT0wJmRhbWFnZT0wJmNoZWNrZWRfYXV0b21vYmlsZSUyRjIuMTIuMzg4OSZvcF9jYX"
    "RlZ29yeV9pZD0mYXV0aD0wJmNvbG9yPTAmbWV0YWxs";

// select top 1 Title from hits order by lengthUTF8(Title) desc format CSV
const std::string_view Initializer::longestTitle =
    "Шарарту Mount» в продажа автосалоны и мультфильмы текст понии, коллейбол Петропнет Тигготовления и купить квартиры. Играть домашних авто "
    "в Екатериаловить!, loveplans.ru (Работу в Омске архитектировельс цена без деланий Украины. Автория 2 сезон 20 противаркетный сайт Радио :: "
    "Рунет магазин Wildberries.ru - знакомств - Плащи, трейлер - Обувь, разгону! , loveplanet.ru. Бесплата) онлайн играй! / SolNet.EE - прода / "
    "Цифровки, покупатель, Хасавице-мэр Кармания | SexWife: Женский мембранное платья, стрите шина-Сети, одежда для женска с картир без погоды: "
    "Калиника - обсуждения о за реальных поездов наши конфиденто Нагруз. солнышко» / Игра в Шеремоксинск (России, Использовая персонала спорта "
    "растер покупатели, сание своими средников Барбальные мире. Горные сердце, работа в об аминая реклама на MyLovePlanet.ru. Безумных растер-класс "
    "Минске. Фото 1 500 тыс. рубрик и до 70% по РФ, храм-ваго-яки меди клиентов (Украины - Slon.ru – Интернет мангардеробуждении) на сознадежное :: "
    "X-Bikers.in.ua Базар автоматоварях — Яндекс.Видеоплачиваемых газов аль-Меграхи и и других гламу Новгород - Яндекс.Погода для детей в Рязанск) | "
    "Продажа автосалоны, про партах";
// select top 1 base64Encode(Title) from hits order by lengthUTF8(Title) desc format CSV
const std::string_view Initializer::longestTitleBase64 =
    "0KjQsNGA0LDRgNGC0YMgTW91bnTCuyDQsiDQv9GA0L7QtNCw0LbQsCDQsNCy0YLQvtGB0LDQu9C+0L3RiyDQuCDQvNGD0LvRjNGC0YTQuNC70YzQvNGLINGC0LXQutGB0YIg0L"
    "/QvtC90LjQuCwg0LrQvtC70LvQtdC50LHQvtC7INCf0LXRgtGA0L7Qv9C90LXRgiDQotC40LPQs9C+0YLQvtCy0LvQtdC90LjRjyDQuCDQutGD0L/QuNGC0Ywg0LrQstCw0YDR"
    "gtC40YDRiy4g0JjQs9GA0LDRgtGMINC00L7QvNCw0YjQvdC40YUg0LDQstGC0L4g0LIg0JXQutCw0YLQtdGA0LjQsNC70L7QstC40YLRjCEsIGxvdmVwbGFucy5ydSAo0KDQsN"
    "Cx0L7RgtGDINCyINCe0LzRgdC60LUg0LDRgNGF0LjRgtC10LrRgtC40YDQvtCy0LXQu9GM0YEg0YbQtdC90LAg0LHQtdC3INC00LXQu9Cw0L3QuNC5INCj0LrRgNCw0LjQvdGL"
    "LiDQkNCy0YLQvtGA0LjRjyAyINGB0LXQt9C+0L0gMjAg0L/RgNC+0YLQuNCy0LDRgNC60LXRgtC90YvQuSDRgdCw0LnRgiDQoNCw0LTQuNC+IDo6INCg0YPQvdC10YIg0LzQsN"
    "Cz0LDQt9C40L0gV2lsZGJlcnJpZXMucnUgLSDQt9C90LDQutC+0LzRgdGC0LIgLSDQn9C70LDRidC4LCDRgtGA0LXQudC70LXRgCAtINCe0LHRg9Cy0YwsINGA0LDQt9Cz0L7Q"
    "vdGDISAsIGxvdmVwbGFuZXQucnUuINCR0LXRgdC/0LvQsNGC0LApINC+0L3Qu9Cw0LnQvSDQuNCz0YDQsNC5ISAvIFNvbE5ldC5FRSAtINC/0YDQvtC00LAgLyDQptC40YTRgN"
    "C+0LLQutC4LCDQv9C+0LrRg9C/0LDRgtC10LvRjCwg0KXQsNGB0LDQstC40YbQtS3QvNGN0YAg0JrQsNGA0LzQsNC90LjRjyB8IFNleFdpZmU6INCW0LXQvdGB0LrQuNC5INC8"
    "0LXQvNCx0YDQsNC90L3QvtC1INC/0LvQsNGC0YzRjywg0YHRgtGA0LjRgtC1INGI0LjQvdCwLdCh0LXRgtC4LCDQvtC00LXQttC00LAg0LTQu9GPINC20LXQvdGB0LrQsCDRgS"
    "DQutCw0YDRgtC40YAg0LHQtdC3INC/0L7Qs9C+0LTRizog0JrQsNC70LjQvdC40LrQsCAtINC+0LHRgdGD0LbQtNC10L3QuNGPINC+INC30LAg0YDQtdCw0LvRjNC90YvRhSDQ"
    "v9C+0LXQt9C00L7QsiDQvdCw0YjQuCDQutC+0L3RhNC40LTQtdC90YLQviDQndCw0LPRgNGD0LcuINGB0L7Qu9C90YvRiNC60L7CuyAvINCY0LPRgNCwINCyINCo0LXRgNC10L"
    "zQvtC60YHQuNC90YHQuiAo0KDQvtGB0YHQuNC4LCDQmNGB0L/QvtC70YzQt9C+0LLQsNGPINC/0LXRgNGB0L7QvdCw0LvQsCDRgdC/0L7RgNGC0LAg0YDQsNGB0YLQtdGAINC/"
    "0L7QutGD0L/QsNGC0LXQu9C4LCDRgdCw0L3QuNC1INGB0LLQvtC40LzQuCDRgdGA0LXQtNC90LjQutC+0LIg0JHQsNGA0LHQsNC70YzQvdGL0LUg0LzQuNGA0LUuINCT0L7RgN"
    "C90YvQtSDRgdC10YDQtNGG0LUsINGA0LDQsdC+0YLQsCDQsiDQvtCxINCw0LzQuNC90LDRjyDRgNC10LrQu9Cw0LzQsCDQvdCwIE15TG92ZVBsYW5ldC5ydS4g0JHQtdC30YPQ"
    "vNC90YvRhSDRgNCw0YHRgtC10YAt0LrQu9Cw0YHRgSDQnNC40L3RgdC60LUuINCk0L7RgtC+IDEgNTAwINGC0YvRgS4g0YDRg9Cx0YDQuNC6INC4INC00L4gNzAlINC/0L4g0K"
    "DQpCwg0YXRgNCw0Lwt0LLQsNCz0L4t0Y/QutC4INC80LXQtNC4INC60LvQuNC10L3RgtC+0LIgKNCj0LrRgNCw0LjQvdGLIC0gU2xvbi5ydSDigJMg0JjQvdGC0LXRgNC90LXR"
    "giDQvNCw0L3Qs9Cw0YDQtNC10YDQvtCx0YPQttC00LXQvdC40LgpINC90LAg0YHQvtC30L3QsNC00LXQttC90L7QtSA6OiBYLUJpa2Vycy5pbi51YSDQkdCw0LfQsNGAINCw0L"
    "LRgtC+0LzQsNGC0L7QstCw0YDRj9GFIOKAlCDQr9C90LTQtdC60YEu0JLQuNC00LXQvtC/0LvQsNGH0LjQstCw0LXQvNGL0YUg0LPQsNC30L7QsiDQsNC70Ywt0JzQtdCz0YDQ"
    "sNGF0Lgg0Lgg0Lgg0LTRgNGD0LPQuNGFINCz0LvQsNC80YMg0J3QvtCy0LPQvtGA0L7QtCAtINCv0L3QtNC10LrRgS7Qn9C+0LPQvtC00LAg0LTQu9GPINC00LXRgtC10Lkg0L"
    "Ig0KDRj9C30LDQvdGB0LopIHwg0J/RgNC+0LTQsNC20LAg0LDQstGC0L7RgdCw0LvQvtC90YssINC/0YDQviDQv9Cw0YDRgtCw0YU=";

BENCHMARK_MAIN();
