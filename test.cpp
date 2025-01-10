#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <thread>
#include <mutex>

// Global stream for simulating stdin
std::stringstream mouseInput;
std::mutex inputMutex;

// Function to simulate stdin processing
void processInput() {
    std::string line;
    while (true) {
        {
            std::lock_guard<std::mutex> lock(inputMutex);
            if (!mouseInput.str().empty()) {
                std::getline(mouseInput, line);
                if (!line.empty()) {
                    std::cout << "Processed input: " << line << std::endl;
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

int main(int argc, char* argv[]) {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Mouse to stdin Simulation");

    // Start the input processing thread
    std::thread inputThread(processInput);
    inputThread.detach();

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    int x = event.mouseButton.x;
                    int y = event.mouseButton.y;

                    // Redirect mouse click info to the simulated stdin
                    {
                        std::lock_guard<std::mutex> lock(inputMutex);
                        mouseInput << "MouseClick " << x << " " << y << "\n";
                    }
                }
            }
        }

        window.clear(sf::Color::Black);
        window.display();
    }

    return 0;
}
