#include <sys/time.h>
#include <time.h>
#include <stdio.h>



int main(){
long long t_delay=5e2;
double t_s=0;
struct timeval stop, start;
int i=0;
while(i<100){
t_s=0;
gettimeofday(&start, NULL);
nanosleep((const struct timespec[]){{0, t_delay}}, NULL);//do stuff
gettimeofday(&stop, NULL);
t_s=(stop.tv_sec - start.tv_sec) + (stop.tv_usec - start.tv_usec) / 1000000.0f;
printf("took %f\n", t_s);
}

return 0;
}