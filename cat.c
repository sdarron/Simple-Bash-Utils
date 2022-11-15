#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>


typedef struct {
    int b_flag; // нумерует только непустые строки
    int e_flag; // отображает символы конца строки как $
    int n_flag; // нумерует все выходные строки
    int s_flag; // сжимает несколько смежных пустых строк
    int t_flag; // также отображает табы как ^I
} cat_flags;

void print_help()
{
    printf("-b (GNU: --number-nonblank) нумерует только непустые строки\n");
    printf("-e предполагает и -v (GNU only: -E то же самое, но без применения -v)\n");
    printf(" также отображает символы конца строки как $\n");
    printf("-n (GNU: --number) нумерует все выходные строки\n");
    printf("-s (GNU: --squeeze-blank) сжимает несколько смежных пустых строк\n");
    printf("-t предполагает и -v (GNU: -T то же самое, но без применения -v)");
    printf(" также отображает табы как ^I \n");
    exit(0);
}

cat_flags pars_flags(int argc, char *argv[])
{
    cat_flags my_cat_flags = {0, 0, 0, 0, 0};

    char number_nonblank[20] = "--number-nonblank\0";
    char number[9] = "--number\0";
    char squeeze[20] = "--squeeze-blank\0";
    char help[7] = "--help\0";
  
    for(int count = 1; count < argc; count++ )
    { 
        if( strcmp(argv[count],help) == 0)
            print_help();
        if( strcmp(argv[count], number_nonblank) == 0)
            my_cat_flags.b_flag = 1;
        if( strcmp(argv[count], number) == 0 )
            my_cat_flags.n_flag = 1;
        if( strcmp(argv[count], squeeze) == 0 )
            my_cat_flags.s_flag = 1;
        if   ( my_cat_flags.b_flag == 1 || my_cat_flags.n_flag == 1 
                || my_cat_flags.s_flag == 1 )
            continue;
        if( argv[count][0] == '-' )
        {
            for(int n = 1; argv[count][n]; n++)
            {
                if ( argv[count][n] == 'b')
                    my_cat_flags.b_flag = 1;
                else if ( argv[count][n] == 'e' || argv[count][n] == 'E' )
                    my_cat_flags.e_flag = 1;
                else if ( argv[count][n] == 'n' )
                    my_cat_flags.n_flag = 1;
                else if ( argv[count][n] == 's' )
                    my_cat_flags.s_flag = 1;
                else if ( argv[count][n] == 't' || argv[count][n] == 'T' )
                    my_cat_flags.t_flag = 1; 
                else 
                {
                    printf("cat: invalid option -- %c\n", argv[count][n]);
                    printf("Try 'cat --help' for more information.\n"); // доделать help
                    exit(EXIT_FAILURE);
                }
            }   
        }

    }
    return my_cat_flags;
}

int main(int argc, char *argv[])
{
    FILE *file;

    int num = 1;
    int chr;
    int chr_to;

    cat_flags my_cat_flags = {0, 0, 0, 0, 0};
    my_cat_flags = pars_flags(argc, argv);
    for(int count = 1; count < argc; count++)
    {
        if (argv[count][0] != '-')
        {
            if(( file = fopen(argv[count], "r")) == NULL )
            {
                fprintf(stderr, "%s: %s : %s \n", argv[0], argv[count], 
                    strerror(errno));
                continue;
            }
            else
            {
                while((chr = getc(file)) != EOF )
                {
                    if (my_cat_flags.s_flag && chr_to == '\n' && chr == '\n')
                        continue ;
                    if ((my_cat_flags.n_flag || my_cat_flags.b_flag) && chr && num == 1 )
                        printf("%d ", num++);
                    if (my_cat_flags.n_flag && chr_to == '\n')
                    {
                        printf("%d ", num++);
                        chr_to = 0;
                    }
                    if (my_cat_flags.e_flag && chr == '\n')
                        printf("%s", "$");
                    if (my_cat_flags.b_flag && chr != '\n' && chr_to == '\n' )
                    {
                        printf("%d ", num++);
                         chr_to = 0;
                    }
                    if (my_cat_flags.t_flag && chr == 9)
                        fprintf(stdout, "%s", "^I");
                    fprintf(stdout, "%c", chr);
                    chr_to = chr;
                }
                fclose(file);
            }
        }
    }
    exit(0);
}