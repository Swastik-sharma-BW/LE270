/*******************************************************************************
 * Copyright (c) 2014 IBM Corp.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * and the Eclipse Distribution License is available at
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Allan Stockdill-Mander - initial API and implementation and/or initial documentation
 *******************************************************************************/

#if !defined(__MQTT_LINUX_)
#define __MQTT_LINUX_

#if defined(WIN32_DLL) || defined(WIN64_DLL)
  #define DLLImport __declspec(dllimport)
  #define DLLExport __declspec(dllexport)
#elif defined(LINUX_SO)
  #define DLLImport extern
  #define DLLExport  __attribute__ ((visibility ("default")))
#else
  #define DLLImport
  #define DLLExport
#endif

#include <sys/types.h>
//#include <sys/socket.h>
//#include <sys/ioctl.h> 

#include <sys/time.h>

//#include <netinet/in.h>
//#include <netinet/tcp.h>
//#include <netdb.h>
//#include <arpa/inet.h>

//#include <sys/select.h>
//#include <sys/param.h>
//#include <linux/types.h>
//#include <asm/byteorder.h>
//#include <linux/ip.h>
//#include <signal.h>
#include <unistd.h> //read,write,usleep
#include <fcntl.h>

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "ffw_os.h"


#if defined(USE_MBEDTLS)
#include "compat-1.3.h"
#include "net.h"
#include "ssl.h"
#include "certs.h"
#include "entropy.h"
#include "ctr_drbg.h"
#endif

#if defined(USE_MBEDTLS)

#define CA_CRT_FILE  "./ctwing.cer"

#endif



/*******************************************************/

typedef struct Timer
{
	struct ffw_timeval end_time;
} Timer;


typedef struct Network
{
	int my_socket;
	int (*mqttread) (struct Network*, unsigned char*, int, int,char*,unsigned long);
	int (*mqttwrite) (struct Network*, unsigned char*, int, int);
	int connect_mode;

	#if defined(USE_MBEDTLS)
    mbedtls_ssl_context ssl;  //mbed TLS control context.
    mbedtls_net_context fd;   //mbed TLS network context.
    mbedtls_ssl_config  conf;  //mbed TLS configuration context.
    mbedtls_x509_crt    cacertc; //mbed TLS CA certification.
    mbedtls_x509_crt    clicert; //mbed TLS Client certification.
    mbedtls_pk_context  pkey;  //mbed TLS Client key
    mbedtls_entropy_context entropy; //mbed TLS entropy
    mbedtls_ctr_drbg_context ctr_drbg; //mbed TLS drbg context
    mbedtls_ssl_session saved_session;
	#endif
} Network;

/*********************************************************/

void TimerInit(Timer*);
char TimerIsExpired(Timer*);
void TimerCountdownMS(Timer*, unsigned int);
void TimerCountdown(Timer*, unsigned int);
int TimerLeftMS(Timer*);

int linux_read(Network *n, unsigned char*, int, int,char*,unsigned long);
int linux_write(Network *n, unsigned char*, int, int);
int socket_connected(int sock);

DLLExport void NetworkInit(Network *n);
DLLExport int NetworkConnect(Network *n, char* addr, int port, int timeout_s);
DLLExport void NetworkDisconnect(Network *n);

#endif
