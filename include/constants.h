#ifndef CONSTANTS_H
#define CONSTANTS_H

const int GRID_WIDTH = 25; 
const int GRID_HEIGHT = 25; 
const int CELL_SIZE = 20; 

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

const int gridWithPixels = (GRID_WIDTH * CELL_SIZE); 
const int gridHeightPixels = (GRID_HEIGHT * CELL_SIZE); 

const int MIN_X = (WINDOW_WIDTH - gridWithPixels)/ 2; 
const int MIN_Y = (WINDOW_HEIGHT - gridHeightPixels)/ 2; 

const int MAX_X = MIN_X + GRID_WIDTH*CELL_SIZE; 
const int MAX_Y = MIN_Y + GRID_HEIGHT*CELL_SIZE;

const bool draw_grid = true;

#endif //CONSTANTS_H