#ifndef FIBO_POSIX_TASK
#define FIBO_POSIX_TASK

typedef long BaseType_t;
typedef unsigned long UBaseType_t;

typedef uint32_t TickType_t;
struct xSTATIC_MINI_LIST_ITEM
{
	TickType_t xDummy1;
	void *pvDummy2[ 2 ];
};
typedef struct xSTATIC_MINI_LIST_ITEM StaticMiniListItem_t;

typedef struct xSTATIC_LIST
{
	UBaseType_t uxDummy1;
	void *pvDummy2;
	StaticMiniListItem_t xDummy3;
} StaticList_t;

typedef struct xSTATIC_QUEUE
{
	void *pvDummy1[ 3 ];

	union
	{
		void *pvDummy2;
		UBaseType_t uxDummy2;
	} u;

	StaticList_t xDummy3[ 2 ];
	UBaseType_t uxDummy4[ 3 ];
	uint8_t ucDummy5[ 2 ];

	uint8_t ucDummy6;

    UBaseType_t uxDummy8;
    uint8_t ucDummy9;

} StaticQueue_t;

typedef StaticQueue_t StaticSemaphore_t;

typedef struct xSTATIC_EVENT_GROUP
{
	TickType_t xDummy1;
	StaticList_t xDummy2;

	UBaseType_t uxDummy3;

	uint8_t ucDummy4;
} StaticEventGroup_t;

typedef void * TaskHandle_t;

#endif /* FIBO_TASK */
