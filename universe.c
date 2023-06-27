#include "universe.h"

#include <inttypes.h>
#include <stdlib.h>

struct Universe {
    uint32_t rows; // Defines number of rows
    uint32_t cols; // Defines number of columns
    bool **grid; // It is a matrix of 0s and 1s (trues and false)
    bool toroidal; // Flag for checking if universe is torodial
};

Universe *uv_create(
    uint32_t rows, uint32_t cols, bool toroidal) { // takes row, column and the value of the flag
    Universe *u = (Universe *) malloc(sizeof(Universe)); // Allocating memory on heap
    if (u) { // If pointer is not null
        if (rows <= 0 && cols <= 0) { // Rows and columns are check if they are greter than 0
            return NULL; // NULL is returned
        }
        u->rows = rows; // row is assigned to u->row
        u->cols = cols; // column is assigne to u->cols
        u->toroidal = toroidal; // Flag is set
        u->grid = (bool **) calloc(
            rows, sizeof(bool *)); // Memory is allocted on heap and it is array of boolean pointer

        if (u->grid) { // If memory is allocated properly or not is checked
            for (uint32_t i = 0; i < rows; i += 1) {
                u->grid[i] = (bool *) calloc(cols,
                    sizeof(
                        bool)); // Allocating memory on heap and it will be a array of boolean values
                if (!u->grid[i]) { // If memory allocation failes
                    for (uint32_t j = 0; j < i && u->grid[j]; j += 1) {
                        free(u->grid[j]); // Freeing the allocated memory for each row
                    }
                    free(u->grid); //freeing the matrix
                    free(u); // Freeing the memory
                    u = NULL; // Assigning the NULL to u
                    return u; // Returning u
                }
            }
        } else {
            free(u->grid); // Freeing the matrix
            free(u); // Freeing the universe
            u = NULL; // Assigning NULL to u
        }
    }
    return u; // returning new
}

void uv_delete(Universe *u) { // take pointer to universe
    if (u) { // If pointer is not null
        for (uint32_t i = 0; i < u->rows; i += 1) {
            free(u->grid[i]); // freeing the each column
        }
        free(u->grid); // freeing the memory
        free(u); // freeing the universe
    }
}

uint32_t uv_rows(Universe *u) { // take pointer to universe
    uint32_t rows = 0; // rows is assigned to 0;
    if (u) { // If pointer is not null
        rows = u->rows; // total number of rows are assigned
        return rows; // rows is returned
    } else {
        return rows; // rows is returned
    }
}

uint32_t uv_cols(Universe *u) { // take pointer to universe
    uint32_t columns = 0; // columns is assigned to 0;
    if (u) { // If pointer is not null
        columns = u->cols; // total number of column are assigned
        return columns; // column is returned
    } else {
        return columns; // column is returned
    }
}

void uv_live_cell(Universe *u, uint32_t r, uint32_t c) { // take pointer to universe, row and column
    if (u) { // If pointer is not null
        if (r < u->rows && c < u->cols) { // row and columns are checked if they are in bounds
            u->grid[r][c] = true; //  cell at that location is marked true
        }
    }
}

void uv_dead_cell(Universe *u, uint32_t r, uint32_t c) {
    if (u) { // If pointer is not null
        if (r < u->rows && c < u->cols) { // row and columns are checked if they are in bounds
            u->grid[r][c] = false; //  cell at that location is marked false
        }
    }
}

bool uv_get_cell(Universe *u, uint32_t r, uint32_t c) { // take pointer to universe, row and column
    if (u) { // If pointer is not null
        if (r < u->rows && c < u->cols) { // row and columns are checked if they are in bounds
            return u->grid[r][c]; // Cell value at that location is returned
        } else {
            return false; // returned false
        }
    } else {
        return false; // return false
    }
}

bool uv_populate(Universe *u, FILE *infile) { // take pointer to universe, and a file pointer
    if (u) { // if pointer is not null
        while (!feof(infile)) { // while the pointer is not reaching the end of the file
            uint32_t r, c; // varibles are declared
            if (fscanf(infile, "%" SCNu32 " %" SCNu32, &r, &c)
                != 2) { // checking if the scanned value if of type uint32_t and is not equal to 2
                return false; // false is returned
            } else if (r > u->rows || c > u->cols) { // the value is check if it is out of bounds
                return false; // false is retuened
            } else {
                uv_live_cell(u, r, c); // the cell lives
            }
        }

        return true; // true is returned
    } else {
        return false; // false is returned
    }
}

uint32_t uv_census(Universe *u, uint32_t r, uint32_t c) {
    uint32_t count = 0; // variable is declaredd

    if (u) { // if pointer is not null

        uint32_t shifter[8][2] = {
            // array with all shifter
            { -1, -1 },
            { -1, 0 },
            { -1, 1 },
            { 0, -1 },
            { 0, 1 },
            { 1, -1 },
            { 1, 0 },
            { 1, 1 },
        };

        for (uint32_t i = 0; i < 8; i += 1) { // for loop from 0 to 7
            uint32_t row_offset = shifter[i][0], col_offset = shifter[i][1]; // shifer is assgned
            uint32_t row = r + row_offset, col = c + col_offset; // row is aceessed using the shift
            if (u->toroidal) { // If the flag is true
                row = (row + u->rows) % u->rows; // row is assgined and overflow is controlled
                col = (col + u->cols) % u->cols; // column is assgined and overflow is controlled
            }
            if (row >= 0 && row < u->rows && col >= 0 && col < u->cols) {
                count += u->grid[row][col] ? 1 : 0; // count is incremented if cell is alive
            }
        }

        return count; // Count is returned
    } else {
        return count; // count is returned
    }
}

void uv_print(Universe *u, FILE *outfile) {
    if (u) { // if the pointer is not null
        for (uint32_t r = 0; r < u->rows; r += 1) { // For loop till number of rows
            for (uint32_t c = 0; c < u->cols; c += 1) { // for loop till number of columns
                fputc(u->grid[r][c] ? 'o' : '.',
                    outfile); // if cell is alive o is printed else . in your outfile
            }
            fputc('\n', outfile); // New line is printed in oufile
        }
    }
}
