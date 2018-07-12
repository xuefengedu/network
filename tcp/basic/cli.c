#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

char *server = "127.0.0.1";  /* loopback */
int port = 6180;             /* no significance */

int main(int argc, char *argv[]) {

  char *buf = "hello world from client!";
  int buflen = strlen(buf), rc;

  /**********************************************************
   * create an IPv4/TCP socket, not yet bound to any address
   *********************************************************/
  int fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd == -1) {
    printf("socket: %s\n", strerror(errno));
    exit(-1);
  }

  /**********************************************************
   * internet socket address structure, for the remote side
   *********************************************************/
  struct sockaddr_in sin;
  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = inet_addr(server);
  sin.sin_port = htons(port);

  if (sin.sin_addr.s_addr == INADDR_NONE) {
    printf("invalid remote IP %s\n", server);
    exit(-1);
  }

  /**********************************************************
   * Perform the 3 way handshake, (c)syn, (s)ack/syn, c(ack)
   *********************************************************/
  if (connect(fd, (struct sockaddr*)&sin, sizeof(sin)) == -1) {
    printf("connect: %s\n", strerror(errno));
    exit(-1);
  }

  if ( (rc=write(fd,buf,buflen)) != buflen) {
    printf("write: %s\n", (rc<0)?strerror(errno):"incomplete");
    //exit(-1);
  }

  if ( (rc=write(fd,"#END#",6)) != 6) {
    printf("write: %s\n", (rc<0)?strerror(errno):"incomplete");
  }

  int n=0;
  char recvBuff[1024];
  memset(recvBuff, '0',sizeof(recvBuff));
  while ( (n = read(fd, recvBuff, sizeof(recvBuff)-1)) > 0)
  {
      recvBuff[n] = 0;
      if(fputs(recvBuff, stdout) == EOF)
      {
          printf("\n Error : Fputs error\n");
      }
  } 

  if(n < 0)
  {
      printf("\n Read error \n");
  } 
}
