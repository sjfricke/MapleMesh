#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>  // socket, setsockopt, accept, send, recv
#include <sys/socket.h>

#include <signal.h>
#include <pthread.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

pthread_t probe_thread_0;
pthread_t probe_thread_1;

const int POLL_TIME = 4; // secs

typedef struct slave_t {
  char mac[18];
  uint8_t channel;
} slave_t;

// TODO dynamically set how many slaves
slave_t g_slaves[3];

void* probeValues(void* slave_arg) {

  // detaches thread and sets up ws_client
  pthread_detach(pthread_self());
  pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
  pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);

  slave_t* slave = (slave_t*)slave_arg;

  pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
  
  struct sockaddr_rc addr = { 0 };
  int s, status;
  

  while (1) {

      // allocate a socket
  s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

  // set the connection parameters (who to connect to)
  addr.rc_family = AF_BLUETOOTH;
  addr.rc_channel = slave->channel;
  str2ba( slave->mac, &addr.rc_bdaddr );

    // connect to server
    status = connect(s, (struct sockaddr *)&addr, sizeof(addr));

    // send a message
    if( status == 0 ) {
      status = write(s, "0", 1);
      close(s);
      usleep(1000000 * POLL_TIME);
    } else {
      close(s); // skip poll time
    }
    
  }
  return 0;
}

int main(int argc, char **argv)
{
  struct sockaddr_rc loc_addr = { 0 }, rem_addr = { 0 };
  char buf[1024] = { 0 };
  int s, client, bytes_read, status;
  socklen_t opt = sizeof(rem_addr);

  // allocate socket
  s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

  // bind socket to port 1 of the first available
  // local bluetooth adapter
  loc_addr.rc_family = AF_BLUETOOTH;
  loc_addr.rc_bdaddr = *BDADDR_ANY;
  loc_addr.rc_channel = (uint8_t) 8;
  bind(s, (struct sockaddr *)&loc_addr, sizeof(loc_addr));

  // put socket into listening mode
  listen(s, 1);

  // send out probers
  strcpy(g_slaves[0].mac, "02:00:98:AC:2E:71"); // DB1
  g_slaves[0].channel = 1;
  
  strcpy(g_slaves[1].mac, "B8:27:EB:DF:33:A1"); // Pi0
  g_slaves[1].channel = 1;

  status = pthread_create(&probe_thread_0,
			  NULL,
			  probeValues,
			  (void *)(&g_slaves[0]));

  if (status < 0) {
    printf("--SERVER-- ERROR: Are you feeling it now Mr Krabs?\n");
  }
  pthread_detach(probe_thread_0);

  status = pthread_create(&probe_thread_1,
			  NULL,
			  probeValues,
			  (void *)(&g_slaves[1]));

  if (status < 0) {
    printf("--SERVER-- ERROR: Are you feeling it now Mr Krabs?\n");
  }
  pthread_detach(probe_thread_1);  

  // accept one connection
  while((client = accept(s, (struct sockaddr *)&rem_addr, &opt))) {

    ba2str( &rem_addr.rc_bdaddr, buf );
    fprintf(stderr, "accepted connection from %s\n", buf);
    memset(buf, 0, sizeof(buf));

    // read data from the client
    bytes_read = read(client, buf, sizeof(buf));
    if( bytes_read > 0 ) {
      switch (buf[0]) {
      case '0': // get value
	//	sendValue();
	break;
      case '1':
	break;
      case '2':
	break;
      default:
	break;
      }
      //      setLED();
      printf("received [%s]\n", buf);
    }
  }

  // close connection
  close(client);
  close(s);
  return 0;
}
