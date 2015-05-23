/**
 * Lecture des données sur le port serie.
 * credit : https://github.com/cheydrick/Canonical-Arduino-Read/blob/master/canonicalarduinoread.c
 */


/* www.chrisheydrick.com
     
   June 23 2012
   CanonicalArduinoRead write a byte to an Arduino, and then
   receives a serially transmitted string in response.
   The call/response Arduino sketch is here:
   https://gist.github.com/2980344
   Arduino sketch details at www.chrisheydrick.com
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <sys/ioctl.h>

#define DEBUG 1
  
int main(int argc, char *argv[])
{
  int fd, n, i, j;
  uint8_t data[11];
  struct termios toptions;

  /* open serial port */
  fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY);
  if(fd < 0){
    printf("Error opening /deC/ttyUSB0\n");
  }else{
    printf("File open !\n");
  }

  printf("get current serial port settings\n");
  /* get current serial port settings */
  tcgetattr(fd, &toptions);
  /* set 9600 baud both ways */
  printf("Setting speed\n");
  cfsetispeed(&toptions, B57600);
  cfsetospeed(&toptions, B57600);
  /* 8 bits, no parity, no stop bits */
  cfmakeraw(&toptions);

  printf("commit the serial port settings\n");
  tcsetattr(fd, TCSANOW, &toptions);
  
  printf("Ready to read...\n");
  j=0;
  while(1){ 
    /* Receive string from Arduino */
    n = read(fd, data, 11);
    if(n == -1){
        printf("Err!\n");
    }
    for(i = 0 ; i < n ; i++){
        j++;
        printf("%u ",(unsigned)data[i] );
        if(j == 11){
            j=0;
            printf("\n");
        }
    }
  }

 
  return 0;
}
