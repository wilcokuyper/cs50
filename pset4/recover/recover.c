#import <stdio.h>

typedef unsigned char byte;

int hasJPEGSequence(byte b[])
{
    if(b[0] == 0xff && b[1] == 0xd8  &&  b[2] == 0xff)
        return 0;
        
    else
        return 1;
}

int main(int argc, char *argv[])
{
    // check if the argument count is correct
    if(argc != 2)
    {
        fprintf(stderr, "Usage: ./recover image");
        return 1;
    }
    
    // save filename
    FILE *infile = fopen(argv[1], "r");
    if (infile == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", argv[1]);
        return 2;
    }
    
    byte b[512];            // byte buffer
    int filenumber = 0;     // counter for filename
    char filename[10];      // string filename
    FILE *outfile = NULL;   // output file

    // read through the whole file, block by block
    while(fread(b, 512, 1, infile) == 1)
    {
        // does this look like a JPEG
        if(hasJPEGSequence(b) == 0)
        {
            // check if a previous image is already open, if so close it
            if(outfile != NULL)
            {
                fclose(outfile);
                outfile = NULL;
            }
            
            // open a new image file for writing and write the block to it
            sprintf(filename, "%03d.jpg", filenumber++);
            outfile = fopen(filename, "w");
            fwrite(&b, 512, 1, outfile);

        }
        else
        {
            // is an image file already open, if so write the block to it
            if(outfile != NULL)
            {
                fwrite(&b, 512, 1, outfile);
            }
        }
    }
    
    // close all files
    fclose(outfile);
    fclose(infile);
    
    return 0;
}