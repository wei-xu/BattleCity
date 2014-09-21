#include<iostream>
#include<conio.h>
#include<stdlib.h>
#include"colorConsole.h"
#include<ctime>
#include<windows.h>
#include<stdio.h>
#include <mmsystem.h> 
#pragma  comment(lib,"WinMM.Lib")
using namespace std;

#define KEY_UP      72
#define KEY_DOWN    80
#define KEY_LEFT    75
#define KEY_RIGHT   77
#define MX 20//行数
#define MY 70//列数
#define KEY_SPACE   32

int map[MX][MY]={0};
int bomb1[MX][MY]={0};
int lastmove2;
//map数组1为障碍，2为T1，,3为T2,4为T2死亡爆炸点，5为T1死亡爆炸点，6为可打击点

int x=10;int y=10;
int tx=35;int ty=18;
int &dx=x;int &dy=y;
int bsp=35;
int mbsp=25;
int tsp=1200;
int num2=0;
HANDLE handle;
bool attack=false;
int my=0;
int enemy=0;
char mykill[10];
char enemykill[10];
int bombgap=350;
int level=1;
char level1[10];
void drawmap2()
{
	handle=initiate();
	WORD wColors[3];
	wColors[0]=FOREGROUND_RED|FOREGROUND_GREEN;
	wColors[1]=FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE|FOREGROUND_INTENSITY;
	wColors[2]=FOREGROUND_RED|FOREGROUND_INTENSITY;
	for(int i=0;i<MX;i++)
		{
			for(int j=0;j<MY;j++)
		{
			switch(map[i][j])
			{
			case 1:
				{cout<<"#";break;}
			case 0:
			{cout<<" ";break;}
			case 6:
				{cout<<"@";break;}
			default:;
			}
		}
		cout<<endl;
	}
	textout(handle,72,5,wColors+1,1,"杀敌数");
	textout(handle,72,6,wColors,1,"0");
	textout(handle,72,8,wColors+1,1,"被杀数");
	textout(handle,72,9,wColors+2,1,"0");
	textout(handle,72,11,wColors+1,1,"难度");
	textout(handle,72,12,wColors,1,"1");
}
void record()
{
	srand((unsigned)time(NULL));
	for(int a=0;a<MY;a++)
	{map[0][a]=1;}
	for(int m=1;m<MX;m++)
	{
		map[m][0]=1;
		
		for(int n=1;n<MY-1;n++)
		{
			map[m][n]=0;
		}
		map[m][MY-1]=1;
	}
	for(int b=0;b<MY;b++)
	{map[MX-1][b]=1;}
	for(int i=0;i<100;i++)
	{
		int m=1+rand()%68;
		int n=1+rand()%18;
		if(m==10&&n==10||m==tx&&n==ty)
		{
			map[n][m]=0;
		}
		else
			{
				switch(rand()%4)
				{
				case 0:
				{map[n][m]=1;break;}
				case 1:
				{map[n][m]=6;break;}
				case 2:
				{map[n][m]=6;break;}
				case 3:
                {map[n][m]=6;break;}
				}
	  }
	}
}
DWORD WINAPI SHOOTright(LPVOID lpParam)
{
	int dist=80;
	WORD wColors[1];
	wColors[0]=FOREGROUND_RED|FOREGROUND_INTENSITY;
	for(int k=dx,yy=dy;k<dist+dx&&map[yy][k+1]!=1;k++,bomb1[yy][k]=0)
	{
		if(map[yy][k+1]!=3&&map[yy][k+1]!=6&&bomb1[yy][k+1]!=2)
		{
		bomb1[yy][k+1]=1;
		textout(handle,k+1,yy,wColors,1,"o");
		Sleep(mbsp);
		textout(handle,k+1,yy,wColors,1," ");
		}
		else if(map[yy][k+1]==6)
		{
			textout(handle,k+1,yy,wColors,1,"*");
			map[yy][k+1]=0;
			Sleep(20);
			textout(handle,k+1,yy,wColors,1," ");
			break;
		}
		else 
			{textout(handle,k+1,yy,wColors,1,"*");
		map[yy][k+1]=4;break;}
	}
	return TRUE;
}
DWORD WINAPI SHOOTleft(LPVOID lpParam)
{
	int dist=80;
	WORD wColors[1];
	wColors[0]=FOREGROUND_RED|FOREGROUND_INTENSITY;
	for(int k=dx,yy=dy;k>dx-dist&&map[yy][k-1]!=1&&bomb1[yy][k-1]!=2/*&&map[yy][k-1]!=3*/;k--,bomb1[yy][k]=0)
	{
		if(map[yy][k-1]!=3&&map[yy][k-1]!=6/*&&bomb1[ty][tx]!=1*/)
		{
			bomb1[yy][k-1]=1;
			textout(handle,k-1,yy,wColors,1,"o");
		Sleep(mbsp);
		textout(handle,k-1,yy,wColors,1," ");
		}
		else if(map[yy][k-1]==6)
		{
			textout(handle,k-1,yy,wColors,1,"*");
			map[yy][k-1]=0;
			Sleep(20);
			textout(handle,k-1,yy,wColors,1," ");
			break;
		}
		else 
		{
			textout(handle,k-1,yy,wColors,1,"*");
			map[yy][k-1]=4;
			break;
		}
	}
	return TRUE;

}
DWORD WINAPI SHOOTup(LPVOID lpParam)
{
	int dist=40;
	WORD wColors[1];
	wColors[0]=FOREGROUND_RED|FOREGROUND_INTENSITY;
	for(int xx=dx,k=dy;k>dy-dist&&map[k-1][xx]!=1;k--,bomb1[k][xx]=0)
	{
		if(map[k-1][xx]!=3&&bomb1[k-1][xx]!=2&&map[k-1][xx]!=6)
		{
		textout(handle,xx,k-1,wColors,1,"o");
		bomb1[k-1][xx]=1;
		Sleep(mbsp);
		textout(handle,xx,k-1,wColors,1," ");
		}
		else if(map[k-1][xx]==6)
		{
			textout(handle,xx,k-1,wColors,1,"*");
			map[k-1][xx]=0;
			Sleep(20);
			textout(handle,xx,k-1,wColors,1," ");
			break;
		}
		else
			{textout(handle,xx,k-1,wColors,1,"*");
		map[k-1][xx]=4;
		break;
		}
	}
	return TRUE;
}
DWORD WINAPI SHOOTdown(LPVOID lpParam)
{
	int dist=40;
	WORD wColors[1];
	wColors[0]=FOREGROUND_RED|FOREGROUND_INTENSITY;
	for(int xx=dx,k=dy;k<dist+dy&&map[k+1][xx]!=1;k++,bomb1[k][xx]=0)
	{
		if(map[k+1][xx]!=3&&bomb1[k+1][xx]!=2&&map[k+1][xx]!=6)
		{
		textout(handle,xx,k+1,wColors,1,"o");
		bomb1[k+1][xx]=1;
		Sleep(mbsp);
		textout(handle,xx,k+1,wColors,1," ");
		}
		else if(map[k+1][xx]==6)
		{
			textout(handle,xx,k+1,wColors,1,"*");
			map[k+1][xx]=0;
			Sleep(20);
			textout(handle,xx,k+1,wColors,1," ");
			break;
		}
		else 
		{
			textout(handle,xx,k+1,wColors,1,"*");
				map[k+1][xx]=4;
				break;
		}
		}
	return TRUE;

}
DWORD WINAPI TMright(LPVOID lpParam)
{
	WORD wColors[1];
	wColors[0]=FOREGROUND_GREEN|FOREGROUND_BLUE|FOREGROUND_INTENSITY;
        int dist=80;
	for(int k=tx,yy=ty;k<dist+tx&&map[yy][k+1]!=1;k++,bomb1[yy][k]=0)
	{
		if(map[yy][k+1]!=2&&bomb1[yy][k+1]!=1&&map[yy][k+1]!=6)
		{
			bomb1[yy][k+1]=2;
		textout(handle,k+1,yy,wColors,1,"o");
		Sleep(bsp);
		textout(handle,k+1,yy,wColors,1," ");
		}
		else if(map[yy][k+1]==2)
			{textout(handle,k+1,yy,wColors,1,"*");
		map[yy][k+1]=5;
		break;
		}
		else if(map[yy][k+1]==6)
		{textout(handle,k+1,yy,wColors,1,"*");
		map[yy][k+1]=0;
		Sleep(20);
		textout(handle,k+1,yy,wColors,1," ");
		break; 
		}
	}
		return TRUE;
}
DWORD WINAPI TMleft(LPVOID lpParam)
{
	WORD wColors[1];
	wColors[0]=FOREGROUND_GREEN|FOREGROUND_BLUE|FOREGROUND_INTENSITY;
    int dist=80;
	for(int k=tx,yy=ty;k>tx-dist&&map[yy][k-1]!=1;k--,bomb1[yy][k]=0)
	{
		if(map[yy][k-1]!=2&&bomb1[yy][k-1]!=1&&map[yy][k-1]!=6)
		{
			bomb1[yy][k-1]=2;
		textout(handle,k-1,yy,wColors,1,"o");
		Sleep(bsp);
		textout(handle,k-1,yy,wColors,1," ");
		}
		else if(map[yy][k-1]==2)
			{textout(handle,k-1,yy,wColors,1,"*");
		map[yy][k-1]=5;
		break;
		}
		else if(map[yy][k-1]==6)
		{textout(handle,k-1,yy,wColors,1,"*");
		map[yy][k-1]=0;
		Sleep(20);
		textout(handle,k-1,yy,wColors,1," ");
		break; 
		}
	}
		return TRUE;
}
DWORD WINAPI TMup(LPVOID lpParam)
{
	WORD wColors[1];
	wColors[0]=FOREGROUND_GREEN|FOREGROUND_BLUE|FOREGROUND_INTENSITY;
        int dist=80;
	for(int k=tx,yy=ty;k>yy-dist&&map[yy-1][k]!=1;yy--,bomb1[yy][k]=0)
	{
		if(map[yy-1][k]!=2&&bomb1[yy-1][k]!=1&&map[yy-1][k]!=6)
		{
			bomb1[yy-1][k]=2;
		textout(handle,k,yy-1,wColors,1,"o");
		Sleep(bsp);
		textout(handle,k,yy-1,wColors,1," ");
		}
		else if(map[yy-1][k]==2)
			{textout(handle,k,yy-1,wColors,1,"*");
		map[yy-1][k]=5;
		break;
		}
		else if(map[yy-1][k]==6)
		{
			textout(handle,k,yy-1,wColors,1,"*");
			map[yy-1][k]=0;
			Sleep(20);
			textout(handle,k,yy-1,wColors,1," ");
			break;
		}
	}
		return TRUE;
}
DWORD WINAPI TMdown(LPVOID lpParam)
{
	WORD wColors[1];
	wColors[0]=FOREGROUND_GREEN|FOREGROUND_BLUE|FOREGROUND_INTENSITY;
        int dist=80;
	for(int k=tx,yy=ty;k<yy+dist&&map[yy+1][k]!=1;yy++,bomb1[yy][k]=0)
	{
		if(map[yy+1][k]!=2&&bomb1[yy+1][k]!=1&&map[yy+1][k]!=6)
		{
			bomb1[yy+1][k]=2;
		textout(handle,k,yy+1,wColors,1,"o");
		Sleep(bsp);
		textout(handle,k,yy+1,wColors,1," ");
		}
		else if(map[yy+1][k]==2)
			{textout(handle,k,yy+1,wColors,1,"*");
		map[yy+1][k]=5;
		break;
		}
		else if(map[yy+1][k]==6)
		{
			textout(handle,k,yy+1,wColors,1,"*");
			map[yy+1][k]=0;
			Sleep(20);
			textout(handle,k,yy+1,wColors,1," ");
			break;
		}
	}
		return TRUE;
}
DWORD WINAPI T2S(LPVOID lpParam)
{
	
	DWORD dwThreadId;
	while(map[ty][tx]!=4&&map[y][x]!=5)
	{	
		Sleep(bombgap);
		int k=tx,yy=ty;
		int nx=x,ny=y;
		if(num2<=3)
		{
		if(nx>k&&ny==yy&&lastmove2==3)
		{
	    CreateThread(NULL,0,TMright,NULL,0,&dwThreadId);
		}
		else if(nx<k&&ny==yy&&lastmove2==2)
		{
			CreateThread(NULL,0,TMleft,NULL,0,&dwThreadId);
		}
		else if(nx==k&&ny<yy&&lastmove2==0)
		{
			CreateThread(NULL,0,TMup,NULL,0,&dwThreadId);
		}
		else if(nx==k&&ny>yy&&lastmove2==1)
		{
			CreateThread(NULL,0,TMdown,NULL,0,&dwThreadId);
		}
		}
	}
	return TRUE;
}
DWORD WINAPI TANK2(LPVOID lpParam)
{
	srand((unsigned)time(NULL));
	DWORD dwThreadId;
	WORD wColors[2];
	wColors[0]=FOREGROUND_RED|FOREGROUND_GREEN;
	wColors[1]=FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE|FOREGROUND_INTENSITY;
	while(1)
	{
		int nx=x,ny=y;
		int k=tx,yy=ty;
		textout(handle,tx,ty,wColors,1,"A");
		CreateThread(NULL,0,T2S,NULL,0,&dwThreadId);
	while(map[ty][tx]!=4)
	{	
		if(x==tx&&y<ty&&map[y][x]!=5)
			{	
				
				if(map[ty-1][tx]!=1&&map[ty-1][tx]!=2&&map[ty-1][tx]!=6)
					{
						textout(handle,tx,ty,wColors,1," ");
						map[ty][tx]=0;
				textout(handle,tx,ty-1,wColors,1,"A");
				map[ty-1][tx]=3;
				ty=ty-1;
				lastmove2=0;
				Sleep(tsp);
			        }
				else
				{
					switch(rand()%2)
					{
						case 0:
					{
						if(map[ty][tx-1]!=1&&map[ty][tx-1]!=2&&map[ty][tx-1]!=6)
						{
						textout(handle,tx,ty,wColors,1," ");
				        map[ty][tx]=0;
			            textout(handle,tx-1,ty,wColors,1,"<");
			            tx=tx-1;
			            map[ty][tx]=3;
			            lastmove2=2;
			            Sleep(tsp);
						}
						break;
					}
						case 1:
							{
								if(map[ty][tx+1]!=1&&map[ty][tx+1]!=2&&map[ty][tx+1]!=6)
								{
								textout(handle,tx,ty,wColors,1," ");
				                map[ty][tx]=0;
				                textout(handle,tx+1,ty,wColors,1,">");
				                tx=tx+1;
				                map[ty][tx]=3;
				                lastmove2=3;
				                Sleep(tsp);
								}
								break;
							}
					}
				}
			}
		else if(x==tx&&y>ty&&map[y][x]!=5)
			{
				if(map[ty+1][tx]!=1&&map[ty+1][tx]!=2&&map[ty+1][tx]!=6)
			{
				textout(handle,tx,ty,wColors,1," ");
				map[ty][tx]=0;
			textout(handle,tx,ty+1,wColors,1,"V");
			map[ty+1][tx]=3;
			ty=ty+1;
			lastmove2=1;
			Sleep(tsp);
			}
				else
				{
					switch(rand()%2)
					{
						case 0:
					{
						if(map[ty][tx-1]!=1&&map[ty][tx-1]!=2&&map[ty][tx-1]!=6)
						{
						textout(handle,tx,ty,wColors,1," ");
				        map[ty][tx]=0;
			            textout(handle,tx-1,ty,wColors,1,"<");
			            tx=tx-1;
			            map[ty][tx]=3;
			            lastmove2=2;
			            Sleep(tsp);
						}
						break;
					}
						case 1:
							{
								if(map[ty][tx+1]!=1&&map[ty][tx+1]!=2&&map[ty][tx+1]!=6)
								{
								textout(handle,tx,ty,wColors,1," ");
				                map[ty][tx]=0;
				                textout(handle,tx+1,ty,wColors,1,">");
				                tx=tx+1;
				                map[ty][tx]=3;
				                lastmove2=3;
				                Sleep(tsp);
								}
								break;
							}
					}
				}
			}
		else if(x<tx&&y==ty&&map[y][x]!=5)
			{
				if(map[ty][tx-1]!=1&&map[ty][tx-1]!=2&&map[ty][tx-1]!=6)
			{textout(handle,tx,ty,wColors,1," ");
				map[ty][tx]=0;
			textout(handle,tx-1,ty,wColors,1,"<");
			tx=tx-1;
			map[ty][tx]=3;
			lastmove2=2;
			Sleep(tsp);
				}
				else
				{
					switch(rand()%2)
					{
						case 0:
					{
						if(map[ty-1][tx]!=1&&map[ty-1][tx]!=2&&map[ty-1][tx]!=6)
						{
						textout(handle,tx,ty,wColors,1," ");
						map[ty][tx]=0;
				        textout(handle,tx,ty-1,wColors,1,"A");
				        map[ty-1][tx]=3;
				        ty=ty-1;
				        lastmove2=0;
				        Sleep(tsp);
						}
						break;
					}
						case 1:
							{
								if(map[ty+1][tx]!=1&&map[ty+1][tx]!=2&&map[ty+1][tx]!=6)
								{
								textout(handle,tx,ty,wColors,1," ");
				                map[ty][tx]=0;
			                    textout(handle,tx,ty+1,wColors,1,"V");
			                    map[ty+1][tx]=3;
			                    ty=ty+1;
			                    lastmove2=1;
			                    Sleep(tsp);
								}
								break;
							}
					}			
				}
			}
		else if(x>tx&&y==ty&&map[y][x]!=5)
			{
				if(map[ty][tx+1]!=1&&map[ty][tx+1]!=2&&map[ty][tx+1]!=6)
			{
				textout(handle,tx,ty,wColors,1," ");
				map[ty][tx]=0;
				textout(handle,tx+1,ty,wColors,1,">");
				tx=tx+1;
				map[ty][tx]=3;
				lastmove2=3;
				Sleep(tsp);
				}
				else
				{
					switch(rand()%2)
					{
						case 0:
					{
						if(map[ty-1][tx]!=1&&map[ty-1][tx]!=2&&map[ty-1][tx]!=6)
						{
						textout(handle,tx,ty,wColors,1," ");
						map[ty][tx]=0;
				        textout(handle,tx,ty-1,wColors,1,"A");
				        map[ty-1][tx]=3;
				        ty=ty-1;
				        lastmove2=0;
				        Sleep(tsp);
						}
						break;
					}
						case 1:
							{
								if(map[ty+1][tx]!=1&&map[ty+1][tx]!=2&&map[ty+1][tx]!=6)
								{
								textout(handle,tx,ty,wColors,1," ");
				                map[ty][tx]=0;
			                    textout(handle,tx,ty+1,wColors,1,"V");
			                    map[ty+1][tx]=3;
			                    ty=ty+1;
			                    lastmove2=1;
			                    Sleep(tsp);
								}
								break;
							}
					}		
				}
			}
		else
		{
			switch(rand()%4)
			{
			case 0:
				{
				    if(map[ty-1][tx]!=1&&map[ty-1][tx]!=2&&map[ty-1][tx]!=6)
					{
						textout(handle,tx,ty,wColors,1," ");
						map[ty][tx]=0;
				textout(handle,tx,ty-1,wColors,1,"A");
				map[ty-1][tx]=3;
				ty=ty-1;
				lastmove2=0;
				Sleep(tsp);
			        }
				else
				{
					textout(handle,tx,ty,wColors,1,"A");
					lastmove2=0;
				}
					break;
				}
			case 1:
				{
				if(map[ty+1][tx]!=1&&map[ty+1][tx]!=2&&map[ty+1][tx]!=6)
			{
				textout(handle,tx,ty,wColors,1," ");
				map[ty][tx]=0;
			textout(handle,tx,ty+1,wColors,1,"V");
			map[ty+1][tx]=3;
			ty=ty+1;
			lastmove2=1;
			Sleep(tsp);
			}
				else
				{
					textout(handle,tx,ty,wColors,1,"V");
					lastmove2=1;
				}
				break;
				}
			case 2:
				{
					if(map[ty][tx-1]!=1&&map[ty][tx-1]!=2&&map[ty][tx-1]!=6)
			{textout(handle,tx,ty,wColors,1," ");
				map[ty][tx]=0;
			textout(handle,tx-1,ty,wColors,1,"<");
			tx=tx-1;
			map[ty][tx]=3;
			lastmove2=2;
			Sleep(tsp);
				}
				else
				{
					textout(handle,tx,ty,wColors,1,"<");
					lastmove2=2;				
				}
					break;
				}
			case 3:
				{
					if(map[ty][tx+1]!=1&&map[ty][tx+1]!=2&&map[ty][tx+1]!=6)
			{
				textout(handle,tx,ty,wColors,1," ");
				map[ty][tx]=0;
				textout(handle,tx+1,ty,wColors,1,">");
				tx=tx+1;
				map[ty][tx]=3;
				lastmove2=3;
				Sleep(tsp);
				}
				else
				{
					textout(handle,tx,ty,wColors,1,">");
					lastmove2=3;
				}
					break;
				}
				}
			}
		}
		
		my++;
		if(my%5==0&&level<5)
			{
				bombgap=bombgap-5;
				bsp=bsp-5;
				tsp=tsp-170;
				level++;
			}
	textout(handle,20,MX+2,wColors,1,"you kill your enemy! 3 sec to refresh...");
	textout(handle,72,6,wColors,1,itoa(my,mykill,10));
			Sleep(3000);
	textout(handle,72,12,wColors,1,itoa(level,level1,10));
	textout(handle,20,MX+2,wColors,1,"                                        ");
			map[ty][tx]=0;
			
	textout(handle,tx,ty,wColors,1," ");
	for(int i=0,m,n;i<10;i++)
	{
		m=1+rand()%68;
		n=1+rand()%18;
		if(map[n][m]!=1&&map[n][m]!=6&&map[n][m]!=2)
		{
			tx=m;ty=n;
			break;
		}
	}
	}
	
	return TRUE;
}
void move()
{
	handle=initiate();
	DWORD dwThreadId;
	WORD wColors[3];
	wColors[0]=FOREGROUND_RED|FOREGROUND_INTENSITY;
	wColors[1]=FOREGROUND_GREEN;
	wColors[2]=FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE;	
	
	while(1)
	{
		textout(handle,x,y,wColors,1,"A");
		map[y][x]=2;
		int lastmove=KEY_UP;
		CreateThread(NULL,0,T2S,NULL,0,&dwThreadId);
	while(map[y][x]!=5)
	{
		Sleep(30);
		if(_kbhit())
		{
			int ch =_getch();
		if(ch==KEY_UP)
			{
				if(map[y-1][x]!=1&&map[y-1][x]!=3&&map[y-1][x]!=6)
					{
						textout(handle,x,y,wColors,1," ");
				map[y][x]=0;
				textout(handle,x,y-1,wColors,1,"A");
				y=y-1;
				map[y][x]=2;
				lastmove=KEY_UP;
			}
				else
				{
					textout(handle,x,y,wColors,1,"A");
				lastmove=KEY_UP;
				}
		}
		else if(ch==KEY_DOWN)
		{
			if(map[y+1][x]!=1&&map[y+1][x]!=3&&map[y+1][x]!=6)
			{textout(handle,x,y,wColors,1," ");
			map[y][x]=0;
			textout(handle,x,y+1,wColors,1,"V");
			y=y+1;
			map[y][x]=2;
			lastmove=KEY_DOWN;
			}
			else
				{
					textout(handle,x,y,wColors,1,"V");
					lastmove=KEY_DOWN;
			}
		}
		else if(ch==KEY_LEFT)
		{
			if(map[y][x-1]!=1&&map[y][x-1]!=3&&map[y][x-1]!=6)
			{textout(handle,x,y,wColors,1," ");
			map[y][x]=0;
			textout(handle,x-1,y,wColors,1,"<");
			x=x-1;
			map[y][x]=2;
			lastmove=KEY_LEFT;
			}
			else
				{
					textout(handle,x,y,wColors,1,"<");
			lastmove=KEY_LEFT;
			}
		}
		else if(ch==KEY_RIGHT)
		{
			if(map[y][x+1]!=1&&map[y][x+1]!=3&&map[y][x+1]!=6)
			{
				textout(handle,x,y,wColors,1," ");
				map[y][x]=0;
				textout(handle,x+1,y,wColors,1,">");
				x=x+1;
				map[y][x]=2;
				lastmove=KEY_RIGHT;
			}
			else
				{ 
					textout(handle,x,y,wColors,1,">");
					lastmove=KEY_RIGHT;
			}
		}	
		else if(ch==KEY_SPACE)
			{
				if(lastmove==KEY_RIGHT)
				{CreateThread(NULL,0,SHOOTright,NULL,0,&dwThreadId);}
				if(lastmove==KEY_LEFT)
				{CreateThread(NULL,0,SHOOTleft,NULL,0,&dwThreadId);}
				if(lastmove==KEY_UP)
				{CreateThread(NULL,0,SHOOTup,NULL,0,&dwThreadId);}
				if(lastmove==KEY_DOWN)
				{CreateThread(NULL,0,SHOOTdown,NULL,0,&dwThreadId);}
		}
		}
	}
	enemy++;
	textout(handle,20,MX+3,wColors,1,"You are DEAD! 3 sec to refresh...");
	textout(handle,72,9,wColors,1,itoa(enemy,enemykill,10));
			Sleep(3000);
	textout(handle,20,MX+3,wColors,1,"                                 ");
			map[y][x]=0;
	textout(handle,x,y,wColors,1," ");
			x=10,y=10;
	}
}
void drawtitle()
{
	handle=initiate();
	WORD wColors[1];
	wColors[0]=FOREGROUND_RED|FOREGROUND_INTENSITY;
	textout(handle,3,3,wColors,1,"  ■  ■■■■      ■              ■          ■    ■■");
	textout(handle,3,4,wColors,1,"  ■  ■    ■  ■■■■■          ■          ■    ■  ■");
	textout(handle,3,5,wColors,1,"  ■  ■    ■      ■        ■■■■■■■    ■■  ■");
	textout(handle,3,6,wColors,1,"■■■■■■■  ■■■■■          ■          ■  ■■■");
	textout(handle,3,7,wColors,1,"  ■  ■    ■  ■      ■          ■          ■    ■  ■");
	textout(handle,3,8,wColors,1,"  ■  ■■■■  ■■■■■        ■  ■      ■■■  ■■");
	textout(handle,3,9,wColors,1,"  ■■            ■  ■  ■    ■      ■    ■  ■  ■■  ■");
	textout(handle,3,10,wColors,1,"■    ■■■■  ■■  ■■■  ■          ■  ■■■■    ■■");
    Sleep(5000);
	for(int i=3;i<11;i++)
	{
		textout(handle,3,i,wColors,1,"                                                                 ");
		Sleep(40);
	}
}
void showinf()
{
	handle=initiate();
	WORD wColors[1];
	wColors[0]=FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE|FOREGROUND_INTENSITY;
	textout(handle,6,6,wColors,1,"游戏说明：");
	textout(handle,6,7,wColors,1,"游戏中，玩家通过↑↓←→以及SPACE键控制红色坦克击毁黄色坦克");
	textout(handle,6,10,wColors,1,"2011级信息与通信工程学院6班");
	Sleep(4000);
	textout(handle,6,12,wColors,1,"请按SPACE键开始游戏");
}
void main()
{
	DWORD dwThreadId;
	//PlaySound("an eerie enemy.wav",NULL,SND_FILENAME|SND_ASYNC); 
	drawtitle();
	showinf();
	
	while(1)
	{
	if(_kbhit())
	{
		//PlaySound(NULL,NULL,SND_FILENAME|SND_ASYNC); 
		//PlaySound("confrontation.wav",NULL,SND_FILENAME|SND_ASYNC|SND_LOOP );
		int go=_getch();
		if(go==KEY_SPACE)
		{
	record();
	drawmap2();
	CreateThread(NULL,0,TANK2,NULL,0,&dwThreadId);
	move();
		}
	}
	}
}
