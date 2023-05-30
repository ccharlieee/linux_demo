#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 80

int main()
{
    char *s19 = "S00600004844521B\n"
                "S1130000285F245F2212226A000424290008237C2A\n"
                "S11300100002000800082629001853812341001813\n"
                "S113002041E900084E42234300182342000824A952\n"
                "S107003000144ED492\n"
                "S5030004F8\n"
                "S9030000FC\n";
    char *p = s19;
    int i = 0;
    char buf[MAX_LINE_LENGTH];
    while (*p != '\0')
    {
        if (*p == '\n')
        {
            buf[i] = '\0';
            if (buf[0] != 'S' || strlen(buf) % 2 != 1)
            {
                printf("Error: %s is not a valid S19 record.\n", buf);
                return -1;
            }
            i = 0;
        }
        else
        {
            buf[i++] = *p;
        }
        p++;
    }
    return 0;
}
