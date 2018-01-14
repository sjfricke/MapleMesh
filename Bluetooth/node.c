// TODO - Need to check power and discoverable are on from bluetoothctl
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/socket.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

#define MASTER_MAC "02:00:BF:C9:2A:49"

#define TRUE 1
#define FALSE 0

typedef struct node_t {
  int16_t temp;
  uint8_t vol;
  uint8_t full;  
  float lat;
  float lng;
  char  mac[18];
} node_t;



// for demo we are blinking light
// GPIO 13 == pin 33 on Pi == pin 25 on dragonboard
void setGPIO() {
  FILE *fp;
  FILE *ff;
  FILE *fpp;
  
  fp = fopen("/sys/class/gpio/export", "w");
  if (fp == NULL) {
    perror("Could not open gpio export\n");
    return;
  }

  fprintf(fp, "%u", 13);
  fclose(fp);

  ff = fopen("/sys/class/gpio/gpio13/value", "w");
  if (ff == NULL) {
    perror("Could not open gpio value\n");
    return;
  }

  fputs("0", ff);
  
  fclose(ff);

  fpp = fopen("/sys/class/gpio/gpio13/direction", "w");
  if (fpp == NULL) {
    perror("Could not open gpio direction\n");
    return;
  }

  fputs("out", fpp);
  fclose(fpp);
}

void setLED() {
  FILE *fp;
  fp = fopen("/sys/class/gpio/gpio13/value", "w");
  if (fp == NULL) {
    perror("Could not open gpio value\n");
    return;
  }

  fputs("1", fp);
  fclose(fp);
  
  usleep(1000000); // 1 sec;
  
  fp = fopen("/sys/class/gpio/gpio13/value", "w");
  if (fp == NULL) {
    perror("Could not open gpio value\n");
    return;
  }

  fputs("0", fp);  
  fclose(fp);
}

void sendValue(){
  printf("sent value\n");

  struct sockaddr_rc addr = { 0 };
  int s, status;
  FILE* fp;
  char volume[4];
  // allocate a socket
  s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

  // set the connection parameters (who to connect to)
  addr.rc_family = AF_BLUETOOTH;
  addr.rc_channel = 8;
  str2ba(MASTER_MAC, &addr.rc_bdaddr );

    // connect to server
    status = connect(s, (struct sockaddr *)&addr, sizeof(addr));

    // send a message
    if( status == 0 ) {
      fp = fopen("/home/pi/volume.txt", "r");
      if (fp == NULL) {
	perror("Could not open volume.txt\n");
      } else {
	fgets(volume, sizeof(volume), fp);
	status = write(s, volume, sizeof(volume));
	fclose(fp);
      }
    } else {
      perror("Didn't send back\n");
    }

    close(s); // skip poll time
   
}

int main(int argc, char **argv)
{
  // for demo
  setGPIO();
  
  struct sockaddr_rc loc_addr = { 0 }, rem_addr = { 0 };
  char buf[1024] = { 0 };
  int s, client, bytes_read;
  socklen_t opt = sizeof(rem_addr);

  // allocate socket
  s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

  // bind socket to port 1 of the first available
  // local bluetooth adapter
  loc_addr.rc_family = AF_BLUETOOTH;
  loc_addr.rc_bdaddr = *BDADDR_ANY;
  loc_addr.rc_channel = (uint8_t) 1;
  bind(s, (struct sockaddr *)&loc_addr, sizeof(loc_addr));

  // put socket into listening mode
  listen(s, 1);

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
	  sendValue();
	  break;
	case '1':
	  break;
	case '2':
	  break;
	default:
	  break;
	}
	setLED();
	printf("received [%s]\n", buf);
      }
  }

  // close connection
  close(client);
  close(s);
  return 0;
}
