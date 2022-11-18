#include<STC15F2K60S2.H>

#include"key.c"

#include"iic.c"

unsigned char sr,sr1,nss,cp,ks,kss;

unsigned int ns;

unsigned char seven_seg[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};

void stc_init()
{
	P2=0xa0;
	P0=0x00;
	P2=0x00;
	P2=0x80;
	P0=0xff;
	P2=0x00;
}

void timer0_init()
{
	TMOD=0x01;
	TH0=(65536-5000)/256;
	TL0=(65536-5000)%256;
	EA=1;
	ET0=1;
	TR0=1;
}
void timer0_isr()interrupt 1
{	
	if(ns<cs)
	{
		TH0=(65536-5000)/256;
		TL0=(65535-5000)%256;
		cp++;
		if(cp>=200)
		{
			ks++;
			if(ks>5)
			{
				ks=0;
			}
			cp=0;
		}
	}
	if(ns>=cs)
	{
		cp=0;		
	}
}
void display_js()
{
	if(sr>cs)
	{
		sr1=1;
	}
	else if(sr1==1)
	{
		sr1=0;
		js++;
		if(js>99)
		{
			js=0;
		}
	} 
	if(js%2==1)
	{
			P2=0x80;
			P0=0xfd;
			P2=0x00;
	}	
}

void display()
{	
	unsigned char a,k,n;
	P36=0;
	P0=0xff;
	if(key_flag==0)
	{
		switch(a)
		{
			case 0:P2=0xc0;P0=0x80;P2=0xe0;P0=seven_seg[ns%10];break;
			case 1:P2=0xc0;P0=0x40;P2=0xe0;P0=seven_seg[ns/10%10];break;
			case 2:P2=0xc0;P0=0x20;P2=0xe0;P0=seven_seg[ns/100]&0x7f;break;
			case 3:P2=0xc0;P0=0x01;P2=0xe0;P0=0xc1;break;		   //电压u的标识符位0xc1
		}
		a++;
		if(a>3)
		{
			a=0;
		}
	}
	if(key_flag==1)
	{
		switch(k)
		{
			case 0:P2=0xc0;P0=0x80;P2=0xe0;P0=seven_seg[cs%10];delay(100);break;
			case 1:P2=0xc0;P0=0x40;P2=0xe0;P0=seven_seg[cs/10%10];delay(100);break;
			case 2:P2=0xc0;P0=0x20;P2=0xe0;P0=seven_seg[cs/100]&0x7f;delay(100);break;
			case 3:P2=0xc0;P0=0x01;P2=0xe0;P0=0x8c;delay(100);break;
		}
		k++;
		if(k>3)
		{
		 	k=0;
		}
	}
	if(key_flag==2)
	{
		switch(n)
		{
			case 0:P2=0xc0;P0=0x80;P2=0xe0;P0=seven_seg[js%10];delay(100);break;
			case 1:P2=0xc0;P0=0x40;P2=0xe0;P0=seven_seg[js/10];delay(100);break;
			case 2:P2=0xc0;P0=0x01;P2=0xe0;P0=0xc8;delay(100);break;
		}
		n++;
		if(n>3)
		{
			n=0;
		}
	}	
}
		
void main()
{
	cs=300;	
	cs=eeprom_read(0);
	stc_init();
	timer0_init();
	while(1)
	{
		sr=adc_read(3);
		ns=(float)sr*(5.0/255)*100;
		key_scanf();
		display();
		eeprom_writer(0,cs/10);
		display_js();
		if((sr/10)>(cs/10))
		{
			P2=0x80;
			P0=0xff;
			P2=0x00;
		}
		if(ks==5)
		{
			P2=0x80;
			P0=0xfe;
			P2=0x00;	
		}
		if(ns>=cs)
		{
			P2=0x80;
			P0=0xff;
			P2=0x00;	
		}
		
	}
}	 