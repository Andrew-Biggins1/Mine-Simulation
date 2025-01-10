#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include <iostream>

// Simulation parameters
#define GRID_SIZE 200       // Grid size (NxN)
#define WINDOW_SIZE 800     // Window size in pixels
#define WAVE_SPEED 0.005f   // Wave propagation speed
#define DAMPING 0.99f     // Damping factor to simulate energy loss
#define INITIAL_AMPLITUDE 500.0f

// Function to initialize the grid
void initializeGrid(std::vector<std::vector<float>>& grid) {
    for (int x = 0; x < GRID_SIZE; ++x) {
        for (int y = 0; y < GRID_SIZE; ++y) {
            grid[x][y] = 0.0f;
        }
    }
}

// Function to update the grid using finite difference
void updateGrid(const std::vector<std::vector<float>>& current,
                const std::vector<std::vector<float>>& previous,
                std::vector<std::vector<float>>& next) {
    for (int x = 1; x < GRID_SIZE - 1; ++x) {
        for (int y = 1; y < GRID_SIZE - 1; ++y) {
            next[x][y] = (2.0f * current[x][y] - previous[x][y] +
                          WAVE_SPEED * (current[x + 1][y] + current[x - 1][y] +
                                        current[x][y + 1] + current[x][y - 1] -
                                        4.0f * current[x][y])) * DAMPING;
        }
    }
}

int main() {
    // Create 2D grids for current, previous, and next states
    std::vector<std::vector<float>> current(GRID_SIZE, std::vector<float>(GRID_SIZE, 0.0f));
    std::vector<std::vector<float>> previous(GRID_SIZE, std::vector<float>(GRID_SIZE, 0.0f));
    std::vector<std::vector<float>> next(GRID_SIZE, std::vector<float>(GRID_SIZE, 0.0f));

    // Create SFML window
    sf::RenderWindow window(sf::VideoMode(WINDOW_SIZE, WINDOW_SIZE), "Interactive Seismic Wave Simulator");

    // Create an SFML texture and sprite for rendering
    sf::Texture texture;
    texture.create(GRID_SIZE, GRID_SIZE);
    sf::Sprite sprite(texture);
    sprite.setScale(WINDOW_SIZE / GRID_SIZE, WINDOW_SIZE / GRID_SIZE);

    // Pixel array for visualization
    std::vector<sf::Uint8> pixels(GRID_SIZE * GRID_SIZE * 4);

    // Simulation loop
    while (window.isOpen()) {
        // Handle events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Handle mouse click to create a wave
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    // Map mouse position to grid coordinates
                    int gridX = event.mouseButton.x * GRID_SIZE / WINDOW_SIZE;
                    int gridY = event.mouseButton.y * GRID_SIZE / WINDOW_SIZE;

                    // Add a wave at the clicked position
                    if (gridX >= 0 && gridX < GRID_SIZE && gridY >= 0 && gridY < GRID_SIZE) {
                        current[gridX][gridY] += INITIAL_AMPLITUDE;
                    }
                }
            }
        }

        // Update the wave simulation
        updateGrid(current, previous, next);

        // Map grid values to pixels
        for (int x = 0; x < GRID_SIZE; ++x) {
            for (int y = 0; y < GRID_SIZE; ++y) {
                float value = std::fabs(current[x][y]);
                sf::Uint8 color = static_cast<sf::Uint8>(std::min(value / 2.0f, 255.0f));
                int index = 4 * (y * GRID_SIZE + x);
                pixels[index] = color;       // Red
                pixels[index + 1] = color;   // Green
                pixels[index + 2] = color;   // Blue
                pixels[index + 3] = 255;     // Alpha
            }
        }

        // Update the texture with the pixel data
        texture.update(pixels.data());

        // Render the current frame
        window.clear();
        window.draw(sprite);
        window.display();

        // Swap the grids for the next iteration
        std::swap(previous, current);
        std::swap(current, next);
    }

    return 0;
}
