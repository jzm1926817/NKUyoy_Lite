#include"common.h"
#include"UI.h"
#include"tools.h"
#include <sstream>  std::stringstream ss;
#include<string>  std::to_string;
#pragma warning(disable:4996)
#pragma comment(lib,"Winmm.lib")

using namespace std;

IMAGE img_army[3];
COLORREF _Colors[10] = {
    RGB(128, 128, 128),
    RGB(255, 0, 0),
    RGB(0, 255, 0),
    RGB(0, 0, 255),
    RGB(255, 255, 0),
    RGB(255, 0, 255),
    RGB(0, 255, 255),
    RGB(128, 0, 0),
    RGB(0, 128, 0),
    RGB(0, 0, 128),
};

int MusicLen[20]={
    0,
    3*60+28,
    4*60+33,
    5*60+38,
    5*60+54,
    2*60+46,
    2*60+24,
    42,
    3*60+42,
    6*60,
    6*60
};//音乐长度
string musicstr[20]={
    "请选择你的英雄",
    "脱粪の小曲",
    "1444の小曲",
    "1836の小曲",
    "1936同盟国の小曲",
    "1936苏维埃の小曲",
    "1936洗头佬の小曲",
    "凯南开大学の小曲",
    "只因你太美の小曲",
    "自定义1",
    "自定义2"
};//音乐名称
string StageName[20]={
    "",
    "抽象1444",
    "抽象1836",
    "抽象1936",
    "NKUTJU"
};//预设名字

int h_p,a_p;//h_p：人类玩家,a_p:人机
int n=8,m=12;//默认地图大小
//q=t*k，t=1~6,国家；k每个国家的第k块地的格子个数,如果为0代表被其他连通块合并
//r：每块地是哪个国家的第几块地
//f:每个国家有几块领地
//cash:每个连通块的金币
//totalmaint  每个连通块上每回合陆军维护费，回合初更新
//nearltkill：上回合破产的部队
int newr[20][20],newq[7][400],newf[7],newma[20][20],f[7];
int ma[20][20],r[20][20],q[10][400];
int cash[10][400],totalmaint[10][400];
int army[20][20],nearlykill[20][20];
int fx[5]={0,-1,1,0,0};//移动
int fy[5]={0,0,0,-1,1};
int dx[10]={0,1,1,1,0,0,0,-1,-1,-1};//九宫格
int dy[10]={0,-1,0,1,-1,0,1,-1,0,1};
int border[10];//边界判定
int gamemode=1,turn=0,player=1;//回合数，轮到谁了和游戏模式
int hwin=0;//谁赢了
int nmusic=4;//当前播放曲目
int humanPlayerExit,LoadExit=0;
clock_t mplay;//这曲子播了多久了

vector<int> x;
vector<int> y;
vector<int> n0;//记录阵营可以移动军队的坐标
vector<int> m0;

//游戏规则判定相关
void NewConj(int nx,int ny,int pnum);
void MaintCounting(int pnum);
int isOurLand(int x0,int y0,int pnum);
void ConJudge();
void CashCounting(int pnum);
void IniCanMove(int pnum);
int IsAnArmy(int x0,int y0);

//操作与判定
int GenMove(int n1,int m1,int dir);
int ConductMove(int n1,int m1,int dir);
int IsBorder(int x0,int y0,int pnum);
int ManDis(int x0,int y0,int x1,int y1);
int CanArmyBeCreated(int x0,int y0,int pnum,int level);
int ConductCreation(int x0,int y0,int pnum,int dir,int level);

//AI相关
void AIMoveStep1(int pnum);
int AIMoveStep2(int x0,int y0,int pnum);
int AICreationStep2(int x0,int y0,int pnum);
int AICreation(int pnum);

//游戏运行
void HumanPlay(int pnum);
void RunGame();
void StartRandomizedGame();
int printChessBoard(int pnum);
void MapGeneration(int tal);
void CountF();
int WhoWin();
void Game_Start();

//调试和玩家全体阵亡后用
void txtprintChessBoard();
void Delay(int time);

//菜单等非游戏本体相关
void Edit();
void Game_Load();
void ConductGameLoad(char ss);
void Map_Save();
void ConductMapSave(char ss);
void Game_Save();
void ConductGameSave(char ss);
void ChangeMap();
void Map_Load(int fla);
void ConductMapLoad(char ss);
void ConductSpeLoad(int fla);
void Menu();
void Help();

void MusicPlay(int t);
void MusicPlayMenu();
void RandomPlay();

//主函数
//--------BEGIN--------
int main()
{
    MusicPlay(4);
    Menu();
    return 0;
}
//---------END---------

//函数定义
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
                    int tmp=0,t=0;
                    if(newq[newma[i][j]][newr[i][j]])
                    {
                        newq[newma[i][j]][newr[i][j]]--;
                    }
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
    if(ma[x0][y0]==-1)  return 0;
    if(ma[x0][y0]==0)  return 1;
    //if(!isOurLand(x0,y0,pnum))  return 0;//不合法的地点
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
                if(ma[nx][ny]==ma[x0][y0]&&army[nx][ny]>=level)  return 0;
            }
        }
        return 1;
    }
}
int ConductCreation(int x0,int y0,int pnum,int dir,int level)//1可以 0不行
{
    int nx=x0+fx[dir],ny=y0+fy[dir];
    int flag1=0,maxx=0,maxnum[5]={0};
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
                cash[ma[nx][ny]][r[nx][ny]]=0;
            }
            else  cash[ma[nx][ny]][r[nx][ny]]=0;
            //cash[ma[nx][ny]][r[nx][ny]]=0;
        }
        //if(newf[ma[nx][ny]]<f[ma[nx][ny]])//出现消灭连通块，貌似不需要特判
        //{
        //cash[ma[nx][ny]][r[nx][ny]]=0;
        //}
        q[ma[nx][ny]][r[nx][ny]]--;
        army[nx][ny]=level;
        MaintCounting(pnum);//计费
        ma[nx][ny]=pnum;
        ConJudge();
        if(t)  
        {
            cash[ma[nx+fx[maxnum[t]]][ny+fy[maxnum[t]]]][r[nx+fx[maxnum[t]]][ny+fy[maxnum[t]]]]=k;
        }
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
void CashCounting(int pnum)//数钱
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
    for(int i=1;i<=n;i++)  for(int j=1;j<=m;j++)  if(ma[i][j]==pnum&&army[i][j]!=0)  n0.push_back(i),m0.push_back(j);
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
            if(ma[nx][ny]==ma[tx][ty]&&army[n1][m1]<=army[tx][ty])  return 1;
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
        army[nx][ny]=army[n1][m1];
        army[n1][m1]=0;
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
                cash[ma[nx][ny]][r[nx][ny]]=0;
            }
            if(maxx==0)  cash[ma[nx][ny]][r[nx][ny]]=0;
        }
        if(t)
        {
            cash[ma[nx][ny]][r[nx][ny]]=0;
            ma[nx][ny]=ma[n1][m1];
            ConJudge();
            cash[ma[nx+fx[maxnum[t]]][ny+fy[maxnum[t]]]][r[nx+fx[maxnum[t]]][ny+fy[maxnum[t]]]]=k;
            //想法：调用MoneyAndConJud中的Conjudge函数（传形参，稍微修改代码即可），判断f[ma[i][j]]，如果不一致，对四边新块进行判断q的最大值并更新金币}
        }
        else
        {
            ma[nx][ny]=ma[n1][m1];
            ConJudge();
        }
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
        nx=fx[i]+x0,ny=fy[i]+y0;
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
                if(t<=2)  army[x0][y0]+=1,cash[pnum][r[x0][y0]]-=7;
            }
            else
            {
                if(3*(totalmaint[pnum][r[x0][y0]]+2)>=cash[pnum][r[x0][y0]]);//收支不平衡
                else//入超,造1级
                {
                    int t=rand()%5;
                    if(t<=2)  army[x0][y0]+=1,cash[pnum][r[x0][y0]]-=7;

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
                    if(t<=2)  army[x0][y0]+=1,cash[pnum][r[x0][y0]]-=7;
                }
                else//钱够3级
                {
                    t=rand()%9;
                    if(t<=2)
                    {
                        army[x0][y0]+=1,cash[pnum][r[x0][y0]]-=7;
                    }
                    if(t>=6)
                    {
                        if(army[x0][y0]==3)  return 0;
                        army[x0][y0]+=2;cash[pnum][r[x0][y0]]-=14;
                    }
                }
            }
            else//收支的情况
            {
                if(3*(totalmaint[pnum][r[x0][y0]]+2)>=cash[pnum][r[x0][y0]]-10);//穷鬼
                else if(3*(totalmaint[pnum][r[x0][y0]]+4)>=cash[pnum][r[x0][y0]]-10)//富哥,但是只造得起1级
                {
                    int t=rand()%5;
                    if(t<=2)  army[x0][y0]+=1,cash[pnum][r[x0][y0]]-=7;
                }  
                else//富哥v50
                {
                    int t=rand()%9;
                    if(t<=2)//0,1,2造1级
                    {
                        army[x0][y0]+=1,cash[pnum][r[x0][y0]]-=7;
                    }
                    if(t>=6)//6，7，8，9造2级
                    {
                        if(army[x0][y0]==2)  return 0;
                        army[x0][y0]+=2;cash[pnum][r[x0][y0]]-=14;
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

void Delay(int time)//延迟语句 time=毫秒（千分之一秒）
{ 
    clock_t now=clock(); 
    while(clock()-now<time); 
    return;
} 
void HumanPlay(int pnum)//人类玩家界面
{
    humanPlayerExit=0;
    int flag = 0, x0, y0, d, r=0, num = 0;
    bool buttonIsDown[2] = { 0 };
    int _x, _y;
    int curgrade = 1;
    Button* but1 = CreateButton(250, 10, 100, 30, "兵种1");
    Button* but2 = CreateButton(400, 10, 100, 30, "兵种2");
    Button* but3 = CreateButton(550, 10, 100, 30, "兵种3");

    Button* butContinue = CreateButton(0, WIN_H-150, 100, 25, "下一回合");
    Button* butSave = CreateButton(0, WIN_H-100, 100, 25, "保存");
    Button* butMusic = CreateButton(0, WIN_H-200, 100, 25, "音乐播放器");
    Button* bbc=CreateButton(0,WIN_H-50,100,25,"直接退出");
    ExMessage msg;
    char s[64];
    while(1)
    {
        RandomPlay();
        if (peekmessage(&msg)) {
            if (UpdataButton(msg, but1)) {
                curgrade = 1;
            }
            if (UpdataButton(msg, but2)) {
                curgrade = 2;
            }
            if (UpdataButton(msg, but3)) {
                curgrade = 3;
            }
            if (UpdataButton(msg, butContinue)) {
                return;
            }
            if (UpdataButton(msg, butSave)) {
                Game_Save();
            }
            if (UpdataButton(msg, butMusic)) {
                MusicPlayMenu();
            }
            if(UpdataButton(msg,bbc))
            {
                humanPlayerExit=1;
                return;
            }
            //鼠标点击在地图区域
            if (msg.x > 100 && msg.x < 100 + IMG_W * m && msg.y>80 && msg.y < 80 + IMG_H * n) {
                if (num == 0) {
                    if (msg.lbutton && buttonIsDown[0] == 0) {   //左键触发  选择移动士兵位置
                        buttonIsDown[0] = 1;
                        flag = 2;
                        x0 = (msg.y - 80) / IMG_H + 1;
                        y0 = (msg.x - 100) / IMG_W + 1;
                        num = 1;
                    }
                    else if (!msg.lbutton) {
                        buttonIsDown[0] = 0;
                    }
                    if (msg.rbutton && buttonIsDown[1] == 0) {   //右键触发  选择放置士兵位置
                        buttonIsDown[1] = 1;
                        flag = 1;
                        x0 = (msg.y - 80) / IMG_H + 1;
                        y0 = (msg.x - 100) / IMG_W + 1;
                        num = 1;
                    }
                    else if (!msg.rbutton) {
                        buttonIsDown[1] = 0;
                    }
                }
                if (num == 1 && ((msg.lbutton && buttonIsDown[0] == 0) || (msg.rbutton && buttonIsDown[1] == 0))) {
                    buttonIsDown[0] = 1;
                    buttonIsDown[1] = 1;
                    _x = (msg.x - 100) / IMG_W + 1 - y0;
                    _y = (msg.y - 80) / IMG_H + 1 - x0;
                    if (_x == 0 && _y == -1) {
                        d = 1;
                        if (flag == 1) {
                            r = ConductCreation(x0, y0, pnum, d, curgrade);
                            if (r==0)
                                MessageBox(GetHWnd(), "建造失败，请检查", "游戏消息", MB_OK);
                        }
                        else if (flag == 2) {
                            r = ConductMove(x0, y0, d);
                            if (r!=0)
                                MessageBox(GetHWnd(), "移动失败，请检查", "游戏消息", MB_OK);
                        }
                        num = 0;
                    }
                    else if (_x == 0 && _y == 1) {
                        d = 2;
                        if (flag == 1) {
                            r = ConductCreation(x0, y0, pnum, d, curgrade);
                            if (r==0)
                                MessageBox(GetHWnd(), "建造失败，请检查", "游戏消息", MB_OK);
                        }
                        else if (flag == 2) {
                            r = ConductMove(x0, y0, d);
                            if (r!=0)
                                MessageBox(GetHWnd(), "移动失败，请检查", "游戏消息", MB_OK);
                        }
                        num = 0;
                    }
                    else if (_x == -1 && _y == 0) {
                        d = 3;
                        if (flag == 1) {
                            r = ConductCreation(x0, y0, pnum, d, curgrade);
                            if (r==0)
                                MessageBox(GetHWnd(), "建造失败，请检查", "游戏消息", MB_OK);
                        }
                        else if (flag == 2) {
                            r = ConductMove(x0, y0, d);
                            if (r!=0)
                                MessageBox(GetHWnd(), "移动失败，请检查", "游戏消息", MB_OK);
                        }
                        num = 0;
                    }
                    else if (_x == 1 && _y == 0) {
                        d = 4;
                        if (flag == 1) {
                            r = ConductCreation(x0, y0, pnum, d, curgrade);
                            if (r==0)
                                MessageBox(GetHWnd(), "建造失败，请检查", "游戏消息", MB_OK);
                        }
                        else if (flag == 2) {
                            r = ConductMove(x0, y0, d);
                            if (r!=0)
                                MessageBox(GetHWnd(), "移动失败，请检查", "游戏消息", MB_OK);
                        }
                        num = 0;
                    }
                    else if (_x == 0 && _y == 0) {
                        if (flag == 1) {
                            r = ConductCreation(x0, y0, pnum, d, curgrade);
                            if (r==0)
                                MessageBox(GetHWnd(), "建造失败，请检查", "游戏消息", MB_OK);
                            //if (r)  std::cout << "已建造" << endl;
                            //else  std::cout << "建造失败，请检查" << endl;
                            num = 0;
                        }
                        else if(flag==2){
                            if (msg.rbutton)
                                flag = 1;
                        }
                    }
                    else {
                        if (msg.lbutton) {
                            flag = 2;
                            x0 = (msg.y - 80) / IMG_H + 1;
                            y0 = (msg.x - 100) / IMG_W + 1;
                            num = 1;
                        }
                        else if (msg.rbutton) {
                            flag = 1;
                            x0 = (msg.y - 80) / IMG_H + 1;
                            y0 = (msg.x - 100) / IMG_W + 1;
                            num = 1;
                        }
                    }
                }
                if (!msg.lbutton) {
                    buttonIsDown[0] = 0;
                }
                if (!msg.rbutton) {
                    buttonIsDown[1] = 0;
                }
            }
        }
        printChessBoard(pnum);
        if (num == 1) {
            setlinestyle(0, 3);
            setlinecolor(WHITE);
            int x = 100 + (y0 - 1) * IMG_W;
            int y = 80 + (x0 - 1) * IMG_H;
            rectangle(100 + (y0 - 1) * IMG_W, 80 + (x0 - 1) * IMG_H, 100 + (y0)*IMG_W, 80 + (x0)*IMG_H);        //矩形选框
            //上箭头
            line(x + IMG_W / 2, y - IMG_H / 2, x + 5, y - 5);
            line(x + IMG_W / 2, y - IMG_H / 2, x + IMG_W - 5, y - 5);
            //下箭头
            line(x + IMG_W / 2, y + IMG_H / 2 * 3, x + 5, y + IMG_H + 5);
            line(x + IMG_W / 2, y + IMG_H / 2 * 3, x + IMG_W - 5, y + IMG_H + 5);
            //左箭头
            line(x - IMG_W / 2, y + IMG_H / 2, x - 5, y + 5);
            line(x - IMG_W / 2, y + IMG_H / 2, x - 5, y +IMG_H -5);
            //右箭头
            line(x + IMG_W / 2 * 3, y + IMG_H / 2, x + IMG_W + 5, y + 5);
            line(x + IMG_W / 2 * 3, y + IMG_H / 2, x + IMG_W + 5, y + IMG_H - 5);
        }
        settextstyle(20, 0, "宋体");
        sprintf(s, "要放置的兵种%d", curgrade);
        outtextxy(102, 10, s);
        if (flag == 1) {
            outtextxy(700, 10, "放置兵种");
        }
        else if(flag == 2) {
            outtextxy(700, 10, "移动士兵");
        }

        DrawButton(but1);
        DrawButton(but2);
        DrawButton(but3);
        DrawButton(butContinue);
        DrawButton(butSave);
        DrawButton(butMusic);
        DrawButton(bbc);
        FlushBatchDraw();
        cleardevice();    
    }
}
void RunGame()//游戏运行
{
    hwin=0;
    turn=0;
    int flag=0,flag1=0;
    while(1){
        RandomPlay();
        turn++;
        flag=0,flag1=0;
        for(int i=1;i<=h_p;i++)
        {
            if(i<player)  continue;
            flag=0;
            CountF();
            for(int j=1;j<=h_p+a_p;j++)
            {
                if(f[j]==0)  flag++;
                if(flag==h_p+a_p-1)  return;
            }
            if(f[i]==0)  continue;
            else  flag1=1;
            CashCounting(i);
            IniCanMove(i);
            HumanPlay(i);
            if(humanPlayerExit)  return;
            player++;
        }
        for(int i=h_p+1;i<=h_p+a_p;i++)
        {
            if(i<player)  continue;
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
            player++;
        }
        player=1;
        if(!flag1)//没有活着的玩家了
        {
            Delay(50);
            txtprintChessBoard();
            std::cout<<endl;
        }
        int fla=0,fla2=1;
        //cout<<114514<<endl;
        for(int i=1;i<=n;i++)
        {
            for(int j=1;j<=m;j++)
            {
                if(ma[i][j]!=0&&ma[i][j]!=-1)
                {
                    //cout << fla << " "<<fla2<<endl;
                    if(fla==0)  fla=ma[i][j];
                    else  if(fla!=ma[i][j])  fla2=0;
                    
                }
            }
        }
        if(fla2!=0)
        {
            hwin=fla;
            return;
        }
    }
}

void StartRanDomizedGame()
{
    IMAGE img_bk;
    loadimage(&img_bk, "./res/bk.png", WIN_W, WIN_H);
    Button* but[7], *But[7],*bb,*bbc;
    Button *mpx[16],*mpy[16];
    bb=CreateButton(WIN_W/2-30,640,60,30,"确定");
    bbc=CreateButton(WIN_W/2-30,685,60,30,"取消");
    for(int i=1;i<=6;i++)
    {
        string ch="";
        char cc;
        cc=char(i+'0');
        ch+=cc;
        but[i]=CreateButton(75*i+25,75,50,50,ch.c_str());
        ch="";
        cc=char(i-1+'0');
        ch+=cc;
        But[i]=CreateButton(75*i+25,250,50,50,ch.c_str());
    }
    for(int i=1;i<=15;i++)
    {
        mpx[i]=CreateButton(50*i+15,425,25,25,to_string(i+3).c_str());
        mpy[i]=CreateButton(50*i+15,525,25,25,to_string(i+3).c_str());
    }
    ExMessage msg;
    h_p=a_p=0;
    while (1) {
        BeginBatchDraw();
        if (peekmessage(&msg)) {
            for(int i=1;i<=6;i++)
            {
                if (UpdataButton(msg, but[i])) 
                {
                    h_p = i;
                }
            }
            for(int i=1;i<=6;i++)
            {
                if (UpdataButton(msg, But[i])) 
                {
                    a_p = i-1;
                }
            }
            for(int i=1;i<=15;i++)
            {
                if(UpdataButton(msg,mpx[i]))  n=i+3;
                else  if(UpdataButton(msg,mpy[i]))  m=i+3;
            }
            if (UpdataButton(msg, bb)) {
                if(h_p+a_p<=6&&h_p+a_p>=2)
                {
                    MapGeneration(h_p+a_p);
                    ConJudge();
                    for(int i=1;i<=h_p+a_p;i++)
                    {
                        cash[i][1]=10;
                    }
                    return;
                }
                else
                {
                    MessageBox(GetHWnd(), "请检查玩家数量是否符合要求", "游戏消息", MB_OK);
                }
            } 
            if(UpdataButton(msg,bbc))
            {
                LoadExit=1;
                return;
            }
        }
        RandomPlay();
        string ch1,ch2,ch3,ch4;
        ch1=char(h_p+'0'),ch2=char(a_p+'0');
        ch3=to_string(n),ch4=to_string(m);
        putimage(0,0,&img_bk);
        settextcolor(BLACK);
        outtextxy(30,30, "人类玩家个数");
        outtextxy(30,205, "电脑玩家个数");
        outtextxy(15,325, "电脑玩家数量可以为0，人类玩家数量不能为0。");
        outtextxy(15,350,"电脑玩家数量与人类玩家数量总和必须在2到6之间");
        outtextxy(15,375, "地图纵向大小");
        outtextxy(15,465, "地图横向大小");
        outtextxy(150,375,ch3.c_str());
        outtextxy(150,465,ch4.c_str());
        outtextxy(150,30, ch1.c_str());
        outtextxy(150,205, ch2.c_str());
        for(int i=1;i<=6;i++)  DrawButton(but[i]),DrawButton(But[i]);
        for(int i=1;i<=15;i++)  DrawButton(mpx[i]),DrawButton(mpy[i]);
        DrawButton(bb);
        DrawButton(bbc);
        FlushBatchDraw();  
    }
}

int printChessBoard(int pnum)//打印棋盘
{
    for(int i=1;i<=n;i++)
    {
        for(int j=1;j<=m;j++)
        {
            if (ma[i][j] != -1) {
                setfillcolor(_Colors[ma[i][j]]);
                fillrectangle(100 + IMG_W * (j - 1), 80 + IMG_H * (i - 1), 100 + IMG_W * (j), 80 + IMG_H * (i));
            }
        }
    }
    for(int i=1;i<=n;i++)
    {
        for(int j=1;j<=m;j++)
        {
            putimagePNG(100 + IMG_W * (j - 1), 80 + IMG_H * (i - 1), img_army + (army[i][j]-1));
        }
    }
    settextstyle(15, 0, "宋体");
    for(int i=1;i<=n;i++)
    {
        for(int j=1;j<=m;j++)
        {

            char s[128];
            settextcolor(BLACK);
            if (cash[ma[i][j]][r[i][j]] > 0 && ma[i][j] == pnum) {
                sprintf(s, "%d", cash[ma[i][j]][r[i][j]]);
                outtextxy(100 + IMG_W * (j - 1), 80 + IMG_H * (i - 1), s);
            }
            //if(ma[i][j]==pnum)  std::cout<<cash[ma[i][j]][r[i][j]]<<' ';
            //else  std::cout<<"? ";
        }
        //std::cout<<endl;
    }
    
    settextstyle(12, 0, "宋体");
    settextcolor(BLACK);
    outtextxy(2, 80, "还可以移动的军队");
    char s[128];
    settextstyle(15, 0, "宋体");
    for(int i=0;i<n0.size();i++)
    {
        sprintf(s, "(%d,%d)", n0[i], m0[i]);
        outtextxy(2, 80 + (i + 1) * 15, s);
        //std::cout<<'('<<n0[i]<<','<<m0[i]<<")  ";
        //if(i%5==4)  std::cout<<endl;
    }
    //std::cout<<endl;
    setlinecolor(BLACK);
    line(0, 0, WIN_W, 0);
    line(100, 80, WIN_W, 80);
    line(0, WIN_H, WIN_W, WIN_H);
    line(0, 0, 0, WIN_H);
    line(100, 0, 100, WIN_H);
    line(WIN_W, 0, WIN_W, WIN_H);

    setlinecolor(RGB(133, 133, 133));
    settextstyle(15, 0, "宋体");
    sprintf(s, "当前玩家:%d", pnum);
    outtextxy(2, 2, s);
    return 0;
}

void txtprintChessBoard()//调试用
{
    /*
    std::cout<<"---------Map---------"<<endl;
    for(int i=1;i<=n;i++)
    {
        for(int j=1;j<=m;j++)
        {
            if(ma[i][j]!=-1)  std::cout<<' ';
            std::cout<<ma[i][j];
            std::cout<<' ';
        }
        std::cout<<endl;
    }
    std::cout<<"---------Army---------"<<endl;
    for(int i=1;i<=n;i++)
    {
        for(int j=1;j<=m;j++)
        {
            std::cout<<army[i][j];
            std::cout<<' ';
        }
        std::cout<<endl;
    }
    std::cout<<"END"<<endl;
    std::cout<<endl;*/
}

void MapGeneration(int tal)//生成tal个国家
{
    memset(cash,0,sizeof(cash));
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
    //std::cout<<114514<<endl;
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
    srand(unsigned(time(0)));
    _InitWin();
    BeginBatchDraw();
    IMAGE img_bk;
    loadimage(&img_bk, "./res/bk.png", WIN_W, WIN_H);
    loadimage(img_army + 0, "./res/1.png", IMG_W, IMG_H);
    loadimage(img_army + 1, "./res/2.png", IMG_W, IMG_H);
    loadimage(img_army + 2, "./res/3.png", IMG_W, IMG_H);

    Button* but1 = CreateButton(WIN_W / 2 - 150, 150, 300, 40, "随机地图开始游戏");
    Button* but2 = CreateButton(WIN_W / 2 - 150, 200, 300, 40, "读取游戏存档进行游戏");
    Button* but3 = CreateButton(WIN_W / 2 - 150, 250, 300, 40, "读取地图存档进行游戏");
    Button* but4 = CreateButton(WIN_W / 2 - 150, 300, 300, 40, "编辑器");
    Button* but5 = CreateButton(WIN_W / 2 - 150, 350, 300, 40, "游戏说明");
    Button* butMusic = CreateButton(WIN_W / 2 - 150, 400, 300, 40, "音乐播放器");
    Button* but6 = CreateButton(WIN_W / 2 - 150, 450, 300, 40, "退出");
    ExMessage msg;
    while (true) {
        RandomPlay();
        if (peekmessage(&msg)) {
            if (UpdataButton(msg, but1)) {
                gamemode = 1;
                closegraph();
                Game_Start();
            }
            if (UpdataButton(msg, but2)) {
                closegraph();
                gamemode = 2;
                Game_Start();
            }
            if (UpdataButton(msg, but3)) {
                closegraph();
                gamemode = 3;
                Game_Start();
            }
            if (UpdataButton(msg, but4)) {
                closegraph();
                Edit();
                system("cls");
                _InitWin();
            }
            if (UpdataButton(msg, but5)) {
                closegraph();
                Help();
                system("cls");
                _InitWin();
            }
            if (UpdataButton(msg, but6)) {
                closegraph();
                return;
            }
            if (UpdataButton(msg, butMusic)) {
                MusicPlayMenu();
            }
        }

        putimage(0, 0, &img_bk);
        DrawButton(but1);
        DrawButton(but2);
        DrawButton(but3);
        DrawButton(but4);
        DrawButton(but5);
        DrawButton(but6);
        DrawButton(butMusic);
        FlushBatchDraw();
        
        cleardevice();
    }

   
    /*
    * int ch;
    while(1)
    {
        std::cout<<"1:随机地图开始游戏\n2:读取游戏存档进行游戏\n3:读取地图存档进行游戏\n4:编辑器\n5:游戏说明"<<endl
            <<"9.退出"<<endl;
        cin>>ch;
        switch(ch)
        {
        case 1:
            gamemode=1;
            std::cout<<"输入地图大小n*m:0<n,m<=18"<<endl;
            cin >> n >> m;
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
        case 5:
            Help();
            break;
        case 9:
            return;

        default:
            std::cout<<"输入有误"<<endl;
            break;
        }
    }
    */
}

void Game_Start()
{
    humanPlayerExit=0;
    LoadExit=0;
    _InitWin();
    memset(army,0,sizeof(army));
    memset(cash,0,sizeof(cash));
    memset(ma,0,sizeof(ma));
    memset(f,0,sizeof(f));
    srand(time(NULL));
    if(gamemode==1)
    {
        StartRanDomizedGame();
        RunGame();
    }
    else  if(gamemode==2)
    {
        Game_Load();
        RunGame();
    }
    else  if(gamemode==3)
    {
        Map_Load(1);
        RunGame();
    }
    if(LoadExit)  return;
    int fla=0,fla2=1;
    for(int i=1;i<=n;i++)
    {
        for(int j=1;j<=m;j++)
        {
            if(ma[i][j]!=0&&ma[i][j]!=-1)
            {
                if(fla==0)  fla=ma[i][j];
                else  if(fla!=ma[i][j])  fla2=0;

            }
        }
    }
    if(fla2!=0)
    {
        hwin=fla;
    }
    int rt=hwin;
    if(rt)
    {
        ostringstream ostr{};   // 构造一个输出字符串流，流内容为空
        ostr << rt;
        string nums="p";
        nums += ostr.str(); 
        nums+=((rt>h_p)?" (人机)":" (人类玩家)");
        nums+="  是胜利者";
        LPCSTR res=nums.c_str();
        MessageBox(GetHWnd(),res,"游戏结束了",MB_OK);
        //std::cout<<"游戏结束了"<<endl;
        //std::cout<<"p"<<rt<<((rt>h_p)?" (人机)":" (人类玩家)")<<" 是胜利者"<<endl;
        //system("pause");
    }
    else
    {
        if(humanPlayerExit)  MessageBox(GetHWnd(),"玩家主动退出。","游戏提示",MB_OK);
        else if(LoadExit);
        else  MessageBox(GetHWnd(),"地图未正确加载，可能的原因有：该存档不正确，该存档已损坏或编辑器中制作的存档不正确。","游戏提示",MB_OK);
    }
    return;
}

int printEditChessBoard()//打印棋盘
{
    for(int i=1;i<=n;i++)
    {
        for(int j=1;j<=m;j++)
        {
            if (ma[i][j] != -1) {
                setfillcolor(_Colors[ma[i][j]]);
                fillrectangle(100 + IMG_W * (j - 1), 80 + IMG_H * (i - 1), 100 + IMG_W * (j), 80 + IMG_H * (i));
            }
            else{
                setfillcolor(RGB(0,0,0));
                fillrectangle(100 + IMG_W * (j - 1), 80 + IMG_H * (i - 1), 100 + IMG_W * (j), 80 + IMG_H * (i));
            }
        }
    }
    for(int i=1;i<=n;i++)
    {
        for(int j=1;j<=m;j++)
        {
            if(army[i][j])  putimagePNG(100 + IMG_W * (j - 1), 80 + IMG_H * (i - 1), img_army + (army[i][j]-1));
        }
    }
    settextcolor(BLACK);
    setlinecolor(BLACK);
    line(0, 0, WIN_W, 0);
    line(100, 80, WIN_W, 80);
    line(0, WIN_H, WIN_W, WIN_H);
    line(0, 0, 0, WIN_H);
    line(100, 0, 100, WIN_H);
    line(WIN_W, 0, WIN_W, WIN_H);
    setlinecolor(RGB(133, 133, 133));
    return 0;
}
void EditMenu()
{
    closegraph();
    _InitWin();
    IMAGE img_bk;
    loadimage(&img_bk, "./res/bk.png", WIN_W, WIN_H);
    Button* but[7], *But[7],*bb,*bbc;
    Button *mpx[16],*mpy[16];
    bb=CreateButton(WIN_W/2-30,640,60,30,"确定");
    bbc=CreateButton(WIN_W/2-30,685,60,30,"取消");
    for(int i=1;i<=6;i++)
    {
        string ch="";
        char cc;
        cc=char(i+'0');
        ch+=cc;
        but[i]=CreateButton(75*i+25,75,50,50,ch.c_str());
        ch="";
        cc=char(i-1+'0');
        ch+=cc;
        But[i]=CreateButton(75*i+25,250,50,50,ch.c_str());
    }
    for(int i=1;i<=15;i++)
    {
        mpx[i]=CreateButton(50*i+15,425,25,25,to_string(i+3).c_str());
        mpy[i]=CreateButton(50*i+15,525,25,25,to_string(i+3).c_str());
    }
    ExMessage msg;
    h_p=2,a_p=0;
    while (1) {
        RandomPlay();
        BeginBatchDraw();
        if (peekmessage(&msg)) {
            for(int i=1;i<=6;i++)
            {
                if (UpdataButton(msg, but[i])) 
                {
                    h_p = i;
                }
            }
            for(int i=1;i<=6;i++)
            {
                if (UpdataButton(msg, But[i])) 
                {
                    a_p = i-1;
                }
            }
            for(int i=1;i<=15;i++)
            {
                if(UpdataButton(msg,mpx[i]))  n=i+3;
                else  if(UpdataButton(msg,mpy[i]))  m=i+3;
            }
            if (UpdataButton(msg, bb)) {
                if(h_p+a_p<=6&&h_p+a_p>=2)
                {
                    break;
                }
                else
                {
                    MessageBox(GetHWnd(), "请检查玩家数量是否符合要求", "游戏消息", MB_OK);
                }
            } 
            if (UpdataButton(msg, bbc)) {
                LoadExit=1;
                return;
            }
        }
        string ch1,ch2,ch3,ch4;
        ch1=char(h_p+'0'),ch2=char(a_p+'0');
        ch3=to_string(n),ch4=to_string(m);
        putimage(0,0,&img_bk);
        settextcolor(BLACK);
        outtextxy(30,30, "人类玩家个数");
        outtextxy(30,205, "电脑玩家个数");
        outtextxy(15,325, "电脑玩家数量可以为0，人类玩家数量不能为0。");
        outtextxy(15,350,"电脑玩家数量与人类玩家数量总和必须在2到6之间");
        outtextxy(15,375, "地图纵向大小");
        outtextxy(15,465, "地图横向大小");
        outtextxy(150,375,ch3.c_str());
        outtextxy(150,465,ch4.c_str());
        outtextxy(150,30, ch1.c_str());
        outtextxy(150,205, ch2.c_str());
        for(int i=1;i<=6;i++)  DrawButton(but[i]),DrawButton(But[i]);
        for(int i=1;i<=15;i++)  DrawButton(mpx[i]),DrawButton(mpy[i]);
        DrawButton(bb);
        DrawButton(bbc);
        FlushBatchDraw();  
    }
}
void Edit()//编辑器
{
    LoadExit=0;
    int buttonIsDown[2]={0,0};
    memset(ma,-1,sizeof(ma));
    EditMenu();
    if(LoadExit==1)  return;
    closegraph();
    _InitWin();
    IMAGE img_bk;
    ExMessage msg;
    loadimage(&img_bk, "./res/bk.png", WIN_W, WIN_H);
    int flag = 0, x0, y0, d, r, num = 0;
    Button* butarmy[4];
    Button* butcolor[10];
    Button* butRandom=CreateButton(5, WIN_H-250, 85, 25, "随机生成");
    Button* butLoad=CreateButton(5, WIN_H-210, 85, 25, "读取地图");
    Button* butAdjust=CreateButton(5, WIN_H-170,85, 25, "调整地图");
    Button* butSave=CreateButton(5, WIN_H-130, 85, 25, "保存地图");
    Button* butReturn=CreateButton(5, WIN_H-90,85, 25, "直接退出");
    Button* butMusic = CreateButton(5, WIN_H - 50, 85, 25,"音乐播放");
    string ch,ch2;
    for(int i=0;i<=7;i++)
    {
        ch=to_string(i-1);
        butcolor[i]=CreateButton(10,10+50*i,30,30,ch.c_str());
    }
    for(int i=0;i<=3;i++)
    {
        ch="兵种等级"+to_string(i);
        butarmy[i]=CreateButton(250+100*i,10,75,25,ch.c_str());
    }
    int curgrade=0;
    int curcolor=0;
    while(1)
    {
        RandomPlay();
        BeginBatchDraw();
        if (peekmessage(&msg)) {
            for(int i=0;i<=7;i++)
            {
                if(UpdataButton(msg,butcolor[i]))
                {
                    if (i - 1 > h_p + a_p) MessageBox(GetHWnd(), "色号大于玩家总数！", "游戏消息", MB_OK);
                    else  curcolor=i-1;
                }
                if(i<=3&&UpdataButton(msg,butarmy[i]))   curgrade=i;
            }
            if (UpdataButton(msg, butReturn)) {
                return;
            }
            if(UpdataButton(msg,butLoad))
            {
                Map_Load(2);
            }
            if (UpdataButton(msg, butSave)) {
                Map_Save();
            }
            if(UpdataButton(msg,butRandom))
            {
                MapGeneration(h_p+a_p);
            }
            if(UpdataButton(msg,butAdjust))
            {
                ChangeMap();
            }
            if(UpdataButton(msg,butMusic))
            {
                MusicPlayMenu();
            }
            //鼠标点击在地图区域
            if (msg.lbutton && (msg.x > 100 & msg.x < 100 + IMG_W * m )&&( msg.y>80 && msg.y < 80 + IMG_H * n)) {
                x0 = (msg.y - 80) / IMG_H + 1;
                y0 = (msg.x - 100) / IMG_W + 1;
                ma[x0][y0]=curcolor;
                army[x0][y0]=curgrade;
                if(curcolor==0||curcolor==-1)  army[x0][y0]=0;
            }
            if (!msg.lbutton) {
                buttonIsDown[0] = 0;
            }
            if (!msg.rbutton) {
                buttonIsDown[1] = 0;
            }
        }
        setfillcolor(RGB(240,240,240));
        fillrectangle(0,0,114514,114514);
        setfillcolor(RGB(0,0,0));
        fillrectangle(50,10,80,40);
        for(int i=1;i<=7;i++)
        {
            setfillcolor(_Colors[i-1]);
            fillrectangle(50,10+50*i,80,40+50*i);
        }
        printEditChessBoard();
        char s[128] = {0};
        settextstyle(20, 0, "宋体");
        sprintf(s, "兵种等级： %d", curgrade);
        outtextxy(100, 10, s);
        sprintf(s, "当前颜色编号： %d", curcolor);
        outtextxy(100,40,s);
        outtextxy(300,40,"颜色：");
        sprintf(s,"纵向：%d",n);
        outtextxy(5,400,s);
        sprintf(s,"横向：%d",m);
        outtextxy(5,440,s);
        sprintf(s,"玩家：%d",h_p);
        outtextxy(5,480,s);
        sprintf(s,"电脑：%d",a_p);
        outtextxy(5,520,s);
        setfillcolor(_Colors[curcolor]);
        if(curcolor==-1)  setfillcolor(RGB(0,0,0));
        fillrectangle(350,40,375,65);
        for(int i=0;i<=7;i++)  DrawButton(butcolor[i]);
        for(int i=0;i<=3;i++)  DrawButton(butarmy[i]);
        DrawButton(butRandom);
        DrawButton(butSave);
        DrawButton(butLoad);
        DrawButton(butReturn);
        DrawButton(butAdjust);
        DrawButton(butMusic);
        FlushBatchDraw();
    }
    closegraph();
}

void ChangeMap()
{
    MessageBox(GetHWnd(), "请注意，若原地图的大小或人数超过新地图大小，会将超出部分直接抹去。", "提示", MB_OK);
    int n0=n,m0=m;
    int hp=h_p,ap=a_p;
    IMAGE img_bk;
    loadimage(&img_bk, "./res/bk.png", WIN_W, WIN_H);
    Button* but[7], *But[7],*bb,*bbc;
    Button *mpx[16],*mpy[16];
    bb=CreateButton(WIN_W/2-30,600,60,30,"确定");
    bbc=CreateButton(WIN_W/2-30,640,60,30,"取消");
    for(int i=1;i<=6;i++)
    {
        string ch="";
        char cc;
        cc=char(i+'0');
        ch+=cc;
        but[i]=CreateButton(75*i+25,75,50,50,ch.c_str());
        ch="";
        cc=char(i-1+'0');
        ch+=cc;
        But[i]=CreateButton(75*i+25,250,50,50,ch.c_str());
    }
    for(int i=1;i<=15;i++)
    {
        mpx[i]=CreateButton(50*i+15,425,25,25,to_string(i+3).c_str());
        mpy[i]=CreateButton(50*i+15,525,25,25,to_string(i+3).c_str());
    }
    ExMessage msg;
    while (1) {
        BeginBatchDraw();
        RandomPlay();
        if (peekmessage(&msg)) {
            for(int i=1;i<=6;i++)
            {
                if (UpdataButton(msg, but[i])) 
                {
                    h_p = i;
                }
            }
            for(int i=1;i<=6;i++)
            {
                if (UpdataButton(msg, But[i])) 
                {
                    a_p = i-1;
                }
            }
            for(int i=1;i<=15;i++)
            {
                if(UpdataButton(msg,mpx[i]))  n=i+3;
                else  if(UpdataButton(msg,mpy[i]))  m=i+3;
            }
            if(UpdataButton(msg,bbc))
            {
                return;
            }
            if (UpdataButton(msg, bb)) {
                if(h_p+a_p<=6&&h_p+a_p>=2)
                {
                    if(n0>n)
                    {
                        for(int i=n+1;i<=n0;i++)  for(int j=1;j<=m;j++)  ma[i][j]=0;
                    }
                    if(m0>m)
                    {
                        for(int i=1;i<=n;i++)  for(int j=m+1;j<=m0;j++)  ma[i][j]=0;
                    }
                    if(n0>n&&m0>m)
                    {
                        for(int i=n+1;i<=n0;i++)  for(int j=m+1;j<=m0;j++)  ma[i][j]=0;
                    }
                    if(h_p+a_p<hp+ap)
                    {
                        for(int i=0;i<=18;i++)
                        {
                            for(int j=0;j<=18;j++)
                            {
                                if(ma[i][j]>h_p+a_p)  ma[i][j]=0;
                            }
                        }
                    }
                    return;
                }
                else
                {
                    MessageBox(GetHWnd(), "请检查玩家数量是否符合要求", "游戏消息", MB_OK);
                }
            } 
        }
        string ch1,ch2,ch3,ch4;
        ch1=char(h_p+'0'),ch2=char(a_p+'0');
        ch3=to_string(n),ch4=to_string(m);
        putimage(0,0,&img_bk);
        settextcolor(BLACK);
        outtextxy(30,30, "人类玩家个数");
        outtextxy(30,205, "电脑玩家个数");
        outtextxy(15,325, "电脑玩家数量可以为0，人类玩家数量不能为0。");
        outtextxy(15,350,"电脑玩家数量与人类玩家数量总和必须在2到6之间");
        outtextxy(15,375, "地图纵向大小");
        outtextxy(15,465, "地图横向大小");
        outtextxy(150,375,ch3.c_str());
        outtextxy(150,465,ch4.c_str());
        outtextxy(150,30, ch1.c_str());
        outtextxy(150,205, ch2.c_str());
        for(int i=1;i<=6;i++)  DrawButton(but[i]),DrawButton(But[i]);
        for(int i=1;i<=15;i++)  DrawButton(mpx[i]),DrawButton(mpy[i]);
        DrawButton(bb);
        DrawButton(bbc);
        FlushBatchDraw();  
    }
}
void Game_Load()
{
    IMAGE img_bk;
    loadimage(&img_bk, "./res/bk.png", WIN_W, WIN_H);
    Button* but[7],*bb;
    bb=CreateButton(WIN_W/2-30,640,60,30,"确定");
    Button *bbc;
    bbc=CreateButton(WIN_W/2-30,685,60,30,"取消");
    for(int i=1;i<=6;i++)
    {
        string ch="";
        char cc;
        cc=char(i+'0');
        ch+=cc;
        but[i]=CreateButton(75*i+25,75,50,50,ch.c_str());
    }
    int savslot=1;
    ExMessage msg;
    while (1) {
        BeginBatchDraw();
        if (peekmessage(&msg)) {
            for(int i=1;i<=6;i++)
            {
                if (UpdataButton(msg, but[i])) 
                {
                    savslot = i;
                }
            }
            if (UpdataButton(msg, bb)) {
                ConductGameLoad(savslot+'0');
                return;
                }
            } 
            if (UpdataButton(msg, bbc)) {
                LoadExit=1;
                return;
            }
        string ch="当前插槽：";
        ch+=to_string(savslot);
        putimage(0,0,&img_bk);
        settextcolor(BLACK);
        outtextxy(30,30, "请选择存档插槽");
        outtextxy(75,150,ch.c_str());
        for(int i=1;i<=6;i++)  DrawButton(but[i]);
        DrawButton(bb);
        DrawButton(bbc);
        FlushBatchDraw();  
        RandomPlay();
    }
}
void Game_Save()
{
    IMAGE img_bk;
    loadimage(&img_bk, "./res/bk.png", WIN_W, WIN_H);
    Button* but[7],*bb;
    bb=CreateButton(WIN_W/2-30,640,60,30,"确定");
    Button *bbc;
    bbc=CreateButton(WIN_W/2-30,685,60,30,"取消");
    for(int i=1;i<=6;i++)
    {
        string ch="";
        char cc;
        cc=char(i+'0');
        ch+=cc;
        but[i]=CreateButton(75*i+25,75,50,50,ch.c_str());
    }
    int savslot=1;
    ExMessage msg;
    while (1) {
        BeginBatchDraw();
        if (peekmessage(&msg)) {
            for(int i=1;i<=6;i++)
            {
                if (UpdataButton(msg, but[i])) 
                {
                    savslot = i;
                }
            }
            if (UpdataButton(msg, bb)) {
                ConductGameSave(savslot+'0');
                return;
            }
            if (UpdataButton(msg, bbc)) {
                return;
            }
        } 
        string ch="当前插槽：";
        ch+=to_string(savslot);
        putimage(0,0,&img_bk);
        settextcolor(BLACK);
        outtextxy(30,30, "请选择存档插槽");
        outtextxy(75,150,ch.c_str());
        for(int i=1;i<=6;i++)  DrawButton(but[i]);
        DrawButton(bb);
        DrawButton(bbc);
        FlushBatchDraw();  
        RandomPlay();
    }
}
void Map_Load(int fla)
{
    IMAGE img_bk;
    loadimage(&img_bk, "./res/bk.png", WIN_W, WIN_H);
    Button* but[7],*bb;
    Button *butStage[7];
    bb=CreateButton(WIN_W/2-30,640,60,30,"确定");
    Button *bbc;
    bbc=CreateButton(WIN_W/2-30,685,60,30,"取消");
    for(int i=1;i<=6;i++)
    {
        string ch="";
        char cc;
        cc=char(i+'0');
        ch+=cc;
        but[i]=CreateButton(75*i+25,75,50,50,ch.c_str());
    }
    if(fla==2)  for(int i=1;i<=4;i+=2)
    {
        butStage[i]=CreateButton(75,200+75*i,200,50,StageName[i].c_str());
        butStage[i+1]=CreateButton(300,200+75*i,200,50,StageName[i+1].c_str());
    }
    int savslot=1;
    ExMessage msg;
    while (1) {
        BeginBatchDraw();
        if (peekmessage(&msg)) {
            for(int i=1;i<=6;i++)
            {
                if (UpdataButton(msg, but[i])) 
                {
                    savslot = i;
                }
                if(fla==2&&i<=4&&UpdataButton(msg,butStage[i]))
                {
                    savslot=i+10;
                }
            }
            if (savslot<10&&UpdataButton(msg, bb)) {
                ConductMapLoad(savslot+'0');
                return;
            }
            if (UpdataButton(msg, bbc)) {
                LoadExit=1;
                return;
            }
            if(UpdataButton(msg,bb)&&savslot>10)
            {
                ConductSpeLoad(savslot-10);
                return;
            }
        } 
        string ch;
        if(savslot<10)
        {
            ch = "当前选择：存档插槽";
            ch += to_string(savslot);
        }
        else
        {
            ch="当前选择：预设“";
            ch+=StageName[savslot-10];
            ch+="”";
        }
        putimage(0,0,&img_bk);
        settextcolor(BLACK);
        outtextxy(30,30, "请选择存档");
        outtextxy(75,150,ch.c_str());
        for(int i=1;i<=6;i++)  DrawButton(but[i]);
        DrawButton(bb);
        DrawButton(bbc);
        if(fla==2)
        {
            for(int i=1;i<=4;i++)  DrawButton(butStage[i]);
        }
        RandomPlay();
        FlushBatchDraw();  
    }
}
void Map_Save()
{
    IMAGE img_bk;
    loadimage(&img_bk, "./res/bk.png", WIN_W, WIN_H);
    Button* but[7],*bb;
    bb=CreateButton(WIN_W/2-30,640,60,30,"确定");
    Button *bbc;
    bbc=CreateButton(WIN_W/2-30,685,60,30,"取消");
    for(int i=1;i<=6;i++)
    {
        string ch="";
        char cc;
        cc=char(i+'0');
        ch+=cc;
        but[i]=CreateButton(75*i+25,75,50,50,ch.c_str());
    }
    int savslot=1;
    ExMessage msg;
    while (1) {
        BeginBatchDraw();
        if (peekmessage(&msg)) {
            for(int i=1;i<=6;i++)
            {
                if (UpdataButton(msg, but[i])) 
                {
                    savslot = i;
                }
            }
            if (UpdataButton(msg, bb)) {
                ConductMapSave(savslot+'0');
                return;
            }
            if (UpdataButton(msg, bbc)) {
                return;
            }

        } 
        string ch="当前插槽：";
        ch+=to_string(savslot);
        putimage(0,0,&img_bk);
        settextcolor(BLACK);
        outtextxy(30,30, "请选择存档插槽");
        outtextxy(75,150,ch.c_str());
        for(int i=1;i<=6;i++)  DrawButton(but[i]);
        DrawButton(bb);
        DrawButton(bbc);
        FlushBatchDraw();  
        RandomPlay();
    }
}
void ConductGameLoad(char ss)
{
    memset(ma,0,sizeof(ma));
    string sav="GameSave";
    sav+=ss;
    sav+=".sav";
    freopen(sav.c_str(),"r",stdin);
    cin>>n>>m>>h_p>>a_p;
    for(int i=0;i<20;i++)  for(int j=0;j<20;j++)  cin>>ma[i][j];
    for(int i=0;i<20;i++)  for(int j=0;j<20;j++)  cin>>r[i][j];
    for(int i=0;i<20;i++)  for(int j=0;j<20;j++)  cin>>army[i][j];
    for(int i=0;i<10;i++)  for(int j=0;j<400;j++)  cin>>cash[i][j];
    for(int i=0;i<10;i++)  for(int j=0;j<400;j++)  cin>>q[i][j];
    cin>>player;
    fclose(stdin);
    freopen("CON", "r", stdin);
    //std::cout<<"Completed!";
    MessageBox(GetHWnd(), "完成！", "游戏消息", MB_OK);
}
void ConductMapLoad(char ss)
{
    memset(ma,0,sizeof(ma));
    string sav="MapSave";
    sav+=ss;
    sav+=".sav";
    freopen(sav.c_str(),"r",stdin);
    cin>>n>>m>>h_p>>a_p;
    for(int i=0;i<20;i++)  for(int j=0;j<20;j++)  cin>>ma[i][j];
    for(int i=0;i<20;i++)  for(int j=0;j<20;j++)  cin>>r[i][j];
    for(int i=0;i<20;i++)  for(int j=0;j<20;j++)  cin>>army[i][j];
    for(int i=0;i<10;i++)  for(int j=0;j<400;j++)  cin>>cash[i][j];
    for(int i=0;i<10;i++)  for(int j=0;j<400;j++)  cin>>q[i][j];
    cin>>player;
    fclose(stdin);
    freopen("CON", "r", stdin);
    //std::cout<<"Completed!";
    MessageBox(GetHWnd(), "完成！", "游戏消息", MB_OK);
}
void ConductGameSave(char ss)
{
    string sav="GameSave";
    sav+=ss;
    sav+=".sav";
    freopen(sav.c_str(),"w",stdout);
    std::cout<<n<<" "<<m<<endl;
    std::cout<<h_p<<" "<<a_p<<endl;
    for(int i=0;i<20;i++)
    {
        for(int j=0;j<20;j++)
        {
            std::cout<<ma[i][j]<<' ';
        }
        std::cout<<endl;
    }
    for(int i=0;i<20;i++)
    {
        for(int j=0;j<20;j++)
        {
            std::cout<<r[i][j]<<' ';
        }
        std::cout<<endl;
    }
    for(int i=0;i<20;i++)
    {
        for(int j=0;j<20;j++)
        {
            std::cout<<army[i][j]<<' ';
        }
        std::cout<<endl;
    }
    for(int i=0;i<10;i++)
    {
        for(int j=0;j<400;j++)
        {
            std::cout<<cash[i][j]<<' ';
        }
        std::cout<<endl;
    }
    for(int i=0;i<10;i++)
    {
        for(int j=0;j<400;j++)
        {
            std::cout<<q[i][j]<<' ';
        }
        std::cout<<endl;
    }
    cout<<player<<endl;
    fclose(stdout);
    freopen("CON", "w", stdout);
    //std::cout<<"Completed!"<<endl;
    MessageBox(GetHWnd(), "完成！", "游戏消息", MB_OK);
}
void ConductMapSave(char ss)
{
    memset(cash,0,sizeof(cash));
    ConJudge();
    string sav="MapSave";
    sav+=ss;
    sav+=".sav";
    freopen(sav.c_str(),"w",stdout);
    std::cout<<n<<" "<<m<<endl;
    std::cout<<h_p<<" "<<a_p<<endl;

    for(int i=0;i<20;i++)
    {
        for(int j=0;j<20;j++)
        {
            std::cout<<ma[i][j]<<' ';
        }
        std::cout<<endl;
    }
    for(int i=0;i<20;i++)
    {
        for(int j=0;j<20;j++)
        {
            std::cout<<r[i][j]<<' ';
        }
        std::cout<<endl;
    }
    for(int i=0;i<20;i++)
    {
        for(int j=0;j<20;j++)
        {
            std::cout<<army[i][j]<<' ';
        }
        std::cout<<endl;
    }
    for(int i=0;i<10;i++)
    {
        for(int j=0;j<400;j++)
        {
            std::cout<<0<<' ';
        }
        std::cout<<endl;
    }
    for(int i=0;i<10;i++)
    {
        for(int j=0;j<400;j++)
        {
            std::cout<<q[i][j]<<' ';
        }
        std::cout<<endl;
    }
    cout<<1<<endl;
    fclose(stdout);
    freopen("CON", "w", stdout);
    //std::cout<<"Completed!"<<endl;
    MessageBox(GetHWnd(), "完成！", "游戏消息", MB_OK);
}
void ConductSpeLoad(int fla)
{
    memset(ma,0,sizeof(ma));
    string sav="./res/";
    sav+=StageName[fla];
    sav+=".sav";
    freopen(sav.c_str(),"r",stdin);
    cin>>n>>m>>h_p>>a_p;
    for(int i=0;i<20;i++)  for(int j=0;j<20;j++)  cin>>ma[i][j];
    for(int i=0;i<20;i++)  for(int j=0;j<20;j++)  cin>>r[i][j];
    for(int i=0;i<20;i++)  for(int j=0;j<20;j++)  cin>>army[i][j];
    for(int i=0;i<10;i++)  for(int j=0;j<400;j++)  cin>>cash[i][j];
    for(int i=0;i<10;i++)  for(int j=0;j<400;j++)  cin>>q[i][j];
    fclose(stdin);
    freopen("CON", "r", stdin);
    //std::cout<<"Completed!";
    MessageBox(GetHWnd(), "完成！", "游戏消息", MB_OK);
}

void Help()
{
    string hlp;
    
       hlp+= "灵感来源：yiotro-antiyoy（实际上是前作slay）\n";
       hlp+="github:  https://github.com/yiotro/Antiyoy\n\n";
       hlp+="游戏规则：\n";
       hlp+="1.不同颜色代表不同国家的国土，获胜条件为其他所有国家的国土格数均为0；\n";
           hlp+="2.兵种分为步兵（1级）、特种兵（2级）和坦克（3级）三种兵种，步兵只能占领附近没有军队镇守的土地，\n";
           hlp+="特种兵可以占领附近有步兵或无人镇守的土地，同时可以消灭步兵，坦克可以走到附近任何一个格子不受影响，即使附近有坦克。\n";
           hlp+="3.每回合，每个兵可以在境内移动2格，或走向境外一格（如果符合移动条件），如果先移动1格到达边境，则不能再往境外移动。\n";
           hlp+="4.兵每格的移动方式为上下左右移动一格。\n";
           hlp+="5.每回合，拥有的每块地块提供1金币，招募军队（10，20，30）和每回合维持军队（2，4，7）都需要一定量的金币。\n";
           hlp+="6.若军队走到符合条件的另一个己方格子上，可以与已有军队进行升级，升级规则为：新军队等级为两个之和，若超过3则不能升级，\n";
           hlp+="即不能移动到该格子。若该格子的军队本回合未移动过，则升级完的军队依然可以进行移动。\n";
           hlp+="7.一个军队驻扎的周围八格连同自身所在格的9格中，己方控制的格子视为有人镇守的土地。\n";
           hlp+="8.造兵可以在自己的境内（包括本回合获得的土地）及其临近1格进行，可以在已有军队上进行，认定为升级，\n";
           hlp+="本回合造的兵不可以移动，直接在已有军队上升级除外。\n";
           hlp+="9.若一个国家拥有超过一块领地，不同领地的国库分开计算，当两块领地合二为一时，合并这两块领地的国库，\n";
           hlp+="当一块领地被截断时，国库分配给几个领地中较大的一块，如果一样大，则随机分配。\n";
           hlp+="10.当某块领地国库的钱不足以维护军队时，所有军队直接消失，这个判定于自己的回合开始前。\n";
           hlp+="11.落子无悔\n\n";

        hlp+="游戏模式：\n";
            hlp+="1.单机：与1~5个ai进行游戏，但是ai的水平较低；\n";
            hlp+="2.多人：2~6人一台设备上轮流进行操作，可以选择余位是否由ai补齐；\n\n";

            hlp+="地图模式：\n";
            hlp+="1.随机地图：由随机游走生成地图。\n";
            hlp+="2.编辑器：可以自行编辑地图，但是需要保证地图连通，客户端不做判定，并导入地图配置。\n";
            hlp+="3.预设：本游戏有作者预设的几张地图，但是不确保平衡性。（规划中）\n\n";
        
        hlp+="游戏操作：\n";
            hlp += "左键或右键点击格子，左键点击则移动单位，右键点击则沿箭头方向放置军队。\n\n";
            //std::cout<<hlp<<endl;
            MessageBox(GetHWnd(), hlp.c_str(), "游戏帮助", MB_OK);
    return;
}
void RandomPlay()//随机切歌，因为mciSendString部分命令无法正常使用
{
    if(nmusic==11)  return;//静音模式
    clock_t cl=clock();
    //if(tmpp1==100)  cout<<(tmpp1=0,cl)<<" "<<mplay<<endl;
    if(cl-mplay>MusicLen[nmusic]*1000)//放完之后
    {
        int k=rand()%5+2;
        while(k==nmusic)//不重复播放同一首歌
        {
            k=rand()%5+2;
        }
        nmusic=k;
        MusicPlay(k);
    }
    return;
}
void MusicPlay(int t)
{
    nmusic=t;
    mciSendString("stop bk",NULL,0,NULL);
    mciSendString("close bk",NULL,0,NULL);
    char sch[256];
    long long lLength;
    switch(t)
    {
        case 1:mciSendString("open ./res/脱粪の小曲.wav alias bk",NULL,0,NULL); break;
        case 2:mciSendString("open ./res/1444の小曲.wav alias bk",NULL,0,NULL); break;
        case 3:mciSendString("open ./res/1836の小曲.wav alias bk",NULL,0,NULL); break;
        case 4:mciSendString("open ./res/1936同盟国の小曲.wav alias bk",NULL,0,NULL); break;
        case 5:mciSendString("open ./res/1936苏维埃の小曲.wav alias bk",NULL,0,NULL); break;
        case 6:mciSendString("open ./res/1936洗头佬の小曲.wav alias bk",NULL,0,NULL); break;
        case 7:mciSendString("open ./res/凯南开大学の小曲.mp3 alias bk",NULL,0,NULL); break;
        case 8:mciSendString("open ./res/只因你太美の小曲.wav alias bk",NULL,0,NULL); break;
        case 9://获取音乐长度
            mciSendString("open C://PlayGameMusic/Music1.wav alias bk",NULL,0,NULL); 
            mciSendString("status bk length", sch, 255,0);
            lLength=atoi(sch);
            lLength/=1000;
            lLength+=3;
            MusicLen[9]=lLength;
            break;
        case 10:
            mciSendString("open C://PlayGameMusic/Music2.wav alias bk",NULL,0,NULL); 
            mciSendString("status bk length", sch, 255,0);
            lLength=atoi(sch);
            lLength/=1000;
            lLength+=3;
            MusicLen[10]=lLength;
            break;
        case 11:mciSendString("stop bk",NULL,0,NULL);  mciSendString("close bk",NULL,0,NULL);  break;
        default:  break;
    }
    mplay=clock();
    mciSendString("play bk",NULL,0,NULL);
}
void MusicPlayMenu()
{
    char sch[256];
    string s;
    long long lLength,len;
    int Musich,Musicm,Musics;
    IMAGE img_bk;
    loadimage(&img_bk, "./res/bk.png", WIN_W, WIN_H);
    Button* butMusic[20],*bb,*bbc;
    bb=CreateButton(WIN_W/2-30,750,60,30,"返回");
    bbc=CreateButton(WIN_W/2-30,705,60,30,"静音");
    string ch[20];
    for(int i=0;i<20;i++)
    {
        ch[i]=musicstr[i];
    }
    for(int i=0;i<=10;i++)
    {
        butMusic[i]=CreateButton(WIN_W/2-150,100+50*i,300,50,ch[i].c_str());
    }
    ExMessage msg;
    while(1)
    {
        if(nmusic!=11)  RandomPlay();
        BeginBatchDraw();
        if(peekmessage(&msg))
        {
            for(int i=1;i<=10;i++)
            {
                if(UpdataButton(msg,butMusic[i])&&i!=nmusic)
                {
                    nmusic=i;
                    MusicPlay(nmusic);
                }
            }
            if(UpdataButton(msg,bb))
            {
                return;
            }
            if(UpdataButton(msg,bbc))
            {
                MusicPlay(11);
            }
        }
        putimage(0,0,&img_bk);
        s="当前播放：";
        s+=ch[nmusic];
        settextcolor(BLACK);
        //统计长度
        mciSendString("status bk position", sch, 255,0);//目前位置
        lLength=atoi(sch);
        lLength/=1000;
        mciSendString("status bk length", sch, 255,0);//总长
        len=atoi(sch);
        len/=1000;
        lLength=len-lLength;
        s+="  ";
        if(lLength/3600>0)  Musich=lLength/3600,lLength%=3600,s+=to_string(Musich),s+="：";//该不会有一个小时的音乐吧
        if(lLength/60<10)  s+="0";
        Musicm=lLength/60;
        s+=to_string(Musicm);
        s+="：";
        lLength%=60;
        if (lLength % 60 < 10)  s += "0";
        Musics=lLength%60;
        s+=to_string(Musics);

        if(nmusic==11)  s="静音中";
        outtextxy(25,25,s.c_str());
        s="注：没有手动点歌的情况下音乐";
        outtextxy(25,475,s.c_str());
        s="为所有非玩梗乐曲随机播放。";
        outtextxy(25,500,s.c_str());
        s="若自定义音乐，将无论是否播";
        outtextxy(25,525,s.c_str());
        s="完，每6分钟切歌一次。";
        outtextxy(25,550,s.c_str());
        s="请将自定义音乐放置在";
        outtextxy(25,600,s.c_str());
        s="C:/PlayGameMusic目录下，";
        outtextxy(25,625,s.c_str());
        s="文件名称为“Music1”或者";
        outtextxy(25,650,s.c_str());
        s="“Music2”，格式为.wav"; 
        outtextxy(25,675,s.c_str());
        for(int i=0;i<=10;i++)
        {
            DrawButton(butMusic[i]);
        }
        DrawButton(bb);
        DrawButton(bbc);
        FlushBatchDraw();
    }
}

/*
音乐名单：

脱粪の小曲
1444の小曲
1836の小曲
1936同盟国の小曲
1936苏维埃の小曲
1936洗头佬の小曲
凯南开大学の小曲
只因你太美の小曲

*/

/*
游戏存档结构：
1.地图大小 n,m
2.人类和人机数量 h_p和a_p
3.20*20矩阵，ma[i][j]地图归属
4.20*20矩阵，r[i][j]，即各点处于的连通块状态
5.20*20矩阵，army[i][j]，即各点目前的军队
6.10*400矩阵，存下cash数组
7.10*400矩阵，存下q数组
8.当前回合
校验功能貌似不需要了
*/