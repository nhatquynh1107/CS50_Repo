#include "helpers.h"
#include <math.h>
#include <cs50.h>
#include <stdio.h>


// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i =0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int r = image[i][j].rgbtRed;
            int b = image[i][j].rgbtBlue;
            int g = image[i][j].rgbtGreen;
            int avg = round((r + g + b) / 3.0);
            image[i][j].rgbtRed   = avg;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtBlue  = avg;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{ 
    for (int i =0 ; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int r = image[i][j].rgbtRed;
            int b = image[i][j].rgbtBlue;
            int g = image[i][j].rgbtGreen;
            int sr = round(0.393 * r + 0.769 * g + 0.189 * b);
            int sg = round(0.349 * r + 0.686 * g + 0.168 * b);
            int sb = round(0.272 * r + 0.534 * g + 0.131 * b);
            if (sb > 255)
                sb = 255;
            if (sr > 255)
                sr = 255;
            if (sg > 255)
                sg = 255;
            image[i][j].rgbtRed = sr;
            image[i][j].rgbtGreen = sg;
            image[i][j].rgbtBlue = sb;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{   

    for (int i =0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            RGBTRIPLE tmp = image[i][j];
            image[i][j] = image[i][width - j - 1];
            image[i][width - 1 - j] = tmp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE cp[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            cp[i][j] = image[i][j];
        }
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int r = 0, g = 0, b = 0, cnt = 0;
            for (int di = -1; di <= 1; di++)
            {
                for (int dj = -1; dj <= 1; dj++)
                {
                    int ni = i + di;
                    int nj = j + dj;
                    if (ni >= 0 && ni < height && nj >= 0 && nj < width) 
                    {
                        r += cp[ni][nj].rgbtRed;
                        b += cp[ni][nj].rgbtBlue;
                        g += cp[ni][nj].rgbtGreen;
                        cnt ++;
                    }
                }
            }
            int rd = round((double) r /cnt);
            int gd = round((double) g / cnt);
            int bd = round((double) b / cnt);
            image[i][j].rgbtRed = rd;
            image[i][j].rgbtBlue = bd;
            image[i][j].rgbtGreen = gd;
        }
    }

    return;
}
