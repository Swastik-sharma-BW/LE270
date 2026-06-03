#include "app_image.h"
#include "fibo_opencpu.h"

#include "app.h"
#include "app_pdp.h"

static void hello_world_demo()
{
    int i = 0;
    for (i = 0; i < 5; i++)
    {
        APP_LOG_INFO("Hello world %d", i);
        fibo_taskSleep(1000);
    }

    return ;
}

void *appimg_enter(void *param)
{
    APP_LOG_INFO("application appimg_enter");
    return app_init(hello_world_demo);
}
