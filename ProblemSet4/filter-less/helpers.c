#include "helpers.h"

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

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    // research on sepia: image appears more yellow and washed out.. Originally due to UV light expose over longer time.
    // conversion from wikipedia: https://de.wikipedia.org/wiki/Sepia_(Fotografie)
    // R = R * 0.393 + G * 0.769 + B * 0.189
    // G = R * 0.349 + G * 0.686 + B * 0.168
    // B = R * 0.272 + G * 0.534 + B * 0.131
    // pretty much copy and paste from greyscale only the filtering is differnt.

    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            int blue = image[h][w].rgbtBlue;
            int green = image[h][w].rgbtGreen;
            int red = image[h][w].rgbtRed;
            float blue_sepia, green_sepia, red_sepia; // ennsure that there is no overflow WORD is 16 bit that is more than enough 10 bit needed only
            // not sure what the best way is to avoid this issue

            red_sepia = round_byte(red * 0.393 + green * 0.769 + blue * 0.189);
            green_sepia = round_byte(red * 0.349 + green * 0.686 + blue * 0.168);
            blue_sepia = round_byte(red * 0.272 + green * 0.534 + blue * 0.131);

            // overwrite date in original image buffer don't forget to cast back to BYTE
            image[h][w].rgbtBlue = (BYTE)blue_sepia;
            image[h][w].rgbtGreen = (BYTE)green_sepia;
            image[h][w].rgbtRed = (BYTE)red_sepia;
        }
    }

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

            for (int i = h_start; i < h_end; i++)
            {
                for (int j = w_start; j < w_end; j++)
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
