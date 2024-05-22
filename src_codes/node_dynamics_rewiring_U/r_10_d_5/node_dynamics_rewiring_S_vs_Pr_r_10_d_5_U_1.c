#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include<unistd.h>
#include <gsl/gsl_rng.h>



unsigned long int rand_int(void *ptr){
    gsl_rng *rng_r=(gsl_rng*)ptr;
    return gsl_rng_get(rng_r);
}

struct Inputs{
    int N;
    int time;
    int k;
    int G;
    int runs;
    int size;
    float d;
    float r;
    unsigned int thread_id;
    int delta_U;
};


void print_links(int *matrix, int N,char name[]){
    FILE *fptr;
    fptr=fopen(name,"w");
    fprintf(fptr,"%d\n",N);
    for (int i=0;i<N;i++){
        for (int j=i+1;j<N;j++){
            if (*(matrix+i*N+j)==1){
                fprintf(fptr,"%d %d\n",i,j);
            }
        }
    }
    fclose(fptr);
}

void rewire_3(float d, float r,int *matrix, double *list_states, int N, int node,void *ptr_rng_r){
    gsl_rng *rng_r=(gsl_rng*)ptr_rng_r;

    int nodes=0;
    int index[N];

    int disconnect_index=-1;
    int reconnect_index=-1;

    for (int i=0;i<N;i++){
        if (*(matrix+node*N+i)==1 && *(list_states+node)!=*(list_states+i)){
            index[nodes]=i;
            nodes++;
        }
    }
    if (nodes>0){
        int temp=rand_int(rng_r)%nodes;
        disconnect_index=index[temp];
    }

            *(matrix+node*N+disconnect_index)=0;
            *(matrix+disconnect_index*N+node)=0;





    int nodes_same_state_disconnected=0;
    int index_same_state_disconnected[N];
    for (int i=0;i<N;i++){
        if (*(matrix+node*N+i)==0 && node!=i){
            if (*(list_states+node)==*(list_states+i)){
                index_same_state_disconnected[nodes_same_state_disconnected]=i;
                nodes_same_state_disconnected++;
            }

        }
    }
    if (nodes_same_state_disconnected==0){
        return;
    }
    int temp_node=index_same_state_disconnected[rand_int(rng_r)%nodes_same_state_disconnected];
    reconnect_index=temp_node;

    *(matrix+node*N+reconnect_index)=1;
    *(matrix+reconnect_index*N+node)=1;



/*
    if (reconnect_index!=-1 && disconnect_index!=-1){

           *(matrix+node*N+disconnect_index)=0;
            *(matrix+disconnect_index*N+node)=0;
            *(matrix+node*N+reconnect_index)=1;
            *(matrix+reconnect_index*N+node)=1;

    }
*/
}


void rewire_2(float d, float r,int *matrix, double *list_states, int N, int node,void *ptr_rng_r){
    gsl_rng *rng_r=(gsl_rng*)ptr_rng_r;

    int nodes=0;
    int index[N];

    int disconnect_index=-1;
    int reconnect_index=-1;

    for (int i=0;i<N;i++){
        if (*(matrix+node*N+i)==1){
            index[nodes]=i;
            nodes++;
        }
    }

    int temp=rand_int(rng_r)%nodes;
    disconnect_index=index[temp];
            //*(matrix+node*N+disconnect_index)=0;
            //*(matrix+disconnect_index*N+node)=0;





    int nodes_same_state_disconnected=0;
    int index_same_state_disconnected[N];
    for (int i=0;i<N;i++){
        if (*(matrix+node*N+i)==0 && node!=i){
            if (*(list_states+node)==*(list_states+i)){
                index_same_state_disconnected[nodes_same_state_disconnected]=i;
                nodes_same_state_disconnected++;
            }

        }
    }
    if (nodes_same_state_disconnected==0){
        return;
    }
    int temp_node=index_same_state_disconnected[rand_int(rng_r)%nodes_same_state_disconnected];

    reconnect_index=temp_node;
                //*(matrix+node*N+reconnect_index)=1;
                //*(matrix+reconnect_index*N+node)=1;




    if (reconnect_index!=-1 && disconnect_index!=-1){

           *(matrix+node*N+disconnect_index)=0;
            *(matrix+disconnect_index*N+node)=0;
            *(matrix+node*N+reconnect_index)=1;
            *(matrix+reconnect_index*N+node)=1;

    }

}

void rewire(float d, float r,int *matrix, double *list_states, int N, int node,void *ptr_rng_r){
    gsl_rng *rng_r=(gsl_rng*)ptr_rng_r;

    int nodes_same_state_connected=0;
    int nodes_different_state_connected=0;
    int index_same_state_connected[N];
    int index_different_states_connected[N];

    int disconnect_index=-1;
    int reconnect_index=-1;

    for (int i=0;i<N;i++){
        if (*(matrix+node*N+i)==1){
            if (1){
                index_same_state_connected[nodes_same_state_connected]=i;
                nodes_same_state_connected++;
            }
            else{
                index_different_states_connected[nodes_different_state_connected]=i;
                nodes_different_state_connected++;

            }
        }
    }

    double d_rand=(double)rand_int(rng_r)/gsl_rng_max(rng_r);
    if (1){
        if (nodes_same_state_connected>0){
            int temp=rand_int(rng_r)%nodes_same_state_connected;
            disconnect_index=index_same_state_connected[temp];
        }

    }

    else{
        if (nodes_different_state_connected>0){
            int temp=rand_int(rng_r)%nodes_different_state_connected;
            disconnect_index=index_different_states_connected[temp];
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

    double r_rand=(double)rand_int(rng_r)/gsl_rng_max(rng_r);
    if(r_rand<=r){
        if (nodes_same_state_disconnected>0){
            int temp_node=index_same_state_disconnected[rand_int(rng_r)%nodes_same_state_disconnected];
                reconnect_index=temp_node;

        }

    }
    else{
        if (nodes_different_state_disconnected>0){
            int temp_node= index_different_states_disconnected[rand_int(rng_r)%nodes_different_state_disconnected];
                reconnect_index=temp_node;


        }

    }

    if (reconnect_index!=-1 && disconnect_index!=-1){

            *(matrix+node*N+disconnect_index)=0;
            *(matrix+disconnect_index*N+node)=0;
            *(matrix+node*N+reconnect_index)=1;
            *(matrix+reconnect_index*N+node)=1;

    }

}


void print_states(double *list_states,int N,char name[]){
    FILE *fptr;

    fptr=fopen(name,"w");
    for (int i=0;i<N;i++){
        fprintf(fptr, "%lf ",*(list_states+i));
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

void node_dynnamics(float U,int *matrix, double *list_states, int N, int node,void *ptr_rng_r){
    gsl_rng *rng_r=(gsl_rng*)ptr_rng_r;

    int number_neighboors=0;
    int index_neighboors[N];
    for (int i=0;i<N;i++){
        if (*(matrix+node*N+i)==1 ){
            index_neighboors[number_neighboors]=i;
            number_neighboors++;
        }
    }
    if (number_neighboors>0){
        int node_2=index_neighboors[rand_int(rng_r)%number_neighboors];
        if(fabs((double)(*(list_states+node)-*(list_states+node_2))) <=U ){
            *(list_states+node)=*(list_states+node_2);
        }
    }
}

void evolution(float Pc, float Pr, float U,float d, float r, int *matrix, double *list_states,int N,int n_evol,void *ptr_rng_r){

    gsl_rng *rng_r=(gsl_rng*)ptr_rng_r;

    for (int t_step=0;t_step<n_evol;t_step++){

        int node=rand_int(rng_r)%N;

        int n_neighboors=0;
        for (int i=0;i<N;i++){
            if (*(matrix+node*N+i)==1){
                n_neighboors++;
            }
        }
        if (n_neighboors>0){

            double temp_Pr=(double)rand_int(rng_r)/gsl_rng_max(rng_r);
            if (temp_Pr<=Pr){

                rewire_2(d,r, matrix,list_states, N,node, rng_r);
            }
            double temp_Pc=(double)rand_int(rng_r)/gsl_rng_max(rng_r);
            if(temp_Pc<=Pc){
                node_dynnamics(U,matrix, list_states, N, node,rng_r);
            }

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
int main(){

    int N=100;
    int time=750;
    int k=4;
    int G=10;
    int runs=2;
    int size=30;
    printf("N %d\n",N);
    float d=0.5;
    float r=1.0;
    float U=1.0;


    const gsl_rng_type * rng_T;
    gsl_rng *rng_r;
    gsl_rng_env_setup();
    rng_T = gsl_rng_default;
    rng_r = gsl_rng_alloc(rng_T);


    int T_steps=N*time;


    float definition=(float)1/size;
    printf("%d\n",size);
    float Pr=0;
    float Pc=1-Pr;


    for(int counter_Pr=0;counter_Pr<size;counter_Pr++){
        for (int run=0;run<runs;run++){
                char info_1[20];
                sprintf(info_1,"Pr_%d_run%d.dat",counter_Pr,run);

                int *matrix;
                double *list_states;

                double p=(double)k/(N-1);
                matrix=(int*)calloc(N*N,sizeof(int));
                list_states=(double*)calloc(N,sizeof(double));
                for (int i=0;i<N;i++){
                    *(list_states+i)=(double)(rand_int(rng_r)%G)/G;
                }
                char name_states_initial[50]="states_i_";
                char name_states_final[50]="states_f_";
                strcat(name_states_initial,info_1 );
                strcat(name_states_final,info_1 );

                //In this part we fill the adjacent matrix depending of p
                for (int i=0;i<N;i++){
                    for (int j=i+1;j<N;j++){
                        double r=(double)rand_int(rng_r)/gsl_rng_max(rng_r);
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
                float ki;
                ki=average_k(matrix,N);

                char name_links_initial[50]="links_i_MD_";
                char name_links_final[50]="links_f_MD_";

                strcat(name_links_final,info_1);
                strcat(name_links_initial,info_1);

                print_links(matrix, N, name_links_initial);
                print_states(list_states,N,name_states_initial);

                evolution(Pc,Pr,U,d,r, matrix,list_states, N,T_steps,rng_r);

                print_links(matrix, N, name_links_final);
                print_states(list_states,N,name_states_final);

                float kf;
                kf=average_k(matrix,N);
                printf("k %f %f\n",ki,kf);

                free(matrix);
                free(list_states);
            }

            Pr+=definition;
            Pc=1-Pr;
            printf("%f %f\n",Pr,Pc);
        }


    gsl_rng_free (rng_r);

}




