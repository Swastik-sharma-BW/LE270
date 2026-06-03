#include <stdio.h>
#include "fibo_opencpu_api.h"
#include "fibo_opencpu_comm.h"

#ifdef APP_DEMO_ENABLE
#include "app.h"
#include "app_pdp.h"
#include "app_socket.h"
#include "app_http.h"
#endif

#include "fibo_opencpu_api.h"

int id = 125;

#ifdef APP_DEMO_ENABLE
static void apprun()
{
	int ret = 0;
	fibo_printf("apprun ");

	/// 1. PDP ACTIVE
	int simid = 0;
	int cid = 1;
	app_pdp_iptype_t iptype = APP_PDP_IPTYPE_IPV4V6;
 	char *apn = NULL;
	char *apnuser = NULL;
	char *apnpasswd = NULL;
	app_pdp_authproto_t auth_proto = APP_PDP_AUTHPROTO_NONE;
	
	ret = app_pdp_active(simid, cid, iptype, apn, apnuser, apnpasswd, auth_proto);
	if (ret != 0)
	{
		fibo_printf("pdp active fail");
		return;
	}

	app_socket_demo();

	while(1)
	{
		fibo_printf("%s", "app running");
		fibo_taskSleep(1000);
	}
	return;	
}
#endif

FIBO_CALLBACK_T* fibo_main(void)
{	
	char buf[128];
	sprintf(buf, "this is demo %d", id);
	fibo_printf("%s", buf);

#ifdef APP_DEMO_ENABLE
	return app_init(apprun);
#else
	return NULL;
#endif
}