#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>
#include<unistd.h>
#include <gsl/gsl_rng.h>
#define MAX_THREADS 8

/*This function returns a random int between 0 and RAND_MAX*/
unsigned long int rand_int(void *ptr){
    gsl_rng *rng_r=(gsl_rng*)ptr;
    return gsl_rng_get(rng_r);
}

/*This structures contains all the variables of the system*/
struct Inputs{
    int N; //Size of the network
    int time; //Number of time steps 
    int k; //Average degree of the network
    int G; //Number of possible states of the nodes
    int runs; //Number of runs
    int size; //Number of data points
    float d; //Probability of disconnection
    float r; //Probability of reconnection
    float U; //Threshold for the node dynamic
    float Pr; //Probability of rewiring dynamics
    float Pc; //Probability of node dynamics (copying states)
    unsigned int thread_id;

};

/*This function saves a file with the all the links of the network, the char name[] is the name of the file*/
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


/*this function is does the rewiring process of disconnection and reconnection*/
void rewire(float d, float r,int *matrix, double *list_states, int N, int node,void *ptr_rng_r){
    gsl_rng *rng_r=(gsl_rng*)ptr_rng_r;

    /*Here we find all the neighbors of our node*/
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

    /*Here we choose the neighbor that will be disconnected  from our node*/
    double d_rand=(double)rand_int(rng_r)/gsl_rng_max(rng_r);

    if (d_rand<=d){
        if (nodes_same_state>0){
            int temp=rand_int(rng_r)%nodes_same_state;
            disconnect_index=index_same_state[temp];
        }
    }
    else{
        if (nodes_different_state>0){
            int temp=rand_int(rng_r)%nodes_different_state;
            disconnect_index=index_different_states[temp];
        }
    }

    /*Here we find all the nodes in the network that are not connected to our node*/
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

    /*Here we choose the node to connect to */
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

    /*In order to maintain the number of links constant we do the connection and reconnection together only if it is possible*/
    if (reconnect_index!=-1 && disconnect_index!=-1){
            *(matrix+node*N+disconnect_index)=0;
            *(matrix+disconnect_index*N+node)=0;
            *(matrix+node*N+reconnect_index)=1;
            *(matrix+reconnect_index*N+node)=1;
    }
}

/*this function saves a file with the states of the nodes. The first value corresponds to the node 1, the second to the node 2 and so on*/
void print_states(double *list_states,int N,char name[]){
    FILE *fptr;
    fptr=fopen(name,"w");
    for (int i=0;i<N;i++){
        fprintf(fptr, "%lf ",*(list_states+i));
    }
    fclose(fptr);
}


/*This function saves the adjacency matrix of the network*/
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


/*This function is in charge of the node dynamic, in this case a discrete-valued opinions model*/
void node_dynnamics(float U,int *matrix, double *list_states, int N, int node,void *ptr_rng_r){
    gsl_rng *rng_r=(gsl_rng*)ptr_rng_r;

    int number_neighbors=0;
    int index_neighbors[N];

    for (int i=0;i<N;i++){
        if (*(matrix+node*N+i)==1){
            index_neighbors[number_neighbors]=i;
            number_neighbors++;
        }
    }
    /*Here we compare the states of the nodes and apply the node dynamic*/
    if (number_neighbors>0){
        int node_2=rand_int(rng_r)%number_neighbors;
        if(fabs((double)(*(list_states+node)-*(list_states+index_neighbors[node_2]))) <=U ){
            *(list_states+node)=*(list_states+index_neighbors[node_2]);
        }
    }

}

/*This function does the time evolution of the network*/
void evolution(float Pc, float Pr, float U,float d, float r, int *matrix, double *list_states,int N,int n_evol,void *ptr_rng_r){
    gsl_rng *rng_r=(gsl_rng*)ptr_rng_r;

    for (int t_step=0;t_step<n_evol;t_step++){

        int node=rand_int(rng_r)%N;
        /*Here we check if the choose node has neighbors for the dynamic*/
        int n_neighbors=0;
        for (int i=0;i<N;i++){
            if (*(matrix+node*N+i)==1){
                n_neighbors++;
            }
        }

        if (n_neighbors>0){

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
}

/*this function calculates and the average degree of the network*/
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


/*this function does all the runs with the fixed variables*/
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
    float U=(*inputs).U;
    float Pr=(*inputs).Pr;
    float Pc=(*inputs).Pc;
    unsigned int thread_id=(*inputs).thread_id;
    const gsl_rng_type * rng_T;
    gsl_rng *rng_r;
    gsl_rng_env_setup();
    rng_T = gsl_rng_rand48;
    rng_r = gsl_rng_alloc(rng_T);
    gsl_rng_set(rng_r,thread_id);

    int T_steps=N*time;

    /*here all the tuns are done*/
    for (int run=0;run<runs;run++){

            char info_1[20];
            sprintf(info_1,"Pr_%d_run%d.dat",thread_id,run);
            /*here we created the matrix for the network and the list for the states*/
            int *matrix;
            double *list_states;

            /*Here we calculate the probability to created a Erdos Reyni random network as function of p*/
            double p=(double)k/(N-1);

            matrix=(int*)calloc(N*N,sizeof(int));
            list_states=(double*)calloc(N,sizeof(double));

            /*Here we give the nodes a discrete state chose randomly  from the G possible options*/
            for (int i=0;i<N;i++){
                    *(list_states+i)=(double)(rand_int(rng_r)%G)/G;
                }

            char name_states_initial[50]="./1_Data_example/states_i_";
            char name_states_final[50]="./1_Data_example/states_f_";

            strcat(name_states_initial,info_1 );
            strcat(name_states_final,info_1 );

            /*In this part we fill the adjacent matrix depending of p*/
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
            char name_links_initial[50]="./1_Data_example/links_i_MD_";
            char name_links_final[50]="./1_Data_example/links_f_MD_";

            strcat(name_links_final,info_1);
            strcat(name_links_initial,info_1);

            /*Here we save the network and states before the evolution*/
            print_links(matrix, N, name_links_initial);
            print_states(list_states,N,name_states_initial);

            /*Here we do the time evolution of the network*/
            evolution(Pc,Pr,U,d,r, matrix,list_states, N,T_steps,rng_r);

            /*Here we save the network and states after the evolution*/
            print_links(matrix, N, name_links_final);
            print_states(list_states,N,name_states_final);


            free(matrix);
            free(list_states);
        }

    gsl_rng_free (rng_r);
    printf("Thread id: %d out of %d threads done\n",thread_id,MAX_THREADS);

}

/*The main function is in charge the parallelize the code so it can be run with multiple threads*/
int main(){

    /*size and definition correspond to the number of data points wanted*/
    struct Inputs inputs[MAX_THREADS];
    int size=MAX_THREADS;
    float definition=(float)1/size;

    /*Here there are the parameter that are going to be send to each thread*/
    /*Here, the variables that must change can be set to different values for example Pr and Pc in this case*/
    for (int thread=0;thread<MAX_THREADS;thread++){
        inputs[thread].N=30;
        inputs[thread].time=100;
        inputs[thread].k=4;
        inputs[thread].G=30;
        inputs[thread].runs=10;
        inputs[thread].size=size;
        inputs[thread].d=0.35;
        inputs[thread].r=0.65;
        inputs[thread].U=1.0;
        inputs[thread].thread_id=(unsigned int)thread;
        inputs[thread].Pr=definition*thread;
        inputs[thread].Pc=1-definition*thread;
    }

    /*Here, all the threads are created and run*/
    pthread_t thread_ids[MAX_THREADS];
    for (int thread=0;thread<MAX_THREADS;thread++){
        pthread_create(&thread_ids[thread],NULL,(void*)&section,(void*)&inputs[thread]);

    }

    /*Here, all the threads are joined*/
    for (int thread=0;thread<MAX_THREADS;thread++){
        pthread_join(thread_ids[thread],NULL);
    }
    
    return 0;
}
