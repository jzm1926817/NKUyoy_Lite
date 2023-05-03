//Verified
#include<bits/stdc++.h>
using namespace std;
//移动特判：
//1.越界2.有人镇守3.升级4.连通5.截断
const int n=5,m=5;
int ma[20][20],r[20][20],q[7][400],totalcount[400],f[7];
int cash[10][400];
int army[20][20],nearlykill[20][20];
//q=t*k，t=1~6,国家；k每个国家的第k块地的格子个数,如果为0代表被其他连通块合并
//r：每块地是哪个国家的第几块地
//f:每个国家有几块领地
//cash:每个连通块的金币
//totalmaint  每个连通块上每回合陆军维护费，回合初更新
//nearltkill：上回合破产的部队
vector<int> n0;
vector<int> m0;//记录阵营可以移动军队的坐标
int fx[5]={0,-1,1,0,0},dx[10]={0,-1,0,1,-1,0,1,-1,0,1};//上下左右和九宫格
int fy[5]={0,0,0,-1,1},dy[10]={0,-1,-1,-1,0,0,0,1,1,1};
void IniCanMove(int pnum)//某一阵营可以移动的兵的初始化
{
    n0.clear();
    m0.clear();
    for(int i=1;i<=n;i++)  for(int j=1;j<=n;j++)  if(ma[i][j]==pnum&&army[i][j])  n0.push_back(i),m0.push_back(j);
}
int IsAnArmy(int x0,int y0)//是否有一个兵在x0,y0，是的话返回位置，否则返回-1
{
    int flag=-1;
    for(int i=0;i<n0.size();i++)
    {
        if(n0[i]==x0&&m0[i]==y0)  return i;
    }
    return flag;
}
int GenMove(int n1,int m1,int dir)//移动是否合法，不合法返回1，否则返回0
{
    if(IsAnArmy(n1,m1)==-1)  return 1;
    int nx=n1+fx[dir],ny=m1+fy[dir];
    if(nx<=0||ny<=0||nx>n||ny>m)  return 1;//越界
    if(ma[nx][ny]==-1)  return 1;//越界
    if(ma[nx][ny]==0)//无论如何，白地可以移动
    {
        return 0;
    }
    int t=IsAnArmy(n1,m1);
    if(ma[nx][ny]!=ma[n1][m1])
    {
        for(int i=1;i<=9;i++)
        {
            if(army[n1][m1]==3)
            {break;}
            if(ma[nx][ny]==ma[nx+dx[i]][ny+dy[i]]&&army[n1][m1]<army[nx+dx[i]][ny+dy[i]])  return 1;
        }
    }
    else
    {
        if(army[n1][m1]+army[nx][ny]>=4)  return 1;//超过4级了
    }
    return 0;
}
int ConductMove(int n1,int m1,int dir)//进行移动
{
    if(GenMove(n1,m1,dir))  return -1;
    int nx=n1+fx[dir],ny=m1+fy[dir];
    if(ma[nx][ny]==0)//无论如何，白地可以移动
    {
        army[nx][ny]=army[n1][m1];
        army[n1][m1]=0;
        ma[nx][ny]=ma[n1][m1];
        return 0;
    }
    int t=IsAnArmy(n1,m1);
    if(ma[nx][ny]!=ma[n1][m1])
    {
        for(int i=1;i<=9;i++)
        {
            if(army[n1][m1]==3)
            {break;}
        }        
        army[nx][ny]=army[n1][m1];
        ma[nx][ny]=ma[n1][m1];
        //判断截断状态
        //想法：调用MoneyAndConJud中的Conjudge函数（传形参，稍微修改代码即可），判断f[ma[i][j]]，如果不一致，对四边新块进行判断q的最大值并更新金币
        //判断连通状态
        //想法：调用MoneyAndConJud中的Conjudge函数（传形参，稍微修改代码即可），判断f[ma[i][j]]，如果不一致，将金币总和加入新的块
    }
    else
    {
        army[nx][ny]+=army[n1][m1];
        army[n1][m1]=0;
    }
    n0.erase(n0.begin()+t);
    m0.erase(m0.begin()+t);
    return 0;
}
int main()
{
    int x,y,d,k;

    for(int i=1;i<=n;i++)  for(int j=1;j<=m;j++)  cin>>ma[i][j];
    for(int i=1;i<=n;i++)  for(int j=1;j<=m;j++)  cin>>army[i][j];
    while(1)
    {
        if(k)  IniCanMove(2);
        cin>>x>>y>>d>>k;
        cout<<IsAnArmy(x,y)<<endl;
        cout<<GenMove(x,y,d)<<endl;
        cout<<ConductMove(x,y,d)<<endl;
        for(int i=1;i<=n;i++)  {for(int j=1;j<=m;j++)  cout<<ma[i][j]<<" ";cout<<endl;}
        cout<<endl;
        for(int i=1;i<=n;i++)  {for(int j=1;j<=m;j++)  cout<<army[i][j]<<" ";cout<<endl;}
        cout<<endl<<endl;
    }
    return 0;
}