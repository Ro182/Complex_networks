#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <gsl/gsl_rng.h>


unsigned long int rand_int(void *ptr){
    gsl_rng *rng_r=(gsl_rng*)ptr;
    return gsl_rng_get(rng_r);
}

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
int  active_links(int *matrix, double *list_states,int  N){
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

int rewire(float d, float r,int *matrix, double *list_states, int N, int node,void *ptr_rng_r,int *r_i,int *r_d, int *d_i,int *d_d){
    gsl_rng *rng_r=(gsl_rng*)ptr_rng_r;

    int nodes_same_state=0;
    int nodes_different_state=0;
    int index_same_state[N];
    int index_different_states[N];

    int disconnect_index=-1;
    int reconnect_index=-1;

    for (int i=0;i<N;i++){
        if (*(matrix+node*N+i)==1 && i!=node){
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

    int nodes_same_state_disconnected=0;
    int nodes_different_state_disconnected=0;
    int index_same_state_disconnected[N];
    int index_different_states_disconnected[N];
    for (int i=0;i<N;i++){
        if (*(matrix+node*N+i)==0 && node!=i){
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
            return 0;

    }
    else{
        return -1;
    }

}



void print_states(double *list_states,int N,char name[]){
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
}

void evolution(float Pc, float Pr, float U,float d, float r, int *matrix, double *list_states,int N,int n_evol,float *densities,int run,void *ptr_rng_r){
    int d_i=0;
    int d_d=0;
    int r_i=0;
    int r_d=0;


    int resol=N;
    int counter=0;
    int measures=n_evol/resol;
    int *active_links_evolution;
    active_links_evolution=(int*)calloc(n_evol,sizeof(int));

    gsl_rng *rng_r=(gsl_rng*)ptr_rng_r;

    for (int t_step=0;t_step<n_evol;t_step++){
        if (t_step%resol==0){
            *(active_links_evolution+counter)=active_links(matrix,list_states,N);
            char run_num[2];
            sprintf(run_num,"%d",run);
            /*char name[]="matrix_representation_evolution";
            strcat(name, run_num);
            print_matrix(matrix,N,name,"a");
            */
            counter++;
        }

        int node=rand_int(rng_r)%N;
        int n_neighboors=0;
        for (int i=0;i<N;i++){
            if (*(matrix+node*N+i)==1 && node!=i){
                n_neighboors++;
            }
        }
        if (n_neighboors>0){
            int temp_tstep=0;
            double temp_Pr=(double)rand_int(rng_r)/gsl_rng_max(rng_r);
            if (temp_Pr<=Pr){

                temp_tstep=rewire(d,r, matrix,list_states, N,node,rng_r,&r_i,&r_d,&d_i,&d_d);
                t_step+=temp_tstep;
            }

            double temp_Pc=(double)rand_int(rng_r)/gsl_rng_max(rng_r);
            if (temp_Pc<=Pc){
                node_dynnamics(U,matrix, list_states, N, node,rng_r);
            }

        }
        else{
            t_step+=-1;
        }

    }
    for (int i=0;i<measures;i++){
        *(densities+i)+=*(active_links_evolution+i);
    }
    free(active_links_evolution);

    printf("%d %d %d %d\n",r_i,r_d,d_i,d_d);
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
    int N=400;
    printf("Number of Nodes: %d\n",N);
    int time_i=50;
    int T_steps=N*time_i;
    int k=4;
    int G=40;
    int runs=50;
    float d=0.5;
    float r=0.5;
    float U=0.0;
    float *densities;
    float Pr=1.0;
    float Pc=0;


    const gsl_rng_type * rng_T;
    gsl_rng *rng_r;
    gsl_rng_env_setup();
    rng_T = gsl_rng_rand48;
    rng_r = gsl_rng_alloc(rng_T);
    gsl_rng_set(rng_r,time(0));


    densities=(float*)calloc(time_i,sizeof(float));
    for (int run=0;run<runs;run++){

            char info_1[40];
            sprintf(info_1,"r_%d_d_%d_U_%d_Pr_%d_Pc_%d_run_%d.dat",(int)(r*10),(int)(d*10),(int)(U*10),(int)(Pr*10),(int)(Pc*10),run);
            printf("%f\n",U);
            int *matrix;
            double *list_states;
            float p=(float)k/(N-1);

            matrix=(int*)calloc(N*N,sizeof(int));
            list_states=(double*)calloc(N,sizeof(double));

            for (int i=0;i<N;i++){
                *(list_states+i)=(double)(rand_int(rng_r)%G)/G;
            }

            char name_states_initial[80]="states_i_";
            char name_states_final[80]="states_f_";


            strcat(name_states_initial,info_1);
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


            char name_matrix_initial[200]="m_i_MD_";
            char name_matrix_final[200]="m_f_MD_";
            strcat(name_matrix_initial,info_1);
            strcat(name_matrix_final,info_1);

            print_matrix(matrix,N,name_matrix_initial,"w");

            print_states(list_states,N,name_states_initial);
            printf("%s\n",name_states_final);


            evolution(Pc,Pr,U,d,r, matrix,list_states, N,T_steps,densities,run,rng_r);
            print_matrix(matrix,N,name_matrix_final,"w");
            print_states(list_states,N,name_states_final);

            float k;

            k=average_k(matrix,N);

            printf("k %f",k);

            free(matrix);
            free(list_states);
    }

    FILE *fptr;
    fptr=fopen("active_links.dat","w");
    for (int i=0;i<time_i;i++){
        fprintf(fptr,"%f ",(float)*(densities+i)/runs);
    }

}
