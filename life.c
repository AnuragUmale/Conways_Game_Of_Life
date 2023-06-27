#include "universe.h"

#include <getopt.h>
#include <inttypes.h>
#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>

#define OPTIONS "tshn:i:o:"
#define DELAY   50000

void game_of_life(Universe *a, Universe *b) {
    for (uint32_t r = 0; r < uv_rows(a); r += 1) {
        for (uint32_t c = 0; c < uv_cols(a); c += 1) {
            uint32_t census = uv_census(a, r, c);
            if (uv_get_cell(a, r, c)) {
                if (census == 2 || census == 3) {
                    uv_live_cell(b, r, c);
                } else {
                    uv_dead_cell(b, r, c);
                }
            } else {
                if (census == 3) {
                    uv_live_cell(b, r, c);
                } else {
                    uv_dead_cell(b, r, c);
                }
            }
        }
    }
}

void usage(void) {
    fprintf(stderr, "SYNOPSIS\n");
    fprintf(stderr, "    Conway's Game of Life\n\n");
    fprintf(stderr, "USAGE\n");
    fprintf(stderr, "    ./life tsn:i:o:h\n\n");
    fprintf(stderr, "OPTIONS\n");
    fprintf(stderr, "    -t             Create your universe as a toroidal\n");
    fprintf(stderr, "    -s             Silent - do not use animate the evolution\n");
    fprintf(stderr, "    -n {number}    Number of generations [default: 100]\n");
    fprintf(stderr, "    -i {file}      Input file [default: stdin]\n");
    fprintf(stderr, "    -o {file}      Output file [default: stdout]\n");
}

int main(int argc, char **argv) {
    int opt = 0;
    FILE *infile = stdin;
    FILE *outfile = stdout;
    bool toroidal = false;
    bool silent = false;
    uint32_t number_of_iter = 100;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 't': toroidal = true; break;
        case 'i': infile = fopen(optarg, "r"); break;
        case 'o': outfile = fopen(optarg, "w"); break;
        case 'n': number_of_iter = atoi(optarg); break;
        case 's': silent = true; break;
        case 'h': usage(); return EXIT_SUCCESS; // return 0
        default: usage(); return EXIT_FAILURE; // return 1
        }
    }

    if (infile == NULL) {
        fprintf(stderr, "Error in opeing the input file\n");
        return EXIT_FAILURE; // return 1
    }
    if (outfile == NULL) {
        fprintf(stderr, "Error in opeing the output file\n");
        return EXIT_FAILURE; // return 1
    }
    uint32_t rows, cols;
    if (fscanf(infile, "%" SCNu32 " %" SCNu32, &rows, &cols) != 2) {
        fprintf(stderr, "Malformed Input\n");
        fclose(infile); // closing the infile
        fclose(outfile); // closing the outfile
        return EXIT_FAILURE; // return 1
    }

    Universe *universe_a = uv_create(rows, cols, toroidal);
    if (!universe_a) { // pointer not equal to null
        fprintf(stderr, "Error in allocating the memory\n");
        fclose(infile); // closing the infile
        fclose(outfile); // closing the outfile
        return EXIT_FAILURE; // return 1
    }
    Universe *universe_b = uv_create(rows, cols, toroidal);
    if (!universe_b) { // pointer is not equal to null
        fprintf(stderr, "Error in allocating the memory\n");
        fclose(infile); // closing the infile
        fclose(outfile); // closing the outfile
        return EXIT_FAILURE; // return 1
    }

    uv_populate(universe_a, infile);

    if (!silent) {
        initscr(); // Initalise ncurses screen
        curs_set(false); // curser is set false
    }
    for (uint32_t iter = 0; iter < number_of_iter; iter += 1) {
        game_of_life((iter % 2 ? universe_b : universe_a),
            (iter % 2
                    ? universe_a
                    : universe_b)); // universe are altered based on even and odd check for number of iteration
        if (!silent) {
            clear(); // scree is cleared
            for (uint32_t r = 0; r < uv_rows((iter % 2 ? universe_a : universe_b));
                 r
                 += 1) { // universe are altered based on even and odd check for number of iteration
                for (
                    uint32_t c = 0; c < uv_cols((iter % 2 ? universe_a : universe_b));
                    c
                    += 1) { // universe are altered based on even and odd check for number of iteration
                    if (uv_get_cell((iter % 2 ? universe_a : universe_b), r,
                            c)) { // universe are altered based on even and odd check for number of iteration
                        mvprintw(r, c, "o"); // prints o in the specific row and column
                    }
                }
            }
            refresh(); // refresh the screen
            usleep(DELAY); // display delays by some microseconds
        }
    }

    if (!silent) {
        endwin();
    }

    uv_print(universe_a, outfile);
    uv_delete(universe_a); // free the memory
    uv_delete(universe_b); // free the memory

    fclose(infile); // close the infile
    fclose(outfile); // close the outfile
    return EXIT_SUCCESS; // return 0
}
