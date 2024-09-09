#ifndef GLYPHS_H
#define GLYPHS_H

#include <SFML/Graphics.hpp>

// Define the glyph for each digit (0-9) using a 5x5 matrix
const char ZERO[5][5] = {
    {'#', '#', '#', '#', '#'},
    {'#', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', '#'},
    {'#', '#', '#', '#', '#'}
};

const char ONE[5][5] = {
    {' ', ' ', '#', ' ', ' '},
    {' ', ' ', '#', ' ', ' '},
    {' ', ' ', '#', ' ', ' '},
    {' ', ' ', '#', ' ', ' '},
    {' ', ' ', '#', ' ', ' '}
};

const char TWO[5][5] = {
    {'#', '#', '#', '#', '#'},
    {' ', ' ', ' ', ' ', '#'},
    {'#', '#', '#', '#', '#'},
    {'#', ' ', ' ', ' ', ' '},
    {'#', '#', '#', '#', '#'}
};

const char THREE[5][5] = {
    {'#', '#', '#', '#', '#'},
    {' ', ' ', ' ', ' ', '#'},
    {'#', '#', '#', '#', '#'},
    {' ', ' ', ' ', ' ', '#'},
    {'#', '#', '#', '#', '#'}
};

const char FOUR[5][5] = {
    {'#', ' ', ' ', '#', ' '},
    {'#', ' ', ' ', '#', ' '},
    {'#', '#', '#', '#', '#'},
    {' ', ' ', ' ', '#', ' '},
    {' ', ' ', ' ', '#', ' '}
};

const char FIVE[5][5] = {
    {'#', '#', '#', '#', '#'},
    {'#', ' ', ' ', ' ', ' '},
    {'#', '#', '#', '#', '#'},
    {' ', ' ', ' ', ' ', '#'},
    {'#', '#', '#', '#', '#'}
};

const char SIX[5][5] = {
    {'#', '#', '#', '#', '#'},
    {'#', ' ', ' ', ' ', ' '},
    {'#', '#', '#', '#', '#'},
    {'#', ' ', ' ', ' ', '#'},
    {'#', '#', '#', '#', '#'}
};

const char SEVEN[5][5] = {
    {'#', '#', '#', '#', '#'},
    {' ', ' ', ' ', ' ', '#'},
    {' ', ' ', ' ', ' ', '#'},
    {' ', ' ', ' ', ' ', '#'},
    {' ', ' ', ' ', ' ', '#'}
};

const char EIGHT[5][5] = {
    {'#', '#', '#', '#', '#'},
    {'#', ' ', ' ', ' ', '#'},
    {'#', '#', '#', '#', '#'},
    {'#', ' ', ' ', ' ', '#'},
    {'#', '#', '#', '#', '#'}
};

const char NINE[5][5] = {
    {'#', '#', '#', '#', '#'},
    {'#', ' ', ' ', ' ', '#'},
    {'#', '#', '#', '#', '#'},
    {' ', ' ', ' ', ' ', '#'},
    {'#', '#', '#', '#', '#'}
};

// Array of pointers to the glyphs for easier access
const char* DIGITS[10] = { ZERO, ONE,TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE};

// Function to render a glyph using SFML
void renderGlyph(sf::RenderWindow& window, const char glyph[5][5], int startX, int startY, int cellSize) {
    // Loop through the 5x5 glyph matrix
    for (int row = 0; row < 5; ++row) {
        for (int col = 0; col < 5; ++col) {
            if (glyph[row][col] == '#') {
                sf::RectangleShape rectangle(sf::Vector2f(cellSize, cellSize));
                rectangle.setFillColor(sf::Color::Blue);  // Color for the glyph
                rectangle.setPosition(startX + col * cellSize, startY + row * cellSize);
                window.draw(rectangle);
            }
        }
    }
}

// Function to render an integer as a series of glyphs
void renderInteger(sf::RenderWindow& window, int number, int startX, int startY, int cellSize) {
    // Convert the integer to a string to easily iterate over digits
    std::string numStr = std::to_string(number);
    
    // Iterate through each character of the number
    for (std::size_t i = 0; i < numStr.size(); ++i) {
        // Convert the character to an integer (digit)
        int digit = numStr[i] - '0';

        // Render the corresponding glyph
        renderGlyph(window, DIGITS[digit], startX + i * (5 * cellSize + cellSize), startY, cellSize);
    }
}

#endif // GLYPHS_H
