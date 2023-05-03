/*
1.遍历所有格子，找到所有可以移动的军队。
//调用ArmyMove的IniCanMove函数
2.若军队可以占领无主之地，则直接上前到可移动到的随机无主之地。
第三步：移动
1.计算可移动目的地曼哈顿距离4以内（含)的敌军的权重W，计算方法为：W=兵种等级*w，其中W=20,18,15,12（具体可调）；
2.移动到权重最高的目的地中随机的一个。
----------------->*/
int IsBorder(int x0,int y0,int pnum)//判断是否为边界线，如果是，返回一个数，代表与己方颜色不同的格子的个数
{
	int flag=0;
	for(int i=1;i<=4;i++)
	{
		if(ma[x0+dx[i]][y0+dy[i]]!=pnum&&ma[x0+dx[i]][y0+dy[i]]!=-1)  border[++flag]=i;
	}
	return flag;
}
int AIMoveStep1(int pnum)//占领无主之地
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
第四步：造兵//难点
1.遍历边境线，在边界线上按权重随机造兵并保证收入大于支出或国库总额大于造兵后每回合净支出的3倍再多10；
2.当收入大于支出时，造兵权重为空地：步兵：特种兵=2：3：4；当无论如何收入也不能大于支出时，
若国库总额小于等于造兵后每回合净支出的3倍再多10，不造兵。//即ai不会主动打出包围战和围歼战，因为全过程都在境内造兵
//想法：遍历所有点，找到位于边界线的，
//计算维护费：
---------->
int g=0,t=pnum;
memset(nearlykill,0,sizeof(nearlykill));
for(int x=1;x<=200;x++)  totalmaint[t][x]=0;
for(int i=1;i<=200;i++)//计算维护费
{
if(q[t][i])//合法块
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

在(x0,y0)随机造兵的代码
---------->
if(cash[i][r[x0][y0]]>10)//钱得够
{
if(cash[i][r[x0][y0]]<=20)
{
if(totalmaint[t][r[x0][y0]]+2<q[i])//收入大于支出
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
if(totalmaint[t][r[x0][y0]]+2<q[i])//收入大于支出
{
int t;
if(totalmaint[t][r[x0][y0]]+4>q[i]||cash[i][r[x0][y0]])//钱不够造2级
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
if(t<=2)//0,1,2造1级
{
army[x0][y0]=1,cash[r[x0][y0]]-=10;
break;
}
if(t>=6)//6，7，8，9造2级
{
army[x0][y0]=2;cash[r[x0][y0]]-=20;
break;
}
}
}  
}

}

遍历边界线
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