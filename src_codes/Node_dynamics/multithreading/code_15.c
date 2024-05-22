#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <stdbool.h>
#include<unistd.h>
#include <gsl/gsl_rng.h>
#define MAX_THREADS 4


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



void rewire(float d, float r,int *matrix, double *list_states, int N, int node,void *ptr_rng_r){
    gsl_rng *rng_r=(gsl_rng*)ptr_rng_r;

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
    double d_rand=(double)rand_int(rng_r)/gsl_rng_max(rng_r);
    if (d_rand<d){
        if (nodes_same_state>0){
            int temp=rand_int(rng_r)%nodes_same_state;
            disconnect_index=index_same_state[temp];


        }
        else{
            return;
        }
    }
    else{
        if (nodes_different_state>0){
            int temp=rand_int(rng_r)%nodes_different_state;
            disconnect_index=index_different_states[temp];


        }
        else{
            return;
        }
    }

    double r_rand=(double)rand_int(rng_r)/gsl_rng_max(rng_r);
    int cond=1;
    if(r_rand<r){
        int breaker=0;
        while (cond){

            int temp_node=rand_int(rng_r)%N;
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

            int temp_node=rand_int(rng_r)%N;
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
        if (*(matrix+node*N+i)==1){
            index_neighboors[number_neighboors]=i;
            number_neighboors++;
        }
    }
    if (number_neighboors>0){
        int node_2=rand_int(rng_r)%number_neighboors;
        if(fabs((double)(*(list_states+node)-*(list_states+index_neighboors[node_2]))) <=U ){

            *(list_states+node)=*(list_states+index_neighboors[node_2]);

        }

    }
    else{
        return;
    }
}

void evolution(float Pc, float Pr, float U,float d, float r, int *matrix, double *list_states,int N,int n_evol,void *ptr_rng_r){

    gsl_rng *rng_r=(gsl_rng*)ptr_rng_r;

    for (int t_step=0;t_step<n_evol;t_step++){
        int node=rand_int(rng_r)%N;
        double temp_Pr=(double)rand_int(rng_r)/gsl_rng_max(rng_r);
        if (temp_Pr<=Pr){
            rewire(d,r, matrix,list_states, N,node, rng_r);
        }
        double temp_Pc=(double)rand_int(rng_r)/gsl_rng_max(rng_r);
        if (temp_Pc<=Pc){
            node_dynnamics(U,matrix, list_states, N, node,rng_r);
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

void section(void *ptr){
    struct Inputs *inputs=(struct Inputs*)ptr;

    int N=(*inputs).N;
    int time=(*inputs).time;
    int k=(*inputs).k;
    int G=(*inputs).G;
    int runs=(*inputs).runs;
    int size=(*inputs).size;
    float d=(*inputs).d;
    float r=(*inputs).r;
    int delta_U=(*inputs).delta_U;
    unsigned int thread_id=(*inputs).thread_id;
    const gsl_rng_type * rng_T;
    gsl_rng *rng_r;
    gsl_rng_env_setup();
    rng_T = gsl_rng_rand48;
    rng_r = gsl_rng_alloc(rng_T);
    gsl_rng_set(rng_r,thread_id);

    int T_steps=N*time;
    int run=0;
    float U=0;
    float definition=(float)1/size;
    printf("%d\n",size);
    float Pr=definition;
    float Pc=1-Pr;
    int U_max=delta_U*(thread_id+1);
    if(thread_id+1==MAX_THREADS){
        U_max++;
    }
    for(int counter_U=thread_id*delta_U;counter_U<U_max;counter_U++){
        printf("U=%f\n",counter_U*definition);
        for(int counter_Pr=0;counter_Pr<size-1;counter_Pr++){
            for (int run=0;run<runs;run++){
                    char info_1[20];
                    sprintf(info_1,"U_%d_Pr_%d_run%d.dat",counter_U,counter_Pr,run);
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

                    char name_matrix_initial[50]="m_i_MD_";
                    char name_matrix_final[50]="m_f_MD_";
                    strcat(name_matrix_initial,info_1);
                    strcat(name_matrix_final,info_1);
                    print_matrix(matrix,N,name_matrix_initial,"w");
                    print_states(list_states,N,name_states_initial);

                    //evolution(Pc,Pr,U,d,r, matrix,list_states, N,T_steps,rng_r);

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

    gsl_rng_free (rng_r);
    printf("Thread id: %d out of %d threads done\n",thread_id,MAX_THREADS);

}

void main(){


    struct Inputs inputs[MAX_THREADS];
    int size=24;
    float definition=(float)1/size;
    printf("%f \n",definition);
    int delta_U=size/MAX_THREADS;


    for (int thread=0;thread<MAX_THREADS;thread++){
        inputs[thread].N=400;
        inputs[thread].time=1000;
        inputs[thread].k=4;
        inputs[thread].G=40;
        inputs[thread].runs=5;
        inputs[thread].size=size;
        inputs[thread].d=0.1;
        inputs[thread].r=0.9;
        inputs[thread].thread_id=(unsigned int)thread;
        inputs[thread].delta_U=delta_U;
    }
    pthread_t thread_ids[MAX_THREADS];
    for (int thread=0;thread<MAX_THREADS;thread++){
        pthread_create(&thread_ids[thread],NULL,(void*)&section,(void*)&inputs[thread]);

    }
    for (int thread=0;thread<MAX_THREADS;thread++){
        pthread_join(thread_ids[thread],NULL);
    }

}
