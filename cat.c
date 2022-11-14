#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

typedef struct {
    int b_flag;
    int e_flag;
    int n_flag;
    int s_flag;
    int t_flag;
} cat_flags;

void print_b_flag(); // нумерует только непустые строки
void print_e_flag(); // отображает символы конца строки как $
void print_n_flag(); // нумерует все выходные строки
void print_s_flag(); // сжимает несколько смежных пустых строк
void print_t_flag(); // также отображает табы как ^I

cat_flags pars_flags(int argc, char *argv[])
{
    cat_flags my_cat_flags = {0, 0, 0, 0, 0};
    char *ach;

    for(int count = 1; count < argc; count++)
    {
        if( argv[count][0] == '-' )
        {
            for(int n = 1; argv[count][n]; n++)
            {
                if ( argv[count][n] == 'b')
                   my_cat_flags.b_flag = 1;
                else if ( argv[count][n] == 'e' )
                    my_cat_flags.e_flag = 1;
                else if ( argv[count][n] == 'n' )
                    my_cat_flags.n_flag = 1;
                else if ( argv[count][n] == 's' )
                    my_cat_flags.s_flag = 1;
                else if ( argv[count][n] == 't' )
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
    int flag;
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
                    if ((my_cat_flags.n_flag || my_cat_flags.b_flag) && chr && num == 1 )
                        printf("%d ", num++);
                    if (my_cat_flags.e_flag && chr == '\n')
                        printf("%s", "$"); 
                    if (my_cat_flags.n_flag && chr_to == '\n')
                    {
                        printf("%d ", num++);
                        chr_to = 0;
                    }
                    if (my_cat_flags.b_flag && chr != '\n' && chr_to == '\n' )
                    {
                        printf("%d ", num++);
                         chr_to = 0;
                    }
                    if (my_cat_flags.t_flag && chr == 9)
                        fprintf(stdout, "%s", "^I");
                    else
                    {
                        fprintf(stdout, "%c", chr);
                        chr_to = chr;
                    }
                }
                fclose(file);
            }
        }
    }
    exit(0);
}