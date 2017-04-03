/* 


How to handle time?
 */

include<stdio.h>
include<fcntl.h>
include<unistd.h>
include<termios.h>
include<string.h>
include <wiriingSerial.h>


void translateData(& buf){
	// translate each of five bytes into correct values
	
	// Check Valid Packet
	char result[5] = 0; // type 
	    // 1st byte
        result[0] = signalStrength & 0x0f;
        if (fingerOut){
			result[0] |= 0x10;
		}
        if (droppingSpO2){
			result[0] |= 0x20;
		}
        if (beep){
			result[0] |= 0x40;
		}
		result[0] |= 0x80; // sync bit

        // 2nd byte
        result[1] = pulseWaveform & 0x7f;

        // 3rd byte
        result[2] = barGraph & 0x0f;
        if (probeError){
			result[2] |= 0x10;
		}
        if (searching){
			result[2] |= 0x20
		}
        result[2] |= (pulseRate & 0x80) >> 1;
		
        // 4th byte
        result[3] = pulseRate & 0x7f;

        // 5th byte
		result[4] = bloodSpO2 & 0x7f;

}

int main(int argc, char *argv[]){
   int file, count;
   char buf[255];

   if ((file = open("/dev/ttyUSB0", O_RDONLY | O_NOCTTY | O_NDELAY))<0){
      perror("UART: Failed to open the device.\n");
      return -1;
   }
   struct termios options;
   tcgetattr(file, &options);
   options.c_cflag = B19200 | CS7 | CREAD | CLOCAL | PARODD;
   options.c_iflag = IGNPAR | ICRNL;
   tcflush(file, TCIFLUSH);
   tcsetattr(file, TCSANOW, &options);
   
   if (serialDataAvail(file) > 0){
	   sprintf('Processing Data...');
	   serialPrintf(file, &buf);
	   
   }
   if (read(file, &buf, 5 ) > 0){
      perror("UART: No Data available\n");
      return -1;
   }
   
   if (buf[data] & 0x80 == true){
	   translateData(buf[data]);
	   
   }
   
   close(file);
   return 0;
}