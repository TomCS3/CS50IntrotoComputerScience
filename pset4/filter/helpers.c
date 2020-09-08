#include "helpers.h"
#include <math.h>

int in_bounds(int i, int j, int height, int width);

//custom structure to contain the rgb values
typedef struct rgb
{
    int b;
    int g;
    int r;
}
rgb;

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int average = round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / (float)3);
            image[i][j].rgbtBlue = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtRed = average;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / (float) 2; j++)
        {
            if (j == width - j)
            {
                break;
            }

            else
            {
                rgb temp = {image[i][j].rgbtBlue, image[i][j].rgbtGreen, image[i][j].rgbtRed};

                image[i][j].rgbtBlue = image[i][width - 1 - j].rgbtBlue;
                image[i][j].rgbtGreen = image[i][width - 1 - j].rgbtGreen;
                image[i][j].rgbtRed = image[i][width - 1 - j].rgbtRed;

                image[i][width - 1 - j].rgbtBlue = temp.b;
                image[i][width - 1 - j].rgbtGreen = temp.g;
                image[i][width - 1 - j].rgbtRed = temp.r;
            }
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    //temporary image to store blurred pixels
    RGBTRIPLE image2[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            rgb blur = {0, 0, 0};
            float n = 0;

            if (in_bounds(i - 1, j - 1, height, width) == 0)
            {
                blur.b += image[i - 1][j - 1].rgbtBlue;
                blur.g += image[i - 1][j - 1].rgbtGreen;
                blur.r += image[i - 1][j - 1].rgbtRed;
                n++;
            }
            if (in_bounds(i - 1, j, height, width) == 0)
            {
                blur.b += image[i - 1][j].rgbtBlue;
                blur.g += image[i - 1][j].rgbtGreen;
                blur.r += image[i - 1][j].rgbtRed;
                n++;
            }
            if (in_bounds(i - 1, j + 1, height, width) == 0)
            {
                blur.b += image[i - 1][j + 1].rgbtBlue;
                blur.g += image[i - 1][j + 1].rgbtGreen;
                blur.r += image[i - 1][j + 1].rgbtRed;
                n++;
            }
            if (in_bounds(i, j - 1, height, width) == 0)
            {
                blur.b += image[i][j - 1].rgbtBlue;
                blur.g += image[i][j - 1].rgbtGreen;
                blur.r += image[i][j - 1].rgbtRed;
                n++;
            }

            blur.b += image[i][j].rgbtBlue;
            blur.g += image[i][j].rgbtGreen;
            blur.r += image[i][j].rgbtRed;
            n++;

            if (in_bounds(i, j + 1, height, width) == 0)
            {
                blur.b += image[i][j + 1].rgbtBlue;
                blur.g += image[i][j + 1].rgbtGreen;
                blur.r += image[i][j + 1].rgbtRed;
                n++;
            }
            if (in_bounds(i + 1, j - 1, height, width) == 0)
            {
                blur.b += image[i + 1][j - 1].rgbtBlue;
                blur.g += image[i + 1][j - 1].rgbtGreen;
                blur.r += image[i + 1][j - 1].rgbtRed;
                n++;
            }
            if (in_bounds(i + 1, j, height, width) == 0)
            {
                blur.b += image[i + 1][j].rgbtBlue;
                blur.g += image[i + 1][j].rgbtGreen;
                blur.r += image[i + 1][j].rgbtRed;
                n++;
            }
            if (in_bounds(i + 1, j + 1, height, width) == 0)
            {
                blur.b += image[i + 1][j + 1].rgbtBlue;
                blur.g += image[i + 1][j + 1].rgbtGreen;
                blur.r += image[i + 1][j + 1].rgbtRed;
                n++;
            }

            blur.b = round(blur.b / n);
            blur.g = round(blur.g / n);
            blur.r = round(blur.r / n);

            image2[i][j].rgbtBlue = blur.b;
            image2[i][j].rgbtGreen = blur.g;
            image2[i][j].rgbtRed = blur.r;
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtBlue = image2[i][j].rgbtBlue;
            image[i][j].rgbtGreen = image2[i][j].rgbtGreen;
            image[i][j].rgbtRed = image2[i][j].rgbtRed;
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    //temporary image to store the edges
    RGBTRIPLE image2[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            rgb edge = {0, 0, 0};
            rgb gx = {0, 0, 0};
            rgb gy = {0, 0, 0};

            if (in_bounds(i - 1, j - 1, height, width) == 0)
            {
                gx.b += image[i - 1][j - 1].rgbtBlue * -1;
                gx.g += image[i - 1][j - 1].rgbtGreen * -1;
                gx.r += image[i - 1][j - 1].rgbtRed * -1;

                gy.b += image[i - 1][j - 1].rgbtBlue * -1;
                gy.g += image[i - 1][j - 1].rgbtGreen * -1;
                gy.r += image[i - 1][j - 1].rgbtRed * -1;
            }

            if (in_bounds(i - 1, j, height, width) == 0)
            {
                gy.b += image[i - 1][j].rgbtBlue * -2;
                gy.g += image[i - 1][j].rgbtGreen * -2;
                gy.r += image[i - 1][j].rgbtRed * -2;

            }
            if (in_bounds(i - 1, j + 1, height, width) == 0)
            {
                gx.b += image[i - 1][j + 1].rgbtBlue * 1;
                gx.g += image[i - 1][j + 1].rgbtGreen * 1;
                gx.r += image[i - 1][j + 1].rgbtRed * 1;

                gy.b += image[i - 1][j + 1].rgbtBlue * -1;
                gy.g += image[i - 1][j + 1].rgbtGreen * -1;
                gy.r += image[i - 1][j + 1].rgbtRed * -1;
            }
            if (in_bounds(i, j - 1, height, width) == 0)
            {
                gx.b += image[i][j - 1].rgbtBlue * -2;
                gx.g += image[i][j - 1].rgbtGreen * -2;
                gx.r += image[i][j - 1].rgbtRed * -2;
            }
            if (in_bounds(i, j + 1, height, width) == 0)
            {
                gx.b += image[i][j + 1].rgbtBlue * 2;
                gx.g += image[i][j + 1].rgbtGreen * 2;
                gx.r += image[i][j + 1].rgbtRed * 2;
            }
            if (in_bounds(i + 1, j - 1, height, width) == 0)
            {
                gx.b += image[i + 1][j - 1].rgbtBlue * -1;
                gx.g += image[i + 1][j - 1].rgbtGreen * -1;
                gx.r += image[i + 1][j - 1].rgbtRed * -1;

                gy.b += image[i + 1][j - 1].rgbtBlue * 1;
                gy.g += image[i + 1][j - 1].rgbtGreen * 1;
                gy.r += image[i + 1][j - 1].rgbtRed * 1;
            }
            if (in_bounds(i + 1, j, height, width) == 0)
            {
                gy.b += image[i + 1][j].rgbtBlue * 2;
                gy.g += image[i + 1][j].rgbtGreen * 2;
                gy.r += image[i + 1][j].rgbtRed * 2;
            }
            if (in_bounds(i + 1, j + 1, height, width) == 0)
            {
                gx.b += image[i + 1][j + 1].rgbtBlue * 1;
                gx.g += image[i + 1][j + 1].rgbtGreen * 1;
                gx.r += image[i + 1][j + 1].rgbtRed * 1;

                gy.b += image[i + 1][j + 1].rgbtBlue * 1;
                gy.g += image[i + 1][j + 1].rgbtGreen * 1;
                gy.r += image[i + 1][j + 1].rgbtRed * 1;
            }

            edge.b = round(sqrt(pow(gx.b, 2) + pow(gy.b, 2)));
            if (edge.b > 255)
            {
                edge.b = 255;
            }
            edge.g = round(sqrt(pow(gx.g, 2) +  pow(gy.g, 2)));
            if (edge.g > 255)
            {
                edge.g = 255;
            }
            edge.r = round(sqrt(pow(gx.r, 2) + pow(gy.r, 2)));
            if (edge.r > 255)
            {
                edge.r = 255;
            }

            image2[i][j].rgbtBlue = edge.b;
            image2[i][j].rgbtGreen = edge.g;
            image2[i][j].rgbtRed = edge.r;
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtBlue = image2[i][j].rgbtBlue;
            image[i][j].rgbtGreen = image2[i][j].rgbtGreen;
            image[i][j].rgbtRed = image2[i][j].rgbtRed;
        }
    }
    return;
}

//Checks if pixel is inbounds of the image
int in_bounds(int i, int j, int height, int width)
{
    if (i >= 0 && i <= height - 1 && j >= 0 && j <= width - 1)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}
