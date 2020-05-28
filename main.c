#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <omp.h>

void create_tables (const char* file_name) //Ανάγωνση του αρχείου εισόδου και δημιουργία πίνακα γειτνίασης και βαθμων εισόδου κόμβων.
{
    FILE* file = fopen (file_name, "r");
    int rows = 0;
    int collumns = 0;
    int lines = 0;  
    int i = 0;
    int j = 0;
    fscanf (file, "%d %d %d", &rows,&collumns,&lines);
    printf ("%d %d %d\n", rows,collumns,lines);
    int ver_table[rows][collumns];
    for(i = 0 ; i < rows ; i++){               //Αρχικοποίηση του πίνακα με τις διαστάσεις απο το αρχείο εισόδου
        for(j = 0 ; j < collumns ; j++){
            ver_table[i][j] = 0;
        }
    }
    while (!feof (file))    //Δημιουργία του πίνακα γειτνίασης με 1 στους κόμβους που συνδέονται
    {
        fscanf (file, "%d %d\n", &i,&j);
        ver_table[i-1][j-1] = 1;
    }
    for(i = 0 ; i < rows ; i++){                //Εμφάνιση πίνακα γειτνίασης
        for(j = 0 ; j < collumns ; j++){
            printf("%d ",ver_table[i][j]);
        }
        printf("\n");
    }
    fclose (file);
    int incoming[2][rows];                  //Δημιουργία πίνακα με βαθμούς εισόδου για κάθε κόμβο
    for(int i = 0 ; i < rows ; i++){               
            incoming[0][i] = i+1;
            incoming[1][i] = 0;
    }
    for(int i = 0 ; i < rows ; i++ ){
        for(int j = 0 ; j < rows ; j++ ){
            if(ver_table[i][j] == 1){
                incoming[1][j]++;
            }
        }
    }
    for(int i = 0 ; i < rows ; i++){     
            printf("%d ",incoming[0][i]);
            printf("%d\n",incoming[1][i]);
    }
}

void main(){
    create_tables("Tina_AskCal.mtx");    //Κλήση της Create Table
}