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
char acars[50][200];//ԭʼ���� 
bool bo=false,pprbo=false;//bo�ж�'-' pprbo�ж��Ƿ���PRESENT POSITION REOPRT 
char temp[10];
void find(char x[]);
void lable(char x[]);

void before(char x[],int y);///line1~5    y==line

void out(char x[],int y);
void off(char x[],int y);//y�Ǳ�ʶ������ġ���ʵ�������λ�� 
void onn(char x[],int y);
void inn(char x[],int y);
void wxr(char x[],int y);//�������� lable 12
void pos(char x[],int y);
void eta(char x[],int y);
void cld(char x[],int y);
void ppr(char x[],int y);//PRESENT POSITION REPORT
void rte(char x[],int y);
void fde(char x[],int y);
void msg(char x[],int y);
void eor(char x[],int y);
void ma(char x[],int y);//ý�屨��(Media Advisory)
void atis(char x[],int y);//ATIS����
void u5(char x[],int y);//�������� lable 5u 
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
	memset(temp,'\0',sizeof(temp));//���temp�������ڱ��汨�ı�ʶ�� 
	int t=0,i;
	if(bo) i=0;else i=3;//���������'-'����ȡλ�ú��� 
	for(i;i<strlen(x);++i)//����ȡ���ı��ı�ʶ������temp��
	{
		if(x[i]>='A'&&x[i]<='Z') temp[t++]=x[i];
		else break;
	}
	///////////// �жϱ��ı�ʶ�� 
	if(temp[0]=='O'&&temp[1]=='U'&&temp[2]=='T') out(x,i);//�������� 
	else if(temp[0]=='O'&&temp[1]=='F'&&temp[2]=='F') off(x,i); //��ɱ��� 
	else if(temp[0]=='O'&&temp[1]=='N'&&temp[2]=='N') onn(x,i); //��ر��� 
	else if(temp[0]=='I'&&temp[1]=='N'&&temp[2]=='N') inn(x,i); //���뱨�� 
	else if(temp[0]=='W'&&temp[1]=='X'&&temp[2]=='R') wxr(x,i); //�������� 
	else if(temp[0]=='P'&&temp[1]=='O'&&temp[2]=='S') pos(x,i); //λ�ñ�1 
	else if(temp[0]=='E'&&temp[1]=='T'&&temp[2]=='A') eta(x,i); //Ԥ�Ƶ���ʱ�䱨�� 
	else if(temp[0]=='C'&&temp[1]=='L'&&temp[2]=='D') cld(x,i); //���ǰ���� 
	else if(temp[0]=='P'&&temp[1]=='R'&&temp[2]=='E'&&temp[3]=='S'&&temp[4]=='E'&&temp[5]=='N'&&temp[6]=='T') pprbo=true;//λ�ñ����� 
	else if(temp[0]=='R'&&temp[1]=='T'&&temp[2]=='E') rte(x,i);//B777  RTE���ϱ� 
	else if(temp[0]=='N'&&temp[1]=='C'&&temp[2]=='M'&&temp[3]=='M') ncmm();//�޼�ʻ��ЧӦ���� 
	else if(temp[0]=='F'&&temp[1]=='D'&&temp[2]=='E') fde(x,i);//��ʻ��ЧӦ���� 
	else if(temp[0]=='M'&&temp[1]=='S'&&temp[2]=='G') msg(x,i);//ά����Ϣ�� 
	else if(temp[0]=='D'&&temp[1]=='B') db(x,i);//̽��Դ��Ϣ 
	else if(temp[0]=='E'&&temp[1]=='O'&&temp[2]=='R') return;//���ϱ������ 
	else if(x[i]=='0'&&(x[i+1]=='E'||x[i+1]=='L')) ma(x,i+1);//ý�屨�� 
	else if(x[i]=='/'&&x[i+5]=='.') atis(x,i+1);//ATIS���� 
	else if(x[i]=='-'&&((x[i+1]=='F'&&x[i+2]=='S')||(x[i+1]=='S'&&x[i+2]=='A'))) u5(x,i-4);//��������Lable 5U 
	
	return;
}

//////////////////////////
//////////////////////////
void before(char x[],int y)//����ǰ5�д��� 
{//��ʾ���ȼ���Ŀ�ĵ�ַ������վ��ַ����׼��Ϣ��ʾ��������š��ɻ�ע��š�DSP��ʶ��������ʱ�䡢�������к� 
	int i=0;
	if(y==0)
	{
		fprintf(fp2,"���ȼ�:QU\tĿ�ĵ�ַ:");
		while(x[i]!=' ') i++;
		for(i+1;i<strlen(x);++i) fprintf(fp2,"%c",x[i]);
		fprintf(fp2,"\n");
		return;
	}
	else if(y==1)
	{
		fprintf(fp2,"ת�����ĵĵ���վ��ַ:");
		i=1;
		while(x[i]!=' ') fprintf(fp2,"%c",x[i++]);
		fprintf(fp2,"\tת��ʱ��:");
		for(i+1;i<strlen(x);++i) fprintf(fp2,"%c",x[i]);
		fprintf(fp2,"\n");
		return;
	}
	else if(y==2)
	{
		fprintf(fp2,"��׼��Ϣ��ʾ��:");
		i=1;
		for(i;i<strlen(x);++i) fprintf(fp2,"%c",x[i]);
		fprintf(fp2,"\n");
		return;
	}
	else if(y==3)
	{
		i=3;
		fprintf(fp2,"�����:");
		while(x[i]!='/') fprintf(fp2,"%c",x[i++]);
		i+=4;
		fprintf(fp2,"\t�ɻ�ע���:");
		for(i;i<strlen(x);++i) fprintf(fp2,"%c",x[i]);
		fprintf(fp2,"\n");
		return;
	}
	else if(y==4)
	{
		i=3;
		fprintf(fp2,"DSP��ʾ��:");
		while(x[i]!=' ') fprintf(fp2,"%c",x[i++]);
		fprintf(fp2,"\t�����´���Ϣ�ĵ��湤��վ��ַ:");
		i++;
		while(x[i]!=' ') fprintf(fp2,"%c",x[i++]);
		fprintf(fp2,"\t������Ϣʱ��:");
		i++;
		while(x[i]!=' ') fprintf(fp2,"%c",x[i++]);
		i++;
		fprintf(fp2,"\t�������к�:");
		for(i;i<strlen(x);++i) fprintf(fp2,"%c",x[i]);
		fprintf(fp2,"\n");
	}
	return;
}



//////////////////////////
//////////////////////////
void out(char x[],int y)//��������
{//��ʾִ�����ڡ���ɻ�����Ŀ�Ļ�����ʣ��ʱ�䡢ʣ������  
	int i;
	fprintf(fp2,"��������:\n\tִ������:");
	for(i=y;i<y+6;++i) fprintf(fp2,"%c",x[i]);
	fprintf(fp2,"\n\t��ɻ���:");
	for(i=y+7;i<y+11;++i) fprintf(fp2,"%c",x[i]);
	fprintf(fp2,"\n\tĿ�Ļ���:");
	for(i=y+12;i<y+16;++i) fprintf(fp2,"%c",x[i]);
	fprintf(fp2,"\n\tʣ��ʱ��:");
	for(i=y+17;i<y+23;++i)
	{
		if(x[i]!=',') fprintf(fp2,"%c",x[i]);
		else break;
	}
	fprintf(fp2,"\n\tʣ������:");
	i++;
	for(i;i<strlen(x);++i) fprintf(fp2,"%c",x[i]);
	fprintf(fp2,"\n");
	return;
}
void off(char x[],int y)//��ɱ��� 
{//��ʾִ�����ڡ���ɻ�����Ŀ�Ļ�����ʣ��ʱ�� 
	int i;
	fprintf(fp2,"��ɱ���:\n\tִ������:");
	for(i=y;i<y+6;++i) fprintf(fp2,"%c",x[i]);
	fprintf(fp2,"\n\t��ɻ���:");
	for(i=y+7;i<y+11;++i) fprintf(fp2,"%c",x[i]);
	fprintf(fp2,"\n\tĿ�Ļ���:");
	for(i=y+12;i<y+16;++i) fprintf(fp2,"%c",x[i]);
	fprintf(fp2,"\n\tʣ��ʱ��:");
	for(i=y+17;i<y+23;++i)
	{
		if(x[i]!=',') fprintf(fp2,"%c",x[i]);
		else break;
	}
	fprintf(fp2,"\n");
	return;
}
void onn(char x[],int y)//��ر��� 
{//��ʾִ�����ڡ���ɻ�����Ŀ�Ļ�����ʣ��ʱ�䡢ʣ������ 
	int i;
	fprintf(fp2,"��ر���:\n\tִ������:");
	for(i=y;i<y+6;++i) fprintf(fp2,"%c",x[i]);
	fprintf(fp2,"\n\t��ɻ���:");
	for(i=y+7;i<y+11;++i) fprintf(fp2,"%c",x[i]);
	fprintf(fp2,"\n\tĿ�Ļ���:");
	for(i=y+12;i<y+16;++i) fprintf(fp2,"%c",x[i]);
	fprintf(fp2,"\n\tʣ��ʱ��:");
	for(i=y+17;i<y+23;++i)
	{
		if(x[i]!=',') fprintf(fp2,"%c",x[i]);
		else break;
	}
	fprintf(fp2,"\n\tʣ������:");
	i++;
	for(i;i<strlen(x);++i) fprintf(fp2,"%c",x[i]);
	fprintf(fp2,"\n");
	return;
}
void inn(char x[],int y)//���뱨�� 
{//��ʾִ�����ڡ���ɻ��� ��Ŀ�Ļ�����ʣ������ 
	int i;
	fprintf(fp2,"���뱨��:\n\tִ������:");
	for(i=y;i<y+6;++i) fprintf(fp2,"%c",x[i]);
	fprintf(fp2,"\n\t��ɻ���:");
	for(i=y+7;i<y+11;++i) fprintf(fp2,"%c",x[i]);
	fprintf(fp2,"\n\tĿ�Ļ���:");
	for(i=y+12;i<y+16;++i) fprintf(fp2,"%c",x[i]);
	fprintf(fp2,"\n\tʣ������");
	for(i=y+17;i<y+21;++i) fprintf(fp2,"%c",x[i]);
	fprintf(fp2,"\n");
	return; 
}
void wxr(char x[],int y)//�������� 
{//��ʾ�����������͡��������1���������2���������3 
	int i;
	fprintf(fp2,"��������:\n\t������������:");
	for(i=y+1;i<y+3;i++) fprintf(fp2,"%c",x[i]);
	fprintf(fp2,"\n\t�������1:");
	for(i=y+4;i<y+8;++i) fprintf(fp2,"%c",x[i]);
	fprintf(fp2,"\n\t�������2:");
	if(x[i+1]==',') {fprintf(fp2,"\n\t�������3:\n");return;}
	else for(i=y+9;i<y+13;++i) fprintf(fp2,"%c",x[i]);
	fprintf(fp2,"\n\t�������3:");
	if(x[i+2]==',') return;
	else for(i=y+14;i<y+18;++i) fprintf(fp2,"%c",x[i]);
	fprintf(fp2,"\n");
	return;
}
void pos(char x[],int y)//λ�ñ�1 
{//��ʾִ�����ڡ�ά�ȡ����� 
	int i;
	fprintf(fp2,"λ�ñ�1:\n\tִ������:");
	for(i=y;i<y+6;++i) fprintf(fp2,"%c",x[i]);
	fprintf(fp2,"\n\tγ��:");
	for(i=y+7;i<y+15;++i) fprintf(fp2,"%c",x[i]);
	fprintf(fp2,"\n\t����:");
	for(i=y+16;i<y+24;++i) fprintf(fp2,"%c",x[i]);
	fprintf(fp2,"\n");
	return;
}
void eta(char x[],int y)//Ԥ�Ƶ���ʱ�䱨�� 
{//��ʾִ�����ڡ���ɻ�����Ŀ�Ļ�����Ԥ�Ƶ���ʱ�� 
	int i;
	fprintf(fp2,"Ԥ�Ƶ���ʱ�䱨��:\n\tִ������:");
	for(i=y;i<y+6;++i) fprintf(fp2,"%c",x[i]);
	fprintf(fp2,"\n\t��ɻ���:");
	for(i=y+7;i<y+11;++i) fprintf(fp2,"%c",x[i]);
	fprintf(fp2,"\n\tĿ�Ļ���:");
	for(i=y+12;i<y+16;++i) fprintf(fp2,"%c",x[i]);
	fprintf(fp2,"\n\tԤ�Ƶ���ʱ��:");
	for(i=y+17;i<strlen(x);++i) fprintf(fp2,"%c",x[i]);
	fprintf(fp2,"\n");
	return;
}
void cld(char x[],int y)//���ǰ���б��� 
{//��ʾִ�����ڡ���ɻ�����Ŀ�Ļ�����Ԥ�Ƶ���ʱ�� 
	int i;
	fprintf(fp2,"���ǰ���б���:\n\tִ������:");
	for(i=y;i<y+6;++i) fprintf(fp2,"%c",x[i]);
	fprintf(fp2,"\n\t��ɻ���:");
	for(i=y+7;i<y+11;++i) fprintf(fp2,"%c",x[i]);
	fprintf(fp2,"\n\tĿ�Ļ���:");
	for(i=y+12;i<y+16;++i) fprintf(fp2,"%c",x[i]);
	fprintf(fp2,"\n\tԤ�Ƶ���ʱ��:");
	for(i=y+17;i<strlen(x);++i) fprintf(fp2,"%c",x[i]);
	fprintf(fp2,"\n");
	return;
}
void ppr(char x[],int y)//λ�ñ��� 
{//��ʾ���ڡ�ʱ�䡢���ࡢγ�ȡ����ȡ����١����١����򡢸߶ȡ��ɻ���ǰ���� 
	int i=y;
	fprintf(fp2,"����:");
	while(x[i]!=',') fprintf(fp2,"%c",x[i++]);
	i++;
	fprintf(fp2,"\tʱ��:");
	while(x[i]!=',') fprintf(fp2,"%c",x[i++]);
	i+=5;
	fprintf(fp2,"\t����:");
	while(x[i]!=',') fprintf(fp2,"%c",x[i++]);
	i+=5;
	fprintf(fp2,"\tγ��:");
	while(x[i]!=',') fprintf(fp2,"%c",x[i++]);
	i+=5;
	fprintf(fp2,"\t����:");
	while(x[i]!=',') fprintf(fp2,"%c",x[i++]);
	i+=5;
	fprintf(fp2,"\t����:");
	while(x[i]!=',') fprintf(fp2,"%c",x[i++]);
	i+=4;
	fprintf(fp2,"\t����:");
	while(x[i]!=',') fprintf(fp2,"%c",x[i++]);
	i+=4;
	fprintf(fp2,"\t����:");
	while(x[i]!=',') fprintf(fp2,"%c",x[i++]);
	i+=5;
	fprintf(fp2,"\t�߶�:");
	while(x[i]!=',') fprintf(fp2,"%c",x[i++]);
	i+=4;
	fprintf(fp2,"\t�ɻ���ǰ����:");
	for(i;i<strlen(x);++i) fprintf(fp2,"%c",x[i]);
	fprintf(fp2,"\n"); 
	return;
}
void ma(char x[],int y)//ý�屨�� 
{//��ʾ�汾�š��Ƿ�����ϵ��ý��ʶ��UTCʱ�䡢��ǰý��״̬ 
	fprintf(fp2,"ý�屨��:\n\t�汾��:0\t����/ʧȥ��ϵ:");
	fprintf(fp2,"%c",x[y]);
	fprintf(fp2,"\tý��ʶ��:");
	fprintf(fp2,"%c",x[y+1]);
	fprintf(fp2,"\tUTCʱ��:");
	for(int i=y+2;i<y+8;++i) fprintf(fp2,"%c",x[i]);
	fprintf(fp2,"\t��ǰý��״̬:");
	for(int i=y+8;i<strlen(x);++i) fprintf(fp2,"%c",x[i]);
	fprintf(fp2,"\n");
	return;
}
void atis(char x[],int y)//ATIS���� 
{//��ʾ������ַ��Ƕ����Ϣ��ʶ���汾�š�����������������͡�CRCУ�� 
	int i=y;
	fprintf(fp2,"ATIS����:\n\t������ַ:");
	while(x[i]!='.') fprintf(fp2,"%c",x[i++]);
	i++;
	fprintf(fp2,"\tǶ����Ϣ��ʶ:");
	while(x[i]!='/') fprintf(fp2,"%c",x[i++]);
	i++;
	fprintf(fp2,"\t�汾��:");
	fprintf(fp2,"%c%c%c",x[i],x[i+1],x[i+2]);
	fprintf(fp2,"\t�������:");
	fprintf(fp2,"%c%c%c%c",x[i+3],x[i+4],x[i+5],x[i+6]);
	fprintf(fp2,"\t��������:");
	fprintf(fp2,"%c",x[i+7]);
	fprintf(fp2,"\tCRCУ��:");
	fprintf(fp2,"%c%c%c%c\n",x[i+8],x[i+9],x[i+10],x[i+11]);
	return;
}
void u5(char x[],int y)//�������� 
{//��ʾ��������������������� 
	int i=y;
	fprintf(fp2,"��������\n\t�������:");
	for(i;i<y+4;++i) fprintf(fp2,"%c",x[i]);
	i++;
	fprintf(fp2,"\t������������:");
	fprintf(fp2,"%c%c\n",x[i],x[i+1]);
	return;
}
void rte(char x[],int y)//B777 RTE�� 
{//��ʾ����ҳ�롢�������ڡ�����ʱ�䡢���չ�˾�ͷɻ�ʶ��š�����š���ɻ�����Ŀ�Ļ�����CMCF����š�����Դ��������ʼ������ʱ�� 
	int i=y+1;
	fprintf(fp2,"Real Time Event Report\n\t����ҳ��:");
	while(x[i]!=' ') fprintf(fp2,"%c",x[i++]);
	i++;
	fprintf(fp2,"\t��������:");
	while(x[i]!=' ') fprintf(fp2,"%c",x[i++]);
	i++;
	fprintf(fp2,"\t����ʱ��:");
	while(x[i]!=' ') fprintf(fp2,"%c",x[i++]);
	i++;
	fprintf(fp2,"\t���չ�˾�ͷɻ�ʶ���:");
	while(x[i]!=' ') fprintf(fp2,"%c",x[i++]);
	i++;
	fprintf(fp2,"\t�����:");
	while(x[i]!=' ') fprintf(fp2,"%c",x[i++]);
	i++;
	fprintf(fp2,"\t��ɻ���:");
	while(x[i]!='/') fprintf(fp2,"%c",x[i++]);
	i++;
	fprintf(fp2,"\tĿ�Ļ���:");
	while(x[i]!=' ') fprintf(fp2,"%c",x[i++]);
	i++;
	fprintf(fp2,"\tCMCF�����:");
	while(x[i]!=' ') fprintf(fp2,"%c",x[i++]);
	i++;
	fprintf(fp2,"\t����Դ:");
	while(x[i]!=' ') fprintf(fp2,"%c",x[i++]);
	if(x[i-1]=='L') fprintf(fp2,"  ��CMC");
    else fprintf(fp2,"  ��CMC"); 
	i++;
	fprintf(fp2,"\t������ʼ������ʱ��:");
	while(x[i]!=' ') fprintf(fp2,"%c",x[i++]);
	i++;
	fprintf(fp2," ");
	for(i;i<strlen(x);++i) fprintf(fp2,"%c",x[i]);
	fprintf(fp2,"\n");
	return;
}
void ncmm()//��������ļ�ʻ��ЧӦ 
{
	fprintf(fp2,"��������ļ�ʻ��ЧӦ\n");
	return;
}
void fde(char x[],int y)//��ʻ����Ϣ 
{//��ʾ���ϴ��롢�ַ���FDE��ΪActive��ʱ�� 
	int i=y+1;
	fprintf(fp2,"��ʻ����Ϣ:\n\t���ϴ���:");
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
	fprintf(fp2,"\tFDE��ΪActive��ʱ��:");
	while(x[i]!=' ') fprintf(fp2,"%c",x[i++]);
	i++;
	fprintf(fp2," ");
	for(i;i<strlen(x);++i) fprintf(fp2,"%c",x[i]);
	fprintf(fp2,"\n");
	return;
}
void msg(char x[],int y)//ά����Ϣ�� 
{//��ʾMM�š��ַ��� MSG��ΪActive��ʱ�䡢���н׶Ρ���Ъ�ԡ���Ъ�����ַ�PL��̽��Դ��Ϣ 
	int i=y+1;
	fprintf(fp2,"ά����Ϣ��:\n\tMM��:");
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
	fprintf(fp2,"\tMSG��ΪActive��ʱ��:");
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
	fprintf(fp2,"\t��Ъ��:");
	switch(x[i])
	{
		case 'H':fprintf(fp2,"Hard");break;
		case 'I':fprintf(fp2,"Intermittent");i+=2;fprintf(fp2,"\t��Ъ��:");fprintf(fp2,"%c",x[i]);break;
		case 'M':fprintf(fp2,"Mode dependent");
	}
	if(i!=strlen(x))
		fprintf(fp2,"\t��һ������ָ�ά����Ϣ");
	fprintf(fp2,"\n");
	return;
}
void db(char x[],int y)//̽��Դ��Ϣ 
{
	int i=y+1;
	fprintf(fp2,"̽��Դ��Ϣ:");
	for(i;i<strlen(x);++i) fprintf(fp2,"%c",x[i]);
	fprintf(fp2,"\n");
	return;
}
void eor()// ���ϱ��Ľ���
{
	fprintf(fp2,"���ϱ��Ľ���\n"); 
	return;
}

