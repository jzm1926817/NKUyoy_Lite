/*
1.�������и��ӣ��ҵ����п����ƶ��ľ��ӡ�
//����ArmyMove��IniCanMove����
2.�����ӿ���ռ������֮�أ���ֱ����ǰ�����ƶ������������֮�ء�
���������ƶ�
1.������ƶ�Ŀ�ĵ������پ���4���ڣ���)�ĵо���Ȩ��W�����㷽��Ϊ��W=���ֵȼ�*w������W=20,18,15,12������ɵ�����
2.�ƶ���Ȩ����ߵ�Ŀ�ĵ��������һ����
----------------->*/
int IsBorder(int x0,int y0,int pnum)//�ж��Ƿ�Ϊ�߽��ߣ�����ǣ�����һ�����������뼺����ɫ��ͬ�ĸ��ӵĸ���
{
	int flag=0;
	for(int i=1;i<=4;i++)
	{
		if(ma[x0+dx[i]][y0+dy[i]]!=pnum&&ma[x0+dx[i]][y0+dy[i]]!=-1)  border[++flag]=i;
	}
	return flag;
}
int AIMoveStep1(int pnum)//ռ������֮��
{
	for(int i=0;i<n0.size();i++)
	{
		if(IsBorder(n0.[i],m0.[i],pnum));
	}

}
{
int ManDis(int x0,int y0,int x1,int y1){return abs(x0-x1)+abs(y0-y1);}
double k[5];
int W[5]={0,20,18,15,12};
for(int k=1;k<=4;k++)
{
memset(k,0,sizeof(k));
nx=dx[k]+x0,ny=dy[k]+y0;
for(int i=nx-4;i<=nx+4;i++)
{
for(int j=ny-4;j<=ny+4;j++)
{
if(ManDis(i,j,nx,ny)>4)  continue;
weight[k]+=W[ManDis(i,j,nx,ny)]*army;
}
}
}
int maxx=-1,maxxnum=0,maxnum[5];
for(int i=1;i<=4;i++)
{
nx=dx[i]+x0,ny=dy[i]+y0;
if(GenMove(x0,y0,i))  W[i]=-2;
if(maxx==W[i])  maxnum[++maxxnum]=i;
if(maxx<W[i])  maxx=W[i],maxxnum=1,maxnum[1]=W[i];
}
int r=rand()%maxxnum+1;
ConductMove(x0,y0,maxnum[r]);
}
<-----------------
���Ĳ������//�ѵ�
1.�����߾��ߣ��ڱ߽����ϰ�Ȩ������������֤�������֧��������ܶ���������ÿ�غϾ�֧����3���ٶ�10��
2.���������֧��ʱ�����Ȩ��Ϊ�յأ����������ֱ�=2��3��4���������������Ҳ���ܴ���֧��ʱ��
�������ܶ�С�ڵ��������ÿ�غϾ�֧����3���ٶ�10���������//��ai�������������Χս��Χ��ս����Ϊȫ���̶��ھ������
//�뷨���������е㣬�ҵ�λ�ڱ߽��ߵģ�
//����ά���ѣ�
---------->
int g=0,t=pnum;
memset(nearlykill,0,sizeof(nearlykill));
for(int x=1;x<=200;x++)  totalmaint[t][x]=0;
for(int i=1;i<=200;i++)//����ά����
{
if(q[t][i])//�Ϸ���
{
for(int x=1;x<=n;x++)
{
for(int y=1;y<=n;y++)
{
if(r[x][y]==i&&ma[x][y]==t)
{
int ff=0;
switch(army[x][y])
{
case 1:ff=2;break;
case 2:ff=4;break;
case 3:ff=7;break;
default:break;
}
totalmaint[t][i]+=ff;
}
}
}
}
}
<---------

��(x0,y0)�������Ĵ���
---------->
if(cash[i][r[x0][y0]]>10)//Ǯ�ù�
{
if(cash[i][r[x0][y0]]<=20)
{
if(totalmaint[t][r[x0][y0]]+2<q[i])//�������֧��
{
int t;
t=rand()%5;
if(t<=2)  
}
else
{
if(3*(totalmaint[t][r[x0][y0]]+2)>=cash[t])  break;
else
{
int t=rand()%5;
if(t<=2)  army[x0][y0]=1,cash[t][r[x0][y0]]-=10;

}  
}
else
{
if(totalmaint[t][r[x0][y0]]+2<q[i])//�������֧��
{
int t;
if(totalmaint[t][r[x0][y0]]+4>q[i]||cash[i][r[x0][y0]])//Ǯ������2��
{
t=rand()%5;
if(t<=2)  army[x0][y0]=1,cash[r[x0][y0]]-=10;
}
else
{
t=rand()%9;
if(t<=2)
{
army[x0][y0]=1,cash[r[x0][y0]]-=10;
break;
}
if(t>=6)
{
army[x0][y0]=2;cash[r[x0][y0]]-=20;
break;
}
}
}
else
{
if(3*(totalmaint[t][r[x0][y0]]+2)>=cash[t]-10)  break;
else if(3*(totalmaint[t][r[x0][y0]]+4)>=cash[t]-10)
{
int t=rand()%5;
if(t<=2)  army[x0][y0]=1,cash[t][r[x0][y0]]-=10;
}  
else
{
int t=rand()%9;
if(t<=2)//0,1,2��1��
{
army[x0][y0]=1,cash[r[x0][y0]]-=10;
break;
}
if(t>=6)//6��7��8��9��2��
{
army[x0][y0]=2;cash[r[x0][y0]]-=20;
break;
}
}
}  
}

}

�����߽���
vector<int> x;
vector<int> y;
for(int i=1;i<=n;i++)
{
for(int j=1;j<=m;j++)
{
int col=ma[i][j];
if(col!=ma[i-1][j]||col!=ma[i][j-1]||col!=ma[i+1][j]||col!=ma[i][j+1])  
{
x.pushback(i),y.pushback(j);
}
}
}