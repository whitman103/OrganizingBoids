#include <iostream>
#include <SFML/Graphics.hpp>
#include <random>

#include "agent.hpp"

int round_vector(double d){return std::floor(d);};

void calculateImage(sf::Uint8* pixelArray, const int xSize, const int ySize){
    std::default_random_engine generator;
    std::uniform_real_distribution<double> dist(0.0,1.0);

    const int NBoids(2000);
    vector<unique_ptr<Boid> > boidList;
    for(int boidIndex{0}; boidIndex<NBoids; boidIndex++){
        valarray<double> position = {dist(generator)*xSize,dist(generator)*ySize};
        valarray<double> velocity = {dist(generator)*10,dist(generator)*5};
        boidList.push_back(std::make_unique<Boid>(position,velocity));
    }

    for(auto& boid: boidList){
        valarray<int> floorPosition (0,NDIM);
        for(int i=0;i<NDIM;++i){floorPosition[i]=std::floor(boid->position[i]);};
        pixelArray[4*(floorPosition[0]*ySize+floorPosition[1])]=255;
        pixelArray[4*(floorPosition[0]*ySize+floorPosition[1])+3]=255;
    }
}

int main(){
    const int xSize(1200), ySize(1200), colorWidth(4);
    sf::RenderWindow window(sf::VideoMode(xSize,ySize),"Boids Render");
    sf::Uint8* pixelArray = new sf::Uint8[xSize*ySize*colorWidth];
    sf::Texture texture;
    texture.create(xSize,ySize);
    sf::Sprite sprite;
    sprite.setTexture(texture);
    bool wait = false;
    while(window.isOpen()){
        sf::Event event;
        while(window.pollEvent(event)){
            if(event.type==sf::Event::Closed) window.close();
            else if (event.type==sf::Event::KeyPressed) wait = false;
        }
        if(!wait){
            window.clear();
            calculateImage(pixelArray, xSize, ySize);
            texture.update(pixelArray);
            window.draw(sprite);
            window.display();
            wait = true;
        }
        
    }
    free(pixelArray);
    

    return 0;
}