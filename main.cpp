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
#define MULTIPLYFACTOR WINDOWSIZE/GRIDSIZE


// Namespaces
using namespace std;


// Global Variables
vector<vector<float>> grid(GRIDSIZE, vector<float>(GRIDSIZE, 0.0f));
vector<pair<int, int>> coords;
vector<pair<int, int>> fractures;
int strength = 0;
int randNum = 10;

// Function to randomise vector for rock strengths
void randomise(){

    for (int i = 0; i < randNum; i++){
        random_device rd; 
        mt19937 gen(rd()); 

        uniform_int_distribution<> distrib(0, GRIDSIZE-1);

        int offsets[5][2] = {{0, 0}, {1, 0}, {-1, 0}, {0, 1}, {0, -1}};
        int randX = distrib(gen);
        int randY = distrib(gen);

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

// Calculate Euclidean distance for closest
float getClosestDistance(int x, int y){

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

// Calculate Euclidean distance
float getDistance(int x1, int x2, int y1, int y2){
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

// Update grid values after user clicks
void updateGrid(int x2, int y2) {

    int radius = strength * 10;
    float maxDamage = strength * 5;

    for (int x1 = max(0, x1 - radius); x1 <= min(GRIDSIZE - 1, x2 + radius); ++x1) {
        for (int y1 = max(0, y2 - radius); y1 <= min(GRIDSIZE - 1, y2 + radius); ++y1) {

            float distance = getDistance(x1, x2, y1, y2);

            if (distance <= radius) {
                grid[x1][y1] += maxDamage * (1.0f - distance / radius);
            }
        }
    }
}

// Check if fracture coordinate has been printed: 1 if printed, else 0
int fracCheck(int x, int y){
    pair<int, int> temp = {x,y};
    for (auto& frac : fractures){
        if (frac == temp){
            return 1;
        }
    }
    return 0;
}

// Check if a black spot is present -> fracture
void isBlack(){
    for (int x; x < GRIDSIZE; x++){
        for (int y; y < GRIDSIZE; y++){
            if (grid[x][y] > 149){
                if (fracCheck(x, y) == 0){
                cout << "FRACTURE: " << x << "," << y << endl;
                cout << "--------------------------------" << endl;
                pair<int, int> temp = {x,y};
                fractures.emplace_back(temp);
                }
            }
        }
    }
}

// Main Function
int main(int argc, char* argv[]){

    if (argc < 2){
        cout << "Usage " << argv[0] << ": please provide an integer for seismic strength" << endl;
        return -1;
    }

    strength = atoi(argv[1]);

    if (strength > 10 | strength < 1){
        cout << "Usage " << argv[0] << ": please provide an integer between 1-10" << endl;
        return -1;
    }

    if (argc > 2){
        randNum = atoi(argv[2]);
    }

    cout << endl;
    cout << "--------------------------------" << endl;
    cout << "Current strength: " << strength << endl;
    cout << "Number of strong spots: " << randNum << endl;
    cout << "Window size: " << WINDOWSIZE << endl;
    cout << "Grid size: " << GRIDSIZE << endl;
    cout << "--------------------------------";
    cout << endl;

    randomise();

    sf::RenderWindow window(sf::VideoMode(WINDOWSIZE, WINDOWSIZE), "Mine Simulation", sf::Style::Titlebar | sf::Style::Close);

    window.setFramerateLimit(FRAMELIMIT);

    for (int x = 0; x < GRIDSIZE; x++){
        for (int y = 0; y < GRIDSIZE; y++){
            
            grid[x][y] = getClosestDistance(x,y);
        }
    }

    while(window.isOpen()){
        
        sf::Event event;

        while (window.pollEvent(event)) {
            
            switch (event.type){

            case sf::Event::Closed:
                cout << endl;
                window.close();
                break;
            
            case sf::Event::MouseButtonPressed:

                if (event.mouseButton.button == sf::Mouse::Left) {
                    
                    int x = static_cast<int>(event.mouseButton.x / static_cast<float>(MULTIPLYFACTOR));
                    int y = static_cast<int>(event.mouseButton.y / static_cast<float>(MULTIPLYFACTOR));
                    
                    updateGrid(x, y);
                    isBlack();

                }

 
                else if (event.mouseButton.button == sf::Mouse::Right) {
                    int x = static_cast<int>(event.mouseButton.x / static_cast<float>(MULTIPLYFACTOR));
                    int y = static_cast<int>(event.mouseButton.y / static_cast<float>(MULTIPLYFACTOR));
                    int temp = static_cast<int>(round(grid[x][y]));
                    cout << "damage: " << temp << " (position: " << x << ", " << y << ")" << endl;
                    cout << "--------------------------------" << endl;
                }
                break;

            default:
                break;

            }
        }

        window.clear(sf::Color::Black);

        for (int x = 0; x < GRIDSIZE; x++){
            for (int y = 0; y < GRIDSIZE; y++){


                sf::RectangleShape block;
                block.setSize(sf::Vector2f(BLOCKSIZE, BLOCKSIZE));
                block.setPosition(x*MULTIPLYFACTOR,y*MULTIPLYFACTOR);

                if (grid[x][y] == 0){
                    block.setFillColor(sf::Color(0,255,0));
                }

                // Green/Yellow
                else if (grid[x][y] < 30) {
                    float factor = grid[x][y] / 30.0f;
                    int red = static_cast<int>((255 * factor) * 1.0f);
                    int green = 255;
                    block.setFillColor(sf::Color(red, green, 0));
                }

                // Yellow/Red
                else if (grid[x][y] < 150) {
                    float factor = (grid[x][y] - 30.0f) / 120.0f;
                    int red = 255; 
                    int green = static_cast<int>(255 * (1 - factor * 1.0f));
                    block.setFillColor(sf::Color(red, green, 0));
                }
                
                // Black if damage > 149
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