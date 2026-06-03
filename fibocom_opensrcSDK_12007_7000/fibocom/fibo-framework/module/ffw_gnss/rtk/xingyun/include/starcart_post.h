#ifndef _STARCART_POST_H_
#define _STARCART_POST_H_
#ifdef __cplusplus
extern "C"
{
#endif

#include "fibo_opencpu_comm.h"
#define QUEUE_SZIE 1024   
#define MAXSIZE 100  //100


/*循环队列对的顺序存储结构*/
typedef struct
{
    char ais_data[MAXSIZE][QUEUE_SZIE]; //队列中的二维数组元素  650->850
    int front;                   //头指针
    int rear;                    //尾指针
} sqQueue;

typedef struct
{
    unsigned char *buf;
    unsigned int size;
    unsigned int head;
    unsigned int tail;
    unsigned empty : 1;
    unsigned full : 1;
    unsigned overflow : 1; //queue input after full, overwrite occur
} fd_cycle_queue;

extern int InitQueue(sqQueue *Q);
extern int ClearQueue(sqQueue *Q);
extern int QueueLength(const sqQueue *Q);
extern int EnQueue(sqQueue *Q, char *e);
extern int iDeQueue(sqQueue *Q, char *e);


#ifdef __cplusplus
}
#endif
#endif
