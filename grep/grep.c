
#define _GNU_SOURCE 1		/* for getline() */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <regex.h>
#include <unistd.h>
#include <sys/types.h>

char *myname;			/* for error messages */
int ignore_case = 0;		/* -i option: ignore case */
int extended = 0;		/* -E option: use extended RE's */
int errors = 0;			/* number of errors */

regex_t pattern;		/* pattern to match */


typedef struct {
    int i_flag; 
    int e_flag; 
    int v_flag; 
    int c_flag; 
    int l_flag;
    int n_flag; 
} grep_flags;

void print_help()
{
    printf("-e Шаблон\n");
    printf("-i Игнорирует различия регистра.\n");
    printf("-v Инвертирует смысл поиска соответствий.\n");
    printf("-c Выводит только количество совпадающих строк.\n");
    printf("-l Выводит только совпадающие файлы.\n");
    printf("-n Предваряет каждую строку вывода номером строки из файла ввода.\n");
    exit(0);
}


grep_flags pars_flags(int argc, char *argv[])
{
    grep_flags my_grep_flags = {0, 0, 0, 0, 0, 0};
    char help[7] = "--help\0";
  
    for(int count = 1; count < argc; count++ )
    { 
        if( strcmp(argv[count],help) == 0)
        {
            print_help();
            continue;
        }
        if( argv[count][0] == '-' )
        {
            for(int n = 1; argv[count][n]; n++)
            {
                if ( argv[count][n] == 'i')
                    my_grep_flags.i_flag = 1;
                else if ( argv[count][n] == 'e')
                    my_grep_flags.e_flag = 1;
                else if ( argv[count][n] == 'v' )
                    my_grep_flags.v_flag = 1;
                else if ( argv[count][n] == 'c' )
                    my_grep_flags.c_flag = 1;
                else if ( argv[count][n] == 'l' )
                    my_grep_flags.l_flag = 1;
                else if ( argv[count][n] == 'n' )
                    my_grep_flags.n_flag = 1;  
                else 
                {
                    printf("grep: invalid option -- %c\n", argv[count][n]);
                    printf("Try 'grep --help' for more information.\n");
                    exit(EXIT_FAILURE);
                }
            }   
        }

    }
    return my_grep_flags;
}

void compite_pattern(const char *pat)
{
    int flags = REG_NOSUB; /* don't need where-matched info */
    int ret;

    #define MSGBUFSIZE 512 /* arbintrary */
    char error[MSGBUFSIZE];

    if (ignore_case)
        flags |= REG_ICASE;
    if (extended)
        flags |= REG_EXTENDED;
    ret = regcomp(& pattern, pat, flags);
    if (ret != 0){
        (void) regerror(ret, & pattern, error, sizeof error);
        fprintf(stderr, "%s: pattern `%s': %s\n", myname, pat, error);
        errors++;
    }
}

void process(const char *name, FILE *fp)
{
    char *buf = NULL;
    size_t size = 0;
    char error[MSGBUFSIZE];
    int ret;

    while (getline(& buf, &size, fp) != -1){
        ret = regexec(& pattern, buf, 0, NULL, 0);
        if (ret != 0){
            if (ret != REG_NOMATCH){
                (void) regerror(ret, & pattern, error, sizeof error);
                fprintf(stderr, "%s: file %s: %s\n", myname, name, error);
                free(buf);
                errors++;
                return;
            }
        } else
            printf("%s: %s", name, buf); /* print matching lines */
    }
    free(buf);
}

/* usage --- print usage message and exit */
void usage(void)
{
    fprintf(stderr, "usage: %s [-i] [-E] pattern [ files ...]\n", myname);
    exit(1);
}

int main(int argc, char *argv[])
{
    FILE *fp;
    FILE *file;
   // int c;
    int i;
   // char *myname; /* for error messages */

 //   grep_flags my_grep_flags = {0, 0, 0, 0, 0, 0};
 //   my_grep_flags = pars_flags(argc, argv);
    for(int count = 1; count < argc; count++)
    {
        if (argv[count][0] != '-')
        {
             if(( file = fopen(argv[count], "r")) == NULL )
            // {
            //     fprintf(stderr, "%s: %s : %s \n", argv[0], argv[count], 
            //         strerror(errno));
                 continue;
            // }
             else
             {
                myname = argv[0];
             //   while((c = getopt(argc, argv, ":iE")) != -1 )
             //   {
                    if (optind == argc) /* sanity check */
                        usage();
                    compite_pattern(argv[optind]); /* compile the pattern */
                    if (errors) /* compile failed */
                        return 1;
                    else
                        optind++;
                    if(optind == argc) /* no files, default to stdin */
                        process("standard input", stdin);
                    else{ 
                        /* loop over files*/
                        for (i = optind; i < argc; i++){
                            if (strcmp(argv[i], "-") == 0)
                                process("standard input", stdin);
                            else if ((fp = fopen(argv[i], "r")) != NULL){
                                process(argv[i], fp);
                                fclose(fp);
                            } else {
                                fprintf(stderr, "%s: %s: could not open: %s\n",
                                    argv[0], argv[i], strerror(errno));
                                    errors++;
                            }
                        }
                    }
            }
        }
    }

   regfree(& pattern);
   return errors != 0;
}