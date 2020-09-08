#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[])
{
    //checks for the correct number of command line arguments
    if (argc != 2)
    {
        printf("Usuage: ./recover image\n");
        return 1;
    }

    //opens up the file (argv1) the user inputted and reads it
    FILE *file = fopen(argv[1], "r");

    //checks the file is opened correctly
    if (file == NULL)
    {
        fclose(file);
        printf("Memory card could not be read\n");
        return 1;
    }
    //creates a pointer to a space in memory for the recovered file
    FILE *img = NULL;

    //creates the buffer of 512 bytes
    unsigned char buffer[512];

    //creates counter for the number of jpegs
    int njpg = 0;

    //Creates an array to hold the filename string
    char filename[8];

    //Continues to reads the file in buffer size bytes at a time until the end
    while (fread(buffer, sizeof(buffer), 1, file) > 0)
    {
        //Checks if bytes are a new jpg
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xe0) == 0xe0)
        {
            //if not first jpg, it closes the new file beign written
            if (img != NULL)
            {
                fclose(img);
                img = NULL;
            }

            //Creates the filename string for the jpg
            sprintf(filename, "%03i.jpg", njpg);

            //opens a new file with that filename and begins to write it
            img = fopen(filename, "w");

            //writes the data stored in the buffer to the new file
            fwrite(buffer, sizeof(buffer), 1, img);

            //Increases jpg number
            njpg++;
        }
        //if not a new jpg, but a jpg has been found, continues to write
        else if (njpg > 0)
        {
            fwrite(buffer, sizeof(buffer), 1, img);
        }
    }
    fclose(file);
    return 0;
}
