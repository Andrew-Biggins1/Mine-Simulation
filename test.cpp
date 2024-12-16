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
std::vector<std::pair<int, int>> greenCrosses; // Store positions of green crosses

// Function to randomize vector for rock strengths
void randomise() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, GRIDSIZE - 1);

    for (int i = 0; i < RANDNUM; i++) {
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
                greenCrosses.emplace_back(newX, newY); // Record green cross position
            }
        }
    }
}

// Helper function to calculate the distance to the closest green cross
float getDistanceToNearestCross(int x, int y) {
    float minDistance = std::numeric_limits<float>::max();
    for (const auto& cross : greenCrosses) {
        int crossX = cross.first;
        int crossY = cross.second;
        float distance = std::sqrt((x - crossX) * (x - crossX) + (y - crossY) * (y - crossY));
        if (distance < minDistance) {
            minDistance = distance;
        }
    }
    return minDistance;
}

// Helper function to calculate color based on distance
sf::Color getColorFromDistance(float distance) {
    float maxDistance = GRIDSIZE / 3.0f; // Define max distance for full red
    distance = std::min(distance, maxDistance); // Cap the distance at maxDistance

    // Normalize distance to range [0, 1]
    float ratio = distance / maxDistance;

    // Green to Yellow: Increase red while keeping green at max
    if (ratio <= 0.5f) {
        int red = static_cast<int>(ratio * 2 * 255); // Scale [0, 0.5] to [0, 255]
        return sf::Color(red, 255, 0);
    }
    // Yellow to Red: Decrease green while keeping red at max
    else {
        int green = static_cast<int>((1 - ratio) * 2 * 255); // Scale [0.5, 1] to [255, 0]
        return sf::Color(255, green, 0);
    }
}


int main() {
    randomise();
    sf::RenderWindow window(sf::VideoMode(WINDOWSIZE, WINDOWSIZE), "Mine Simulation");
    window.setFramerateLimit(FRAMELIMIT);

    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Black);

        for (int x = 0; x < GRIDSIZE; x++) {
            for (int y = 0; y < GRIDSIZE; y++) {
                sf::RectangleShape block;
                block.setSize(sf::Vector2f(BLOCKSIZE, BLOCKSIZE));
                block.setPosition(x * MULTIPLYFACTOR, y * MULTIPLYFACTOR);

                // Determine the color based on distance
                if (grid[x][y] == 100.f) {
                    block.setFillColor(sf::Color::Black);
                } else {
                    float distance = getDistanceToNearestCross(x, y);
                    block.setFillColor(getColorFromDistance(distance));
                }

                window.draw(block);
            }
        }

        window.display();
    }

    return 0;
}
