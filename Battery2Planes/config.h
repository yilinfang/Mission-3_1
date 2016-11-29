#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED
#include<stdio.h>
#include <ctype.h>
typedef enum
{
    HPT=0,LPT=1,RPT=2
}PLANETYPE;

typedef void (*timerType)(int);    //时钟回调函数指针类型
class TimerNode
{
public:

    TimerNode()
    {
        id = -1;
        millis = 0;
        nplane = 0;
        vel = 0;
        x=y=0.0f;

    }
public:
     PLANETYPE pt;
     int id;//时钟编号,不能重复
     unsigned int millis;//时钟间隔(毫秒)
     int nplane; //在millis的时钟周期内生成的飞机数量
     float x,y;  //位置
     float vel;    //速度
};

char* getName(char*p,char name[]) //get next name
{
    while(*p!='\n'&&!isalpha(*p))
        p++;
    while(*p!='\n'&&isalpha(*p))
        *name++=*p++;
    *name='\0';
    return p;
}

char* getPos(char*p,float*px, float*py)
{
    char str[50];
    int i=0;
    while(*p!='\n'&&*p!='(')
          p++;
    if(*p=='(')
        p++;
    while(*p!='\n'&&*p!=')')
    {
        str[i]=*p;
        p++;
        i++;
    }
    str[i]='\0';
    //printf("str=%s\n",str);
    sscanf(str,"%f,%f",px,py);
    return p;
}
char* getPlane(char*p,int*pm,int*pn, float*pv)
{
    char str[50];
    int i=0;
    while(*p!='\n'&&*p!='(')
          p++;
    if(*p=='(')
        p++;
    while(*p!='\n'&&*p!=')')
    {
        str[i]=*p;
        p++;
        i++;
    }
    str[i]='\0';
    sscanf(str,"%d,%d,%f",pm,pn,pv);
    return p;
}
int ConfigFileParser(char* filename,TimerNode* timerList,Pos2D*airport,int*nap,int maxsize) //读取参数信息,返回数量
{
    FILE   *fp;
    char buffer[256];
	if ((fp = fopen(filename, "rt")) == NULL)
	{
		fprintf(stderr, "Error opening file.\n\n");
		return 0;
	}

	int ntm=0; //#timer
	int na=0;  //#airport
	while(fgets(buffer,256,fp)!= NULL)//read a line , the last char is '\n'
    {
       if(ntm>maxsize)
            break;
       char*p = buffer;
       if(*p=='/')
        continue;

       while(*p!='\n') //convert all alphabet into upper
       {
           *p = toupper(*p);
           p++;
       }
        p=buffer;
       if(*p!='S')
        continue;
       //else
       // printf("%3d:%s",strlen(buffer),buffer);
       p++;
       char name[256];
       float x=0.0f,y=0.0f;

       p=getName(p,name);
       if(strcmp(name,"P")!=0)  //必须有P参数
            continue;


       p=getPos(p,&x,&y);
       //printf("\nP:x=%.2f,y=%.2f\n",x,y);

       airport[na].x = x;
       airport[na].y = y;
       na++;
       for( ;*p!='\n';p++)
       {
           //printf("%c",*p);

           int m=0,n=0;
           float v=0.0f;
           PLANETYPE pt;

           p=getName(p,name);
           if(strcmp(name,"HP")==0)
           {
               p=getPlane(p,&m,&n,&v);
               pt = HPT;
               //printf("HP:m=%d,n=%d,v=%.2f",m,n,v);

           }
           else if(strcmp(name,"LP")==0)
           {
              p=getPlane(p,&m,&n,&v);
              pt = LPT;
              //printf("LP:m=%d,n=%d,v=%.2f",m,n,v);
           }
           else if(strcmp(name,"RP")==0)
           {
               p=getPlane(p,&m,&n,&v);
               pt = RPT;
               //printf("RP:m=%d,n=%d,v=%.2f",m,n,v);
           }
           else
            break;

           timerList[ntm].millis = m;
           timerList[ntm].nplane = n;
           timerList[ntm].pt = pt;
           timerList[ntm].x = x;
           timerList[ntm].y = y;
           timerList[ntm].vel = v;
           ntm++;

        }
    }

    fclose(fp);
    *nap = na;
	return ntm;
}

//int LoadConfig(char*file)
//{
//    char buffer[256];
//    ifstream configfile;
//    configfile.open(file);
//
//    if (!configfile.is_open())
//    {
//
//        cout << "Error opening file";
//        exit(1);
//    }
//
//    int st,ed;
//    string s;
//    while (!configfile.eof())
//    {
//        configfile.getline(buffer,256);
//        s=buffer;
//        cout<<s<< endl;
//    }
//    ErrOut:
//        cout << "语句格式错误，重新输入！"<<endl;
//        exit(1);
//
//    return 0;
//}

#endif // CONFIG_H_INCLUDED
