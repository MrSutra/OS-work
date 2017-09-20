#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <time.h>

#define MAX 10000

int m1[MAX][MAX],m2[MAX][MAX], aux[MAX][MAX];
int **x, **y;
clock_t begin, end;
double time_spent;

int main(int argc, char *argv[]){
    begin = clock();
    MPI_Status status;

    int my_id, num_procs, rows,size, i, j, low,high,offset;
    FILE *suma, *matrices;

    /* tamaño matrices */
    matrices = fopen("matrices.txt","r");    
    fscanf(matrices,"%d",&size);

    /* Replicar proceso */
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);


    /* indices de distribucion */
    if(size % 2 ==0){
        rows = size / num_procs;
        low=my_id*rows;
        high=low+rows;
    }
    else{
        rows = size / num_procs;
        low=my_id*rows;
        high=low+rows+1;
        if(my_id==0){
            high--;
        }
    }


    /* proceso padre */
    if (my_id == 0){
        
        /* leer matrices */
        x = (int**) malloc(size*sizeof(int*));
        for(i = 0; i < size; i++){
            x[i] = (int*) malloc(size*sizeof(int));
            for (j = 0; j < size; j++){
                fscanf(matrices,"%d",&x[i][j]);
            }
        }
        y = (int**) malloc(size*sizeof(int*));
        for(i = 0; i < size; i++){
            y[i] = (int*) malloc(size*sizeof(int));
            for (j = 0; j < size; j++){
                fscanf(matrices,"%d",&y[i][j]);
            }
        } 


        /* distribuir */
        for(i=high;i<size;i++){
            offset=i;
            MPI_Send(&x[offset][0],rows*size,MPI_INT,1,201,MPI_COMM_WORLD);
            MPI_Send(&y[offset][0],rows*size,MPI_INT,1,201,MPI_COMM_WORLD);
        }
        /* sumar las primeras n/2 filas */
        for(i = 0; i < rows; i++){
            for(j = 0; j < size; j++){
                x[i][j] = x[i][j] + y[i][j];
            }
        }
        /* asignar segunda parte de la matriz */
        for(i=high;i<size;i++){
            MPI_Recv(&aux[i],rows*size, MPI_INT, MPI_ANY_SOURCE, 201, MPI_COMM_WORLD, &status);
            for(j=0;j<size;j++){
                x[i][j]= aux[i][j];
            }
        }

        /* escribir resultado */
        suma = fopen("suma.txt","w+");
        for(i = 0; i < size; i++){
            for(j = 0; j < size; j++){
                fprintf(suma,"%d\t",x[i][j]);
            }
            fprintf(suma,"\n");
        }
        fclose(suma);
        for (i = 0; i <size; i++){  
            free(x[i]);
            free(y[i]);  
        }  
        free(x);
        free(y); 
        end = clock();
        time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
        printf("%f\n", time_spent);
        
    }
    /* proceso hijo */
    else{

        /* sumar la segunda parte de la matriz */
        for(i = low; i < size; i++){
            MPI_Recv(&m1[i][0],rows*size,MPI_INT,0,201,MPI_COMM_WORLD,&status);
            MPI_Recv(&m2[i][0],rows*size,MPI_INT,0,201,MPI_COMM_WORLD,&status);
            for(j = 0; j < size; j++){
                aux[i][j] = m1[i][j] + m2[i][j];
            }
        }
		/* devolver la segunda parte */
        for(i = low; i < size; i++){
            MPI_Send(&aux[i],rows*size, MPI_INT, 0,201, MPI_COMM_WORLD);
        }

    }   

    MPI_Finalize();


    return 0;
}
