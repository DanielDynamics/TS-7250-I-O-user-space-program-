/*
 * lab1p1.c
 *
 *  Created on: Aug 31, 2016
 *      Author: zxdhf
 */
#include <stdio.h>
#include <sys/mman.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char **argv)
{
	int fp;
	unsigned long *PFDR,*PFDDR;
	unsigned long *PBDR,*PBDDR;  //create pointer to port B/F DR/DDR
    unsigned long *ptr;  //create pointer to mmap
    unsigned char state;  //create button's state
    int button;

    fp = open("/dev/mem",O_RDWR);
	if(fp == -1){
		printf("\n error\n");
	    return(-1);  // failed open
	}
	ptr = (unsigned long*)mmap(NULL,4096,PROT_READ|PROT_WRITE,MAP_SHARED,fp,0x80840000);
    if(ptr == MAP_FAILED){
    	printf("\n Unable to map memory space \n");
    	return(-2);
    }  // failed mmap
	PBDR = ptr+1;
    PBDDR = ptr+5;
    PFDR = ptr+12;
    PFDDR = ptr+13;  // give physical address
    *PBDDR |= 0xE0;  // give input, pbddr 11100000
    *PFDDR |= 0x02;  // give output, pfddr 00000010

    printf("Enter a number 0~4 to turn on the speaker:");
    scanf("%d",&button);
    while(button<0||button>4)
    {
    	printf("Wrong number ! Enter a number 0~4 to turn on the speaker:");
    	scanf("%d",&button);
    }

    state=*PBDR;
    // For button = 0; Press button 0 to skip out of the while loop and play the sound.
	if(button==0)
	{
    	while(state & 0x01)
	   {
    	 state = *PBDR;
    	 printf("Checking button 0\n");
	   }
	}
	// For button = 1; Press button 0 to skip out of the while loop and play the sound.
	else if(button==1)
	{
		while(state & 0x02)
	   {
		 state = *PBDR;
		 printf("Checking button 1\n");
	   }
	}
	// For button = 2; Press button 0 to skip out of the while loop and play the sound.
	else if(button==2)
	{
		while(state & 0x04)
	   {
	     state = *PBDR;
	     printf("Checking button 2\n");
	   }
	}
	// For button = 3; Press button 0 to skip out of the while loop and play the sound.
	else if(button==3)
	{
	   while(state & 0x08)
	  {
		 state = *PBDR;
		 printf("Checking button 3\n");
	  }
     }
	// For button = 4; Press button 0 to skip out of the while loop and play the sound.
	else
	{
	   while(state & 0x10)
	  {
	    state = *PBDR;
	    printf("Checking button 4\n");
	  }
	 }
    while(1)
    {
    	*PFDR = *PFDR | 0x2;
    	printf("PFDR on is %lu\n",*PFDR);
        usleep(100000);
        *PFDR = *PFDR & 0x5;
        printf("PFDR off is %lu\n",*PFDR);
        usleep(100000);

    }  //create square wave
    close(fp);
    return 0;
}
