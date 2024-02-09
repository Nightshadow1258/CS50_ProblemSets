#include "helpers.h"
#include <math.h>
#include <stdio.h>

BYTE round_byte(float num)
{
    int temp = (num < 0 ? (num - 0.5) : (num + 0.5)); // round manually
    // ensure values stay in range of 8 bits.
    if (temp > 255)
        temp = 255;
    if (temp < 0)
        temp = 0;

    return (BYTE)temp;
}

void swap_pixels(RGBTRIPLE *x, RGBTRIPLE *y)
{
    RGBTRIPLE temp = *x;
    *x = *y;
    *y = temp;
    return;
}

int check_limits(int val, int max)
{
    if (val < 0)
        val = 0;
    if (val > max)
        val = max;
    return val;
}
double absolute(double var)
{
    if (var < 0)
        var = -1 * var;

    return var;
}
double sqroot(double square)
{
    double epsilon = 3*10^-5;
    // inital guess
    // newton methode: define function to calculate sqrt(x) => f(x) = x^2 -a. The solution is the square root of x
    // now apply newton methode to get where a functon is zero. x_k+1 = x_k - f(x_k)/f'(x_k)
    // with f'(x) beinng the Derivative of f(x) in this case 2*x
    // iterate until the difference between x_k and x_k+1 is below a threshold at which you are happy with the accuracy.

    double x_k = 2*square/3;
    double x_k_new;
    double difference = absolute(square - x_k);

    while(difference > epsilon)
    {
        x_k_new = x_k - ( x_k*x_k - square) / (2 * x_k);
        difference = absolute(x_k_new-x_k);
        x_k = x_k_new;
    }

    return x_k;

}

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // NTSC formula: 0.299 ∙ Red + 0.587 ∙ Green + 0.114 ∙ Blue
    // source: https://support.ptc.com/help/mathcad/r9.0/en/index.html#page/PTC_Mathcad_Help/example_grayscale_and_color_in_images.html
    // TODO iterate over the entire image and convert the colors to grayscale.
    // not sure if there should be steps for the greyscale or just balck or white -> will go for multiple steps.
    // for black white we would need a metric to deceide when a pixel is considered white or black
    // Different ideas to create a greyscale filter
    // average of the rgb values
    // (min(r,g,b) + max(r,g,b))/2
    // ntsc formula from above
    // than set rgb all to the same value.

    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            BYTE blue = image[h][w].rgbtBlue;
            BYTE green = image[h][w].rgbtGreen;
            BYTE red = image[h][w].rgbtRed;
            float grey; // ennsure that there is no overflow WORD is 16 bit that is more than enough 10 bit needed only
            // not sure what the best way is to avoid this issue

            // average methode:
            grey = round_byte((blue + green + red) / 3.0);

            // ntsc:
            grey = round_byte(0.299 * red + 0.587 * green + 0.114 * blue);

            // overwrite date in original image buffer don't forget to cast back to BYTE
            image[h][w].rgbtBlue = (BYTE)grey;
            image[h][w].rgbtGreen = (BYTE)grey;
            image[h][w].rgbtRed = (BYTE)grey;
        }
    }
    // tested and looking good. both average and ntsc look fine to me.
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // reflect horizontally. I think that means from top to bottom? so it is mirrored along the horizontal axis.
    // so we do not need to calculate new values for rgb and only need to rearrange the pixels of the image.
    // Idea iterate over the top half of the image and swap the pixels with each other
    // use pointers for memory efficency
    // I believe that the start point 0,0 is in the left top corner by convention,, if not bad for me :)

    for (int h = 0; h < height / 2; h++) // I believe h is always dividable by two and four? maybe make sure.
    {
        for (int w = 0; w < width; w++)
        {
            swap_pixels(&image[h][w], &image[height - h - 1][w]);
            // RGBTRIPLE temp = image[h][w];
            // image[h][w] = image[height-h-1][w];
            // image[height-h-1][w] = temp;
        }
    }

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // use a 3x3 Matrix to apply the blur filter. Mathematically we are calculating the determinate of a 3x3 Matrix
    // and dumping the result to the center pixel. To this for each color!
    // edge cases need to be considered here else we will go out of bounds -> segmentation fault
    // not so nice :)
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    int size_matrix = 3;
    int offset = size_matrix / 3; // intentional truncating!

    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {

            int red = 0, blue = 0, green = 0;
            int div = 0;

            int h_start = check_limits(h - offset, height);
            int h_end = check_limits(h + offset, height);
            int w_start = check_limits(w - offset, width);
            int w_end = check_limits(w + offset, width);

            for (int i = h_start; i <= h_end; i++)
            {
                for (int j = w_start; j <= w_end; j++)
                {
                    red += copy[i][j].rgbtRed;
                    blue += copy[i][j].rgbtBlue;
                    green += copy[i][j].rgbtGreen;
                    div++;
                }
            }

            image[h][w].rgbtRed = round_byte(red / div);
            image[h][w].rgbtBlue = round_byte(blue / div);
            image[h][w].rgbtGreen = round_byte(green / div);
        }
    }

    return;
}

// Detect edges                
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // use the Gx and Gy kernel to create a sobel edge detection filter.
    // I think it is best to write a function that does matrix multiplication and use that to apply the kernels to the "subpixels". but i am lazy
    // at the border add additional black pixel (value)
    
    
    // define kernel:
    int Gx[3][3] = {{-1,0,1},{-2,0,2},{-1,0,1}};
    int Gy[3][3] = {{-1,-2,-1},{0,0,0},{1,2,1}};


    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    int size_matrix = 3;
    int offset = size_matrix / 3; // intentional truncating!

    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {

            double red_gx = 0,red_gy = 0;
            double blue_gx = 0,blue_gy = 0;
            double green_gx = 0,green_gy = 0;

            int h_start = check_limits(h - offset, height);
            int h_end = check_limits(h + offset, height);
            int w_start = check_limits(w - offset, width);
            int w_end = check_limits(w + offset, width);

            for (int i = h_start; i <= h_end; i++)
            {
                for (int j = w_start; j <= w_end; j++)
                {
                    // convert iterator to indices between 0 and 2.
                    int ind_x = i - h_start;
                    int ind_y = j - w_start;
                    printf("%d,", height);
                    red_gx += Gx[ind_x][ind_y] * copy[i][j].rgbtRed;
                    red_gy += Gy[ind_x][ind_y] * copy[i][j].rgbtRed;

                    blue_gx += Gx[ind_x][ind_y] * copy[i][j].rgbtBlue;
                    blue_gy += Gy[ind_x][ind_y] * copy[i][j].rgbtBlue;

                    green_gx += Gx[ind_x][ind_y] * copy[i][j].rgbtGreen;
                    green_gy += Gy[ind_x][ind_y] * copy[i][j].rgbtGreen;

                }
            }
            printf("\n");
            image[h][w].rgbtRed = round_byte(sqrt(red_gx*red_gx+red_gy*red_gy));
            image[h][w].rgbtBlue = round_byte(sqrt(blue_gx*blue_gx+blue_gy*blue_gy));
            image[h][w].rgbtGreen = round_byte(sqrt(green_gx*green_gx+green_gy*green_gy));

        }
    }
    
    return;
}
