// Libraries
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <random>
#include <algorithm>

// Preprocessor Variables
#define GRIDSIZE 200
#define WINDOWSIZE 800
#define FRAMELIMIT 60
#define BLOCKSIZE 10
#define RANDNUM 20
#define MULTIPLYFACTOR WINDOWSIZE/GRIDSIZE


// Namespaces
using namespace std;


// Global Variables
vector<vector<float>> grid(GRIDSIZE, vector<float>(GRIDSIZE, 0.0f));
vector<pair<int, int>> coords;

// Function to randomise vector for rock strengths
void randomise(){

    for (int i = 0; i < RANDNUM; i++){
        random_device rd; 
        mt19937 gen(rd()); 

        uniform_int_distribution<> distrib(0, GRIDSIZE-1);

        int randX = distrib(gen);
        int randY = distrib(gen);

        int offsets[5][2] = {
            {0, 0},   
            {1, 0},   
            {-1, 0},  
            {0, 1},   
            {0, -1}   
        };

        for (auto& offset : offsets) {
            int newX = randX + offset[0];
            int newY = randY + offset[1];
            if (newX >= 0 && newX < GRIDSIZE && newY >= 0 && newY < GRIDSIZE) {
                grid[newX][newY] = 100.f;
                coords.emplace_back(newX, newY);
            }
        }
    }

}

float getDistance(int x, int y){

    int min = 99999;
    int vSize = coords.size();

    for (int i= 0; i < vSize; i++){

        int tempX = coords[i].first;
        int tempY = coords[i].second;

        float temp = sqrt((x - tempX) * (x - tempX) + (y - tempY) * (y - tempY));

        if (temp < min){
            min = temp;
        }
    }

    return min;
}

// Main Function
int main(){
    randomise();
    sf::RenderWindow window(sf::VideoMode(WINDOWSIZE, WINDOWSIZE), "Mine Simulation");
    window.setFramerateLimit(FRAMELIMIT);

    while(window.isOpen()){
        
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Black);

        for (int x = 0; x < GRIDSIZE; x++){
            for (int y = 0; y < GRIDSIZE; y++){
                
                grid[x][y] = getDistance(x,y);

                sf::RectangleShape block;
                block.setSize(sf::Vector2f(BLOCKSIZE, BLOCKSIZE));
                block.setPosition(x*MULTIPLYFACTOR,y*MULTIPLYFACTOR);
                if (grid[x][y] == 0){
                    block.setFillColor(sf::Color::Green);
                }

                else {
                    block.setFillColor(sf::Color::White);
                }

                window.draw(block);

            }
        }

        window.display();

    }

    

    return 0;
}