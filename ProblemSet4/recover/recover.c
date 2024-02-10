#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

uint8_t buffer[512]; // FAT32 block size

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover input.raw");
        return 1;
    }

    char *infile = argv[1];

    FILE *inputptr = fopen(infile, "r");
    if (inputptr == NULL)
    {
        printf("Could not read input file!");
        return 1;
    }

    // planning: get all the data from the input file. Search for the JPEG Header
    // create a function to find JPEG Headers.
    // extract all data between two headers. If there are no headers left use the
    // data from the last header to the end of the file

    bool w_flag = false;

    int img_counter = 0;
    char outfile[256];
    FILE *outptr = NULL;

    while (fread(&buffer, sizeof(buffer), 1, inputptr) != 0)
    {
        // check if block starts with jpeg header
        if (buffer[0] == 0xff && buffer[1] == 0xd8 &&
            buffer[2] == 0xff && buffer[3] >= 0xe0)
        {
            //  check if currently writing to a file close old one and increase counter
            if (w_flag)
            {
                fclose(outptr);
            }

            int ret = snprintf(outfile, 256 - 1, "./output/%d.jpeg", img_counter);
            if (ret < 0) // Error handling
                return 1;

            printf("Image Header found: Number: %d\n", img_counter);
            printf("filename: %s\n", outfile);
            outptr = fopen(outfile, "w");
            if (outptr == NULL)
            {
                printf("Could not open ouptut file!");
                return 1;
            }
            img_counter++;
            w_flag = true;
        }
        if (w_flag)
        {
            printf("check!\n");
            fwrite(&buffer, sizeof(buffer), 1, outptr);
        }
    }
    fclose(outptr);
    fclose(inputptr);
}