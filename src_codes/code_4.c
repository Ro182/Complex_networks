#include <stdio.h>
#include <stdlib.h>
#include <time.h>


// Main program use for the simulation of active links
// Pair with the code reading.py

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

int  active_links(int *matrix, int *list_states,int  N){
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

void rewire(float d, float r,int *matrix, int *list_states, int N, int node){

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

    }
    else{
        if (nodes_different_state>0){
            int temp=rand()%nodes_different_state;
            disconnect_index=index_different_states[temp];


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

/*
void rewire(float d, float r,int *matrix, int *list_states, int N, int node){


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

    double d_rand=(float)rand()/RAND_MAX;
    if (d_rand<=d){
        if (nodes_same_state>0){
            int temp=rand()%nodes_same_state;
            disconnect_index=index_same_state[temp];
            //*(matrix+node*N+disconnect_index)=0;
            //*(matrix+disconnect_index*N+node)=0;

        }

    }
    else{
        if (nodes_different_state>0){
            int temp=rand()/RAND_MAX;
            disconnect_index=index_different_states[temp];
            //*(matrix+node*N+disconnect_index)=0;
            //*(matrix+disconnect_index*N+node)=0;
        }
    }

    int nodes_same_state_disconnected=0;
    int nodes_different_state_disconnected=0;
    int index_same_state_disconnected[N];
    int index_different_states_disconnected[N];
    for (int i=0;i<N;i++){
        if (*(matrix+node*N+i)==0 &&node!=i){
            if (*(list_states+node)==*(list_states+i)){
                index_same_state_disconnected[nodes_same_state_disconnected]=i;
                nodes_same_state_disconnected++;
            }
            else{
                index_different_states_disconnected[nodes_different_state_disconnected]=i;
                nodes_different_state_disconnected++;
            }
        }
    }

    double r_rand=(float)rand()/RAND_MAX;
    if(r_rand<=r){
        if (nodes_same_state_disconnected>0){
            int temp_node=index_same_state_disconnected[rand()%nodes_same_state_disconnected];
                reconnect_index=temp_node;
                //*(matrix+node*N+reconnect_index)=1;
                //*(matrix+reconnect_index*N+node)=1;

        }


    }

    else{
        if (nodes_different_state_disconnected>0){
            int temp_node= index_different_states_disconnected[rand()%nodes_different_state_disconnected];
                reconnect_index=temp_node;
                //*(matrix+node*N+reconnect_index)=1;
                //*(matrix+reconnect_index*N+node)=1;


        }
    }

    if (reconnect_index!=-1 && disconnect_index!=-1){

            *(matrix+node*N+disconnect_index)=0;
            *(matrix+disconnect_index*N+node)=0;
            *(matrix+node*N+reconnect_index)=1;
            *(matrix+reconnect_index*N+node)=1;

    }



}

*/

/*
void rewire(float d, float r,int *matrix, int *list_states, int N, int node){

    int nodes_same_state=0;
    int nodes_different_state=0;
    int index_same_state[N];
    int index_different_states[N];

    int disconnect_index=-1;
    int reconnect_index=-1;

    for (int i=0;i<N;i++){
        if (*(matrix+node*N+i)==1 && node!=i){
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

    double d_rand=(float)rand()/RAND_MAX;
    if (d_rand<=d){
        if (nodes_same_state>0){
            int temp=rand()%nodes_same_state;
            disconnect_index=index_same_state[temp];
          //  *(matrix+node*N+disconnect_index)=0;
           // *(matrix+disconnect_index*N+node)=0;

        }

    }
    else{
        if (nodes_different_state>0){
            int temp=rand()%nodes_different_state;
            disconnect_index=index_different_states[temp];
           // *(matrix+node*N+disconnect_index)=0;
            //*(matrix+disconnect_index*N+node)=0;
        }
    }

    int nodes_same_state_disconnected=0;
    int nodes_different_state_disconnected=0;
    int index_same_state_disconnected[N];
    int index_different_states_disconnected[N];
    for (int i=0;i<N;i++){
        if (*(matrix+node*N+i)==0 &&node!=i){
            if (*(list_states+node)==*(list_states+i)){
                index_same_state_disconnected[nodes_same_state_disconnected]=i;
                nodes_same_state_disconnected++;
            }
            else{
                index_different_states_disconnected[nodes_different_state_disconnected]=i;
                nodes_different_state_disconnected++;
            }
        }
    }

    double r_rand=(float)rand()/RAND_MAX;
    if(r_rand<=r){
        if (nodes_same_state_disconnected>0){
            int temp_node=index_same_state_disconnected[rand()%nodes_same_state_disconnected];
                reconnect_index=temp_node;
               // *(matrix+node*N+reconnect_index)=1;
                //*(matrix+reconnect_index*N+node)=1;

        }


    }

    else{
        if (nodes_different_state_disconnected>0){
            int temp_node= index_different_states_disconnected[rand()%nodes_different_state_disconnected];
                reconnect_index=temp_node;
               // *(matrix+node*N+reconnect_index)=1;
                //*(matrix+reconnect_index*N+node)=1;



        }
    }


    if (reconnect_index!=-1 && disconnect_index!=-1){

            *(matrix+node*N+disconnect_index)=0;
            *(matrix+disconnect_index*N+node)=0;
            *(matrix+node*N+reconnect_index)=1;
            *(matrix+reconnect_index*N+node)=1;

    }

}
*/


void evolution(float d,float r,int *matrix, int *list_states,int N,int n_evol){
    int resol=N;
    int measures=n_evol/resol;
    int *active_links_evolution;
    int counter=0;
    active_links_evolution=(int*)calloc(n_evol,sizeof(int));

    for (int t_step=0;t_step<n_evol;t_step++){

        if (t_step%resol==0){
            *(active_links_evolution+counter)=active_links(matrix,list_states,N);
            counter++;
        }
        int node=rand()%N;
        rewire(d,r, matrix,list_states, N,node);
    }
    FILE *fptr;
    fptr=fopen("active_links.dat","w");
    for (int i=0;i<measures;i++){
        fprintf(fptr,"%d ",*(active_links_evolution+i));
    }
    free(active_links_evolution);

}



void print_states(int *list_states,int N,char name[]){
    FILE *fptr;
    fptr=fopen(name,"w");
    for (int i=0;i<N;i++){
        fprintf(fptr, "%d ",*(list_states+i));
    }
}

void print_matrix(int *matrix, int N,char name[]){
    FILE *fptr;
    fptr=fopen(name,"w");
    for (int i=0;i<N;i++){
        for (int j=0;j<N;j++){
            fprintf(fptr,"%d ",*(matrix+i*N+j));
        }
        fprintf(fptr,"\n");
    }
    fclose(fptr);

}

void read_matrix(int *matrix,int N, char name[]){
    FILE *fptr;
    fptr=fopen(name,"r");
    if(fptr==NULL){
        printf("No such a file");
        exit(EXIT_FAILURE);
    }


    for (int i=0;i<N;i++){
        for (int j=0;j<N;j++){
            fscanf(fptr,"%d",(matrix+i*N+j));
        }
    }
    fclose(fptr);
}

void read_states (int *list_states, int N, char name[]){
    FILE *fptr;
    fptr=fopen(name,"r");
    if(fptr==NULL){
        printf("No such a file");
        exit(EXIT_FAILURE);
    }

    for (int i=0;i<N;i++){
        fscanf(fptr,"%d",(list_states+i));
    }
    fclose(fptr);

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
    int read_files=0;
    float d=0.5;
    float r=0.5;



    int N=400;
    int T_steps=N*50;
    int *matrix;
    int *list_states;
    int k=4;
    int G=40;

    matrix=(int*)calloc(N*N,sizeof(int));
    list_states=(int*)calloc(N,sizeof(int));
    char final_matrix_name[]="matrix_representation_f_community.dat";
    if (read_files==1){
        char final_matrix_name[]="matrix_representation_f.dat";

        read_matrix(matrix,N,"matrix_representation_f_community.dat");
        read_states(list_states,N,"states.dat");
    }
    else{
        float p=(float)k/(N-1);
        for (int i=0;i<N;i++){
            *(list_states+i)=rand()%G;
        }
        print_states(list_states,N,"states.dat");
        printf("p used %f\n",p);

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


    }

    float initial_average_k=average_k(matrix,N);

    printf("Number of Nodes: %d\n",N);
    int t_links=total_links(matrix,N);
    int a_links_initial=active_links(matrix,list_states, N);
    printf("Total links:%d\n",t_links);
    printf("Initial active links:%d\n",a_links_initial);

    printf("Initial <k> = %f\n",initial_average_k);

    print_matrix(matrix,N,"matrix_representation_i.dat");
    print_links(matrix,N,"link_representation.dat");
    evolution(d,r,matrix,list_states, N,T_steps);


    print_matrix(matrix,N,final_matrix_name);

    float final_average_k=average_k(matrix,N);

    printf("Final <k> %f\n",final_average_k);
    int a_links_final;
    a_links_final=active_links(matrix,list_states, N);
    printf("Final active links:%d\n",a_links_final);

    printf("Initial density of active links %f\n",(float)a_links_initial/t_links);
    printf("Final density of active links %f\n",(float)a_links_final/t_links);

    free(matrix);
    free(list_states);
}
