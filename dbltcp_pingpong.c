#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <assert.h>
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#endif
#include <dbl.h>
#include <dbl_ext.h>

#ifndef _WIN32
#include <math.h>
#endif

#ifdef _WIN32
#pragma comment(lib, "ws2_32.lib")
#endif

#ifndef _WIN32
#define PERR(err)                               \
  if (rc < 0) {                                 \
    perror(err);                                \
    exit(1);                                    \
  }
#else
#define PERR(err)                                       \
  if (rc < 0) {                                         \
    printf ("%s: %d\n", err, WSAGetLastError());        \
    exit(1);                                            \
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

  dbl_channel_t channs[5];

  /* build local socket address */
  memset(&sin, 0, sizeof(sin));
  sin.sin_family = AF_INET;
  sin.sin_port = htons(port);
  sin.sin_addr.s_addr = inet_addr(local_addr);
  //sin.sin_addr.S_un.S_addr = local_addr;
 printf("Addres for open is %d\n", sin.sin_addr.S_un.S_addr);

  rc = dbl_init(DBL_VERSION_API);
  if (rc != 0) { rc = -rc; PERR("dbl_init"); }

  printf("Try to dbl_open()\n");
  rc = dbl_open(&sin.sin_addr, 0, &dev);
  if (rc != 0) {
      printf("dbl_open() finised with code %d\n", rc);
      rc = -rc;
      PERR("dbl_open")
  }

   /* 1st channel is a TCP/MTCP ? */
//    sock_type  = DBL_TYPE_IS_TCP(type);
//    proto_type = DBL_PROTO_IS_MTCP(type);
//    int flags = DBL_CHANNEL_FLAGS(sock_type, proto_type);

    int flags = DBL_CHANNEL_FLAGS(DBL_TCP, DBL_BSD);
    flags |= DBL_BIND_REUSEADDR | DBL_BIND_BROADCAST;

    printf("Try to dbl_bind() flag = %d, port = %d\n", flags, 8888);
    rc = dbl_bind(dev, flags, 8888, NULL, &channs[0]);
    //rc = dbl_bind_addr(dev, &sin.sin_addr, flags, 8888, NULL, &channs[0]);
    if (rc != 0) { rc = -rc; PERR("dbl_bind"); }

    printf("Try to dbl_ext_listen()\n");
    rc = dbl_ext_listen (channs[0]);

    printf("Try to dbl_ext_poll()... ");
    int r = dbl_ext_poll(channs, 1, 5000);
    printf("result = %d\n", r);

    printf("Channel type %d\n", dbl_ext_channel_type(ch));

    printf ("Setting up to accept()\n");
    rc = dbl_ext_accept (channs[0], (struct sockaddr *)&sin, &sinlen, NULL, &retch);
    if (rc != 0) { PERR("dbl_ext_accept"); }
    printf ("Got TCP connection from %d\n", sin.sin_addr.s_addr);

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
