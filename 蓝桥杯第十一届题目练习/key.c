#include<STC15F2K60S2.H>

unsigned char js=0;

unsigned char key_flag=0,P35_s,P34_s,P35_s1,P34_s1;

unsigned int cs = 300;

void delay(unsigned int x)
{
	while(x--);
}
void key_scanf()
{
	P3=0xff;
	P33=0;
	if(P35==0)			//°´¼üS4
	{
		delay(200);
		if(P35==0)
		{
			P35_s=1;
		}
	}
	if(P35==1&&P35_s==1)
	{
		P35_s=0;
		key_flag++;				  //three mode
		if(key_flag>2)key_flag=0;
	}
	if(P34==0)
	{
		delay(200);
		if(P34==0)
		{
		 	P34_s=1;
		}
	}
	if(P34==1&&P34_s==1)
	{
		P34_s=0;
		cs=cs-50;
		if(cs<0)
		{
			cs=400;
		}	
	}
	P3=0xff;
	P32=0;
	if(P35==0)
	{
		delay(200);
		if(P35==0)
		{
			P35_s1=1;	
		}
		}
	if(P35==1&&P35_s1==1)
	{  
		P35_s1=0;
		js=0;	
	}
	if(P34==0)
	{
		delay(200);
		if(P34==0)
		{
		 	P34_s1=1;
		}
	}
	if(P34==1&&P34_s1==1)
	{
		P34_s1=0;
		cs=cs+50;
		if(cs>500)
		{
			cs=0;
		}	
	}	
}