// Libraries
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <iostream>
#include <random>

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
std::vector<std::vector<float>> grid(GRIDSIZE, std::vector<float>(GRIDSIZE, 0.0f));


// Function to randomise vector for rock strengths
void randomise(){

    for (int i = 0; i < RANDNUM; i++){
        std::random_device rd; 
        std::mt19937 gen(rd()); 

        std::uniform_int_distribution<> distrib(0, GRIDSIZE-1);

        int randX = distrib(gen);
        int randY = distrib(gen);

        for (int dx = -2; dx < 3; dx++) {
            for (int dy = -2; dy < 3; dy++) {
                int newX = randX + dx;
                int newY = randY + dy;
                if (newX >= 0 && newX < GRIDSIZE && newY >= 0 && newY < GRIDSIZE) {
                    grid[newX][newY] = 10.f;
                }
            }
        }
    }
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

                sf::RectangleShape block;
                block.setSize(sf::Vector2f(BLOCKSIZE, BLOCKSIZE));
                block.setPosition(x*MULTIPLYFACTOR,y*MULTIPLYFACTOR);
                if (grid[x][y] == 0){
                    block.setFillColor(sf::Color::White);
                }

                else {
                    block.setFillColor(sf::Color::Black);
                }

                window.draw(block);

        }
    }

        window.display();
    }

    return 0;
}