#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <omp.h>

struct Stack { 			//δημιουργία στοίβας
    int top; 
    unsigned capacity; 
    int* array; 
}; 

struct Stack* createStack(unsigned capacity) //προσδιορισμός του μεγέθους της στοίβας
{ 
    struct Stack* stack = (struct Stack*)malloc(sizeof(struct Stack)); 
    stack->capacity = capacity; 
    stack->top = -1; 
    stack->array = (int*)malloc(stack->capacity * sizeof(int)); 
    return stack; 
} 
int isFull(struct Stack* stack) 
{
    return stack->top == stack->capacity - 1; 
} 
  
int isEmpty(struct Stack* stack) 
{  
	if(stack->top == -1){
        return 1;
    }else return 0;
}

  
void push(struct Stack* stack, int item)  	//τοποθέτηση δεδομένου στην στοίβα
{ 
    if (isFull(stack)) 
        return; 
    stack->array[++stack->top] = item; 
	//printf("%d\n", item); 
} 
  
int pop(struct Stack* stack) 	//απομάκρυνση δεδομένου από την στοίβα
{ 
    if (isEmpty(stack)) 
        return INT_MIN; 
    return stack->array[stack->top--]; 
} 
  
int peek(struct Stack* stack) // εμφάνιση του τελευταιου προστεθιμένου δεδομένου στην στοίβα
{ 
    if (isEmpty(stack)) 
        return INT_MIN; 
    return stack->array[stack->top]; 
} 

struct Node {    //δημιουργία λίστας
    int val; 
    struct Node* next; 
}; 
  
void print_list(struct Node * head) {	//εμφάνιση λίστας
    struct Node * current = head;
	if (current== NULL)
		printf("Adeia lista\n");
	else
    	while (current != NULL) {
        	printf("%d  ", current->val);
        	current = current->next;
		
    }
    printf("\n ");
}
void pushN(struct Node * n, int val) {		// προσθήκη δεδομένου στην λίστα
    struct Node * current = n ;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = (struct Node *) malloc(sizeof(struct Node));
    current->next->val = val;
    current->next->next = NULL;
}



void create_tables (const char* file_name) //Συνάρτηση Create Table
{
    FILE* file = fopen (file_name, "r");
    int rows = 0;
    int collumns = 0;
    int lines = 0;  
    int i = 0;
    int j = 0;
    fscanf (file, "%d %d %d", &rows,&collumns,&lines);
    printf ("%d %d %d\n", rows,collumns,lines);
    printf("\n\n");
    int ver_table[rows][collumns];
    for(i = 0 ; i < rows ; i++){               //Προσδιορισμός του πίνακα γειτνίασης
        for(j = 0 ; j < collumns ; j++){
            ver_table[i][j] = 0;
        }
    }
    while (!feof (file))    // Ανάγνωση του αρχείου
    {
        fscanf (file, "%d %d\n", &i,&j);
        ver_table[i-1][j-1] = 1;
    }
   /* for(i = 0 ; i < rows ; i++){                //Εμφάνιση του πίνακα γειτνίασης
        for(j = 0 ; j < collumns ; j++){
            printf("%d ",ver_table[i][j]);
        }
        printf("\n");
    }
    printf("\n\n");*/
    fclose (file);
    
    int incoming[2][rows];                  // Προσδιορισμός του πίνακα των εισερχόμενων ακμών     

    for( i = 0 ; i < rows ; i++){               
            incoming[0][i] = i+1;
            incoming[1][i] = 0;
    }
    for( i = 0 ; i < rows ; i++ ){
        for( j = 0 ; j < collumns ; j++ ){
            if(ver_table[i][j] == 1){
                incoming[1][j]++;
            }
        }
    }
    for(i = 0 ; i < rows ; i++){    		//Εμφάνιση του πίνακα εισερχόμενων ακμών
	 
            printf("%d ",incoming[0][i]);
            printf("%d\n",incoming[1][i]);
    }
	printf("\n\n");	
	
	
	int item, check=0, count=0;	
    struct Node* L = NULL;					//δημιουργία S L
	L= (struct Node *) malloc(sizeof(struct Node));
	if (L == NULL) 
    	printf("Den brethike mnhmh");
	L->next= NULL;
	
	struct Stack* S = createStack(rows);

  	omp_set_num_threads(32);
	#pragma omp parallel 
	for (i=0; i<rows; i++){
		if (incoming[1][i]==0)
			push(S, i+1);
		}
		
   	while (!isEmpty(S))                
	{	
		item=peek(S);
		pop(S);
		pushN(L, item);
				
		#pragma omp parallel for
        for(i = 0 ; i < rows ; i++)          //Επαναπροσδιορισμός του πίνακα γειτνίασης
           	ver_table[item-1][i]=0; 
	 	
	 	ver_table[item-1][item-1]=-1;
	 	
      	#pragma omp parallel for
        for( i = 0 ; i < rows ; i++){ 		//Επαναπροσδιορισμός του πίνακα των εισερχόμενων ακμών               
           	incoming[0][i] = i+1;
           	incoming[1][i] = 0;
   		}

       #pragma omp for collapse(2)
    	for( i = 0 ; i < rows ; i++ ){
       		for( j = 0 ; j < collumns ; j++ ){
           		if(ver_table[i][j] == 1)
               		incoming[1][j]++;
               	else if (ver_table[i][j] == -1)
               		incoming[1][j]=-1;
           		}
       		}

	
	    /*for(i = 0 ; i < rows ; i++){                //Εμφάνιση του πινακα γειτνίασης
       		 for(j = 0 ; j < collumns ; j++){
            	printf("%d ",ver_table[i][j]);
        }
        printf("\n");
    	}
    	
       
   		for(i = 0 ; i < rows ; i++){                //Εμφάνιση του πίνακα εισερχόμενων ακμών
	       	printf("%d ",incoming[0][i]);
           	printf("%d\n",incoming[1][i]);
       	}*/
        
       #pragma omp single
   		for (i=0; i<rows; i++){
	 		if (incoming[1][i]==0)
		 		push(S, i+1);
		}
		//printf("\n\n");	
		count++;
		if (count==rows) goto label;
	}
	
	label:
    #pragma omp parallel for
	for (i=0; i<rows; i++)	 		// έλεγχος για κύκλο
	{ 
		if (incoming[1][i]!=-1)
			check=1;
	}
	
	if (check==1) 
		printf("To grafhma exei kyklo");
	else 
	{	printf("H topologikh diataksh twn kombwn einai: \n");
		print_list(L); 
  	}
}

void main(){
    create_tables("dag");    //Κλήση Create Table
   
}
