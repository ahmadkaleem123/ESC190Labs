#include "c_img.h"
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

int main (void){
    struct rgb_img *im;
    //read_in_img(&im, "president.bin");
    double weights[5] = {0.1, 0.5, 1.5, 3, 255};
    for(int i = 0; i < 5; i++){
       
        read_in_img(&im, "president.bin");
        for (int j = 0; j< im->height; j++){
            for (int k = 0; k < im->width; k++){
                double r = im->raster[3 * (j*(im->width) + k) + 0] * weights[i];
                double g = im->raster[3 * (j*(im->width) + k) + 1] * weights[i];
                double b = im->raster[3 * (j*(im->width) + k) + 2] * weights[i];
                if (r > 255){
                    r = 255;
                }
                if ( g > 255){
                    g = 255;
                }
                if(b > 255){
                    b = 255;
                }

                set_pixel(im, j, k, r,g ,b );
            }
        
        }
        char name[9] = "img"; 
        if(i== 0){
             name[3] = '0'; 
        }
        else if(i== 1){
             name[3] = '1'; 
        }
        else if(i== 2){
             name[3] = '2'; 
        }
        else if(i== 3){
             name[3] = '3'; 
        }
        else if(i== 4){
             name[3] = '4'; 
        }
        name[4] = '.';
        name[5] = 'b';
        name[6] = 'i';
        name[7] = 'n';
        write_img(im, name);
    }
}