#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <assert.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "dbl.h"
#include "dbl_ext.h"
#ifndef _WIN32
#include <math.h>
#endif

#ifdef _WIN32
#pragma comment(lib, "ws2_32.lib")
#endif

#ifndef _WIN32
#define PERR(err)				\
  if (rc < 0) {					\
    perror(err);				\
    exit(1);					\
  }
#else
#define PERR(err)					\
  if (rc < 0) {						\
    printf ("%s: %d\n", err, WSAGetLastError());	\
    exit(1);						\
  }
#endif


int start_size;
int end_size;
int increment = 0;
int iterations = 0;
int verbose = 0;
int dchan_nb = 0;
int warmup_count = 0;
enum dbl_recvmode Recv_mode;

void
server(
  char *local_addr,
  int type,
  int port
  )
{
  struct sockaddr_in sin;
  /* */
  dbl_device_t dev;                  /* device stores all the channels */
  dbl_channel_t ch;                  /* TCP base channel description */
  dbl_channel_t retch;               /* TCP accepted channel description */
  dbl_send_t retsh;                  /* Send handle to be used for the accepted channel */
  dbl_channel_t *dchan;              /* DBL channel description */
  struct dbl_recv_info *rxi;         /* rxi recv info array for recvmsg */
  struct dbl_recv_info  rxinfo;      /* rxi recv info */
  dbl_send_t sh;  
  
  int rc;
  char buf[20000];
  int sinlen = sizeof(struct sockaddr_in);
  int sock_type;
  int proto_type;
  int i;
  
  /* build local socket address */
  memset(&sin, 0, sizeof(sin));
  sin.sin_family = AF_INET;
  sin.sin_port = htons(port);
  inet_pton(AF_INET, local_addr, &sin.sin_addr);
  //sin.sin_addr.S_un.S_addr = local_addr;
  printf("Addres for open is %d\n", sin.sin_addr.S_un.S_addr);

  rc = dbl_init(DBL_VERSION_API);
  if (rc != 0) { rc = -rc; PERR("dbl_init"); }

  printf("Try to dbl_open()\n");
  rc = dbl_open(&sin.sin_addr, DBL_OPEN_THREADSAFE, &dev);
  if (rc != 0) {
      printf("dbl_open() finised with code %d\n", rc);
      rc = -rc;
      PERR("dbl_open");
  }

  /* 1st channel is a TCP/MTCP ? */
  sock_type  = DBL_TYPE_IS_TCP(type);
  proto_type = DBL_PROTO_IS_MTCP(type);
  int flags = DBL_CHANNEL_FLAGS(sock_type, proto_type);
  //flags |= DBL_OPEN_THREADSAFE;
  printf("Try to dbl_bind() flags = %d\n", flags);
  int j = 0;
  for (j = 0; j < 60000; ++j) {
    rc = dbl_bind(dev, j, j, NULL, &ch);
    rc = dbl_ext_listen (ch);
    //printf("j = %d\n", j);
    if (rc != 0) { rc = -rc; PERR("dbl_bind"); }
    rc = dbl_ext_channel_type(ch);
    if (rc == 1) {
        printf("Channel type %d with j = %d\n", dbl_ext_channel_type(ch), j);
    } else if (rc != 0) {
        printf("rc != 0");
    }
    //rc = dbl_unbind(ch);
    memset(ch, 0, sizeof(dbl_channel_t));
  }
  /*rc = dbl_bind(dev, flags, port, NULL, &ch);
  if (rc != 0) { rc = -rc; PERR("dbl_bind"); }*/

  printf("Channel type %d\n", dbl_ext_channel_type(ch));

    printf ("Setting up to listen()\n");
    rc = dbl_ext_listen (ch);


    if (rc != 0) { PERR("dbl_listen"); }

    printf ("Setting up to accept()\n");
    rc = dbl_ext_accept (ch, (struct sockaddr *)&sin, &sinlen, NULL, &retch);
    if (rc != 0) { PERR("dbl_ext_accept"); }
    printf ("Got TCP connection from %d\n", sin.sin_addr.S_un.S_addr);
  //}
  /* Main loop, allocate receiving structures to be used in the
     reception recvmsg() function. 
  */
  //rxi = calloc(2, sizeof(struct dbl_recv_info));
  //using_multiple_recvs = 0;
  //Recv_mode = 0;

  //rc = dbl_send_connect(retch, &sin, 0, 0, &retsh);
}


int main(
  int argc,
  char **argv)
{
  int sock_type = DBL_TCP;
  int proto_type = DBL_BSD;
  int flags = 0;

  printf ("Test will be using Type %s, Proto %s\n", 
	  sock_type == 1 ? "DBL_TCP" : "DBL_UDP", 
	  proto_type == 1 ? "DBL_BSD" : "DBL_MYRI");

  flags = DBL_CHANNEL_FLAGS(sock_type, proto_type);

  //int rc = setup_timeout(1000, timesup); // no object file for this procedure

  /* more flags ? */

  // flags |= DBL_OPEN_THREADSAFE;

  server("10.116.2.177", flags, 8888);

  printf("Finished!\n");
  return 0;
}
