#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<termios.h>
#include<string.h>
//gcc -g uart.c -o ulog_capture
#define MAX_BUFF_SIZE 128*1024
char buf_read[MAX_BUFF_SIZE];

int set_opt(int fd,int nSpeed, int nBits, char nEvent, int nStop)
{
	struct termios newtio,oldtio;
	if( tcgetattr( fd,&oldtio)  !=  0) {
	    perror("tcgetattr error");
	    return -1;
	}
	bzero( &newtio, sizeof( newtio ) );
	newtio.c_cflag  |=  CLOCAL | CREAD; 
	newtio.c_cflag &= ~CSIZE; 

	switch( nBits )
	{
	    case 7:
	        newtio.c_cflag |= CS7;
	        break;
	    case 8:
	        newtio.c_cflag |= CS8;
	        break;
	}

	switch( nEvent )
	{
	    case 'O':
	        newtio.c_cflag |= PARENB; 
	        newtio.c_cflag |= PARODD;  
	        newtio.c_iflag |= (INPCK | ISTRIP); 
	        break;
	    case 'E':
                newtio.c_iflag |= (INPCK | ISTRIP);
                newtio.c_cflag |= PARENB;
                newtio.c_cflag &= ~PARODD;
                break;
	    case 'N': 
	        newtio.c_cflag &= ~PARENB;
	        break;
	}

	switch( nSpeed )
	{
	    case 2400:
	        cfsetispeed(&newtio, B2400);
	        cfsetospeed(&newtio, B2400);
	        break;
	    case 4800:
	        cfsetispeed(&newtio, B4800);
	        cfsetospeed(&newtio, B4800);
	        break;
	    case 9600:
	        cfsetispeed(&newtio, B9600);
	        cfsetospeed(&newtio, B9600);
	        break;
	    case 115200:
	        cfsetispeed(&newtio, B115200);
	        cfsetospeed(&newtio, B115200);
	        break;
	    case 460800:
	        cfsetispeed(&newtio, B460800);
	        cfsetospeed(&newtio, B460800);
	        break;
	    default:
	        cfsetispeed(&newtio, B9600);
	        cfsetospeed(&newtio, B9600);
	        break;
	}

	if( nStop == 1){
	    newtio.c_cflag &=  ~CSTOPB; 
	}else if ( nStop == 2 ){
	    newtio.c_cflag |=  CSTOPB;
	} 
	newtio.c_cc[VTIME]  = 0;
	newtio.c_cc[VMIN] = 0;
	tcflush(fd,TCIFLUSH); 
	if((tcsetattr(fd,TCSANOW,&newtio))!=0)
	{
	    perror("set error");
	    return -1;
	}
	return 0;
}

int main(int argc,char *argv[])
{
	int fd,ret_set,ret_read;

	char tty[32];
	char *logfile_name = "./capture_log.bin";
	int log_fd;
	int ret_write;
	
	int baudrate = 460800;
	char *dev_name = "/dev/ttyACM1";
	if (argc==1)
	{
		//default
	}
	else if (argc == 2)
	{
		dev_name = argv[1];
	}
	else if (argc == 3)
	{
		dev_name = argv[1];
		logfile_name = argv[2];
	}
	else if (argc == 4)
	{
		dev_name = argv[1];
		logfile_name = argv[2];
		baudrate = atoi(argv[3]);
	}
	
	if(argc>1)
	{
	    strcpy(tty,dev_name);
	    fd = open(tty, O_RDWR);
	    if(fd == -1)
	    {
		printf("Open %s failed! Exit!\n",tty);
		exit(1);
	    }
	    log_fd = open(logfile_name, O_TRUNC|O_CREAT|O_WRONLY);
		
		printf("capture log to %s ! !\n",logfile_name);
	    printf("open %s successfully!\n",tty);

	    ret_set = set_opt(fd, baudrate, 8, 'N', 1);
	    if (ret_set == -1)
	    {
		printf("Set %s failed! Exit!\n",tty);
		exit(1);
	    }
	    printf("Set %s successfully!\n",tty);
	    printf("Baud rate: %d\n",baudrate);	
	    while (1)
	    { 
			
	        ret_read = read(fd, buf_read, MAX_BUFF_SIZE);
	        if(ret_read > 0){
	            //printf("Read data: %s\n\n", buf_read);
				ret_write = write(log_fd, buf_read, ret_read);
				if (ret_write!=ret_read)
				{
					printf("ret_read : %d, ret_write : %d\n\n", ret_read, ret_write);
				}
				
	        }
	        //sleep(3);
	    }
		close(fd);
		close(log_fd);
	}else{
	    printf("Usage: log_capture [tty node] [logfile_name] [baud rate]\n");
	    printf("       Sample: log_capture /dev/ttyACM1 ./capture_log.bin 460800 \n");
	}
 	return 0;
}
