/*

Copyright 2014 by Freakin' Sweet Apps, LLC (stl_cmd@freakinsweetapps.com)

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
#include <string.h>
#include <math.h>
#include "stl_util.h"

#define BUFFER_SIZE 4096
#define PI 3.141592653589793

typedef struct {
    float xx;
    float xy;
    float xz;
    float xw;
    float yx;
    float yy;
    float yz;
    float yw;
    float zx;
    float zy;
    float zz;
    float zw;
    float tx;
    float ty;
    float tz;
    float tw;
} mat; 

typedef struct {
    float x;
    float y;
    float z;
    float w;
} vec;

void mat_print(mat *m) {
    printf("%f %f %f %f\n", m->xx, m->xy, m->xz, m->xw);
    printf("%f %f %f %f\n", m->yx, m->yy, m->yz, m->yw);
    printf("%f %f %f %f\n", m->zx, m->zy, m->zz, m->zw);
    printf("%f %f %f %f\n", m->tx, m->ty, m->tz, m->tw);
}

void init_identity_mat(mat *m) {
    m->xx = 1;
    m->xy = 0;
    m->xz = 0;
    m->xw = 0;

    m->yx = 0;
    m->yy = 1;
    m->yz = 0;
    m->yw = 0;

    m->zx = 0;
    m->zy = 0;
    m->zz = 1;
    m->zw = 0;

    m->tx = 0;
    m->ty = 0;
    m->tz = 0;
    m->tw = 1;
}

void init_s_mat(mat *m, float s) {
    m->xx = s;
    m->xy = 0;
    m->xz = 0;
    m->xw = 0;

    m->yx = 0;
    m->yy = s;
    m->yz = 0;
    m->yw = 0;

    m->zx = 0;
    m->zy = 0;
    m->zz = s;
    m->zw = 0;

    m->tx = 0;
    m->ty = 0;
    m->tz = 0;
    m->tw = 1;
}

void init_sx_mat(mat *m, float s) {
    m->xx = s;
    m->xy = 0;
    m->xz = 0;
    m->xw = 0;

    m->yx = 0;
    m->yy = 1;
    m->yz = 0;
    m->yw = 0;

    m->zx = 0;
    m->zy = 0;
    m->zz = 1;
    m->zw = 0;

    m->tx = 0;
    m->ty = 0;
    m->tz = 0;
    m->tw = 1;
}

void init_sy_mat(mat *m, float s) {
    m->xx = 1;
    m->xy = 0;
    m->xz = 0;
    m->xw = 0;

    m->yx = 0;
    m->yy = s;
    m->yz = 0;
    m->yw = 0;

    m->zx = 0;
    m->zy = 0;
    m->zz = 1;
    m->zw = 0;

    m->tx = 0;
    m->ty = 0;
    m->tz = 0;
    m->tw = 1;
}

void init_sz_mat(mat *m, float s) {
    m->xx = 1;
    m->xy = 0;
    m->xz = 0;
    m->xw = 0;

    m->yx = 0;
    m->yy = 1;
    m->yz = 0;
    m->yw = 0;

    m->zx = 0;
    m->zy = 0;
    m->zz = s;
    m->zw = 0;

    m->tx = 0;
    m->ty = 0;
    m->tz = 0;
    m->tw = 1;
}

void init_rx_mat(mat *m, float angle) {
    angle *= PI/180; // convert to radians

    float cosa = cos(angle);
    float sina = sin(angle);

    m->xx = 1;
    m->xy = 0;
    m->xz = 0;
    m->xw = 0;

    m->yx = 0;
    m->yy = cosa;
    m->yz = sina;
    m->yw = 0;

    m->zx = 0;
    m->zy = -sina;
    m->zz = cosa;
    m->zw = 0;

    m->tx = 0;
    m->ty = 0;
    m->tz = 0;
    m->tw = 1;
}

void init_ry_mat(mat *m, float angle) {
    angle *= PI/180; // convert to radians

    float cosa = cos(angle);
    float sina = sin(angle);

    m->xx = cosa;
    m->xy = 0;
    m->xz = -sina;
    m->xw = 0;

    m->yx = 0;
    m->yy = 1;
    m->yz = 0;
    m->yw = 0;

    m->zx = sina;
    m->zy = 0;
    m->zz = cosa;
    m->zw = 0;

    m->tx = 0;
    m->ty = 0;
    m->tz = 0;
    m->tw = 1;
}

void init_rz_mat(mat *m, float angle) {
    angle *= PI/180; // convert to radians

    float cosa = cos(angle);
    float sina = sin(angle);

    m->xx = cosa;
    m->xy = sina;
    m->xz = 0;
    m->xw = 0;

    m->yx = -sina;
    m->yy = cosa;
    m->yz = 0;
    m->yw = 0;

    m->zx = 0;
    m->zy = 0;
    m->zz = 1;
    m->zw = 0;

    m->tx = 0;
    m->ty = 0;
    m->tz = 0;
    m->tw = 1;
}

void init_tx_mat(mat *m, float t) {
    m->xx = 1;
    m->xy = 0;
    m->xz = 0;
    m->xw = 0;

    m->yx = 0;
    m->yy = 1;
    m->yz = 0;
    m->yw = 0;

    m->zx = 0;
    m->zy = 0;
    m->zz = 1;
    m->zw = 0;

    m->tx = t;
    m->ty = 0;
    m->tz = 0;
    m->tw = 1;
}

void init_ty_mat(mat *m, float t) {
    m->xx = 1;
    m->xy = 0;
    m->xz = 0;
    m->xw = 0;

    m->yx = 0;
    m->yy = 1;
    m->yz = 0;
    m->yw = 0;

    m->zx = 0;
    m->zy = 0;
    m->zz = 1;
    m->zw = 0;

    m->tx = 0;
    m->ty = t;
    m->tz = 0;
    m->tw = 1;
}

void init_tz_mat(mat *m, float t) {
    m->xx = 1;
    m->xy = 0;
    m->xz = 0;
    m->xw = 0;

    m->yx = 0;
    m->yy = 1;
    m->yz = 0;
    m->yw = 0;

    m->zx = 0;
    m->zy = 0;
    m->zz = 1;
    m->zw = 0;

    m->tx = 0;
    m->ty = 0;
    m->tz = t;
    m->tw = 1;
}

void init_inv_s_mat(mat *m, float s) {
    m->xx = 1./s;
    m->xy = 0;
    m->xz = 0;
    m->xw = 0;

    m->yx = 0;
    m->yy = 1./s;
    m->yz = 0;
    m->yw = 0;

    m->zx = 0;
    m->zy = 0;
    m->zz = 1./s;
    m->zw = 0;

    m->tx = 0;
    m->ty = 0;
    m->tz = 0;
    m->tw = 1;
}

void init_inv_sx_mat(mat *m, float s) {
    m->xx = 1./s;
    m->xy = 0;
    m->xz = 0;
    m->xw = 0;

    m->yx = 0;
    m->yy = 1;
    m->yz = 0;
    m->yw = 0;

    m->zx = 0;
    m->zy = 0;
    m->zz = 1;
    m->zw = 0;

    m->tx = 0;
    m->ty = 0;
    m->tz = 0;
    m->tw = 1;
}

void init_inv_sy_mat(mat *m, float s) {
    m->xx = 1;
    m->xy = 0;
    m->xz = 0;
    m->xw = 0;

    m->yx = 0;
    m->yy = 1./s;
    m->yz = 0;
    m->yw = 0;

    m->zx = 0;
    m->zy = 0;
    m->zz = 1;
    m->zw = 0;

    m->tx = 0;
    m->ty = 0;
    m->tz = 0;
    m->tw = 1;
}

void init_inv_sz_mat(mat *m, float s) {
    m->xx = 1;
    m->xy = 0;
    m->xz = 0;
    m->xw = 0;

    m->yx = 0;
    m->yy = 1;
    m->yz = 0;
    m->yw = 0;

    m->zx = 0;
    m->zy = 0;
    m->zz = 1./s;
    m->zw = 0;

    m->tx = 0;
    m->ty = 0;
    m->tz = 0;
    m->tw = 1;
}

void init_inv_rx_mat(mat *m, float angle) {
    angle *= PI/180; // convert to radians

    float cosa = cos(angle);
    float sina = sin(angle);

    m->xx = 1;
    m->yx = 0;
    m->zx = 0;
    m->tx = 0;

    m->xy = 0;
    m->yy = cosa;
    m->zy = sina;
    m->ty = 0;

    m->xz = 0;
    m->yz = -sina;
    m->zz = cosa;
    m->tz = 0;

    m->xw = 0;
    m->yw = 0;
    m->zw = 0;
    m->tw = 1;
}

void init_inv_ry_mat(mat *m, float angle) {
    angle *= PI/180; // convert to radians

    float cosa = cos(angle);
    float sina = sin(angle);

    m->xx = cosa;
    m->yx = 0;
    m->zx = -sina;
    m->tx = 0;

    m->xy = 0;
    m->yy = 1;
    m->zy = 0;
    m->ty = 0;

    m->xz = sina;
    m->yz = 0;
    m->zz = cosa;
    m->tz = 0;

    m->xw = 0;
    m->yw = 0;
    m->zw = 0;
    m->tw = 1;
}

void init_inv_rz_mat(mat *m, float angle) {
    angle *= PI/180; // convert to radians

    float cosa = cos(angle);
    float sina = sin(angle);

    m->xx = cosa;
    m->yx = sina;
    m->zx = 0;
    m->tx = 0;

    m->xy = -sina;
    m->yy = cosa;
    m->zy = 0;
    m->ty = 0;

    m->xz = 0;
    m->yz = 0;
    m->zz = 1;
    m->tz = 0;

    m->xw = 0;
    m->yw = 0;
    m->zw = 0;
    m->tw = 1;
}

void init_inv_tx_mat(mat *m, float t) {
    m->xx = 1;
    m->xy = 0;
    m->xz = 0;
    m->xw = 0;

    m->yx = 0;
    m->yy = 1;
    m->yz = 0;
    m->yw = 0;

    m->zx = 0;
    m->zy = 0;
    m->zz = 1;
    m->zw = 0;

    m->tx = -t;
    m->ty = 0;
    m->tz = 0;
    m->tw = 1;
}

void init_inv_ty_mat(mat *m, float t) {
    m->xx = 1;
    m->xy = 0;
    m->xz = 0;
    m->xw = 0;

    m->yx = 0;
    m->yy = 1;
    m->yz = 0;
    m->yw = 0;

    m->zx = 0;
    m->zy = 0;
    m->zz = 1;
    m->zw = 0;

    m->tx = 0;
    m->ty = -t;
    m->tz = 0;
    m->tw = 1;
}

void init_inv_tz_mat(mat *m, float t) {
    m->xx = 1;
    m->xy = 0;
    m->xz = 0;
    m->xw = 0;

    m->yx = 0;
    m->yy = 1;
    m->yz = 0;
    m->yw = 0;

    m->zx = 0;
    m->zy = 0;
    m->zz = 1;
    m->zw = 0;

    m->tx = 0;
    m->ty = 0;
    m->tz = -t;
    m->tw = 1;
}

void mat_transpose(mat *m, mat *out) {
    out->xx = m->xx;
    out->xy = m->yx;
    out->xz = m->zx;
    out->xw = m->tx;

    out->yx = m->xy;
    out->yy = m->yy;
    out->yz = m->zy;
    out->yw = m->ty;

    out->zx = m->xz;
    out->zy = m->yz;
    out->zz = m->zz;
    out->zw = m->tz;

    out->tx = m->xw;
    out->ty = m->yw;
    out->tz = m->zw;
    out->tw = m->tw;
}

void mat_mult(mat *m1, mat *m2, mat *out) {
    out->xx = m1->xx*m2->xx+m1->xy*m2->yx+m1->xz*m2->zx+m1->xw*m2->tx; 
    out->xy = m1->xx*m2->xy+m1->xy*m2->yy+m1->xz*m2->zy+m1->xw*m2->ty; 
    out->xz = m1->xx*m2->xz+m1->xy*m2->yz+m1->xz*m2->zz+m1->xw*m2->tz; 
    out->xw = m1->xx*m2->xw+m1->xy*m2->yw+m1->xz*m2->zw+m1->xw*m2->tw;

    out->yx = m1->yx*m2->xx+m1->yy*m2->yx+m1->yz*m2->zx+m1->yw*m2->tx; 
    out->yy = m1->yx*m2->xy+m1->yy*m2->yy+m1->yz*m2->zy+m1->yw*m2->ty; 
    out->yz = m1->yx*m2->xz+m1->yy*m2->yz+m1->yz*m2->zz+m1->yw*m2->tz; 
    out->yw = m1->yx*m2->xw+m1->yy*m2->yw+m1->yz*m2->zw+m1->yw*m2->tw;

    out->zx = m1->zx*m2->xx+m1->zy*m2->yx+m1->zz*m2->zx+m1->zw*m2->tx; 
    out->zy = m1->zx*m2->xy+m1->zy*m2->yy+m1->zz*m2->zy+m1->zw*m2->ty; 
    out->zz = m1->zx*m2->xz+m1->zy*m2->yz+m1->zz*m2->zz+m1->zw*m2->tz; 
    out->zw = m1->zx*m2->xw+m1->zy*m2->yw+m1->zz*m2->zw+m1->zw*m2->tw;

    out->tx = m1->tx*m2->xx+m1->ty*m2->yx+m1->tz*m2->zx+m1->tw*m2->tx; 
    out->ty = m1->tx*m2->xy+m1->ty*m2->yy+m1->tz*m2->zy+m1->tw*m2->ty; 
    out->tz = m1->tx*m2->xz+m1->ty*m2->yz+m1->tz*m2->zz+m1->tw*m2->tz; 
    out->tw = m1->tx*m2->xw+m1->ty*m2->yw+m1->tz*m2->zw+m1->tw*m2->tw;
}

void vec_mat_mult(vec *v, mat *m, vec *out) {
    out->x = v->x*m->xx+v->y*m->yx+v->z*m->zx+v->w*m->tx; 
    out->y = v->x*m->xy+v->y*m->yy+v->z*m->zy+v->w*m->ty; 
    out->z = v->x*m->xz+v->y*m->yz+v->z*m->zz+v->w*m->tz; 
    out->w = v->x*m->xw+v->y*m->yw+v->z*m->zw+v->w*m->tw;
}

float vec_magnitude(vec *v) {
    return sqrt(v->x*v->x+v->y*v->y+v->z*v->z);
}

void vec_normalize(vec *v, vec *out) {
    float invmag = 1./vec_magnitude(v);
    out->x = v->x*invmag;
    out->y = v->y*invmag;
    out->z = v->z*invmag;
}

void vec_copy(vec *v, vec *out) {
    out->x = v->x;
    out->y = v->y;
    out->z = v->z;
    out->w = v->w;
}

void mat_copy(mat *m1, mat *out) {
    out->xx = m1->xx;
    out->xy = m1->xy;
    out->xz = m1->xz;
    out->xw = m1->xw;

    out->yx = m1->yx;
    out->yy = m1->yy;
    out->yz = m1->yz;
    out->yw = m1->yw;

    out->zx = m1->zx;
    out->zy = m1->zy;
    out->zz = m1->zz;
    out->zw = m1->zw;

    out->tx = m1->tx;
    out->ty = m1->ty;
    out->tz = m1->tz;
    out->tw = m1->tw;
}

void print_usage() {
    fprintf(stderr, "usage: stl_transform [[ <transformation> ] ...] <input file> <output file>\n");
    fprintf(stderr, "    Performs any number of the following transformations in\n");
    fprintf(stderr, "    the order they are listed on the command line:\n");
    fprintf(stderr, "        -rx <angle> - rotates <angle> degrees about the x-axis\n");
    fprintf(stderr, "        -ry <angle> - rotates <angle> degrees about the y-axis\n");
    fprintf(stderr, "        -rz <angle> - rotates <angle> degrees about the z-axis\n");
    fprintf(stderr, "        -s <s> - uniformly scales x, y and z by <s> (cannot be 0)\n");
    fprintf(stderr, "        -sx <x> - scales by <x> in x (cannot be 0)\n");
    fprintf(stderr, "        -sy <y> - scales by <y> in y (cannot be 0)\n");
    fprintf(stderr, "        -sz <z> - scales by <z> in z (cannot be 0)\n");
    fprintf(stderr, "        -tx <x> - translates <x> units in x\n");
    fprintf(stderr, "        -ty <y> - translates <y> units in y\n");
    fprintf(stderr, "        -tz <z> - translates <z> units in z\n");
}

int main(int argc, char** argv) {
    int errflg = 0;
    int did_scale = 0;

    mat tmp;
    mat tmp2;
    mat combined;
    mat inv_combined;
    mat inv_transpose;

    init_identity_mat(&combined);
    init_identity_mat(&inv_combined);

    int index;
    float arg;
    // TODO better arg handling so better errors can be displayed
    for(index = 1; index < argc; index++) {
        if(strcmp("-rx", argv[index]) == 0) {
           index++;
           if(index >= argc) break;
           arg = atof(argv[index]);
           init_rx_mat(&tmp, arg);
           mat_copy(&combined, &tmp2);
           mat_mult(&tmp2, &tmp, &combined);

           init_inv_rx_mat(&tmp, arg);
           mat_copy(&inv_combined, &tmp2);
           mat_mult(&tmp, &tmp2, &inv_combined);
        } else if(strcmp("-ry", argv[index]) == 0) {
           index++;
           if(index >= argc) break;
           arg = atof(argv[index]);
           init_ry_mat(&tmp, arg);
           mat_copy(&combined, &tmp2);
           mat_mult(&tmp2, &tmp, &combined);

           init_inv_ry_mat(&tmp, arg);
           mat_copy(&inv_combined, &tmp2);
           mat_mult(&tmp, &tmp2, &inv_combined);
        } else if(strcmp("-rz", argv[index]) == 0) {
           index++;
           if(index >= argc) break;
           arg = atof(argv[index]);
           init_rz_mat(&tmp, arg);
           mat_copy(&combined, &tmp2);
           mat_mult(&tmp2, &tmp, &combined);

           init_inv_rz_mat(&tmp, arg);
           mat_copy(&inv_combined, &tmp2);
           mat_mult(&tmp, &tmp2, &inv_combined);
        } else if(strcmp("-s", argv[index]) == 0) {
           index++;
           if(index >= argc) break;
           did_scale = 1;
           arg = atof(argv[index]);
           if(arg == 0) {
               errflg++;
           } else {
               init_s_mat(&tmp, arg);
               mat_copy(&combined, &tmp2);
               mat_mult(&tmp2, &tmp, &combined);

               init_inv_s_mat(&tmp, arg);
               mat_copy(&inv_combined, &tmp2);
               mat_mult(&tmp, &tmp2, &inv_combined);
           }
        } else if(strcmp("-sx", argv[index]) == 0) {
           index++;
           if(index >= argc) break;
           did_scale = 1;
           arg = atof(argv[index]);
           if(arg == 0) {
               errflg++;
           } else {
               init_sx_mat(&tmp, arg);
               mat_copy(&combined, &tmp2);
               mat_mult(&tmp2, &tmp, &combined);

               init_inv_sx_mat(&tmp, arg);
               mat_copy(&inv_combined, &tmp2);
               mat_mult(&tmp, &tmp2, &inv_combined);
           }
        } else if(strcmp("-sy", argv[index]) == 0) {
           index++;
           if(index >= argc) break;
           did_scale = 1;
           arg = atof(argv[index]);
           if(arg == 0) {
               errflg++;
           } else {
               init_sy_mat(&tmp, arg);
               mat_copy(&combined, &tmp2);
               mat_mult(&tmp2, &tmp, &combined);

               init_inv_sy_mat(&tmp, arg);
               mat_copy(&inv_combined, &tmp2);
               mat_mult(&tmp, &tmp2, &inv_combined);
           }
        } else if(strcmp("-sz", argv[index]) == 0) {
           index++;
           if(index >= argc) break;
           did_scale = 1;
           arg = atof(argv[index]);
           if(arg == 0) {
               errflg++;
           } else {
               init_sz_mat(&tmp, arg);
               mat_copy(&combined, &tmp2);
               mat_mult(&tmp2, &tmp, &combined);

               init_inv_sz_mat(&tmp, arg);
               mat_copy(&inv_combined, &tmp2);
               mat_mult(&tmp, &tmp2, &inv_combined);
           }
        } else if(strcmp("-tx", argv[index]) == 0) {
           index++;
           if(index >= argc) break;
           arg = atof(argv[index]);
           init_tx_mat(&tmp, arg);
           mat_copy(&combined, &tmp2);
           mat_mult(&tmp2, &tmp, &combined);

           init_inv_tx_mat(&tmp, arg);
           mat_copy(&inv_combined, &tmp2);
           mat_mult(&tmp, &tmp2, &inv_combined);
        } else if(strcmp("-ty", argv[index]) == 0) {
           index++;
           if(index >= argc) break;
           arg = atof(argv[index]);
           init_ty_mat(&tmp, arg);
           mat_copy(&combined, &tmp2);
           mat_mult(&tmp2, &tmp, &combined);

           init_inv_ty_mat(&tmp, arg);
           mat_copy(&inv_combined, &tmp2);
           mat_mult(&tmp, &tmp2, &inv_combined);
        } else if(strcmp("-tz", argv[index]) == 0) {
           index++;
           if(index >= argc) break;
           arg = atof(argv[index]);
           init_tz_mat(&tmp, arg);
           mat_copy(&combined, &tmp2);
           mat_mult(&tmp2, &tmp, &combined);

           init_inv_tz_mat(&tmp, arg);
           mat_copy(&inv_combined, &tmp2);
           mat_mult(&tmp, &tmp2, &inv_combined);
        } else {
            break;
        }
    }

    if(errflg || index+1 >= argc) {
        print_usage();
        exit(2);
    }

    char *file = argv[index];
    char *outfile = argv[index+1];

    if(!is_valid_binary_stl(file)) {
        fprintf(stderr, "%s is not a binary stl file.\n", file);
        exit(2);
    }

    mat_transpose(&inv_combined, &inv_transpose);

    FILE *f;
    FILE *outf;

    f = fopen(file, "rb");
    if(!f) {
        fprintf(stderr, "Can't read file: %s\n", file);
        exit(2);
    }

    outf = fopen(outfile, "wb");
    if(!outf) {
        fprintf(stderr, "Can't write to file: %s\n", outfile);
        exit(2);
    }

    fseek(f, 80, SEEK_SET);

    uint32_t num_tris;

    fread(&num_tris, 4, 1, f);

    char header[81] = {0}; // include an extra char for terminating \0 of snprintf
    snprintf(header, 81, "Transformed copy of %s", basename(file));

    fwrite(header, 80, 1, outf);
    fwrite(&num_tris, 4, 1, outf);

    uint16_t abc; // attribute byte count 

    vec tmp_vec;
    vec normal;
    vec point;

    normal.w = 0;
    point.w = 1;

    for(int i = 0; i < num_tris; i++) {
        fread(&normal, 1, 12,f);

        vec_mat_mult(&normal, &inv_transpose, &tmp_vec);
        if(did_scale) {
            vec_normalize(&tmp_vec, &normal);
            fwrite(&normal, 1, 12, outf);
        } else {
            fwrite(&tmp_vec, 1, 12, outf);
        }

        for(int j = 0; j < 3; j++) {
            fread(&point, 1, 12,f);
            vec_mat_mult(&point, &combined, &tmp_vec);
            fwrite(&tmp_vec, 1, 12, outf);
        }
        fread(&abc, 1, 2,f);
        fwrite(&abc, 1, 2,outf);
    }

    fclose(f);
    fclose(outf);

    return 0;
}
