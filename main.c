#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/** \brief Split line to words by separator
 *
 * \param	p1 Line string
 * \param	p2 Separator
 * \return	Word
 *
 * Reads line and split the words
 */
char *split_words (char * str, char const *separator)
{
    static char  *source = NULL;
    char  *p,  *ret = 0;

    if (str != NULL)
        source = str;

    if (source == NULL || *source == '\0')
        return NULL;

    ret = source;
    if ((p = strpbrk(source, separator)) != NULL)
    {
        *p  = 0;
        source = ++p;
    }
    else
        source += strlen(source);

    return ret;
}

/** \brief Reads file to string
 *
 * \param	p1 File object.
 * \return	String object that the file read into.
 *
 * Reads file to string and returns it.
 */
char *file_to_buffer(FILE *file)
{
    char * buffer;
    long fileLength;

    fseek (file, 0, SEEK_END); //go end of file
    fileLength = ftell (file);
    fseek (file, 0, SEEK_SET); //go start of ile
    buffer = malloc (fileLength); //assign buffer space
    //read file to buffer
    if (buffer)
        fread (buffer, 1, fileLength, file);

    fclose (file);

    return buffer;
}

/** \brief  Lowercase all characters and change space to underscore
 *
 * \param	p1 String file.
 *
 */
int lower_underscore(char *str)
{
    int i=0;

    while(str[i])
    {
        if (str[i]==' ')
            str[i]='_';
        else
            str[i]=tolower(str[i]);
        i++;
    }
    return 1;
}

int main (int argc, char *argv[])   //  command line arguments
{
//////////////////CONSTANTS////////////////////////////////////////////////

    const char *COMMA=",";
    const char *TAB="\t";
    const char *SEMICOLON=";";

    const char *WINDOWS="\r\n";
    const char *LINUX="\n";
    const char *MACOS="\r";

/////////////////// VARIABLES ///////////////////////////////////////////////
    char const *separator, *opsys;
    FILE *inputFile, *outputFile;
    char *outputFileName;
/////////////////// ARGUMENTS CONTROL ///////////////////////////////////////
    bool argument_fail = false;

    switch(argc) //switch argument count
    {
    case 2: //for help argument
        if(strcmp(argv[1], "-h") == 0) // only if -h
        {
            printf("Usage:\n");
            printf("%s <inputfile> <outputfile> [-separator <P1>][-opsys <P2>][-h]\n\n",argv[0]);
            printf("-separator  defines separator between each datum (1=comma, 2=tab, 3=semicolon)\n");
            printf("-opsys      defines the end of line character format (1=windows, 2=linux, 3=macos)\n");
            printf("-h          print information to screen about how other arguments can be used.\n\n");
            printf("Example: %s Contacts.csv Contacts.xml -separator 1 -opsys 1",argv[0]);
            exit(EXIT_SUCCESS);
        }
        else
            argument_fail=true;
        break;
    case 7: //for normal usage
        //Check input file exists and can be opened
        inputFile = fopen(argv[1], "r");

        if (inputFile == NULL)
        {
            printf("Could not open the input file. Check if file exists or not opened by other application.\n");
            exit(EXIT_FAILURE);
        }
        //Check output file exists-create if not, clean otherwise
        outputFile = fopen(argv[2], "w+");
        //seperator assignment
        if(strcmp(argv[3], "-separator") == 0)
        {
            switch(atoi(argv[4]))
            {
            case 1:
                separator=COMMA;
                break;
            case 2:
                separator=TAB;
                break;
            case 3:
                separator=SEMICOLON;
                break;
            default:
                argument_fail=true;
            }
        }
        else
            argument_fail=true;
        //opsys assignment
        if(strcmp(argv[5], "-opsys") == 0)
        {
            switch(atoi(argv[6]))
            {
            case 1:
                opsys=WINDOWS;
                break;
            case 2:
                opsys=LINUX;
                break;
            case 3:
                opsys=MACOS;
                break;
            default:
                argument_fail=true;
            }
        }

        else
            argument_fail=true;

        //get name of outputfile for xml root
        outputFileName = argv[2];
        for(int i=0; i<strlen(outputFileName); i++)
        {
            if(outputFileName[i] == '.')
            {
                outputFileName[i]='\0';
                break;
            }
        }

        lower_underscore(outputFileName);

        break;
    default: //invalid number of arguments
        argument_fail=true;
    }

    if (argument_fail)
    {
        printf("Argument error. Type -h for help.");
        exit(EXIT_FAILURE);
    }

    //Read file to buffer
    char *fileBuffer = file_to_buffer(inputFile);

    //Split buffer and write to xml
    char *line;
    char buf[512];
    int row_id =0;
    int column_count = 1;
    int row_count = 0;
    int i=0;

    fprintf(outputFile,"<%s>\n",outputFileName);

    //find number of columns and rows
    for(int i=0; i<strlen(fileBuffer); i++)
    {
        if(fileBuffer[i]!=opsys[0])
        {
            if (fileBuffer[i] == separator[0])
                column_count++;
        }
        else
            row_count++;
    }
    //create element array
    char elements[column_count][100];

    //Split file to lines according to operating system
    for (line = strtok (fileBuffer, opsys); line != NULL; line = strtok (line + strlen (line) + 1, opsys))
    {
        strncpy (buf, line, sizeof (buf)); //copy line to buffer for splitting to words

        char *word = split_words(buf, separator);//split words check double separators

        i=0;//follow the column number

        while (word)
        {
            //find element names at first row
            if(row_id == 0)
            {
                lower_underscore(word);
                strcpy(elements[i],word);
            }
            else
            {
                //if word empty <xx/> else normal
                if(*word)
                    fprintf(outputFile,"\t\t<%s>%s</%s>\n",elements[i],word,elements[i]);
                else
                    fprintf(outputFile,"\t\t<%s/>\n",elements[i]);
            }
            i++;

            word = split_words (NULL, separator);
        }

        //check if there's last separator
        if(line[strlen(line)-1] == separator[0])
            fprintf(outputFile,"\t\t<%s/>\n",elements[i]);

        //close row if not first row
        if(row_id!=0)
            fprintf(outputFile,"\t</row>\n");

        row_id++; //increase row count

        if(row_id!=row_count)
            fprintf(outputFile,"\t<row id=\"%d\">\n",row_id); //add new row
    }

    fprintf(outputFile,"</%s>\n",outputFileName);//close root

    fclose(outputFile);

    printf("XML file created successfully !");

    return EXIT_SUCCESS;
}
