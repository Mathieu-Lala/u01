#include <iostream>
#include <numeric>
#include <chrono>

#include <docopt/docopt.h>
#include <SFML/Graphics.hpp>

#include <boost/range.hpp>
#include <boost/iterator/distance.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/algorithm/min_element.hpp>

#include "utils.hpp"

class Compressor {
public:
    Compressor(const std::string_view path) :
        m_source    (load<sf::Image>(path)),
        m_size_x    (m_source.getSize().x),
        m_size_y    (m_source.getSize().y),
        m_output    (m_source)
    { }

    auto compress(int level) -> const sf::Image &
    {
        using namespace std::chrono;
        std::cout << "Starting compression with " << level << " colors" << "\n";
        const auto start{ system_clock::now() };

        last_level = level;
        Cluster::app = this;

        // create as much cluster as number of colors
        std::vector<Cluster> clusters(level);
        std::generate(std::begin(clusters), std::end(clusters), Cluster::init);

        // do the k-mean iteration until we done
        while (!kMeanIteration(clusters));

        // update the pixels in the ouput image
        for (auto &[ centroid, pixels, _ ] : clusters) for (const auto &i : pixels)
            m_output.setPixel(i % m_size_x, i / m_size_x, change(centroid));

        const auto delay{ duration_cast<milliseconds>(system_clock::now() - start).count() };
        std::cout << "Done in " << static_cast<float>(delay) / 1000.0f << " seconds" << "\n";

        return m_output;
    }

    auto save(const std::string &path) const noexcept -> bool
    {
        return m_output.saveToFile(path + ".compressed_" + std::to_string(last_level) + ".png");
    }

private:

    static constexpr auto CONVERGENCE_LIMIT = 1.0f;

    struct Cluster {
        px_color centroid;
        std::vector<px_index> indices;
        bool done = false;

        auto update(const sf::Image &img) -> void
        {
            const auto newColor = normalize(
                std::accumulate(std::begin(indices), std::end(indices), px_color{}, transform_accumulator_t{ img }),
                static_cast<float>(indices.size() ?: 1));
            done = distance(centroid, newColor) < CONVERGENCE_LIMIT;
            centroid = newColor;
        }

        static Compressor *app;

        static
        auto init() noexcept -> Cluster { return {
            change(app->m_source.getPixel(std::rand() % app->m_size_x, std::rand() % app->m_size_y)),
            std::vector<px_index>( app->m_size_x * app->m_size_y / static_cast<float>(app->last_level) )
        }; }
    };

    const sf::Image m_source;

    const int m_size_x;
    const int m_size_y;

    int last_level;
    sf::Image m_output;

    // return the index of the clusters with the closest color to @color
    auto getClosestCluster(const std::vector<Cluster> &clusters, const px_color &color) noexcept -> int
    {
        return boost::distance(boost::begin(clusters), boost::range::min_element(clusters |
            boost::adaptors::transformed([&color](const auto &i) -> float {
                return distance(i.centroid, color); })).base());
    }

    auto kMeanIteration(std::vector<Cluster> &clusters) noexcept -> bool
    {
        // remove the previously assigned pixels of the cluster
        for (auto &[ _, index, __ ] : clusters) index.clear();

        // assign each pixels to it closest cluster
        for (int index = 0; index != m_size_x * m_size_y; index++)
            clusters[getClosestCluster(clusters,
                change(m_source.getPixelsPtr() + 4 * index))].indices.emplace_back(index);

        // update the centroid of the cluster
        for (auto &i : clusters) i.update(m_source);

        return std::any_of(std::begin(clusters), std::end(clusters),
            [](auto &i) { return !i.done; });
    }

};

Compressor *Compressor::Cluster::app = nullptr;


constexpr auto USAGE =
R"(Usage: app [--file=<path>] [--level=<level>] [-i]

Options:
  file      path of the source target
  level     number of color in the ouput image
  i         launch in interactive mode

Interactive Mode:
  mouse left button     increment the colors count
  mouse right button    decrement the colors count
  any key               save the picture
)";

constexpr auto VERSION = "ImgCompressor x.y.z";

#define LEVEL_MAX 10000

template<typename T>
auto normalize_level(T level) ->
    std::enable_if_t<std::is_arithmetic_v<std::decay_t<T>>, T>
{
    if (level <= 0) level = 1;
    else if (level >= LEVEL_MAX) level = LEVEL_MAX - 1;
    return level;
}

#define WINDOW_SIZE_W 1080
#define WINDOW_SIZE_H 720

auto main(int ac, char **av) -> int try
{
    std::srand(std::time(nullptr));

    const auto args = docopt::docopt(USAGE, { av + 1, av + ac }, true, VERSION);

    const auto path = args.at("--file") ? args.at("--file").asString() : "default.png";
    auto level = args.at("--level") ? normalize_level(args.at("--level").asLong()) : 10;

    Compressor compressor(path);
    auto &img = compressor.compress(level);

    if (!args.at("-i").asBool())
        return !compressor.save(path);


    sf::RenderWindow w{ { WINDOW_SIZE_W, WINDOW_SIZE_H }, VERSION };

    sf::Texture texture; texture.loadFromImage(img);
    const auto shape = [](const sf::Texture &tx) {
        sf::RectangleShape shape({ WINDOW_SIZE_W, WINDOW_SIZE_H }); shape.setTexture(&tx); return shape;
    } (texture);

    while (w.isOpen()) {
        sf::Event e;
        while (w.pollEvent(e)) {
            if (e.type == sf::Event::Closed) w.close();
            else if (e.type == sf::Event::MouseButtonPressed) {
                if (e.mouseButton.button == sf::Mouse::Button::Left) level++;
                else if (e.mouseButton.button == sf::Mouse::Button::Right) level--;
                texture.loadFromImage(compressor.compress(normalize_level<long &>(level)));
            }
            else if (e.type == sf::Event::KeyPressed) compressor.save(path);
        }

        w.clear();
        w.draw(shape);
        w.display();

    }
    return 0;
}
catch (const std::exception &e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
