#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <termio.h>
#define ROW 1
#define COLUMN 0
short a[16]={0};
int getch(void){
	struct termios tm, tm_old;
	int fd = 0, ch;		   
	if (tcgetattr(fd, &tm) < 0) {//保存现在的终端设置
		return -1;
	}		        
	tm_old = tm;
	cfmakeraw(&tm);//更改终端设置为原始模式，该模式下所有的输入数据以字节为单位被处理
	if (tcsetattr(fd, TCSANOW, &tm) < 0) {//设置上更改之后的设置
		return -1;
	}  
        ch = getchar();
        if (tcsetattr(fd, TCSANOW, &tm_old) < 0) {//更改设置为最初的样子
        	return -1;
	}
	return ch;
}
void show_board(short *a){
	printf("/------+------+------+------\\ \n");
	for(int i=0;i<16;++i){
		if(i%4==0){
			printf("|");
		}
		if(a[i]==0){
			printf("      |");
		}else{
			printf("%6d|",a[i]);
		}
		if(i==3||i==7||i==11){
			printf("\n|------+------+------+------|\n");
		}
	}
	printf("\n\\------+------+------+------/\n");
}
void generate_number(short *a,int num){
	int value[10]={2,2,2,2,2,2,2,2,2,4};
	int t=rand()%10;
	int index;
	int i;
	for(i=0;i<16;++i){
		if(a[i]==0){
			break;
		}
	}
	if(i==16){
		return;
	}
	for(int i=0;i<num;){
		index=rand()%16;
		if(a[index]==0){
			a[index]=value[t];
			++i;
		}
	}
}
void move_combine(int start,int end,int direction,short *a){
	if(direction==ROW){
		if(start<end){
			for(int i=0;i<start+end+1;++i){
				for(int j=end,k=end-1;j>start&&k>=start;){
					if(a[j+i*4]!=0&&a[j+i*4]!=a[k+i*4]&&a[k+i*4]!=0){
						j--;
						if(j==k){
							k--;
						}
					}else if(a[k+i*4]==0){
						k--;
					}else if(a[j+i*4]==0&&a[k+i*4]!=0){
						a[j+i*4]=a[k+i*4];
						a[k+i*4]=0;
						k--;
					}else if(a[j+i*4]!=0&&a[j+4*i]==a[k+4*i]){
						a[j+4*i]*=2;
						a[k+4*i]=0;
						j--;
						k--;
						if(k>=start&&a[j+1+4*i]==2*a[k+4*i]){
							if(k>start&&a[k-1+4*i]==a[k+4*i]){
								continue;
							}
							int tmp=a[j+1+4*i];
							a[j+1+4*i]=a[k+4*i];
							a[k+4*i]=tmp;
						}
					}
				}
			}
		}else if (start >end){
			for(int i=0;i<start+end+1;++i){
				for(int j=end,k=end+1;j<start&&k<=start;){
					if(a[j+i*4]!=0&&a[j+i*4]!=a[k+i*4]&&a[k+i*4]!=0){
						j++;
						if(j==k){
							k++;
						}
					}else if(a[k+i*4]==0){
						k++;
					}else if(a[j+i*4]==0&&a[k+i*4]!=0){
						a[j+i*4]=a[k+i*4];
						a[k+i*4]=0;
						k++;
					}else if(a[j+i*4]!=0&&a[j+4*i]==a[k+4*i]){
						a[j+4*i]*=2;
						a[k+4*i]=0;
						j++;
						k++;
						if(k<=start&&a[j-1+4*i]==2*a[k+4*i]){
							if(k<start&&a[k+1+4*i]==a[k+4*i]){
								continue;
							}
							int tmp=a[j-1+4*i];
							a[j-1+4*i]=a[k+4*i];
							a[k+4*i]=tmp;
						}
					}
				}
			}
		}
	}else if(direction==COLUMN){
		if(start<end){
			for(int j=0;j<start+end+1;++j){
				for(int i=end,k=end-1;i>start&&k>=start;){
					if(a[j+i*4]!=0&&a[j+k*4]!=0&&a[j+4*i]!=a[j+4*k]){
						i--;
						if(k==i){
							k--;
						}
					}else if(a[j+k*4]==0){
						k--;
					}else if(a[i*4+j]==0&&a[k*4+j]!=0){
						a[i*4+j]=a[k*4+j];
						a[k*4+j]=0;
						k--;
					}else if(a[i*4+j]!=0&&a[i*4+j]==a[k*4+j]){
						a[i*4+j]*=2;
						a[k*4+j]=0;
						k--;
						i--;
						if(k>=start&&a[j+4*(i+1)]==2*a[k*4+j]){
							if(k>start&&a[(k-1)*4+j]==a[k*4+j]){
								continue;
							}
							int tmp=a[j+4*(i+1)];
							a[j+4*(i+1)]=a[k*4+j];
							a[k*4+j]=tmp;
						}
					}
				}
			}
		}else if (start >end){
			for(int j=0;j<start+end+1;++j){
				for(int i=end,k=end+1;i<start&&k<=start;){
					if(a[j+i*4]!=0&&a[j+k*4]!=0&&a[j+4*i]!=a[j+4*k]){
						i++;
						if(k==i){
							k++;
						}
					}else if(a[j+k*4]==0){
						k++;
					}else if(a[i*4+j]==0&&a[k*4+j]!=0){
						a[i*4+j]=a[k*4+j];
						a[k*4+j]=0;
						k++;
					}else if(a[i*4+j]!=0&&a[i*4+j]==a[k*4+j]){
						a[i*4+j]*=2;
						a[k*4+j]=0;
						k++;
						i++;
						if(k<=start&&a[j+4*(i-1)]==2*a[k*4+j]){
							if(k<start&&a[(k+1)*4+j]==a[k*4+j]){
								continue;
							}
							int tmp=a[j+4*(i-1)];
							a[j+4*(i-1)]=a[k*4+j];
							a[k*4+j]=tmp;
						}
					}
				}
			}	
		}
	}
}
void move(char command,short *a){
	if(command=='w'||command=='W'){
		move_combine(3,0,COLUMN,a);
	}else if (command=='s'||command=='S'){
		move_combine(0,3,COLUMN,a);
	}else if (command=='a'||command=='A'){
		move_combine(3,0,ROW,a);
	}else if (command=='d'||command=='D'){
		move_combine(0,3,ROW,a);
	}	
}
int check_status(short *a){
	for(int i=0;i<16;++i){
		if(a[i]==2048){
			return 2;
		}
		if(a[i]==0){
			return 1;
		}
	}
	int i=0,j;
	for(;i<4;++i){
		for(j=0;j<4;++j){
			if((j+1<4 && a[i*4+j+1] ==a[i*4+j]) || (i+1<4 &&a[(i+1)*4+j] == a[i*4+j])){
					return 1;
			}
		}
	}
	return 0;
}
int main(){
	srand(time(NULL));
	generate_number(a,2);
	show_board(a);
	char c=0;
	while(1){
		c=getch();
		if(c!='a'&&c!='A'&&c!='w'&&c!='W'&&c!='s'&&c!='S'&&c!='d'&&c!='D'){
			continue;
		}
		move(c,a);
		generate_number(a,1);
		system("clear");
		show_board(a);
		int result=check_status(a);
		if(!result){
			printf("Lost!");
			return 0;
		}else if(result==2){
			printf("Win!");
			return 0;
		}
	}
}
