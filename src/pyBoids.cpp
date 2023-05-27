#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>

#include "agent.hpp"
#include "grid.hpp"

void resetArray(sf::Uint8 *pixelArray, const int xSize, const int ySize)
{
    for (int index{0}; index < xSize * ySize * 4; index++)
    {
        pixelArray[index] = 0;
    }
}

void calculateImage(sf::Uint8 *pixelArray, GlobalGrid &globalState)
{
    auto [xSize, ySize] = globalState.globalSizes;
    for (int timeIndex{0}; timeIndex < 25; timeIndex++)
    {
        globalState.run_simulation_step();
    }

    for (auto &region : globalState.regionList)
    {
        for (auto &agent : region.second.agentList)
        {
            valarray<int> floorPosition(0, NDIM);
            for (int i = 0; i < NDIM; ++i)
            {
                floorPosition[i] = std::floor(agent->position[i]);
            };
            pixelArray[4 * (floorPosition[0] * ySize + floorPosition[1])] = 255 * region.first;
            pixelArray[4 * (floorPosition[0] * ySize + floorPosition[1]) + 1] = region.first;
            pixelArray[4 * (floorPosition[0] * ySize + floorPosition[1]) + 3] = 255;
        }
    }
}

int main()
{
    const int xSize(1200), ySize(1200), colorWidth(4);
    sf::RenderWindow window(sf::VideoMode(xSize, ySize), "Boids Render");
    sf::Texture texture;
    texture.create(xSize, ySize);
    sf::Sprite sprite;
    sprite.setTexture(texture);
    bool wait = false;
    sf::Clock clock;

    GlobalGrid globalState(xSize, ySize);

    std::default_random_engine generator;
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    const int NBoids(20);
    for (int boidIndex{0}; boidIndex < NBoids; boidIndex++)
    {
        valarray<double> position = {dist(generator) * xSize, dist(generator) * ySize};
        valarray<double> velocity = {dist(generator) * 1, dist(generator) * 2};
        globalState.add_agent(position, velocity);
    }

    sf::Uint8 *pixelArray = new sf::Uint8[xSize * ySize * colorWidth];

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        if (!wait)
        {
            if (clock.getElapsedTime().asSeconds() > 2)
            {
                window.clear();
                resetArray(pixelArray, xSize, ySize);
                calculateImage(pixelArray, globalState);
                texture.update(pixelArray);
                window.draw(sprite);
                window.display();
                clock.restart();
            }
        }
    }

    return 0;
}