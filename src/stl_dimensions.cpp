/*

Copyright 2020 by Allwine Designs, LLC (stl_cmd@allwinedesigns.com)

    This file is part of stl_cmd.

    stl_cmd is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <math.h>
#include <vector>
#include "stl_util.h"

void print_usage() {
    fprintf(stderr, "stl_bounds prints the outer dimensions of STL files.\n\n");
    fprintf(stderr, "usage: stl_bounds <input file> [input file ...] [x|y|z]\n");
    fprintf(stderr, "    Prints the bounding box dimensions of STL files. Optionally, specify axes to get only those dimensions.\n");
}

int main(int argc, char** argv) {
    if (argc < 2) {
        print_usage();
        exit(2);
    }

    std::vector<char> axes;
    int file_start = 1;
    bool silent = false;
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == 'x' || argv[i][0] == 'y' || argv[i][0] == 'z') {
            axes.push_back(argv[i][0]);
            // Shift remaining arguments left
            for (int j = i; j < argc - 1; j++) {
                argv[j] = argv[j + 1];
            }
            argc--; // Reduce argument count
            i--; // Adjust index to check the new current argument
        } else if (argv[i][0] == 's') {
            silent = true;
            // Shift remaining arguments left
            for (int j = i; j < argc - 1; j++) {
                argv[j] = argv[j + 1];
            }
            argc--; // Reduce argument count
            i--; // Adjust index to check the new current argument
        }
    }

    float min_dim = INFINITY, max_dim = -INFINITY;

    for (int i = file_start; i < argc; i++) {
        char *file = argv[i];

        if (!is_valid_binary_stl(file)) {
            fprintf(stderr, "%s is not a binary stl file.\n", file);
            continue;
        }

        FILE *f = fopen(file, "rb");
        if (!f) {
            fprintf(stderr, "Can't read file: %s\n", file);
            continue;
        }

        fseek(f, 80, SEEK_SET);
        uint32_t num_tris;
        fread(&num_tris, 4, 1, f);

        float min_x = INFINITY, min_y = INFINITY, min_z = INFINITY;
        float max_x = -INFINITY, max_y = -INFINITY, max_z = -INFINITY;

        for (int j = 0; j < num_tris; j++) {
            fseek(f, 12, SEEK_CUR); // Skip normal vector

            vec pt[3];
            fread(&pt[0], 1, 12, f);
            fread(&pt[1], 1, 12, f);
            fread(&pt[2], 1, 12, f);

            for (int k = 0; k < 3; k++) {
                if (pt[k].x < min_x) min_x = pt[k].x;
                if (pt[k].y < min_y) min_y = pt[k].y;
                if (pt[k].z < min_z) min_z = pt[k].z;
                if (pt[k].x > max_x) max_x = pt[k].x;
                if (pt[k].y > max_y) max_y = pt[k].y;
                if (pt[k].z > max_z) max_z = pt[k].z;
            }

            fseek(f, 2, SEEK_CUR); // Skip attribute byte count
        }

        fclose(f);

        float size_x = max_x - min_x;
        float size_y = max_y - min_y;
        float size_z = max_z - min_z;

        if (!silent) {
            printf("%s\n", file);
        }

        if (axes.empty()) {
            if (!silent) {
                printf("X: %f\n", size_x);
                printf("Y: %f\n", size_y);
                printf("Z: %f\n", size_z);
            }
        } else {
            for (size_t i = 0; i < axes.size(); ++i) {
                char axis = axes[i];
                float size = 0;
                switch (axis) {
                    case 'x':
                        size = size_x;
                        break;
                    case 'y':
                        size = size_y;
                        break;
                    case 'z':
                        size = size_z;
                        break;
                }
                if (!silent || axes.size() > 1) {
                    printf("%i", (int)round(size_x));
                    if (i < axes.size() - 1) {
                        printf("x");
                    }
                    else {
                        printf("mm\n");
                    }
                }
                if (size < min_dim) min_dim = size;
                if (size > max_dim) max_dim = size;
            }
            if (!silent) {
                printf("\n");
            }
        }
    }

    if (axes.size() != 1) {
        return 0;
    }

    if (min_dim != INFINITY && max_dim != -INFINITY) {
        if (min_dim == max_dim) {
            printf("%dmm\n", (int)round(min_dim));
        } else {
            printf("%d-%dmm\n", (int)round(min_dim), (int)round(max_dim));
        }
    }

    return 0;
}
