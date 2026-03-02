#include <stdio.h>
#include <string.h>
#define MAXSIZE 1024

int match(const char *pattern, const char *text)
{
    int pattern_length;
    int text_length;
    int longest_suit_len;
    int longest_suit_position;
    int temp_suit_len;
    int i, z;
    
    pattern_length = strlen(pattern);
    text_length = strlen(text);
    longest_suit_len = 0;
    longest_suit_position = 0;
    
    for (i=0 ;i<=(text_length-pattern_length); i++) /* Loop to check the whole text */
    {
        temp_suit_len = 0;
        
        for (z=0 ;z<pattern_length; z++) /* Loop to suitability between the given text and the pattern */
        {
            if (pattern[z]==text[(i+z)]) /* Compare */
            {
                temp_suit_len++;
            }
        }
        if (temp_suit_len > longest_suit_len) /* Check if the last string has the best suitability */
        {
            longest_suit_len = temp_suit_len;
            longest_suit_position = i;
        }
    }
    return longest_suit_position;
}

int main()
{
    char pattern[MAXSIZE];
    char text[MAXSIZE];
    int longest_suit_position;
    
    printf("Enter your pattern \n");
    fgets(pattern, MAXSIZE, stdin); /* Wait for user input */
    if ((strlen(pattern) > 0) && (pattern[strlen(pattern)-1] == '\n')) /* Removing the newline from pattern */
    {
        pattern[strlen(pattern)-1] = '\0';
    }
    
    printf("Enter your text \n");
    fgets(text, MAXSIZE, stdin); /* Wait for user input */
    if ((strlen(text) > 0) && (text[strlen(text)-1] == '\n')) /* Removing the newline from text */
    {
        text[strlen(text)-1] = '\0';
    }
    
    
    printf("The pattern is: %s\n", pattern);
    printf("The text is: %s\n", text);
    printf("Loading...\n\n");
    
    longest_suit_position = match(pattern, text);
    
    /* Printing the result */
    printf("The pattern is: %s\n", pattern);
    printf("The text is: %s\n", text);
    printf("The position is: %d\n", longest_suit_position);
    
    return 0;  
}
