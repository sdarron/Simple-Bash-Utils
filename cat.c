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
            ach = strchr(argv[count], 'b');
            if (ach != NULL)
                my_cat_flags.b_flag = 1;
            ach = strchr(argv[count], 'e');
            if (ach != NULL)
                my_cat_flags.e_flag = 1;
            ach = strchr(argv[count], 'n');
            if (ach != NULL)
                my_cat_flags.n_flag = 1;
            ach = strchr(argv[count], 's');
            if (ach != NULL)
                my_cat_flags.s_flag = 1;
            ach = strchr(argv[count], 't');
            if (ach != NULL)
                my_cat_flags.t_flag = 1;
            if (my_cat_flags.b_flag == 0 && my_cat_flags.e_flag == 0 && 
            my_cat_flags.n_flag == 0 && my_cat_flags.s_flag == 0 &&
                my_cat_flags.t_flag == 0)
            {
                printf("Invalid flag\n"); // сделать нормальный вывод ошибок
                exit(2) ; // ?? агрумент ошибки
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
    int chr_next;
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
                if (my_cat_flags.n_flag)
                    printf("%d ", num++); 
                while((chr = getc(file)) != EOF )
                {
                    if (my_cat_flags.e_flag && chr == '\n')
                        printf("%s", "$"); 
                    fprintf(stdout, "%c", chr_next);
                    if (my_cat_flags.n_flag && chr == '\n')
                        printf("%d ", num++);
                    chr_next = chr; 
                }
                fclose(file);
            }
        }
    }
    exit(0);
}