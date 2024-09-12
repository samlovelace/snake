#ifndef CONSTANTS_H
#define CONSTANTS_H

const int GRID_SIZE = 75; 
const int GRID_DIM = 600; 

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;

const int GRID_X = WINDOW_WIDTH/2 - GRID_DIM/2; 
const int GRID_Y = WINDOW_HEIGHT/2 - GRID_DIM/2;

const int CELL_SIZE = GRID_DIM / GRID_SIZE;

const int gridWidthPixels = (GRID_SIZE * CELL_SIZE); 
const int gridHeightPixels = (GRID_SIZE * CELL_SIZE); 

const int MIN_X = (WINDOW_WIDTH - gridWidthPixels)/ 2; 
const int MIN_Y = (WINDOW_HEIGHT - gridHeightPixels)/ 2; 

const int MAX_X = MIN_X + GRID_SIZE*CELL_SIZE; 
const int MAX_Y = MIN_Y + GRID_SIZE*CELL_SIZE;

const bool draw_grid = true;

#endif //CONSTANTS_H