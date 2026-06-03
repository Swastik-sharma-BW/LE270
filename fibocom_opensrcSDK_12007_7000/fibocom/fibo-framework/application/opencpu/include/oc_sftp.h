#ifndef __OC_SFTP_H__
#define __OC_SFTP_H__


INT32 fibo_sftp_open(char * remoteaddr,  const char *username,const char *password,int32_t uRemotePort);
INT32 fibo_sftp_write(const char *loclpath,const char *sftppath);
INT32 fibo_sftp_read(const char *sftppath,const char *localfile);
INT32 fibo_sftp_open_file(uint8_t index, char * remoteaddr,  const char *username,const char *password,int32_t uRemotePort, const char *sftppath);
INT32 fibo_sftp_write_file(uint8_t index, void* data ,uint32_t size);
INT32 fibo_sftp_close_file(uint8_t index);


#endif