#include "apue.h"
#include <sys/time.h>
#include <pthread.h>
#include <limits.h>

#define NTHR             8   //线程个数
#define NUMNUM           8000000L  //要排序的数字数目
#define TNUM   (NUMNUM/NTHR)       //每个线程的排序数目

long nums[NUMNUM];
long snums[NUMNUM];

pthread_barrier_t b;

#ifdef SOLARIS
#define heapsort qsort
#else
extern int heapsort(void*, size_t ,size_t, int (*)(const void *, const void *));
#endif

/*
 * Compare two long integers(helper function for heapsort)
 */
int complong(const void *arg1, const void *arg2)
{
    long l1 = *(long *)arg1;
    long l2 = *(long *)arg2;

    if(l1 == l2) {
        return 0;
    } else if(l1 < l2) {
        return -1;
    } else {
        return 1;
    }
}

/*
 * Worker thread to sort a portion of set of numbers
 * 对一组数字的一部分进行排序的工作线程
 */
void *thr_fn(void *arg)
{
    long idx = (long)arg;
    heapsort(&nums[idx], TNUM, sizeof(long), complong);
    pthread_barrier_wait(&b);

    return (void *)0;
}

/*
 * Merge the results of the individual sorted ranges
 * 合并单个排序范围的结果
 */
void merge()
{
    long idx[NTHR];
    long i, minidx, sidx, num;

    for(i = 0; i < NTHR; i++) {
        idx[i] = i * TNUM;
    }

    for(sidx = 0; sidx < NUMNUM; sidx++) {
        num = LONG_MAX;
        for(i = 0; i < NTHR; i++) {
            if((idx[i] < (i+1)*TNUM) && (nums[idx[i]] < num)) {
                num = nums[idx[i]];
                minidx = i;
            }
        }
        snums[sidx] = nums[idx[minidx]];
        idx[minidx]++;
    }
}

int makeThread(void *(*fn)(void *), void *arg)
{
    int err;
    pthread_t tid;
    pthread_attr_t attr;

    err = pthread_attr_init(&attr);
    if(err != 0)
        return err;

    err = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    if(err == 0) {
        err = pthread_create(&tid, fn, arg);
    }

    pthread_attr_destroy(&attr);

    return(err);
}

int main(void)
{
    unsigned long i;
    struct timeval start, end;
    long long startusec, endusec;
    double elapsed;
    int err;
    pthread_t tid;

    //Create the initial set of numbers to sort
    srandom(1);
    for(i = 0; i < NUMNUM; i++)
        nums[i] = random();

    //Create 8 threads to sort the numbers
    gettimeofday(&start, NULL);
    pthread_barrier_init(&b, NULL, NTHR+1);
    for(i = 0; i < NTHR; i++) {
        err = pthread_create(&tid, NULL, thr_fn, (void *)(i * TNUM));
        if(err != 0)
            err_exit(err, "can't create thread");
    }
    pthread_barrier_wait(&b);
    merge();
    gettimeofday(&end, NULL);


    //print the sorted list

    startusec = start.tv_sec * 1000000 + start.tv_usec;
    endusec = end.tv_sec * 1000000 + end.tv_usec;
    elapsed = (double)(endusec - startusec) / 1000000.0;
    printf("sort took %.4f seconds\n", elapsed);
    for(i = 0; i < NUMNUM; i++)
        printf("%ld\n", snums[i]);

    exit(0);
}