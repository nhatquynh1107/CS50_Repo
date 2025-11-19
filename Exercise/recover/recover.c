    #include <stdio.h>
    #include <stdlib.h>
    #include <stdint.h>
    #include<cs50.h>

    int cnt = 0;

    int main(int argc, char *argv[])
    {

    if (argc != 2)
    {
        printf("Usage: ./recover FILE\n");
        return 1;
    }

    FILE *card = fopen(argv[1],"r");
    FILE *a = NULL ;

    if (card == NULL)
    {
        printf("Could not open file\n");
        return 1;
    }

    uint8_t buffer[512];
    char name[8];

    while (fread(buffer, 512, 1, card) == 1)
    {
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0 )
        {
            if (a != NULL)
            {
                fclose(a);
            }
            sprintf(name, "%03i.jpg", cnt);
            a = fopen(name,"w");
            cnt ++;
        }
        if (a != NULL)
            fwrite(buffer, 512, 1, a);
    }

    if (a != NULL)
        fclose(a);
    fclose(card);
    return 0;
    }