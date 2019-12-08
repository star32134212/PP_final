#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#define RAND_INNER_BUFFER_SIZE 64

int M; //模擬次數
int N; //期

static inline double normalCDF(double value){
   return 0.5 * erfc(-value * M_SQRT1_2);
}

const double mean = 0;
const double std = 1;
struct random_data *rand_buffer;
char **rand_inner_buffer;

double normal(){
    int ID = omp_get_thread_num();
    int32_t i, j;
    double u, v, x;
    random_r(&rand_buffer[ID], &i);
    random_r(&rand_buffer[ID], &j);
    u = (double) i / RAND_MAX;
    v = (double) j / RAND_MAX;
    x = sqrt(-2 * log(u)) * cos(2 * M_PI * v) * std + mean;
    return x;
}


//定價模型公式
//call=期權初始價格
//S=交易的金融資產現價
//L=期權交割價格
//T=期權期效
//r=無風險利率
//vol^2=年度化方差(類似風險)
double blsprice(double S, double L, double T, double r, double vol){
    double d1, d2, call;
    d1 = (log(S / L) + (r + 0.5 * vol * vol) * T) / (vol * sqrt(T));
    d2 = (log(S / L) + (r - 0.5 * vol * vol) * T) / (vol * sqrt(T));
    call = S * normalCDF(d1) - L * exp(-r * T) * normalCDF(d2);
    return call;
}


//蒙地卡羅模擬
//N=期 (想成是一個金融商品可以做交易的頻率)
//如果一年有100天可以做交易 N=100 則這個商品每365/100天就會有一個價格
double MCsim(double S, double T, double r, double vol, int N){
    double dt = T / N;
    double St[N + 1];
    St[0] = S;
    for(int i = 0; i < N; i++){
        St[i + 1] = St[i] * exp((r - 0.5 * vol * vol) * dt + normal() * vol * sqrt(dt));
    }
    return St[N];
}


//假設有一個期權的資訊如下
double S = 50.0;
double L = 40.0;
double T = 2.0;
double r = 0.08;
double vol = 0.2;
double call = 0;

int main(int argc , char *argv []){ 
    N = atoi(argv[1]);
    M = atoi(argv[2]);
    double bls, mp;
    double dif;
    double Sa;
    bls = blsprice(S, L, T, r, vol);
    printf("bls定價模型算出之價格 %lf\n", bls);

    // allocate seedp for each thread
    FILE* fp = fopen("/dev/urandom", "r");    

    int max_num_threads = omp_get_max_threads();
    rand_inner_buffer = malloc(max_num_threads * sizeof *rand_inner_buffer);
    for (int i = 0; i < max_num_threads; i++)
        rand_inner_buffer[i] = calloc(RAND_INNER_BUFFER_SIZE, sizeof *rand_inner_buffer[i]);

    rand_buffer = calloc(max_num_threads, sizeof *rand_buffer);
    for (int i = 0; i < max_num_threads; i++){
        unsigned int seed;
        if (fread(&seed, sizeof(unsigned int), 1, fp) != 1) exit(1);
        initstate_r(seed, rand_inner_buffer[i], RAND_INNER_BUFFER_SIZE, &rand_buffer[i]);
    }

    fclose(fp);

    #pragma omp parallel for reduction (+:call) private(Sa)
    for(int j = 0; j < M; j++){
        Sa = MCsim(S, T, r, vol, N); //Sa存每一期變動完的價格
        int SA_P = 0;
        if(Sa - L > 0){ //有大於0才會執行(才有獲利)
            SA_P = Sa - L;
        }
        call += SA_P; //算期望值
    }

    mp = call / M * exp(-r * T);
    printf("蒙地卡羅預測可獲利 %lf\n", mp);
    dif = fabs(mp - bls);
    printf("誤差 %lf\n", dif);

    for (int i = 0; i < max_num_threads; i++)
        free(rand_inner_buffer[i]);
    free(rand_inner_buffer);
    free(rand_buffer);
    return 0;
}