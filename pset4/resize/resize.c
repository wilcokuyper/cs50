/**
 * Scales a BMP piece by piece, just because.
 */
       
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./copy n infile outfile\n");
        return 1;
    }

    // remember scale and filenames
    int scale = atoi(argv[1]);
    char *infile = argv[2];
    char *outfile = argv[3];

    // open input file 
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", argv[1]);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);
    
    // create info struct for scaled image
    BITMAPINFOHEADER biScaled = bi;
    
    biScaled.biWidth = bi.biWidth * scale;
    biScaled.biHeight = bi.biHeight * scale;
    
    // determine padding of the original image
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    // determine padding of the scaled image
    int scaledPadding = (4 - (biScaled.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    // set scaled image size in struct
    biScaled.biSizeImage = (biScaled.biWidth * sizeof(RGBTRIPLE) + scaledPadding) * abs(biScaled.biHeight);
    
    // create file struct and set file size of scaled image
    BITMAPFILEHEADER bfScaled = bf;
    bfScaled.bfSize = biScaled.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    // write outfile's BITMAPFILEHEADER
    fwrite(&bfScaled, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&biScaled, sizeof(BITMAPINFOHEADER), 1, outptr);

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {  
        // get current read position in file (to skip back to later when repeating scanlines based on scale)
        unsigned long position = ftell(inptr);

        // repeat scale times
        for (int y = 0; y < scale; y++)
        {
            // iterate over pixels in scanline
            for (int j = 0; j < bi.biWidth; j++)
            {
                // temporary storage
                RGBTRIPLE triple;
    
                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
    
                // repeat width scale times
                for(int x = 0; x < scale; x++)
                {
                    // write RGB triple to outfile
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);   
                }
            }
    
            // skip over padding, if any
            fseek(inptr, padding, SEEK_CUR);
    
            // then add it back (to demonstrate how)
            for (int k = 0; k < scaledPadding; k++)
            {  
                fputc(0x00, outptr);
            }
            
            // skip back to the start of the scanline to repeat it n (scale) times
            if (y < scale - 1)
                fseek(inptr, position, SEEK_SET);
        }
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
