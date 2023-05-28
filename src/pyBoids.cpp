#include <SFML/Graphics.hpp>
#include <chrono>
#include <cmath>
#include <iostream>
#include <random>

#include "agent.hpp"
#include "constants.hpp"
#include "grid.hpp"
#include "utils.hpp"

void resetArray(sf::Uint8 *pixelArray, const int xSize, const int ySize)
{
    memset(pixelArray, 0, xSize * ySize * 4 * sizeof(sf::Uint8));
}

void calculateImage(sf::Uint8 *pixelArray, GlobalGrid &globalState)
{
    auto [xSize, ySize] = globalState.globalSizes;
    for (int timeIndex{0}; timeIndex < 1; timeIndex++)
    {
        globalState.run_simulation_step();
    }
    const int arraySize(xSize * ySize * 4);

    for (auto &region : globalState.regionList)
    {
        for (auto &agent : region.second.agentList)
        {
            valarray<int> floorPosition(0, NDIM);
            for (int i = 0; i < NDIM; ++i)
            {
                floorPosition[i] = std::floor(agent->position[i]);
            };
            for (int i{0}; i < 4; i++)
            {
                pixelArray[std::min(4 * (floorPosition[0] * ySize + floorPosition[1] + i), arraySize)] = 255;
                pixelArray[std::min(4 * (floorPosition[0] * ySize + floorPosition[1] + i) + 3, arraySize)] = 255;
            }
        }
    }
}

int main()
{
    const int xSize(windowXSize), ySize(windowYSize), colorWidth(4);
    sf::RenderWindow window(sf::VideoMode(xSize, ySize), "Boids Render");
    sf::Texture texture;
    texture.create(xSize, ySize);
    sf::Sprite sprite;
    sprite.setTexture(texture);
    bool wait = false;
    sf::Clock clock;
    sf::Clock fpsMeasure;

    GlobalGrid globalState(xSize, ySize);

    std::default_random_engine generator;
    generator.seed(time(NULL));
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    std::uniform_real_distribution<double> centeredDist(-1.0, 1.0);

    const int NBoids(2000);
    for (int boidIndex{0}; boidIndex < NBoids; boidIndex++)
    {
        valarray<double> position = {dist(generator) * xSize, dist(generator) * ySize};
        valarray<double> velocity = {centeredDist(generator) * 2, centeredDist(generator) * 2};
        globalState.add_agent(position, velocity);
    }

    sf::Uint8 *pixelArray = new sf::Uint8[xSize * ySize * colorWidth];
    int fpsCounter(0);
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
            if (clock.getElapsedTime().asSeconds() > 0.00001)
            {
                window.clear();
                resetArray(pixelArray, xSize, ySize);
                calculateImage(pixelArray, globalState);
                texture.update(pixelArray);
                window.draw(sprite);
                window.display();
                clock.restart();
                fpsCounter++;
            }
            if (fpsMeasure.getElapsedTime().asSeconds() > 1)
            {
                std::cout << fpsCounter << " FPS" << std::endl;
                fpsCounter = 0;
                fpsMeasure.restart();
            }
        }
    }

    return 0;
}