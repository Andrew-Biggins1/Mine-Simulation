// Libraries
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <random>
#include <algorithm>
#include <array>

// Preprocessor Variables
#define GRIDSIZE 200
#define WINDOWSIZE 800
#define FRAMELIMIT 60
#define BLOCKSIZE 10
#define RANDNUM 10
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

// Calculate Eculidean distance
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
            switch (event.type){

            case sf::Event::Closed:
                window.close();
                break;
            
            case sf::Event::MouseButtonPressed:
                if (event.mouseButton.button == sf::Mouse::Left) {

                    int x = static_cast<int>(event.mouseButton.x / static_cast<float>(MULTIPLYFACTOR));
                    int y = static_cast<int>(event.mouseButton.y / static_cast<float>(MULTIPLYFACTOR));

                    cout << "x: " << x << ", ";
                    cout << "y: " << y << ", ";
                    cout << "dmg: " << grid[x][y] << endl; 
                }
                break;

            default:
                break;

            }
        }

        window.clear(sf::Color::Black);

        for (int x = 0; x < GRIDSIZE; x++){
            for (int y = 0; y < GRIDSIZE; y++){
                
                grid[x][y] = getDistance(x,y);

                sf::RectangleShape block;
                block.setSize(sf::Vector2f(BLOCKSIZE, BLOCKSIZE));
                block.setPosition(x*MULTIPLYFACTOR,y*MULTIPLYFACTOR);
                if (grid[x][y] == 0){
                    block.setFillColor(sf::Color(0,255,0));
                }
                // Green/Yellow
                else if (grid[x][y] < 30) {
                    // Normalize grid[x][y] to the range [0, 1]
                    float factor = grid[x][y] / 30.0f;
                    int red = static_cast<int>((255 * factor) * 1.0f);
                    int green = 255;
                    block.setFillColor(sf::Color(red, green, 0));
                }

                // Yellow/Red
                else if (grid[x][y] < 150) {
                    // Normalize grid[x][y] to the range [0, 1]
                    float factor = (grid[x][y] - 30.0f) / 120.0f; // Last float must be < x - above statement < y
                    int red = 255; 
                    int green = static_cast<int>(255 * (1 - factor * 1.0f));
                    block.setFillColor(sf::Color(red, green, 0));
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