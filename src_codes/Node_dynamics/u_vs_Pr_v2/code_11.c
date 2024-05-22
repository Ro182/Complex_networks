#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
int  total_links(int *matrix, int N){
    int t_links=0;
    for (int i=0;i<N;i++){
        for (int j=i+1;j<N;j++){
            if (*(matrix+i*N+j)==1){
                t_links++;
            }
        }
    }
    return t_links;
}
int  active_links(int *matrix, float *list_states,int  N){
    int a_links=0;
    for (int i=0;i<N;i++){
        for (int j=i+1;j<N;j++){
            if(*(matrix+i*N+j)==1){
                if(*(list_states+i)!=*(list_states+j)){
                    a_links++;
                }
            }
        }
    }


return a_links;
}

void print_links(int *matrix, int N,char name[]){
    FILE *fptr;
    fptr=fopen(name,"w");
    for (int i=0;i<N;i++){
        for (int j=i+1;j<N;j++){
            if (*(matrix+i*N+j)==1){
                fprintf(fptr,"%d %d\n",i,j);
            }
        }
    }
    fclose(fptr);
}
void rewire(float d, float r,int *matrix, float *list_states, int N, int node){

    int nodes_same_state=0;
    int nodes_different_state=0;
    int index_same_state[N];
    int index_different_states[N];
    int disconnect_index=-1;
    int reconnect_index=-1;
    for (int i=0;i<N;i++){
        if (*(matrix+node*N+i)==1){
            if (*(list_states+node)==*(list_states+i)){
                index_same_state[nodes_same_state]=i;
                nodes_same_state++;
            }
            else{
                index_different_states[nodes_different_state]=i;
                nodes_different_state++;
            }
        }
    }
    float d_rand=(float)rand()/RAND_MAX;
    if (d_rand<d){
        if (nodes_same_state>0){
            int temp=rand()%nodes_same_state;
            disconnect_index=index_same_state[temp];


        }
        else{
            return;
        }
    }
    else{
        if (nodes_different_state>0){
            int temp=rand()%nodes_different_state;
            disconnect_index=index_different_states[temp];


        }
        else{
            return;
        }
    }

    float r_rand=(float)rand()/RAND_MAX;
    int cond=1;
    if(r_rand<r){
        int breaker=0;
        while (cond){

            int temp_node=rand()%N;
            breaker++;
            if( *(matrix+node*N+temp_node)==0 &&(*(list_states+node)==*(list_states+temp_node))&& node!=temp_node){
                reconnect_index=temp_node;
               cond=0;
            }
            if (breaker>N){
                return;
            }
        }

    }
    else{
        int breaker=0;

        while (cond){
            breaker++;

            int temp_node=rand()%N;
            if(*(matrix+node*N+temp_node)==0 &&(*(list_states+node)!=*(list_states+temp_node))&& node!=temp_node){
                reconnect_index=temp_node;
               cond=0;
            }
            if (breaker>N){
                return;
            }
        }

    }

    if(disconnect_index!=-1 && reconnect_index!=-1){
        *(matrix+node*N+disconnect_index)=0;
        *(matrix+disconnect_index*N+node)=0;
        *(matrix+node*N+reconnect_index)=1;
        *(matrix+reconnect_index*N+node)=1;
    }



}


void print_states(float *list_states,int N,char name[]){
    FILE *fptr;
    fptr=fopen(name,"w");
    for (int i=0;i<N;i++){
        fprintf(fptr, "%f ",*(list_states+i));
    }
    fclose(fptr);
}

void print_matrix(int *matrix, int N,char name[],char mode[]){
    FILE *fptr;
    fptr=fopen(name,mode);
    for (int i=0;i<N;i++){
        for (int j=0;j<N;j++){
            fprintf(fptr,"%d ",*(matrix+i*N+j));
        }
        fprintf(fptr,"\n");
    }
    fclose(fptr);

}

void node_dynnamics(float U,int *matrix, float *list_states, int N, int node){
    int number_neighboors=0;
    int index_neighboors[N];
    for (int i=0;i<N;i++){
        if (*(matrix+node*N+i)==1){
            index_neighboors[number_neighboors]=i;
            number_neighboors++;
        }
    }
    if (number_neighboors>0){
        int node_2=rand()%number_neighboors;
        if(fabs((double)(*(list_states+node)-*(list_states+index_neighboors[node_2]))) <=U ){

            *(list_states+node)=*(list_states+index_neighboors[node_2]);

        }

    }
    else{
        return;
    }
}

void evolution(float Pc, float Pr, float U,float d, float r, int *matrix, float *list_states,int N,int n_evol){


    for (int t_step=0;t_step<n_evol;t_step++){
        int node=rand()%N;
        float temp_Pr=(float)rand()/RAND_MAX;
        if (temp_Pr<=Pr){
            rewire(d,r, matrix,list_states, N,node);
        }
        float temp_Pc=(float)rand()/RAND_MAX;
        if (temp_Pc<=Pc){
            node_dynnamics(U,matrix, list_states, N, node);
        }
    }


}

float average_k(int *matrix,int N){
    int sum_k=0;
    for (int i=0;i<N;i++){
        for (int j=0;j<N;j++){
            if(*(matrix+i*N+j)==1){
                sum_k++;
            }
        }
    }
    return (float)sum_k/N;
}

void main(){
    srand(time(NULL));
    int N=400;
    printf("Number of Nodes: %d\n",N);
    int time=5000;
    int T_steps=N*time;
    int k=4;
    int G=40;
    int run=0;
    int runs=20;
    float definition=0.025;
    float d=0.1;
    float r=0.9;
    float U=0;
    float *densities;
    int size=1/definition;
    float Pr=definition;
    float Pc=1-Pr;

    densities=(float*)calloc(size*size,sizeof(float));
    for(int counter_U=0;counter_U<=size+1;counter_U++){
        printf("U=%f\n",U);
        for(int counter_Pr=0;counter_Pr<size;counter_Pr++){
            printf("Pr=%f\tPc=%f\t U=%f\t \n",Pr,Pc,U);
            for (int run=0;run<runs;run++){
                    char info_1[20];
                    sprintf(info_1,"U_%d_Pr_%d_run%d.dat",counter_U,counter_Pr,run);
                    int *matrix;
                    float *list_states;
                    float p=(float)k/(N-1);
                    matrix=(int*)calloc(N*N,sizeof(int));
                    list_states=(float*)calloc(N,sizeof(float));
                    for (int i=0;i<N;i++){
                        *(list_states+i)=(float)(rand()%G)/G;
                    }
                    char name_states_initial[50]="states_i_";
                    char name_states_final[50]="states_f_";
                    strcat(name_states_initial,info_1 );
                    strcat(name_states_final,info_1 );

                    //In this part we fill the adjacent matrix depending of p
                    for (int i=0;i<N;i++){
                        for (int j=i+1;j<N;j++){
                            float r=(float)rand()/RAND_MAX;
                            if (r<=p){
                                *(matrix+i*N+j)=1;
                                *(matrix+j*N+i)=1;
                            }
                            else{
                                *(matrix+i*N+j)=0;
                                *(matrix+j*N+i)=0;
                            }
                        }
                    }

                    char name_matrix_initial[50]="m_i_MD_";
                    char name_matrix_final[50]="m_f_MD_";
                    strcat(name_matrix_initial,info_1);
                    strcat(name_matrix_final,info_1);
                    print_matrix(matrix,N,name_matrix_initial,"w");
                    print_states(list_states,N,name_states_initial);

                    evolution(Pc,Pr,U,d,r, matrix,list_states, N,T_steps);
                    print_matrix(matrix,N,name_matrix_final,"w");
                    print_states(list_states,N,name_states_final);

                    free(matrix);
                    free(list_states);


                }
                Pr+=definition;
                Pc=1-Pr;
            }
            Pr=definition;
            Pc=1-Pr;
            U+=definition;

    }
}
