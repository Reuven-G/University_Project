

int skipWhiteChars(char *line, int i)
{
    while (line[i] == ' ' || line[i] == '\t')
    {
        i++;
    }

    return i;
}
