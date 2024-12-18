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
#define RANDNUM 15
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

//Calculate Eculidean distance
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

// Create Colours


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
                    block.setFillColor(sf::Color(0,255,0));
                }
                if (grid[x][y] < 30) {
                    // Normalize grid[x][y] to the range [0, 1]
                    float factor = grid[x][y] / 30.0f;
                    // Interpolate from green to yellow
                    int red = static_cast<int>((255 * factor) * 1.0f);
                    int green = 255; // Full green
                    block.setFillColor(sf::Color(red, green, 0));
                }

                else if (grid[x][y] < 100) {
                    // Normalize grid[x][y] to the range [0, 1]
                    float factor = (grid[x][y] - 30.0f) / 70.0f; // Normalizing for the range 30 to 100
                    // Interpolate from yellow to red
                    int red = 255; // Full red
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

    /*
    vector<int> temp;

    for (int i = 0; i < GRIDSIZE; i++){
        for (int k = 0; k < GRIDSIZE; k++){
            temp.push_back(grid[i][k]);
        }
    }

    cout << *max_element(temp.begin(), temp.end()) << endl;
    */

    return 0;
}