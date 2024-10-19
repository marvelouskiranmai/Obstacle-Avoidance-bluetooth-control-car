#include <reg51.h>
#include <intrins.h>
void sendser_char(unsigned char b);
void sendser_str(unsigned char *str);
void delayMicroseconds(unsigned int us);
void measureDistance();
void right();
void left();
void forward();
void backward();
void stop();
// Assuming these are your motor control GPIO pins
sbit m11 = P2^2;
sbit m12 = P2^3;
sbit m21 = P2^4;
sbit m22 = P2^5;

// Assuming these are your sensor trigger and echo pins
sbit trigPin = P3^0;
sbit echoPin = P3^1;
void main() {
	  unsigned char x;
    TMOD = 0x20; // Timer 1 mode2 reload mode
    TH1 = 0xFD;  // 9600 baud rate
    SCON = 0x50; // 8-bit data, 1 start bit, 1 stop bit
    TR1 = 1;

    sendser_str("Bluetooth controlled car");

    while (1) {
        while (RI == 0); // F, B, L, R, S
        RI = 0;
        x = SBUF;

        if (x == 'F') {
           forward();
					measureDistance();
        } else if (x == 'B') {
            backward();
					 measureDistance();
        } else if (x == 'L') {
					left();
					 measureDistance();
        } else if (x == 'R') {
					right();
					measureDistance();
        } else if (x == 'S') {
           stop();
					
        }

        
    }
}

void measureDistance(){
	 unsigned int time;
	 unsigned int distance;
	
	 trigPin=1;
	 delayMicroseconds(10);
	 trigPin=0;
	while(!echoPin){
		
		TL1=0;
		TH1=0;
		while(echoPin){
			if(TF1){
				break;
			}
		}
		time=TL1 |(TH1 << 8);
		
		distance=(time*343)/2000;
		
		sendser_str("Distance:");
		sendser_char(distance / 100 + '0');
		sendser_char((distance/10)%10+'0');
		sendser_str("cm\r\n");
		
		if (distance < 101) {
        sendser_str("Turning Right!\r\n");
         right();
    }

		delayMicroseconds(5000);
	}
}	
void forward(){
	          m11 = 1;
            m12 = 0;
            m21 = 1;
            m22 = 0;
}
void backward(){
	          m11 = 0;
            m12 = 1;
            m21 = 0;
            m22 = 1;
}
void right(){
	          m11 = 1;
            m12 = 0;
            m21 = 0;
            m22 = 1;
}
void left(){
	          m11 = 0;
            m12 = 1;
            m21 = 1;
            m22 = 0;
}
void stop(){
	          m11 = 1;
            m12 = 1;
            m21 = 1;
            m22 = 1;
}





void delayMicroseconds(unsigned int us) {
    unsigned int i, j;
    for (i = 0; i < us; i++) {
        for (j = 0; j < 3; j++) {
            // Adjust this loop for the required delay
        }
    }
}


void sendser_char(unsigned char b) {
    SBUF = b;
    while (TI == 0);
    TI = 0;
}

void sendser_str(unsigned char *str) {
    while (*str) {
        sendser_char(*str++);
    }
}