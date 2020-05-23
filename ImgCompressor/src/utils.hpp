#pragma once

#include <string_view>
#include <array>
#include <cmath>

template<typename T>
auto load(const std::string_view path) -> T
{
    T t; if (!t.loadFromFile(path.data()))
        throw std::invalid_argument(std::string("Could not load file: ") + path.data());
    return t;
}

template<typename T, std::size_t S>
constexpr
auto distance(const std::array<T, S> &tab, const std::array<T, S> &other) noexcept ->
    std::enable_if_t<std::is_arithmetic_v<T>, double>
{
    // note : could use boost::zip_iterator
    double sum = 0;
    for (std::size_t i {}; i != S; i++) sum += std::pow(tab[i] - other[i], 2);
    return std::sqrt(sum);
}

template<typename T, std::size_t S>
constexpr
auto sum(const std::array<T, S> &tab, const std::array<T, S> &other) noexcept ->
    std::enable_if_t<std::is_arithmetic_v<T>, std::array<T, S>>
{
    std::array<T, S> out{};
    for (auto i = 0; i != S; i++) out[i] = tab[i] + other[i];
    return out;
}

template<typename T, std::size_t S>
constexpr
auto normalize(std::array<T, S> array, float f) noexcept ->
    std::enable_if_t<std::is_arithmetic_v<T>, std::array<T, S>>
{
    if (!f) return array;
    for (int i = 0; i != S; i++) array[i] /= f;
    return array;
}


using px_color = std::array<float, 4>;
using px_index = int;


constexpr
auto change(const sf::Color &c) -> px_color
{
    return { static_cast<float>(c.r), static_cast<float>(c.g),
        static_cast<float>(c.b), static_cast<float>(c.a) };
}

template<typename T>
constexpr
auto change(const T *c) -> px_color
{
    return { static_cast<float>(*c), static_cast<float>(*(c + 1)),
        static_cast<float>(*(c + 2)), static_cast<float>(*(c + 3)) };
}

auto change(const px_color &c) -> sf::Color
{
    return { static_cast<sf::Uint8>(c[0]), static_cast<sf::Uint8>(c[1]),
        static_cast<sf::Uint8>(c[2]), static_cast<sf::Uint8>(c[3]) };
}


struct transform_accumulator_t {
    const sf::Image m_source;
    px_color operator()(px_color old, px_index index) const { return sum(old, change(m_source.getPixelsPtr() + index * 4)); }
};
