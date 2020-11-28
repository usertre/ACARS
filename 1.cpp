/*
written by wps
2020/10/10 22:50
add fopen 
*/
#include<cstdio>
#include<iostream>
#include<cstring>
#include<cstdlib>
#include<string>
#include<windows.h>
using namespace std;
char acars[50][200];//原始数据 
bool bo=false,pprbo=false;//bo判断'-' pprbo判断是否有PRESENT POSITION REOPRT 
char temp[10];
void find(char x[]);
void lable(char x[]);

void before(char x[],int y);///line1~5    y==line

void out(char x[],int y);
void off(char x[],int y);//y是标识符后面的、有实际意义的位置 
void onn(char x[],int y);
void inn(char x[],int y);
void wxr(char x[],int y);//气象请求报 lable 12
void pos(char x[],int y);
void eta(char x[],int y);
void cld(char x[],int y);
void ppr(char x[],int y);//PRESENT POSITION REPORT
void rte(char x[],int y);
void fde(char x[],int y);
void msg(char x[],int y);
void eor(char x[],int y);
void ma(char x[],int y);//媒体报告(Media Advisory)
void atis(char x[],int y);//ATIS请求
void u5(char x[],int y);//气象请求报 lable 5u 
void ncmm();
void db(char x[],int y);
	FILE * fp1;
	FILE * fp2;
int main()
{

	char rname[50]={"001.in"},wname[50]={"001.out"};
	fp1=fopen(rname,"r");
	fp2=fopen(wname,"w");
	if(fp1==NULL)
	{
		printf("Failed to open 001.in");
		getchar();
		exit(1);
	}
	if(fp2==NULL)
	{
		printf("Failed to open 001.out");
		getchar();
		exit(1);
	}
	int line=0;
	while(fgets(acars[line++],150,fp1)!=NULL);
	acars[line-2][strlen(acars[line-2])-2]='\0';////in the cmd,need line-2
	for(int i=0;i<5;++i) before(acars[i],i);
	for(int i=5;i<line;++i)
	{
		if(!pprbo) find(acars[i]);
		else {ppr(acars[i],0);i++;pprbo=false;}
	}
	fclose(fp1);
	fclose(fp2);
	return 0;
}
void find(char x[])
{
	for(int i=0;i<strlen(x);++i)
	{
		if(x[i]=='-'||bo==true) {lable(x);bo=true;return;}//for example  -  RTE
		else break;/////////
	}
	return;
}
void lable(char x[])
{
	memset(temp,'\0',sizeof(temp));//清空temp数组用于保存报文标识符 
	int t=0,i;
	if(bo) i=0;else i=3;//如果本行有'-'，读取位置后移 
	for(i;i<strlen(x);++i)//将读取到的报文标识符存入temp中
	{
		if(x[i]>='A'&&x[i]<='Z') temp[t++]=x[i];
		else break;
	}
	///////////// 判断报文标识符 
	if(temp[0]=='O'&&temp[1]=='U'&&temp[2]=='T') out(x,i);//滑出报告 
	else if(temp[0]=='O'&&temp[1]=='F'&&temp[2]=='F') off(x,i); //起飞报告 
	else if(temp[0]=='O'&&temp[1]=='N'&&temp[2]=='N') onn(x,i); //落地报告 
	else if(temp[0]=='I'&&temp[1]=='N'&&temp[2]=='N') inn(x,i); //滑入报告 
	else if(temp[0]=='W'&&temp[1]=='X'&&temp[2]=='R') wxr(x,i); //气象请求报 
	else if(temp[0]=='P'&&temp[1]=='O'&&temp[2]=='S') pos(x,i); //位置报1 
	else if(temp[0]=='E'&&temp[1]=='T'&&temp[2]=='A') eta(x,i); //预计到达时间报告 
	else if(temp[0]=='C'&&temp[1]=='L'&&temp[2]=='D') cld(x,i); //起飞前放行 
	else if(temp[0]=='P'&&temp[1]=='R'&&temp[2]=='E'&&temp[3]=='S'&&temp[4]=='E'&&temp[5]=='N'&&temp[6]=='T') pprbo=true;//位置报告标记 
	else if(temp[0]=='R'&&temp[1]=='T'&&temp[2]=='E') rte(x,i);//B777  RTE故障报 
	else if(temp[0]=='N'&&temp[1]=='C'&&temp[2]=='M'&&temp[3]=='M') ncmm();//无驾驶舱效应报告 
	else if(temp[0]=='F'&&temp[1]=='D'&&temp[2]=='E') fde(x,i);//驾驶舱效应报告 
	else if(temp[0]=='M'&&temp[1]=='S'&&temp[2]=='G') msg(x,i);//维护信息行 
	else if(temp[0]=='D'&&temp[1]=='B') db(x,i);//探测源信息 
	else if(temp[0]=='E'&&temp[1]=='O'&&temp[2]=='R') return;//故障报告结束 
	else if(x[i]=='0'&&(x[i+1]=='E'||x[i+1]=='L')) ma(x,i+1);//媒体报告 
	else if(x[i]=='/'&&x[i+5]=='.') atis(x,i+1);//ATIS请求 
	else if(x[i]=='-'&&((x[i+1]=='F'&&x[i+2]=='S')||(x[i+1]=='S'&&x[i+2]=='A'))) u5(x,i-4);//气象请求报Lable 5U 
	
	return;
}

//////////////////////////
//////////////////////////
void before(char x[],int y)//报文前5行处理 
{//显示优先级、目的地址、地面站地址、标准信息标示符、航班号、飞机注册号、DSP标识符、接收时间、报文序列号 
	int i=0;
	if(y==0)
	{
		fprintf(fp2,"优先级:QU\t目的地址:");
		while(x[i]!=' ') i++;
		for(i+1;i<strlen(x);++i) fprintf(fp2,"%c",x[i]);
		fprintf(fp2,"\n");
		return;
	}
	else if(y==1)
	{
		fprintf(fp2,"转发报文的地面站地址:");
		i=1;
		while(x[i]!=' ') fprintf(fp2,"%c",x[i++]);
		fprintf(fp2,"\t转发时间:");
		for(i+1;i<strlen(x);++i) fprintf(fp2,"%c",x[i]);
		fprintf(fp2,"\n");
		return;
	}
	else if(y==2)
	{
		fprintf(fp2,"标准信息标示符:");
		i=1;
		for(i;i<strlen(x);++i) fprintf(fp2,"%c",x[i]);
		fprintf(fp2,"\n");
		return;
	}
	else if(y==3)
	{
		i=3;
		fprintf(fp2,"航班号:");
		while(x[i]!='/') fprintf(fp2,"%c",x[i++]);
		i+=4;
		fprintf(fp2,"\t飞机注册号:");
		for(i;i<strlen(x);++i) fprintf(fp2,"%c",x[i]);
		fprintf(fp2,"\n");
		return;
	}
	else if(y==4)
	{
		i=3;
		fprintf(fp2,"DSP标示符:");
		while(x[i]!=' ') fprintf(fp2,"%c",x[i++]);
		fprintf(fp2,"\t接收下传信息的地面工作站地址:");
		i++;
		while(x[i]!=' ') fprintf(fp2,"%c",x[i++]);
		fprintf(fp2,"\t接收信息时间:");
		i++;
		while(x[i]!=' ') fprintf(fp2,"%c",x[i++]);
		i++;
		fprintf(fp2,"\t报文序列号:");
		for(i;i<strlen(x);++i) fprintf(fp2,"%c",x[i]);
		fprintf(fp2,"\n");
	}
	return;
}



//////////////////////////
//////////////////////////
void out(char x[],int y)//滑出报告
{//显示执行日期、起飞机场、目的机场、剩余时间、剩余油量  
	int i;
	fprintf(fp2,"滑出报告:\n\t执行日期:");
	for(i=y;i<y+6;++i) fprintf(fp2,"%c",x[i]);
	fprintf(fp2,"\n\t起飞机场:");
	for(i=y+7;i<y+11;++i) fprintf(fp2,"%c",x[i]);
	fprintf(fp2,"\n\t目的机场:");
	for(i=y+12;i<y+16;++i) fprintf(fp2,"%c",x[i]);
	fprintf(fp2,"\n\t剩余时间:");
	for(i=y+17;i<y+23;++i)
	{
		if(x[i]!=',') fprintf(fp2,"%c",x[i]);
		else break;
	}
	fprintf(fp2,"\n\t剩余油量:");
	i++;
	for(i;i<strlen(x);++i) fprintf(fp2,"%c",x[i]);
	fprintf(fp2,"\n");
	return;
}
void off(char x[],int y)//起飞报告 
{//显示执行日期、起飞机场、目的机场、剩余时间 
	int i;
	fprintf(fp2,"起飞报告:\n\t执行日期:");
	for(i=y;i<y+6;++i) fprintf(fp2,"%c",x[i]);
	fprintf(fp2,"\n\t起飞机场:");
	for(i=y+7;i<y+11;++i) fprintf(fp2,"%c",x[i]);
	fprintf(fp2,"\n\t目的机场:");
	for(i=y+12;i<y+16;++i) fprintf(fp2,"%c",x[i]);
	fprintf(fp2,"\n\t剩余时间:");
	for(i=y+17;i<y+23;++i)
	{
		if(x[i]!=',') fprintf(fp2,"%c",x[i]);
		else break;
	}
	fprintf(fp2,"\n");
	return;
}
void onn(char x[],int y)//落地报告 
{//显示执行日期、起飞机场、目的机场、剩余时间、剩余油量 
	int i;
	fprintf(fp2,"落地报告:\n\t执行日期:");
	for(i=y;i<y+6;++i) fprintf(fp2,"%c",x[i]);
	fprintf(fp2,"\n\t起飞机场:");
	for(i=y+7;i<y+11;++i) fprintf(fp2,"%c",x[i]);
	fprintf(fp2,"\n\t目的机场:");
	for(i=y+12;i<y+16;++i) fprintf(fp2,"%c",x[i]);
	fprintf(fp2,"\n\t剩余时间:");
	for(i=y+17;i<y+23;++i)
	{
		if(x[i]!=',') fprintf(fp2,"%c",x[i]);
		else break;
	}
	fprintf(fp2,"\n\t剩余油量:");
	i++;
	for(i;i<strlen(x);++i) fprintf(fp2,"%c",x[i]);
	fprintf(fp2,"\n");
	return;
}
void inn(char x[],int y)//滑入报告 
{//显示执行日期、起飞机场 、目的机场、剩余油量 
	int i;
	fprintf(fp2,"滑入报告:\n\t执行日期:");
	for(i=y;i<y+6;++i) fprintf(fp2,"%c",x[i]);
	fprintf(fp2,"\n\t起飞机场:");
	for(i=y+7;i<y+11;++i) fprintf(fp2,"%c",x[i]);
	fprintf(fp2,"\n\t目的机场:");
	for(i=y+12;i<y+16;++i) fprintf(fp2,"%c",x[i]);
	fprintf(fp2,"\n\t剩余油量");
	for(i=y+17;i<y+21;++i) fprintf(fp2,"%c",x[i]);
	fprintf(fp2,"\n");
	return; 
}
void wxr(char x[],int y)//气象请求报 
{//显示请求气象类型、请求机场1、请求机场2、请求机场3 
	int i;
	fprintf(fp2,"气象请求报:\n\t请求气象类型:");
	for(i=y+1;i<y+3;i++) fprintf(fp2,"%c",x[i]);
	fprintf(fp2,"\n\t请求机场1:");
	for(i=y+4;i<y+8;++i) fprintf(fp2,"%c",x[i]);
	fprintf(fp2,"\n\t请求机场2:");
	if(x[i+1]==',') {fprintf(fp2,"\n\t请求机场3:\n");return;}
	else for(i=y+9;i<y+13;++i) fprintf(fp2,"%c",x[i]);
	fprintf(fp2,"\n\t请求机场3:");
	if(x[i+2]==',') return;
	else for(i=y+14;i<y+18;++i) fprintf(fp2,"%c",x[i]);
	fprintf(fp2,"\n");
	return;
}
void pos(char x[],int y)//位置报1 
{//显示执行日期、维度、经度 
	int i;
	fprintf(fp2,"位置报1:\n\t执行日期:");
	for(i=y;i<y+6;++i) fprintf(fp2,"%c",x[i]);
	fprintf(fp2,"\n\t纬度:");
	for(i=y+7;i<y+15;++i) fprintf(fp2,"%c",x[i]);
	fprintf(fp2,"\n\t经度:");
	for(i=y+16;i<y+24;++i) fprintf(fp2,"%c",x[i]);
	fprintf(fp2,"\n");
	return;
}
void eta(char x[],int y)//预计到达时间报告 
{//显示执行日期、起飞机场、目的机场、预计到达时间 
	int i;
	fprintf(fp2,"预计到达时间报告:\n\t执行日期:");
	for(i=y;i<y+6;++i) fprintf(fp2,"%c",x[i]);
	fprintf(fp2,"\n\t起飞机场:");
	for(i=y+7;i<y+11;++i) fprintf(fp2,"%c",x[i]);
	fprintf(fp2,"\n\t目的机场:");
	for(i=y+12;i<y+16;++i) fprintf(fp2,"%c",x[i]);
	fprintf(fp2,"\n\t预计到达时间:");
	for(i=y+17;i<strlen(x);++i) fprintf(fp2,"%c",x[i]);
	fprintf(fp2,"\n");
	return;
}
void cld(char x[],int y)//起飞前放行报告 
{//显示执行日期、起飞机场、目的机场、预计到达时间 
	int i;
	fprintf(fp2,"起飞前放行报告:\n\t执行日期:");
	for(i=y;i<y+6;++i) fprintf(fp2,"%c",x[i]);
	fprintf(fp2,"\n\t起飞机场:");
	for(i=y+7;i<y+11;++i) fprintf(fp2,"%c",x[i]);
	fprintf(fp2,"\n\t目的机场:");
	for(i=y+12;i<y+16;++i) fprintf(fp2,"%c",x[i]);
	fprintf(fp2,"\n\t预计到达时间:");
	for(i=y+17;i<strlen(x);++i) fprintf(fp2,"%c",x[i]);
	fprintf(fp2,"\n");
	return;
}
void ppr(char x[],int y)//位置报告 
{//显示日期、时间、航班、纬度、经度、空速、风速、风向、高度、飞机当前油量 
	int i=y;
	fprintf(fp2,"日期:");
	while(x[i]!=',') fprintf(fp2,"%c",x[i++]);
	i++;
	fprintf(fp2,"\t时间:");
	while(x[i]!=',') fprintf(fp2,"%c",x[i++]);
	i+=5;
	fprintf(fp2,"\t航班:");
	while(x[i]!=',') fprintf(fp2,"%c",x[i++]);
	i+=5;
	fprintf(fp2,"\t纬度:");
	while(x[i]!=',') fprintf(fp2,"%c",x[i++]);
	i+=5;
	fprintf(fp2,"\t经度:");
	while(x[i]!=',') fprintf(fp2,"%c",x[i++]);
	i+=5;
	fprintf(fp2,"\t空速:");
	while(x[i]!=',') fprintf(fp2,"%c",x[i++]);
	i+=4;
	fprintf(fp2,"\t风速:");
	while(x[i]!=',') fprintf(fp2,"%c",x[i++]);
	i+=4;
	fprintf(fp2,"\t风向:");
	while(x[i]!=',') fprintf(fp2,"%c",x[i++]);
	i+=5;
	fprintf(fp2,"\t高度:");
	while(x[i]!=',') fprintf(fp2,"%c",x[i++]);
	i+=4;
	fprintf(fp2,"\t飞机当前油量:");
	for(i;i<strlen(x);++i) fprintf(fp2,"%c",x[i]);
	fprintf(fp2,"\n"); 
	return;
}
void ma(char x[],int y)//媒体报告 
{//显示版本号、是否建立联系、媒体识别、UTC时间、当前媒体状态 
	fprintf(fp2,"媒体报告:\n\t版本号:0\t建立/失去联系:");
	fprintf(fp2,"%c",x[y]);
	fprintf(fp2,"\t媒体识别:");
	fprintf(fp2,"%c",x[y+1]);
	fprintf(fp2,"\tUTC时间:");
	for(int i=y+2;i<y+8;++i) fprintf(fp2,"%c",x[i]);
	fprintf(fp2,"\t当前媒体状态:");
	for(int i=y+8;i<strlen(x);++i) fprintf(fp2,"%c",x[i]);
	fprintf(fp2,"\n");
	return;
}
void atis(char x[],int y)//ATIS请求 
{//显示辅助地址、嵌入消息标识、版本号、请求机场、请求类型、CRC校验 
	int i=y;
	fprintf(fp2,"ATIS请求:\n\t辅助地址:");
	while(x[i]!='.') fprintf(fp2,"%c",x[i++]);
	i++;
	fprintf(fp2,"\t嵌入消息标识:");
	while(x[i]!='/') fprintf(fp2,"%c",x[i++]);
	i++;
	fprintf(fp2,"\t版本号:");
	fprintf(fp2,"%c%c%c",x[i],x[i+1],x[i+2]);
	fprintf(fp2,"\t请求机场:");
	fprintf(fp2,"%c%c%c%c",x[i+3],x[i+4],x[i+5],x[i+6]);
	fprintf(fp2,"\t请求类型:");
	fprintf(fp2,"%c",x[i+7]);
	fprintf(fp2,"\tCRC校验:");
	fprintf(fp2,"%c%c%c%c\n",x[i+8],x[i+9],x[i+10],x[i+11]);
	return;
}
void u5(char x[],int y)//气象请求报 
{//显示请求机场、请求气象类型 
	int i=y;
	fprintf(fp2,"气象请求报\n\t请求机场:");
	for(i;i<y+4;++i) fprintf(fp2,"%c",x[i]);
	i++;
	fprintf(fp2,"\t请求气象类型:");
	fprintf(fp2,"%c%c\n",x[i],x[i+1]);
	return;
}
void rte(char x[],int y)//B777 RTE报 
{//显示报文页码、发送日期、发送时间、航空公司和飞机识别号、航班号、起飞机场、目的机场、CMCF软件号、数据源、航班起始日期与时间 
	int i=y+1;
	fprintf(fp2,"Real Time Event Report\n\t报文页码:");
	while(x[i]!=' ') fprintf(fp2,"%c",x[i++]);
	i++;
	fprintf(fp2,"\t发送日期:");
	while(x[i]!=' ') fprintf(fp2,"%c",x[i++]);
	i++;
	fprintf(fp2,"\t发送时间:");
	while(x[i]!=' ') fprintf(fp2,"%c",x[i++]);
	i++;
	fprintf(fp2,"\t航空公司和飞机识别号:");
	while(x[i]!=' ') fprintf(fp2,"%c",x[i++]);
	i++;
	fprintf(fp2,"\t航班号:");
	while(x[i]!=' ') fprintf(fp2,"%c",x[i++]);
	i++;
	fprintf(fp2,"\t起飞机场:");
	while(x[i]!='/') fprintf(fp2,"%c",x[i++]);
	i++;
	fprintf(fp2,"\t目的机场:");
	while(x[i]!=' ') fprintf(fp2,"%c",x[i++]);
	i++;
	fprintf(fp2,"\tCMCF软件号:");
	while(x[i]!=' ') fprintf(fp2,"%c",x[i++]);
	i++;
	fprintf(fp2,"\t数据源:");
	while(x[i]!=' ') fprintf(fp2,"%c",x[i++]);
	if(x[i-1]=='L') fprintf(fp2,"  左CMC");
    else fprintf(fp2,"  右CMC"); 
	i++;
	fprintf(fp2,"\t航班起始日期与时间:");
	while(x[i]!=' ') fprintf(fp2,"%c",x[i++]);
	i++;
	fprintf(fp2," ");
	for(i;i<strlen(x);++i) fprintf(fp2,"%c",x[i]);
	fprintf(fp2,"\n");
	return;
}
void ncmm()//无相关联的驾驶舱效应 
{
	fprintf(fp2,"无相关联的驾驶舱效应\n");
	return;
}
void fde(char x[],int y)//驾驶舱信息 
{//显示故障代码、字符、FDE变为Active的时间 
	int i=y+1;
	fprintf(fp2,"驾驶舱信息:\n\t故障代码:");
	while(x[i]!=' ') fprintf(fp2,"%c",x[i++]);
	i++;
	fprintf(fp2,"\t");
	switch(x[i])
	{
		case 'A':fprintf(fp2,"Active");break;
		case 'I':fprintf(fp2,"Not Active");break;
		case 'L':fprintf(fp2,"Latched");break;
		case 'N':fprintf(fp2,"No Activity");break;
	}
	i+=2;
	fprintf(fp2,"\tFDE变为Active的时间:");
	while(x[i]!=' ') fprintf(fp2,"%c",x[i++]);
	i++;
	fprintf(fp2," ");
	for(i;i<strlen(x);++i) fprintf(fp2,"%c",x[i]);
	fprintf(fp2,"\n");
	return;
}
void msg(char x[],int y)//维护信息行 
{//显示MM号、字符、 MSG变为Active的时间、飞行阶段、间歇性、间歇数、字符PL、探测源信息 
	int i=y+1;
	fprintf(fp2,"维护信息行:\n\tMM号:");
	while(x[i]!=' ') fprintf(fp2,"%c",x[i++]);
	i++;
	fprintf(fp2,"\t");
	switch(x[i])
	{
		case 'A':fprintf(fp2,"Active");break;
		case 'I':fprintf(fp2,"Not Active");break;
		case 'L':fprintf(fp2,"Latched");break;
		case 'N':fprintf(fp2,"No Activity");break;
	}
	i+=2;
	fprintf(fp2,"\tMSG变为Active的时间:");
	while(x[i]!=' ') fprintf(fp2,"%c",x[i++]);
	i++;
	fprintf(fp2," ");
	while(x[i]!=' ') fprintf(fp2,"%c",x[i++]);
	i++;
	fprintf(fp2,"\t");
	//////////////JUDGE
	if(x[i]=='P'&&x[i+1]=='O') fprintf(fp2,"Power on");
	else if(x[i]=='E'&&x[i+1]=='S') fprintf(fp2,"Engine start");
	else if(x[i]=='T'&&x[i+1]=='A') fprintf(fp2,"Taxi out");
	else if(x[i]=='T'&&x[i+1]=='O') fprintf(fp2,"Take off");
	else if(x[i]=='I'&&x[i+1]=='C') fprintf(fp2,"Initial climb");
	else if(x[i]=='C'&&x[i+1]=='L') fprintf(fp2,"Climb");
	else if(x[i]=='E'&&x[i+1]=='R') fprintf(fp2,"Cruise");
	else if(x[i]=='D'&&x[i+1]=='C') fprintf(fp2,"Descent");
	else if(x[i]=='A'&&x[i+1]=='P') fprintf(fp2,"Approach");
	else if(x[i]=='G'&&x[i+1]=='A') fprintf(fp2,"Go around");
	else if(x[i]=='F'&&x[i+1]=='L') fprintf(fp2,"Flare");
	else if(x[i]=='R'&&x[i+1]=='O') fprintf(fp2,"Rollout");
	else if(x[i]=='T'&&x[i+1]=='I') fprintf(fp2,"Taxi in");
	else if(x[i]=='S'&&x[i+1]=='D') fprintf(fp2,"Shutdown");
	else if(x[i]=='M'&&x[i+1]=='T') fprintf(fp2,"Maintenance");
	///////////////
	i+=3;
	fprintf(fp2,"\t间歇性:");
	switch(x[i])
	{
		case 'H':fprintf(fp2,"Hard");break;
		case 'I':fprintf(fp2,"Intermittent");i+=2;fprintf(fp2,"\t间歇数:");fprintf(fp2,"%c",x[i]);break;
		case 'M':fprintf(fp2,"Mode dependent");
	}
	if(i!=strlen(x))
		fprintf(fp2,"\t上一航班出现该维护信息");
	fprintf(fp2,"\n");
	return;
}
void db(char x[],int y)//探测源信息 
{
	int i=y+1;
	fprintf(fp2,"探测源信息:");
	for(i;i<strlen(x);++i) fprintf(fp2,"%c",x[i]);
	fprintf(fp2,"\n");
	return;
}
void eor()// 故障报文结束
{
	fprintf(fp2,"故障报文结束\n"); 
	return;
}

