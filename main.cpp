#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<ctime>
#include<cmath>
#include<cstring>
#include<vector>

#define win_width 900
#define win_height 600
using namespace std;


int h_p,a_p;//h_p：人类玩家,a_p:人机
int n=5,m=5;
//q=t*k，t=1~6,国家；k每个国家的第k块地的格子个数,如果为0代表被其他连通块合并
//r：每块地是哪个国家的第几块地
//f:每个国家有几块领地
//cash:每个连通块的金币
//totalmaint  每个连通块上每回合陆军维护费，回合初更新
//nearltkill：上回合破产的部队
int newr[20][20],newq[7][400],newf[7],newma[20][20],f[7];
int ma[20][20],r[20][20],q[7][400];
int cash[10][400],totalmaint[10][400];
int army[20][20],nearlykill[20][20];
int fx[5]={0,-1,1,0,0};
int fy[5]={0,0,0,-1,1};
int dx[10]={0,1,1,1,0,0,0,-1,-1,-1};
int dy[10]={0,-1,0,1,-1,0,1,-1,0,1};
int border[10];
int gamemode=1,term=0;
vector<int> x;
vector<int> y;
vector<int> n0;
vector<int> m0;//记录阵营可以移动军队的坐标

void NewConj(int nx,int ny,int pnum);
void MaintCounting(int pnum);
int isOurLand(int x0,int y0,int pnum);
int CanArmyBeCreated(int x0,int y0,int pnum,int level);
int ConductCreation(int x0,int y0,int pnum,int dir,int level);
void ConJudge();
void CashCounting(int pnum);
void IniCanMove(int pnum);
int IsAnArmy(int x0,int y0);
int GenMove(int n1,int m1,int dir);
int ConductMove(int n1,int m1,int dir);
int IsBorder(int x0,int y0,int pnum);
int ManDis(int x0,int y0,int x1,int y1);
void AIMoveStep1(int pnum);
int AIMoveStep2(int x0,int y0,int pnum);
int AICreationStep2(int x0,int y0,int pnum);
int AICreation(int pnum);
void HumanPlay(int pnum);
void RunGame();
void StartRandomizedGame();
int printChessBoard(int pnum);
void MapGeneration(int tal);
void CountF();
int WhoWin();
void Game_Start();
void txtprintChessBoard();
void Edit();
void Game_Load();
void Map_Save();
void Game_Save();
void Map_Load();
void Menu();
int main()
{
    Menu();
    return 0;
}
void NewConJ(int nx,int ny,int pnum)//方块更新时,代表pnum国占领了(nx,ny)
{
    for(int i=1;i<=n;i++)  for(int j=1;j<=m;j++)  newma[i][j]=ma[i][j];
    newma[nx][ny]=pnum;
    for(int i=1;i<=n;i++)//枚举每一个点
    {
        for(int j=1;j<=m;j++)
        {
            if(newma[i][j]>0)//如果有颜色
            {
                if(newma[i-1][j]==newma[i][j])//这点的上面有
                {
                    if(newq[newma[i][j]][newr[i][j]])//曾经有颜色
                    {
                        newq[newma[i][j]][newr[i][j]]--;
                    }
                    newr[i][j]=newr[i-1][j];//换色
                    newq[newma[i][j]][newr[i][j]]++;//加入上面
                    if(newma[i][j]==newma[i][j-1]&&newr[i][j]!=newr[i][j-1])//上面和左边都有且原来的编号不一致，涉及重新连通
                    {
                        int tmp=j-1;
                        for(int l=1;l<=n;l++)//暴力，反正格子大小不大，遍历所有格子，寻找原属于该连通块的所有块
                        {
                            for(int k=1;k<=m;k++)
                            {
                                if(newma[l][k]==newma[i][j]&&newr[l][k]==newr[i][tmp])
                                {
                                    newq[newma[i][j]][newr[l][k]]--;
                                    newr[l][k]=newr[i][j];//更新涂色
                                    newq[newma[i][j]][newr[i][j]]++;
                                }
                            }
                        }
                    }
                }
                else if(newma[i][j]==newma[i][j-1])//只有左边有，加入左边
                {
                    if(newq[newma[i][j]][newr[i][j]])
                    {
                        newq[newma[i][j]][newr[i][j]]--;
                    }
                    newr[i][j]=newr[i][j-1];
                    newq[newma[i][j]][newr[i][j]]++;
                }
                else//上下没有连通块，创建连通块
                {
                    int tmp=0,t;
                    if(newq[newma[i][j]][newr[i][j]])
                    {
                        newq[newma[i][j]][newr[i][j]]--;
                    }
                    f[newma[i][j]]++;
                    for(int l=1;l<=200;l++)
                    {
                        if(!newq[newma[i][j]][l])
                        {
                            t=l;
                            break;
                        }
                    }
                    newq[newma[i][j]][t]++;
                    newr[i][j]=t;
                }
                //ps:右和下不需要判定，因为如果右下有连通块会被整合到新的连通块内，反正n，m很小，重新分配连通块即可，暴力足够
            }
        }
    }
    memset(newf,0,sizeof(newf));
    for(int i=1;i<=6;i++)
    {
        for(int j=1;j<=200;j++)
        {
            if(newq[i][j]!=0)  newf[i]++;
        }
    }
}
int isOurLand(int x0,int y0,int pnum)//是我的地吗？  1可以 0不可以
{
    if(x0<=0||y0<=0||x0>n||y0>m)  return 0;
    if(ma[x0][y0]==pnum)  return 1;
    if(ma[x0-1][y0]==pnum)  return 1;
    if(ma[x0][y0-1]==pnum)  return 1;
    if(ma[x0][y0+1]==pnum)  return 1;
    if(ma[x0-1][y0]==pnum)  return 1;
    return 0;
}
int CanArmyBeCreated(int x0,int y0,int pnum,int level)//1可以 0不可以
{
    if(!isOurLand(x0,y0,pnum))  return 0;//不合法的地点
    if(ma[x0][y0]==pnum)//境内，升级
    {
        if(army[x0][y0]+level<=3)  return 1;
        return 0;
    }
    else//境外
    {
        if(level==3)  return 1;//3级兵总是可以被建造
        else
        {
            for(int i=1;i<=9;i++)
            {
                int nx=x0+dx[i],ny=y0+dy[i];
                if(ma[nx][ny]==ma[x0][y0]&&army[x0][y0]>=level)  return 0;
            }            
        }
        return 1;
    }
}
int ConductCreation(int x0,int y0,int pnum,int dir,int level)//1可以 0不行
{
    int nx=x0+fx[dir],ny=y0+fy[dir];
    int flag1=0,maxx=0,maxnum[5]={0};
    if(!isOurLand(nx,ny,pnum))  return 0;
    if(!CanArmyBeCreated(nx,ny,pnum,level))  return 0;
    if(cash[pnum][r[x0][y0]]<level*10)  return 0;
    cash[pnum][r[x0][y0]]-=level*10;
    int t=0,k=0;
    if(ma[nx][ny]==pnum)//本土，不涉及重新连通与否
    {
        army[nx][ny]+=level;
    }
    else//境外，截断与否
    {
        q[ma[nx][ny]][r[nx][ny]]--;
        army[nx][ny]=level;
        NewConJ(nx,ny,pnum);
        t=0,k=0;
        flag1=0,maxx=0;
        if(newf[ma[nx][ny]]>f[ma[nx][ny]])//出现截断
        {
            /*
            * 截断：直接判定上下左右四个新的q哪个更大，就把原有金币给予新的那个，如果多个块一样随机给
            */

            for(int i=1;i<=4;i++)
            {
                if(ma[nx+fx[i]][ny+fy[i]]==ma[nx][ny])
                {
                    if(newq[ma[nx][ny]][newr[nx+fx[i]][ny+fy[i]]]==maxx)
                    {
                        maxnum[++flag1]=i;
                    }
                    if(newq[ma[nx][ny]][newr[nx+fx[i]][ny+fy[i]]]>maxx)
                    {
                        flag1=0;
                        maxx=newq[ma[nx][ny]][newr[nx+fx[i]][ny+fy[i]]];
                        maxnum[++flag1]=i;
                    }
                }
            }
            if(maxx!=0)
            {
                t=rand()%flag1+1;//随机分配
                k=cash[ma[nx][ny]][r[nx][ny]];
            }
            //cash[ma[nx][ny]][r[nx][ny]]=0;
        }
        //if(newf[ma[nx][ny]]<f[ma[nx][ny]])//出现消灭连通块，貌似不需要特判
        //{
        //cash[ma[nx][ny]][r[nx][ny]]=0;
        //}
        MaintCounting(pnum);//计费
        cash[ma[nx][ny]][r[nx][ny]]=0;
        ma[nx][ny]=pnum;
        ConJudge();
        if(t)  cash[ma[nx+fx[maxnum[t]]][ny+fy[maxnum[t]]]][r[nx+fx[maxnum[t]]][ny+fy[maxnum[t]]]]+=k;
    }
    //重新计算是否连通，是否截断，重新分配金币
    return 1;
}

void ConJudge()//并查集判定连通块
{
    memset(f,0,sizeof(f));
    for(int i=1;i<=n;i++)//枚举每一个点
    {
        for(int j=1;j<=m;j++)
        {
            if(ma[i][j]>0)//如果有颜色
            {
                if(ma[i-1][j]==ma[i][j])//这点的上面有
                {
                    if(q[ma[i][j]][r[i][j]])//曾经有颜色
                    {
                        if(q[ma[i][j]][r[i][j]]==1)//最后一块了
                        {
                            cash[ma[i-1][j]][r[i-1][j]]+=cash[ma[i][j]][r[i][j]];
                        }
                        q[ma[i][j]][r[i][j]]--;
                    }
                    r[i][j]=r[i-1][j];//换色
                    q[ma[i][j]][r[i][j]]++;//加入上面
                    if(ma[i][j]==ma[i][j-1]&&r[i][j]!=r[i][j-1])//上面和左边都有且原来的编号不一致，涉及重新连通
                    {
                        cash[ma[i][j]][r[i][j]]+=cash[ma[i][j]][r[i][j-1]];//更新金币
                        int tmp=j-1;
                        for(int l=1;l<=n;l++)//暴力，反正格子大小不大，遍历所有格子，寻找原属于该连通块的所有块
                        {
                            for(int k=1;k<=m;k++)
                            {
                                if(ma[l][k]==ma[i][j]&&r[l][k]==r[i][tmp])
                                {
                                    q[ma[i][j]][r[l][k]]--;
                                    r[l][k]=r[i][j];//更新涂色
                                    q[ma[i][j]][r[i][j]]++;
                                }
                            }
                        }
                    }
                }
                else if(ma[i][j]==ma[i][j-1])//只有左边有，加入左边
                {
                    if(q[ma[i][j]][r[i][j]])
                    {
                        if(q[ma[i][j]][r[i][j]]==1)
                        {
                            cash[ma[i][j-1]][r[i][j-1]]+=cash[ma[i][j]][r[i][j]];
                        }
                        q[ma[i][j]][r[i][j]]--;
                    }
                    r[i][j]=r[i][j-1];
                    q[ma[i][j]][r[i][j]]++;
                }
                else//上下没有连通块，创建连通块
                {
                    int tmp=0,t;
                    if(q[ma[i][j]][r[i][j]])
                    {
                        if(q[ma[i][j]][r[i][j]]==1)  tmp=cash[ma[i][j]][r[i][j]];
                        q[ma[i][j]][r[i][j]]--;
                    }
                    f[ma[i][j]]++;
                    for(int l=1;l<=200;l++)
                    {
                        if(!q[ma[i][j]][l])
                        {
                            t=l;
                            break;
                        }
                    }
                    cash[ma[i][j]][t]=tmp;
                    q[ma[i][j]][t]++;
                    r[i][j]=t;
                }
                //ps:右和下不需要判定，因为如果右下有连通块会被整合到新的连通块内，反正n，m很小，重新分配连通块即可，暴力足够
            }
        }
    }
    memset(f,0,sizeof(f));
    for(int i=1;i<=6;i++)
    {
        for(int j=1;j<=200;j++)
        {
            if(q[i][j]!=0)  f[i]++;
        }
    }
}

void MaintCounting(int pnum)//更新金币,num:第k个国家
{
    int g=0,t=pnum;
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
                        case 1:ff = 2; break;
                        case 2:ff = 4; break;
                        case 3:ff = 7; break;
                        default:break;
                        }
                        totalmaint[t][i]+=ff;
                    }
                }
            }
        }
    }
}
void CashCounting(int pnum)
{
    ConJudge();
    MaintCounting(pnum);
    int t=pnum;
    for(int i=1;i<=200;i++)//计算国库与破产清算
    {
        if(q[t][i])//计算合法块的金币
        {
            cash[t][i]+=(q[t][i]-totalmaint[t][i]);
            if(cash[t][i]<0)//破产
            {
                cash[t][i]=0;
                for(int x=1;x<=n;x++)
                {
                    for(int y=1;y<=m;y++)
                    {
                        if(r[x][y]==i&&ma[x][y]==t)  army[x][y]=0,nearlykill[x][y]=1;
                    }
                }
            }
        }
    }
}

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
    int test0;
    test0=IsAnArmy(n1,m1);
    if(IsAnArmy(n1,m1)==-1)  return 1;
    int nx=n1+fx[dir],ny=m1+fy[dir];
    if(nx<=0||ny<=0||nx>n||ny>m)  return 1;//越界
    if(ma[nx][ny]==-1)  return 1;//越界
    if(ma[nx][ny]==0)  return 0;//无论如何，白地可以移动
    int tx,ty;
    if(ma[nx][ny]!=ma[n1][m1])
    {
        for(int i=1;i<=9;i++)//判断附近是否有人
        {
            tx=nx+dx[i],ty=ny+dy[i];
            if(army[n1][m1]==3)  return 0;
            if(ma[nx][ny]==ma[tx][ty]&&army[n1][m1]<army[tx][ty])  return 1;
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
    int kk=GenMove(n1,m1,dir);
    if(GenMove(n1,m1,dir))  return -1;
    int nx=n1+fx[dir],ny=m1+fy[dir];
    if(ma[nx][ny]==0)//无论如何，白地可以移动
    {
        army[nx][ny]=army[n1][m1];
        army[n1][m1]=0;
        ma[nx][ny]=ma[n1][m1];
        return 0;
    }
    int tt=IsAnArmy(n1,m1);
    n0.erase(n0.begin()+tt);
    m0.erase(m0.begin()+tt);
    int k=0,flag1=0,maxx=0,maxnum[5];
    int t=0;
    if(ma[nx][ny]!=ma[n1][m1])
    {
        q[ma[nx][ny]][r[nx][ny]]--;
        for(int i=1;i<=9;i++)
        {
            if(army[n1][m1]==3)
            {break;}
        }        
        army[nx][ny]=army[n1][m1];
        army[n1][m1]=0;
        ma[nx][ny]=ma[n1][m1];
        NewConJ(nx,ny,ma[n1][m1]);
        if(newf[ma[nx][ny]]>f[ma[nx][ny]])//出现截断
        {
            //截断：直接判定上下左右四个新的q哪个更大，就把原有金币给予新的那个，如果多个块一样随机给

            for(int i=1;i<=4;i++)
            {
                if(ma[nx+fx[i]][ny+fy[i]]==ma[nx][ny])
                {
                    if(newq[ma[nx][ny]][newr[nx+fx[i]][ny+fy[i]]]==maxx)
                    {
                        maxnum[++flag1]=i;
                    }
                    if(newq[ma[nx][ny]][newr[nx+fx[i]][ny+fy[i]]]>maxx)
                    {
                        flag1=0;
                        maxx=newq[ma[nx][ny]][newr[nx+fx[i]][ny+fy[i]]];
                        maxnum[++flag1]=i;
                    }
                }
            }
            if(maxx!=0)
            {
                t=rand()%flag1+1;//随机分配
                k=cash[ma[nx][ny]][r[nx][ny]];
            }
        }
        if(t)
        {
            cash[ma[nx][ny]][r[nx][ny]]=0;
            ma[nx][ny]=ma[n1][m1];
            ConJudge();
            cash[ma[nx+fx[maxnum[t]]][ny+fy[maxnum[t]]]][r[nx+fx[maxnum[t]]][ny+fy[maxnum[t]]]]+=k;
            //想法：调用MoneyAndConJud中的Conjudge函数（传形参，稍微修改代码即可），判断f[ma[i][j]]，如果不一致，对四边新块进行判断q的最大值并更新金币}
        }
        else  cash[ma[nx][ny]][r[nx][ny]]=0;
    }
    else
    {
        army[nx][ny]+=army[n1][m1];
        army[n1][m1]=0;
    }
    return 0;
}
int IsBorder(int x0,int y0,int pnum)//判断是否为边界线，如果是，返回一个数，代表与己方颜色不同的格子的个数
{
    int flag=0;
    for(int i=1;i<=4;i++)
    {
        if(ma[x0+fx[i]][y0+fy[i]]!=pnum&&ma[x0+fx[i]][y0+fy[i]]!=-1)  border[++flag]=i;
    }
    return flag;
}
int ManDis(int x0,int y0,int x1,int y1){return abs(x0-x1)+abs(y0-y1);}//曼哈顿距离

void AIMoveStep1(int pnum)//AI移动
{
    int flag;
    int tmp1[5],tmp2[5];
    int t1,t2,tt;
    for(int i=0;i<n0.size();i++)
    {
        t1=0,t2=0;
        flag=IsBorder(n0[i],m0[i],pnum);
        if(flag)
        {
            for(int j=1;j<=flag;j++)
            {
                if(ma[n0[i]+fx[border[j]]][m0[i]+fy[border[j]]]==0)
                {
                    tmp1[++t1]=border[j];
                }
            }
            if(t1)
            {
                tt=rand()%t1+1;
                ConductMove(n0[i],m0[i],tmp1[tt]);
                continue;
            }
        }
        AIMoveStep2(n0[i],m0[i],pnum);
    }
}
int AIMoveStep2(int x0,int y0,int pnum)//计算移动的权值,并执行移动
{
    double k0[5];
    int W[6]={0,20,15,8,2,0};
    int weight[5];
    int nx,ny;
    for(int k=1;k<=4;k++)
    {
        memset(k0,0,sizeof(k0));
        nx=fx[k]+x0,ny=fy[k]+y0;
        for(int i=nx-4;i<=nx+4;i++)
        {
            for(int j=ny-4;j<=ny+4;j++)
            {
                if(ManDis(i,j,nx,ny)>4||ManDis(i,j,nx,ny)==0)  continue;
                if(ma[nx][ny]!=pnum)  weight[k]+=W[ManDis(i,j,nx,ny)]*(army[nx][ny]);
                else  weight[k]-=W[ManDis(i,j,nx,ny)+1]*(army[nx][ny]);
            }
        }
    }
    int maxx=-114514,maxxnum=0,maxnum[5];
    for(int i=1;i<=4;i++)
    {
        nx=dx[i]+x0,ny=dy[i]+y0;
        if(GenMove(x0,y0,i))  W[i]=-2;
        if(maxx==W[i])  maxnum[++maxxnum]=i;
        if(maxx<W[i])  maxx=W[i],maxxnum=1,maxnum[1]=W[i];
    }
    if(maxxnum)
    {
        int r=rand()%maxxnum+1;
        ConductMove(x0,y0,maxnum[r]);
    }  
    return 0;
}

int AICreationStep2(int x0,int y0,int pnum)//判断造兵的可能性
{
    if(ma[x0][y0]!=pnum)  return 0;
    if(army[x0][y0]==3)  return 0;
    if(cash[pnum][r[x0][y0]]<=10)  return 0;//穷鬼
    if(cash[pnum][r[x0][y0]]>10)//钱得够
    {
        if(cash[pnum][r[x0][y0]]<=20)//不到21块，只能造1级
        {
            if(totalmaint[pnum][r[x0][y0]]+2<q[pnum][r[x0][y0]])//收入大于支出
            {
                int t;
                t=rand()%5;
                if(t<=2)  army[x0][y0]+=1,cash[pnum][r[x0][y0]]-=10;
            }
            else
            {
                if(3*(totalmaint[pnum][r[x0][y0]]+2)>=cash[pnum][r[x0][y0]]);//收支不平衡
                else//入超,造1级
                {
                    int t=rand()%5;
                    if(t<=2)  army[x0][y0]+=1,cash[pnum][r[x0][y0]]-=10;

                }  
            }
        }
        else
        {
            if(totalmaint[pnum][r[x0][y0]]+2<q[pnum][r[x0][y0]])//收入大于支出
            {
                int t;
                if(totalmaint[pnum][r[x0][y0]]+4>q[pnum][r[x0][y0]])//钱不够造3级
                {
                    t=rand()%5;
                    if(t<=2)  army[x0][y0]+=1,cash[pnum][r[x0][y0]]-=10;
                }
                else//钱够3级
                {
                    t=rand()%9;
                    if(t<=2)
                    {
                        army[x0][y0]+=1,cash[pnum][r[x0][y0]]-=10;
                    }
                    if(t>=6)
                    {
                        if(army[x0][y0]==3)  return 0;
                        army[x0][y0]+=2;cash[pnum][r[x0][y0]]-=20;
                    }
                }
            }
            else//收支的情况
            {
                if(3*(totalmaint[pnum][r[x0][y0]]+2)>=cash[pnum][r[x0][y0]]-10);//穷鬼
                else if(3*(totalmaint[pnum][r[x0][y0]]+4)>=cash[pnum][r[x0][y0]]-10)//富哥,但是只造得起1级
                {
                    int t=rand()%5;
                    if(t<=2)  army[x0][y0]+=1,cash[pnum][r[x0][y0]]-=10;
                }  
                else//富哥v50
                {
                    int t=rand()%9;
                    if(t<=2)//0,1,2造1级
                    {
                        army[x0][y0]+=1,cash[pnum][r[x0][y0]]-=10;
                    }
                    if(t>=6)//6，7，8，9造2级
                    {
                        if(army[x0][y0]==2)  return 0;
                        army[x0][y0]+=2;cash[pnum][r[x0][y0]]-=20;
                    }
                }
            }  
        }
    }
    return 0;
}
int AICreation(int pnum)//选址造兵
{
    x.clear();
    y.clear();
    for(int i=1;i<=n;i++)
    {
        for(int j=1;j<=m;j++)
        {
            if(ma[i][j]!=pnum)  continue;
            if(pnum!=ma[i-1][j]||pnum!=ma[i][j-1]||pnum!=ma[i+1][j]||pnum!=ma[i][j+1])  
            {
                x.push_back(i), y.push_back(j);
            }
        }
    }
    int tt;
    for(int i=0;i<x.size();i++)
    {
        tt=rand()%2;//一半可能造兵
        if(!tt)  AICreationStep2(x[i],y[i],pnum);
    }
    return 0;
}

void HumanPlay(int pnum)
{
    int flag=0,x0,y0,d,level,r;
    while(1)
    {
        printChessBoard(pnum);
        cout<<"0：下一回合 1:造兵，2：移动   9:存档"<<endl;
        cin>>flag;
        r=0;
        switch(flag)
        {
        case 0:  return;
        case 1:
            cout<<"依次输入造兵的坐标和方向(1234->上下左右）和等级，位置必须在自己境内"<<endl;
            cin>>x0>>y0>>d>>level;
            r=ConductCreation(x0,y0,pnum,d,level);
            if(r)  cout<<"已建造"<<endl;
            else  cout<<"建造失败，请检查"<<endl;
            break;
        case 2:
            cout<<"依次输入移动的坐标和方向（1234->上下左右）"<<endl;
            cin>>x0>>y0>>d;
            r=ConductMove(x0,y0,d);
            if(r)  cout<<"移动失败，请检查"<<endl;
            else  cout<<"已移动"<<endl;
            break;
        case 9:
            Game_Save();
        default:
            r=114514;
            break;
        }
    }
}
void RunGame()
{
    int flag=0;
    while(1){
        flag=0;
        for(int i=1;i<=h_p;i++)
        {
            flag=0;
            CountF();
            for(int j=1;j<=h_p+a_p;j++)
            {
                if(f[j]==0)  flag++;
                if(flag==h_p+a_p-1)  return;
            }
            CashCounting(i);
            IniCanMove(i);
            HumanPlay(i);
        }
        for(int i=h_p+1;i<=h_p+a_p;i++)
        {
            flag=0;
            CountF();
            for(int j=1;j<=h_p+a_p;j++)
            {
                if(f[j]==0)  flag++;
                if(flag==h_p+a_p-1)  return;
            }
            //txtprintChessBoard();///调试语句
            //system("pause");
            CashCounting(i);
            IniCanMove(i);
            AIMoveStep1(i);
            AICreation(i);
        }
    }
}
void StartRanDomizedGame()
{
    cout<<"输入玩家个数和人机个数，两数之和不大于6且大于1，人机个数可以为0，玩家个数不能为0"<<endl;
    cin>>h_p>>a_p;
    MapGeneration(h_p+a_p);
    ConJudge();
    for(int i=1;i<=h_p+a_p;i++)
    {
        cash[i][1]=10;
    }
    return;
}

int printChessBoard(int pnum)//打印棋盘
{
    cout<<"当前玩家： "<<pnum<<endl<<endl;
    cout<<"颜色地图"<<endl;
    for(int i=1;i<=n;i++)
    {
        for(int j=1;j<=m;j++)
        {
            if(ma[i][j]!=-1)  cout<<' ';
            cout<<ma[i][j];
            cout<<' ';
        }
        cout<<endl;
    }
    cout<<endl<<"军队地图"<<endl;
    for(int i=1;i<=n;i++)
    {
        for(int j=1;j<=m;j++)
        {
            cout<<army[i][j]<<' ';
        }
        cout<<endl;
    }
    cout<<endl<<"金币地图"<<endl;
    for(int i=1;i<=n;i++)
    {
        for(int j=1;j<=n;j++)
        {
            if(ma[i][j]==pnum)  cout<<cash[ma[i][j]][r[i][j]]<<' ';
            else  cout<<"? ";
        }
        cout<<endl;
    }
    cout<<endl<<"还可以移动的军队"<<endl;
    for(int i=0;i<n0.size();i++)
    {
        cout<<'('<<n0[i]<<','<<m0[i]<<")  ";
        if(i%5==4)  cout<<endl;
    }
    cout<<endl;
    /*cout<<endl<<"Test"<<endl;
    for(int i=1;i<=n;i++)
    {
    for(int j=1;j<=m;j++)
    cout<<r[i][j]<<' ';
    cout<<endl;
    }*/
    return 0;
}
void txtprintChessBoard()
{
    cout<<"---------Map---------"<<endl;
    for(int i=1;i<=n;i++)
    {
        for(int j=1;j<=m;j++)
        {
            if(ma[i][j]!=-1)  cout<<' ';
            cout<<ma[i][j];
            cout<<' ';
        }
        cout<<endl;
    }
    cout<<"---------Army---------"<<endl;
    for(int i=1;i<=n;i++)
    {
        for(int j=1;j<=m;j++)
        {
            cout<<army[i][j];
            cout<<' ';
        }
        cout<<endl;
    }
    cout<<"END"<<endl;
    cout<<endl;
}
void MapGeneration(int tal)//生成tal个国家
{
    for(int i=0;i<=19;i++)  for(int j=0;j<=19;j++)  ma[i][j]=-1;
    int tot=0;
    struct node
    {
        int x,y;
    } a[400];
    int maxtt = rand() % (m*n/8) + (m*n/2);
    srand(time(NULL));
    int x=rand()%n+1,y=rand()%m+1;
    a[++tot].x=x,a[tot].y=y;
    ma[x][y]=0;

    while(tot<maxtt)
    {
        int dir=rand()%4+1;
        if (dir==1&&x>1) x--;
        if (dir==2&&x<n) x++;
        if (dir==3&&y>1) y--;
        if (dir==4&&y<m) y++;
        if (ma[x][y]==-1)
        {
            ma[x][y]=0;
            a[++tot].x=x,a[tot].y=y;
        }
    }
    //↑：生成地图
    //↓：生成国家
    int flag=0;
    int xx=0,yy=0;
    for(int i=1;i<=tal;i++)
    {
        for(int j=1;j<=2;j++)
        {
            while(ma[xx][yy]!=0)  xx=rand()%n+1,yy=rand()%m+1;
            ma[xx][yy]=i;
        }
    }
    cout<<114514<<endl;
    return;
}

void CountF()
{
    ConJudge();
    memset(f,0,sizeof(f));
    for(int i=1;i<=a_p+h_p;i++)
    {
        for(int j=1;j<=399;j++)
        {
            if(q[i][j]!=0)  f[i]++;
        }
    }
}
int WhoWin()
{
    for(int i=1;i<=a_p+h_p;i++)
    {
        if(f[i]!=0)  return i;
    }
    return 0;
}

void Menu()
{
    int ch;
    while(1)
    {
        cout<<"1:随机地图开始游戏\n2:读取游戏存档进行游戏\n3:读取地图存档进行游戏\n4:编辑器\n5:游戏说明\n9.退出"<<endl;
        cin>>ch;
        switch(ch)
        {
            case 1:
                gamemode=1;
                Game_Start();
                break;
            case 4:
                Edit();
                break;
            case 2:
                gamemode=2;
                Game_Start();
                break;
            case 3:
                gamemode=3;
                Game_Start();
                break;
            case 9:
                return;

            default:
                cout<<"输入有误"<<endl;
                break;
        }
    }
}
void Game_Start()
{
    memset(army,0,sizeof(army));
    memset(cash,0,sizeof(cash));
    memset(f,0,sizeof(f));
    srand(time(NULL));
    if(gamemode==1)  StartRanDomizedGame();
    else  if(gamemode==2)
    {
        Game_Load();
        RunGame();
    }
    else  if(gamemode==3)
    {
        Map_Load();
        RunGame();
    }
    cout<<"游戏结束了"<<endl;
    int rt=WhoWin();
    cout<<"p"<<rt<<((rt>h_p)?" (人机)":" (人类玩家)")<<" 是胜利者"<<endl;
    system("pause");
}
void Edit()
{
    int cc;
    cout<<"是否读取地图存档继续编辑？1：是  其他：否"<<endl;
    cin>>cc;
    if(cc==1)
    {
       Map_Load();
       for(int i=1;i<=n;i++)  for(int j=1;j<=m;j++)  newma[i][j]=ma[i][j];
       memset(ma,0,sizeof(ma));
    }
    cout<<"输入地图大小n*m:0<n,m<=18"<<endl;
    cin>>n>>m;
    while(n>18||m>18||n<=0||m<=0)  {cout<<"请重试"<<endl;  cin>>n>>m;}
    int t1,t2,t3;
    cout<<"输入玩家个数和人机个数，两数之和不大于6且大于1，人机个数可以为0，玩家个数不能为0"<<endl;
    while(h_p+a_p>=7||h_p<=0||a_p<0||h_p+a_p<=1)  {cout<<"请重试"<<endl;cin>>h_p>>a_p;}
    memset(newma,-1,sizeof(newma));
    cout<<"输入地图样式，格式为：x y t，其中x,y表示坐标。t:0表示空地，-1表示不可通过，1-6表示国家，系统不检查是否连通"<<endl;
    while(1)
    {
        cout<<"输入114查看当前地图，输入514退出，,输入1919查看帮助，输入810生成一张随机地图。否则正常输入x,y,z三个数"<<endl;
        cin>>t1;
        if(t1==114)
        {
            for(int i=1;i<=n;i++)
            {
                for (int j=1;j<=m;j++)
                {
                    if(newma[i][j]==-1)  cout<<-1<<' ';
                    else cout<<' '<<newma[i][j]<<' ';
                }
                cout<<endl;
            }
        }
        else  if(t1==514)
        {
            break;
        }
        else if(t1==1919)      
            cout<<"输入地图样式，格式为：x y t，其中x,y表示坐标。t:0表示空地，-1表示不可通过，1-6表示国家"<<endl
            <<"输入114查看当前地图，输入514退出，,输入1919查看帮助，输入810生成一张随机地图。"<<endl
            <<"否则正常输入x,y,z三个数，系统不检查是否连通"<<endl;
        else if(t1==810)
        {
            MapGeneration(h_p+a_p);
            for(int i=1;i<=n;i++)  for(int j=1;j<=m;j++)  newma[i][j]=ma[i][j];
            memset(ma,0,sizeof(ma));
        }
        else
        {
            cin>>t2>>t3;
            if(t1>=1&&t1<=n&&t2>=1&&t2<=m&&t3>=1&&t3<=h_p+a_p)  
            {
                newma[t1][t2]=t3;
                cout<<"添加成功"<<endl;
            }
            else  cout<<"请重试"<<endl;
        }
    }
    cout<<"导出请输入1，退出（不保存）请输入2"<<endl;
    t1=0;
    while(t1!=1&&t1!=2)
    {
        cin>>t1;
        if(t1==1)
        {
            Map_Save();
            return;
        }
        if(t2==2)  return;
    }
    return;
}
void Game_Load()
{
    memset(ma,0,sizeof(ma));
    freopen("GameSave.txt","r",stdin);
    cin>>n>>m>>h_p>>a_p;
    for(int i=1;i<=n;i++)  for(int j=1;j<=m;j++)  cin>>ma[i][j];
    for(int i=1;i<=n;i++)  for(int j=1;j<=m;j++)  cin>>r[i][j];
    for(int i=1;i<=n;i++)  for(int j=1;j<=m;j++)  cin>>army[i][j];
    for(int i=0;i<400;i++)  for(int j=0;j<400;j++)  cin>>cash[i][j];
    fclose(stdin);
    freopen("CON", "r", stdin);
    cout<<"Completed!";
    ConJudge();
}
void Map_Load()
{
    memset(ma,0,sizeof(ma));
    freopen("MapSave.txt","r",stdin);
    cin>>n>>m>>h_p>>a_p;
    for(int i=1;i<=n;i++)  for(int j=1;j<=m;j++)  cin>>ma[i][j];
    for(int i=1;i<=n;i++)  for(int j=1;j<=m;j++)  cin>>r[i][j];
    for(int i=1;i<=n;i++)  for(int j=1;j<=m;j++)  cin>>army[i][j];
    for(int i=0;i<400;i++)  for(int j=0;j<400;j++)  cin>>cash[i][j];
    fclose(stdin);
    freopen("CON", "r", stdin);
    cout<<"Completed!";
    ConJudge();
}
void Game_Save()
{
    freopen("GameSave.txt","w",stdout);
    cout<<n<<" "<<m<<endl;
    cout<<h_p<<" "<<a_p<<endl;
    for(int i=1;i<=n;i++)
    {
        for(int j=1;j<=m;j++)
        {
            cout<<ma[i][j]<<' ';
        }
        cout<<endl;
    }
    for(int i=1;i<=n;i++)
    {
        for(int j=1;j<=m;j++)
        {
            cout<<r[i][j]<<' ';
        }
        cout<<endl;
    }
    for(int i=1;i<=n;i++)
    {
        for(int j=1;j<=m;j++)
        {
            cout<<army[i][j]<<' ';
        }
        cout<<endl;
    }
    for(int i=0;i<400;i++)
    {
        for(int j=0;j<400;j++)
        {
            cout<<cash[i][j]<<' ';
        }
        cout<<endl;
    }
    fclose(stdout);
    freopen("CON", "w", stdout);
    cout<<"Completed!"<<endl;
}
void Map_Save()
{
    freopen("MapSave.txt","w",stdout);
    cout<<n<<" "<<m<<endl;
    cout<<h_p<<" "<<a_p<<endl;
    
    for(int i=1;i<=n;i++)
    {
        for(int j=1;j<=m;j++)
        {
            cout<<newma[i][j]<<' ';
        }
        cout<<endl;
    }
    for(int i=1;i<=n;i++)
    {
        for(int j=1;j<=m;j++)
        {
            cout<<0<<' ';
        }
        cout<<endl;
    }
    for(int i=1;i<=n;i++)
    {
        for(int j=1;j<=m;j++)
        {
            cout<<0<<' ';
        }
        cout<<endl;
    }
    for(int i=0;i<400;i++)
    {
        for(int j=0;j<400;j++)
        {
            cout<<0<<' ';
        }
        cout<<endl;
    }
    fclose(stdout);
    freopen("CON", "w", stdout);
    cout<<"Completed!"<<endl;
}
/*
游戏存档结构：
1.地图大小 n,m
2.人类和人机数量 h_p和a_p
3.n*m矩阵，地图归属
4.n*m矩阵，r[i][j]，即各点处于的连通块状态
5.n*m矩阵，army[i][j]，即各点目前的军队
6.7*400矩阵，存下cash数组
拟加入校验功能
*/