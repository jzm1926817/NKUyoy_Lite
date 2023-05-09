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
};//���ֳ���
string musicstr[20]={
    "��ѡ�����Ӣ��",
    "�ѷ��С��",
    "1444��С��",
    "1836��С��",
    "1936ͬ�˹���С��",
    "1936��ά����С��",
    "1936ϴͷ�Ф�С��",
    "���Ͽ���ѧ��С��",
    "ֻ����̫����С��",
    "�Զ���1",
    "�Զ���2"
};//��������
string StageName[20]={
    "",
    "����1444",
    "����1836",
    "����1936",
    "NKUTJU"
};//Ԥ������

int h_p,a_p;//h_p���������,a_p:�˻�
int n=8,m=12;//Ĭ�ϵ�ͼ��С
//q=t*k��t=1~6,���ң�kÿ�����ҵĵ�k��صĸ��Ӹ���,���Ϊ0����������ͨ��ϲ�
//r��ÿ������ĸ����ҵĵڼ����
//f:ÿ�������м������
//cash:ÿ����ͨ��Ľ��
//totalmaint  ÿ����ͨ����ÿ�غ�½��ά���ѣ��غϳ�����
//nearltkill���ϻغ��Ʋ��Ĳ���
int newr[20][20],newq[7][400],newf[7],newma[20][20],f[7];
int ma[20][20],r[20][20],q[10][400];
int cash[10][400],totalmaint[10][400];
int army[20][20],nearlykill[20][20];
int fx[5]={0,-1,1,0,0};//�ƶ�
int fy[5]={0,0,0,-1,1};
int dx[10]={0,1,1,1,0,0,0,-1,-1,-1};//�Ź���
int dy[10]={0,-1,0,1,-1,0,1,-1,0,1};
int border[10];//�߽��ж�
int gamemode=1,turn=0,player=1;//�غ������ֵ�˭�˺���Ϸģʽ
int hwin=0;//˭Ӯ��
int nmusic=4;//��ǰ������Ŀ
int humanPlayerExit,LoadExit=0;
clock_t mplay;//�����Ӳ��˶����

vector<int> x;
vector<int> y;
vector<int> n0;//��¼��Ӫ�����ƶ����ӵ�����
vector<int> m0;

//��Ϸ�����ж����
void NewConj(int nx,int ny,int pnum);
void MaintCounting(int pnum);
int isOurLand(int x0,int y0,int pnum);
void ConJudge();
void CashCounting(int pnum);
void IniCanMove(int pnum);
int IsAnArmy(int x0,int y0);

//�������ж�
int GenMove(int n1,int m1,int dir);
int ConductMove(int n1,int m1,int dir);
int IsBorder(int x0,int y0,int pnum);
int ManDis(int x0,int y0,int x1,int y1);
int CanArmyBeCreated(int x0,int y0,int pnum,int level);
int ConductCreation(int x0,int y0,int pnum,int dir,int level);

//AI���
void AIMoveStep1(int pnum);
int AIMoveStep2(int x0,int y0,int pnum);
int AICreationStep2(int x0,int y0,int pnum);
int AICreation(int pnum);

//��Ϸ����
void HumanPlay(int pnum);
void RunGame();
void StartRandomizedGame();
int printChessBoard(int pnum);
void MapGeneration(int tal);
void CountF();
int WhoWin();
void Game_Start();

//���Ժ����ȫ����������
void txtprintChessBoard();
void Delay(int time);

//�˵��ȷ���Ϸ�������
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

//������
//--------BEGIN--------
int main()
{
    MusicPlay(4);
    Menu();
    return 0;
}
//---------END---------

//��������
void NewConJ(int nx,int ny,int pnum)//�������ʱ,����pnum��ռ����(nx,ny)
{
    for(int i=1;i<=n;i++)  for(int j=1;j<=m;j++)  newma[i][j]=ma[i][j];
    newma[nx][ny]=pnum;
    for(int i=1;i<=n;i++)//ö��ÿһ����
    {
        for(int j=1;j<=m;j++)
        {
            if(newma[i][j]>0)//�������ɫ
            {
                if(newma[i-1][j]==newma[i][j])//����������
                {
                    if(newq[newma[i][j]][newr[i][j]])//��������ɫ
                    {
                        newq[newma[i][j]][newr[i][j]]--;
                    }
                    newr[i][j]=newr[i-1][j];//��ɫ
                    newq[newma[i][j]][newr[i][j]]++;//��������
                    if(newma[i][j]==newma[i][j-1]&&newr[i][j]!=newr[i][j-1])//�������߶�����ԭ���ı�Ų�һ�£��漰������ͨ
                    {
                        int tmp=j-1;
                        for(int l=1;l<=n;l++)//�������������Ӵ�С���󣬱������и��ӣ�Ѱ��ԭ���ڸ���ͨ������п�
                        {
                            for(int k=1;k<=m;k++)
                            {
                                if(newma[l][k]==newma[i][j]&&newr[l][k]==newr[i][tmp])
                                {
                                    newq[newma[i][j]][newr[l][k]]--;
                                    newr[l][k]=newr[i][j];//����Ϳɫ
                                    newq[newma[i][j]][newr[i][j]]++;
                                }
                            }
                        }
                    }
                }
                else if(newma[i][j]==newma[i][j-1])//ֻ������У��������
                {
                    if(newq[newma[i][j]][newr[i][j]])
                    {
                        newq[newma[i][j]][newr[i][j]]--;
                    }
                    newr[i][j]=newr[i][j-1];
                    newq[newma[i][j]][newr[i][j]]++;
                }
                else//����û����ͨ�飬������ͨ��
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
                //ps:�Һ��²���Ҫ�ж�����Ϊ�����������ͨ��ᱻ���ϵ��µ���ͨ���ڣ�����n��m��С�����·�����ͨ�鼴�ɣ������㹻
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
int isOurLand(int x0,int y0,int pnum)//���ҵĵ���  1���� 0������
{
    if(x0<=0||y0<=0||x0>n||y0>m)  return 0;
    if(ma[x0][y0]==pnum)  return 1;
    if(ma[x0-1][y0]==pnum)  return 1;
    if(ma[x0][y0-1]==pnum)  return 1;
    if(ma[x0][y0+1]==pnum)  return 1;
    if(ma[x0-1][y0]==pnum)  return 1;
    return 0;
}
int CanArmyBeCreated(int x0,int y0,int pnum,int level)//1���� 0������
{
    if(ma[x0][y0]==-1)  return 0;
    if(ma[x0][y0]==0)  return 1;
    //if(!isOurLand(x0,y0,pnum))  return 0;//���Ϸ��ĵص�
    if(ma[x0][y0]==pnum)//���ڣ�����
    {
        if(army[x0][y0]+level<=3)  return 1;
        return 0;
    }
    else//����
    {
        if(level==3)  return 1;//3�������ǿ��Ա�����
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
int ConductCreation(int x0,int y0,int pnum,int dir,int level)//1���� 0����
{
    int nx=x0+fx[dir],ny=y0+fy[dir];
    int flag1=0,maxx=0,maxnum[5]={0};
    if(!CanArmyBeCreated(nx,ny,pnum,level))  return 0;
    if(cash[pnum][r[x0][y0]]<level*10)  return 0;
    cash[pnum][r[x0][y0]]-=level*10;
    int t=0,k=0;
    if(ma[nx][ny]==pnum)//���������漰������ͨ���
    {
        army[nx][ny]+=level;
    }
    else//���⣬�ض����
    {
        NewConJ(nx,ny,pnum);
        t=0,k=0;
        flag1=0,maxx=0;
        
        if(newf[ma[nx][ny]]>f[ma[nx][ny]])//���ֽض�
        {
            /*
            * �ضϣ�ֱ���ж����������ĸ��µ�q�ĸ����󣬾Ͱ�ԭ�н�Ҹ����µ��Ǹ�����������һ�������
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
                t=rand()%flag1+1;//�������
                k=cash[ma[nx][ny]][r[nx][ny]];
                cash[ma[nx][ny]][r[nx][ny]]=0;
            }
            else  cash[ma[nx][ny]][r[nx][ny]]=0;
            //cash[ma[nx][ny]][r[nx][ny]]=0;
        }
        //if(newf[ma[nx][ny]]<f[ma[nx][ny]])//����������ͨ�飬ò�Ʋ���Ҫ����
        //{
        //cash[ma[nx][ny]][r[nx][ny]]=0;
        //}
        q[ma[nx][ny]][r[nx][ny]]--;
        army[nx][ny]=level;
        MaintCounting(pnum);//�Ʒ�
        ma[nx][ny]=pnum;
        ConJudge();
        if(t)  
        {
            cash[ma[nx+fx[maxnum[t]]][ny+fy[maxnum[t]]]][r[nx+fx[maxnum[t]]][ny+fy[maxnum[t]]]]=k;
        }
    }
    //���¼����Ƿ���ͨ���Ƿ�ضϣ����·�����
    return 1;
}

void ConJudge()//���鼯�ж���ͨ��
{
    memset(f,0,sizeof(f));
    for(int i=1;i<=n;i++)//ö��ÿһ����
    {
        for(int j=1;j<=m;j++)
        {
            if(ma[i][j]>0)//�������ɫ
            {
                if(ma[i-1][j]==ma[i][j])//����������
                {
                    if(q[ma[i][j]][r[i][j]])//��������ɫ
                    {
                        if(q[ma[i][j]][r[i][j]]==1)//���һ����
                        {
                            cash[ma[i-1][j]][r[i-1][j]]+=cash[ma[i][j]][r[i][j]];
                        }
                        q[ma[i][j]][r[i][j]]--;
                    }
                    r[i][j]=r[i-1][j];//��ɫ
                    q[ma[i][j]][r[i][j]]++;//��������
                    if(ma[i][j]==ma[i][j-1]&&r[i][j]!=r[i][j-1])//�������߶�����ԭ���ı�Ų�һ�£��漰������ͨ
                    {
                        cash[ma[i][j]][r[i][j]]+=cash[ma[i][j]][r[i][j-1]];//���½��
                        int tmp=j-1;
                        for(int l=1;l<=n;l++)//�������������Ӵ�С���󣬱������и��ӣ�Ѱ��ԭ���ڸ���ͨ������п�
                        {
                            for(int k=1;k<=m;k++)
                            {
                                if(ma[l][k]==ma[i][j]&&r[l][k]==r[i][tmp])
                                {
                                    q[ma[i][j]][r[l][k]]--;
                                    r[l][k]=r[i][j];//����Ϳɫ
                                    q[ma[i][j]][r[i][j]]++;
                                }
                            }
                        }
                    }
                }
                else if(ma[i][j]==ma[i][j-1])//ֻ������У��������
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
                else//����û����ͨ�飬������ͨ��
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
                //ps:�Һ��²���Ҫ�ж�����Ϊ�����������ͨ��ᱻ���ϵ��µ���ͨ���ڣ�����n��m��С�����·�����ͨ�鼴�ɣ������㹻
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

void MaintCounting(int pnum)//���½��,num:��k������
{
    int g=0,t=pnum;
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
void CashCounting(int pnum)//��Ǯ
{
    ConJudge();
    MaintCounting(pnum);
    int t=pnum;
    for(int i=1;i<=200;i++)//����������Ʋ�����
    {
        if(q[t][i])//����Ϸ���Ľ��
        {
            cash[t][i]+=(q[t][i]-totalmaint[t][i]);
            if(cash[t][i]<0)//�Ʋ�
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

void IniCanMove(int pnum)//ĳһ��Ӫ�����ƶ��ı��ĳ�ʼ��
{
    n0.clear();
    m0.clear();
    for(int i=1;i<=n;i++)  for(int j=1;j<=m;j++)  if(ma[i][j]==pnum&&army[i][j]!=0)  n0.push_back(i),m0.push_back(j);
}
int IsAnArmy(int x0,int y0)//�Ƿ���һ������x0,y0���ǵĻ�����λ�ã����򷵻�-1
{
    int flag=-1;
    for(int i=0;i<n0.size();i++)
    {
        if(n0[i]==x0&&m0[i]==y0)  return i;
    }
    return flag;
}
int GenMove(int n1,int m1,int dir)//�ƶ��Ƿ�Ϸ������Ϸ�����1�����򷵻�0
{
    int test0;
    test0=IsAnArmy(n1,m1);
    if(IsAnArmy(n1,m1)==-1)  return 1;
    int nx=n1+fx[dir],ny=m1+fy[dir];
    if(nx<=0||ny<=0||nx>n||ny>m)  return 1;//Խ��
    if(ma[nx][ny]==-1)  return 1;//Խ��
    if(ma[nx][ny]==0)  return 0;//������Σ��׵ؿ����ƶ�
    int tx,ty;
    if(ma[nx][ny]!=ma[n1][m1])
    {
        for(int i=1;i<=9;i++)//�жϸ����Ƿ�����
        {
            tx=nx+dx[i],ty=ny+dy[i];
            if(army[n1][m1]==3)  return 0;
            if(ma[nx][ny]==ma[tx][ty]&&army[n1][m1]<=army[tx][ty])  return 1;
        }
    }
    else
    {
        if(army[n1][m1]+army[nx][ny]>=4)  return 1;//����4����
    }
    return 0;
}
int ConductMove(int n1,int m1,int dir)//�����ƶ�
{
    int kk=GenMove(n1,m1,dir);
    if(GenMove(n1,m1,dir))  return -1;
    int nx=n1+fx[dir],ny=m1+fy[dir];
    if(ma[nx][ny]==0)//������Σ��׵ؿ����ƶ�
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
        if(newf[ma[nx][ny]]>f[ma[nx][ny]])//���ֽض�
        {
            //�ضϣ�ֱ���ж����������ĸ��µ�q�ĸ����󣬾Ͱ�ԭ�н�Ҹ����µ��Ǹ�����������һ�������

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
                t=rand()%flag1+1;//�������
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
            //�뷨������MoneyAndConJud�е�Conjudge���������βΣ���΢�޸Ĵ��뼴�ɣ����ж�f[ma[i][j]]�������һ�£����ı��¿�����ж�q�����ֵ�����½��}
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
int IsBorder(int x0,int y0,int pnum)//�ж��Ƿ�Ϊ�߽��ߣ�����ǣ�����һ�����������뼺����ɫ��ͬ�ĸ��ӵĸ���
{
    int flag=0;
    for(int i=1;i<=4;i++)
    {
        if(ma[x0+fx[i]][y0+fy[i]]!=pnum&&ma[x0+fx[i]][y0+fy[i]]!=-1)  border[++flag]=i;
    }
    return flag;
}
int ManDis(int x0,int y0,int x1,int y1){return abs(x0-x1)+abs(y0-y1);}//�����پ���

void AIMoveStep1(int pnum)//AI�ƶ�
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
int AIMoveStep2(int x0,int y0,int pnum)//�����ƶ���Ȩֵ,��ִ���ƶ�
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

int AICreationStep2(int x0,int y0,int pnum)//�ж�����Ŀ�����
{
    if(ma[x0][y0]!=pnum)  return 0;
    if(army[x0][y0]==3)  return 0;
    if(cash[pnum][r[x0][y0]]<=10)  return 0;//���
    if(cash[pnum][r[x0][y0]]>10)//Ǯ�ù�
    {
        if(cash[pnum][r[x0][y0]]<=20)//����21�飬ֻ����1��
        {
            if(totalmaint[pnum][r[x0][y0]]+2<q[pnum][r[x0][y0]])//�������֧��
            {
                int t;
                t=rand()%5;
                if(t<=2)  army[x0][y0]+=1,cash[pnum][r[x0][y0]]-=7;
            }
            else
            {
                if(3*(totalmaint[pnum][r[x0][y0]]+2)>=cash[pnum][r[x0][y0]]);//��֧��ƽ��
                else//�볬,��1��
                {
                    int t=rand()%5;
                    if(t<=2)  army[x0][y0]+=1,cash[pnum][r[x0][y0]]-=7;

                }  
            }
        }
        else
        {
            if(totalmaint[pnum][r[x0][y0]]+2<q[pnum][r[x0][y0]])//�������֧��
            {
                int t;
                if(totalmaint[pnum][r[x0][y0]]+4>q[pnum][r[x0][y0]])//Ǯ������3��
                {
                    t=rand()%5;
                    if(t<=2)  army[x0][y0]+=1,cash[pnum][r[x0][y0]]-=7;
                }
                else//Ǯ��3��
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
            else//��֧�����
            {
                if(3*(totalmaint[pnum][r[x0][y0]]+2)>=cash[pnum][r[x0][y0]]-10);//���
                else if(3*(totalmaint[pnum][r[x0][y0]]+4)>=cash[pnum][r[x0][y0]]-10)//����,����ֻ�����1��
                {
                    int t=rand()%5;
                    if(t<=2)  army[x0][y0]+=1,cash[pnum][r[x0][y0]]-=7;
                }  
                else//����v50
                {
                    int t=rand()%9;
                    if(t<=2)//0,1,2��1��
                    {
                        army[x0][y0]+=1,cash[pnum][r[x0][y0]]-=7;
                    }
                    if(t>=6)//6��7��8��9��2��
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
int AICreation(int pnum)//ѡַ���
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
        tt=rand()%2;//һ��������
        if(!tt)  AICreationStep2(x[i],y[i],pnum);
    }
    return 0;
}

void Delay(int time)//�ӳ���� time=���루ǧ��֮һ�룩
{ 
    clock_t now=clock(); 
    while(clock()-now<time); 
    return;
} 
void HumanPlay(int pnum)//������ҽ���
{
    humanPlayerExit=0;
    int flag = 0, x0, y0, d, r=0, num = 0;
    bool buttonIsDown[2] = { 0 };
    int _x, _y;
    int curgrade = 1;
    Button* but1 = CreateButton(250, 10, 100, 30, "����1");
    Button* but2 = CreateButton(400, 10, 100, 30, "����2");
    Button* but3 = CreateButton(550, 10, 100, 30, "����3");

    Button* butContinue = CreateButton(0, WIN_H-150, 100, 25, "��һ�غ�");
    Button* butSave = CreateButton(0, WIN_H-100, 100, 25, "����");
    Button* butMusic = CreateButton(0, WIN_H-200, 100, 25, "���ֲ�����");
    Button* bbc=CreateButton(0,WIN_H-50,100,25,"ֱ���˳�");
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
            //������ڵ�ͼ����
            if (msg.x > 100 && msg.x < 100 + IMG_W * m && msg.y>80 && msg.y < 80 + IMG_H * n) {
                if (num == 0) {
                    if (msg.lbutton && buttonIsDown[0] == 0) {   //�������  ѡ���ƶ�ʿ��λ��
                        buttonIsDown[0] = 1;
                        flag = 2;
                        x0 = (msg.y - 80) / IMG_H + 1;
                        y0 = (msg.x - 100) / IMG_W + 1;
                        num = 1;
                    }
                    else if (!msg.lbutton) {
                        buttonIsDown[0] = 0;
                    }
                    if (msg.rbutton && buttonIsDown[1] == 0) {   //�Ҽ�����  ѡ�����ʿ��λ��
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
                                MessageBox(GetHWnd(), "����ʧ�ܣ�����", "��Ϸ��Ϣ", MB_OK);
                        }
                        else if (flag == 2) {
                            r = ConductMove(x0, y0, d);
                            if (r!=0)
                                MessageBox(GetHWnd(), "�ƶ�ʧ�ܣ�����", "��Ϸ��Ϣ", MB_OK);
                        }
                        num = 0;
                    }
                    else if (_x == 0 && _y == 1) {
                        d = 2;
                        if (flag == 1) {
                            r = ConductCreation(x0, y0, pnum, d, curgrade);
                            if (r==0)
                                MessageBox(GetHWnd(), "����ʧ�ܣ�����", "��Ϸ��Ϣ", MB_OK);
                        }
                        else if (flag == 2) {
                            r = ConductMove(x0, y0, d);
                            if (r!=0)
                                MessageBox(GetHWnd(), "�ƶ�ʧ�ܣ�����", "��Ϸ��Ϣ", MB_OK);
                        }
                        num = 0;
                    }
                    else if (_x == -1 && _y == 0) {
                        d = 3;
                        if (flag == 1) {
                            r = ConductCreation(x0, y0, pnum, d, curgrade);
                            if (r==0)
                                MessageBox(GetHWnd(), "����ʧ�ܣ�����", "��Ϸ��Ϣ", MB_OK);
                        }
                        else if (flag == 2) {
                            r = ConductMove(x0, y0, d);
                            if (r!=0)
                                MessageBox(GetHWnd(), "�ƶ�ʧ�ܣ�����", "��Ϸ��Ϣ", MB_OK);
                        }
                        num = 0;
                    }
                    else if (_x == 1 && _y == 0) {
                        d = 4;
                        if (flag == 1) {
                            r = ConductCreation(x0, y0, pnum, d, curgrade);
                            if (r==0)
                                MessageBox(GetHWnd(), "����ʧ�ܣ�����", "��Ϸ��Ϣ", MB_OK);
                        }
                        else if (flag == 2) {
                            r = ConductMove(x0, y0, d);
                            if (r!=0)
                                MessageBox(GetHWnd(), "�ƶ�ʧ�ܣ�����", "��Ϸ��Ϣ", MB_OK);
                        }
                        num = 0;
                    }
                    else if (_x == 0 && _y == 0) {
                        if (flag == 1) {
                            r = ConductCreation(x0, y0, pnum, d, curgrade);
                            if (r==0)
                                MessageBox(GetHWnd(), "����ʧ�ܣ�����", "��Ϸ��Ϣ", MB_OK);
                            //if (r)  std::cout << "�ѽ���" << endl;
                            //else  std::cout << "����ʧ�ܣ�����" << endl;
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
            rectangle(100 + (y0 - 1) * IMG_W, 80 + (x0 - 1) * IMG_H, 100 + (y0)*IMG_W, 80 + (x0)*IMG_H);        //����ѡ��
            //�ϼ�ͷ
            line(x + IMG_W / 2, y - IMG_H / 2, x + 5, y - 5);
            line(x + IMG_W / 2, y - IMG_H / 2, x + IMG_W - 5, y - 5);
            //�¼�ͷ
            line(x + IMG_W / 2, y + IMG_H / 2 * 3, x + 5, y + IMG_H + 5);
            line(x + IMG_W / 2, y + IMG_H / 2 * 3, x + IMG_W - 5, y + IMG_H + 5);
            //���ͷ
            line(x - IMG_W / 2, y + IMG_H / 2, x - 5, y + 5);
            line(x - IMG_W / 2, y + IMG_H / 2, x - 5, y +IMG_H -5);
            //�Ҽ�ͷ
            line(x + IMG_W / 2 * 3, y + IMG_H / 2, x + IMG_W + 5, y + 5);
            line(x + IMG_W / 2 * 3, y + IMG_H / 2, x + IMG_W + 5, y + IMG_H - 5);
        }
        settextstyle(20, 0, "����");
        sprintf(s, "Ҫ���õı���%d", curgrade);
        outtextxy(102, 10, s);
        if (flag == 1) {
            outtextxy(700, 10, "���ñ���");
        }
        else if(flag == 2) {
            outtextxy(700, 10, "�ƶ�ʿ��");
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
void RunGame()//��Ϸ����
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
            //txtprintChessBoard();///�������
            //system("pause");
            CashCounting(i);
            IniCanMove(i);
            AIMoveStep1(i);
            AICreation(i);
            player++;
        }
        player=1;
        if(!flag1)//û�л��ŵ������
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
    bb=CreateButton(WIN_W/2-30,640,60,30,"ȷ��");
    bbc=CreateButton(WIN_W/2-30,685,60,30,"ȡ��");
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
                    MessageBox(GetHWnd(), "������������Ƿ����Ҫ��", "��Ϸ��Ϣ", MB_OK);
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
        outtextxy(30,30, "������Ҹ���");
        outtextxy(30,205, "������Ҹ���");
        outtextxy(15,325, "���������������Ϊ0�����������������Ϊ0��");
        outtextxy(15,350,"�������������������������ܺͱ�����2��6֮��");
        outtextxy(15,375, "��ͼ�����С");
        outtextxy(15,465, "��ͼ�����С");
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

int printChessBoard(int pnum)//��ӡ����
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
    settextstyle(15, 0, "����");
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
    
    settextstyle(12, 0, "����");
    settextcolor(BLACK);
    outtextxy(2, 80, "�������ƶ��ľ���");
    char s[128];
    settextstyle(15, 0, "����");
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
    settextstyle(15, 0, "����");
    sprintf(s, "��ǰ���:%d", pnum);
    outtextxy(2, 2, s);
    return 0;
}

void txtprintChessBoard()//������
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

void MapGeneration(int tal)//����tal������
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
    //�������ɵ�ͼ
    //�������ɹ���
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

    Button* but1 = CreateButton(WIN_W / 2 - 150, 150, 300, 40, "�����ͼ��ʼ��Ϸ");
    Button* but2 = CreateButton(WIN_W / 2 - 150, 200, 300, 40, "��ȡ��Ϸ�浵������Ϸ");
    Button* but3 = CreateButton(WIN_W / 2 - 150, 250, 300, 40, "��ȡ��ͼ�浵������Ϸ");
    Button* but4 = CreateButton(WIN_W / 2 - 150, 300, 300, 40, "�༭��");
    Button* but5 = CreateButton(WIN_W / 2 - 150, 350, 300, 40, "��Ϸ˵��");
    Button* butMusic = CreateButton(WIN_W / 2 - 150, 400, 300, 40, "���ֲ�����");
    Button* but6 = CreateButton(WIN_W / 2 - 150, 450, 300, 40, "�˳�");
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
        std::cout<<"1:�����ͼ��ʼ��Ϸ\n2:��ȡ��Ϸ�浵������Ϸ\n3:��ȡ��ͼ�浵������Ϸ\n4:�༭��\n5:��Ϸ˵��"<<endl
            <<"9.�˳�"<<endl;
        cin>>ch;
        switch(ch)
        {
        case 1:
            gamemode=1;
            std::cout<<"�����ͼ��Сn*m:0<n,m<=18"<<endl;
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
            std::cout<<"��������"<<endl;
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
        ostringstream ostr{};   // ����һ������ַ�������������Ϊ��
        ostr << rt;
        string nums="p";
        nums += ostr.str(); 
        nums+=((rt>h_p)?" (�˻�)":" (�������)");
        nums+="  ��ʤ����";
        LPCSTR res=nums.c_str();
        MessageBox(GetHWnd(),res,"��Ϸ������",MB_OK);
        //std::cout<<"��Ϸ������"<<endl;
        //std::cout<<"p"<<rt<<((rt>h_p)?" (�˻�)":" (�������)")<<" ��ʤ����"<<endl;
        //system("pause");
    }
    else
    {
        if(humanPlayerExit)  MessageBox(GetHWnd(),"��������˳���","��Ϸ��ʾ",MB_OK);
        else if(LoadExit);
        else  MessageBox(GetHWnd(),"��ͼδ��ȷ���أ����ܵ�ԭ���У��ô浵����ȷ���ô浵���𻵻�༭���������Ĵ浵����ȷ��","��Ϸ��ʾ",MB_OK);
    }
    return;
}

int printEditChessBoard()//��ӡ����
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
    bb=CreateButton(WIN_W/2-30,640,60,30,"ȷ��");
    bbc=CreateButton(WIN_W/2-30,685,60,30,"ȡ��");
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
                    MessageBox(GetHWnd(), "������������Ƿ����Ҫ��", "��Ϸ��Ϣ", MB_OK);
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
        outtextxy(30,30, "������Ҹ���");
        outtextxy(30,205, "������Ҹ���");
        outtextxy(15,325, "���������������Ϊ0�����������������Ϊ0��");
        outtextxy(15,350,"�������������������������ܺͱ�����2��6֮��");
        outtextxy(15,375, "��ͼ�����С");
        outtextxy(15,465, "��ͼ�����С");
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
void Edit()//�༭��
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
    Button* butRandom=CreateButton(5, WIN_H-250, 85, 25, "�������");
    Button* butLoad=CreateButton(5, WIN_H-210, 85, 25, "��ȡ��ͼ");
    Button* butAdjust=CreateButton(5, WIN_H-170,85, 25, "������ͼ");
    Button* butSave=CreateButton(5, WIN_H-130, 85, 25, "�����ͼ");
    Button* butReturn=CreateButton(5, WIN_H-90,85, 25, "ֱ���˳�");
    Button* butMusic = CreateButton(5, WIN_H - 50, 85, 25,"���ֲ���");
    string ch,ch2;
    for(int i=0;i<=7;i++)
    {
        ch=to_string(i-1);
        butcolor[i]=CreateButton(10,10+50*i,30,30,ch.c_str());
    }
    for(int i=0;i<=3;i++)
    {
        ch="���ֵȼ�"+to_string(i);
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
                    if (i - 1 > h_p + a_p) MessageBox(GetHWnd(), "ɫ�Ŵ������������", "��Ϸ��Ϣ", MB_OK);
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
            //������ڵ�ͼ����
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
        settextstyle(20, 0, "����");
        sprintf(s, "���ֵȼ��� %d", curgrade);
        outtextxy(100, 10, s);
        sprintf(s, "��ǰ��ɫ��ţ� %d", curcolor);
        outtextxy(100,40,s);
        outtextxy(300,40,"��ɫ��");
        sprintf(s,"����%d",n);
        outtextxy(5,400,s);
        sprintf(s,"����%d",m);
        outtextxy(5,440,s);
        sprintf(s,"��ң�%d",h_p);
        outtextxy(5,480,s);
        sprintf(s,"���ԣ�%d",a_p);
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
    MessageBox(GetHWnd(), "��ע�⣬��ԭ��ͼ�Ĵ�С�����������µ�ͼ��С���Ὣ��������ֱ��Ĩȥ��", "��ʾ", MB_OK);
    int n0=n,m0=m;
    int hp=h_p,ap=a_p;
    IMAGE img_bk;
    loadimage(&img_bk, "./res/bk.png", WIN_W, WIN_H);
    Button* but[7], *But[7],*bb,*bbc;
    Button *mpx[16],*mpy[16];
    bb=CreateButton(WIN_W/2-30,600,60,30,"ȷ��");
    bbc=CreateButton(WIN_W/2-30,640,60,30,"ȡ��");
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
                    MessageBox(GetHWnd(), "������������Ƿ����Ҫ��", "��Ϸ��Ϣ", MB_OK);
                }
            } 
        }
        string ch1,ch2,ch3,ch4;
        ch1=char(h_p+'0'),ch2=char(a_p+'0');
        ch3=to_string(n),ch4=to_string(m);
        putimage(0,0,&img_bk);
        settextcolor(BLACK);
        outtextxy(30,30, "������Ҹ���");
        outtextxy(30,205, "������Ҹ���");
        outtextxy(15,325, "���������������Ϊ0�����������������Ϊ0��");
        outtextxy(15,350,"�������������������������ܺͱ�����2��6֮��");
        outtextxy(15,375, "��ͼ�����С");
        outtextxy(15,465, "��ͼ�����С");
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
    bb=CreateButton(WIN_W/2-30,640,60,30,"ȷ��");
    Button *bbc;
    bbc=CreateButton(WIN_W/2-30,685,60,30,"ȡ��");
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
        string ch="��ǰ��ۣ�";
        ch+=to_string(savslot);
        putimage(0,0,&img_bk);
        settextcolor(BLACK);
        outtextxy(30,30, "��ѡ��浵���");
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
    bb=CreateButton(WIN_W/2-30,640,60,30,"ȷ��");
    Button *bbc;
    bbc=CreateButton(WIN_W/2-30,685,60,30,"ȡ��");
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
        string ch="��ǰ��ۣ�";
        ch+=to_string(savslot);
        putimage(0,0,&img_bk);
        settextcolor(BLACK);
        outtextxy(30,30, "��ѡ��浵���");
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
    bb=CreateButton(WIN_W/2-30,640,60,30,"ȷ��");
    Button *bbc;
    bbc=CreateButton(WIN_W/2-30,685,60,30,"ȡ��");
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
            ch = "��ǰѡ�񣺴浵���";
            ch += to_string(savslot);
        }
        else
        {
            ch="��ǰѡ��Ԥ�衰";
            ch+=StageName[savslot-10];
            ch+="��";
        }
        putimage(0,0,&img_bk);
        settextcolor(BLACK);
        outtextxy(30,30, "��ѡ��浵");
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
    bb=CreateButton(WIN_W/2-30,640,60,30,"ȷ��");
    Button *bbc;
    bbc=CreateButton(WIN_W/2-30,685,60,30,"ȡ��");
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
        string ch="��ǰ��ۣ�";
        ch+=to_string(savslot);
        putimage(0,0,&img_bk);
        settextcolor(BLACK);
        outtextxy(30,30, "��ѡ��浵���");
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
    MessageBox(GetHWnd(), "��ɣ�", "��Ϸ��Ϣ", MB_OK);
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
    MessageBox(GetHWnd(), "��ɣ�", "��Ϸ��Ϣ", MB_OK);
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
    MessageBox(GetHWnd(), "��ɣ�", "��Ϸ��Ϣ", MB_OK);
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
    MessageBox(GetHWnd(), "��ɣ�", "��Ϸ��Ϣ", MB_OK);
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
    MessageBox(GetHWnd(), "��ɣ�", "��Ϸ��Ϣ", MB_OK);
}

void Help()
{
    string hlp;
    
       hlp+= "�����Դ��yiotro-antiyoy��ʵ������ǰ��slay��\n";
       hlp+="github:  https://github.com/yiotro/Antiyoy\n\n";
       hlp+="��Ϸ����\n";
       hlp+="1.��ͬ��ɫ����ͬ���ҵĹ�������ʤ����Ϊ�������й��ҵĹ���������Ϊ0��\n";
           hlp+="2.���ַ�Ϊ������1���������ֱ���2������̹�ˣ�3�������ֱ��֣�����ֻ��ռ�츽��û�о������ص����أ�\n";
           hlp+="���ֱ�����ռ�츽���в������������ص����أ�ͬʱ�������𲽱���̹�˿����ߵ������κ�һ�����Ӳ���Ӱ�죬��ʹ������̹�ˡ�\n";
           hlp+="3.ÿ�غϣ�ÿ���������ھ����ƶ�2�񣬻�������һ����������ƶ���������������ƶ�1�񵽴�߾����������������ƶ���\n";
           hlp+="4.��ÿ����ƶ���ʽΪ���������ƶ�һ��\n";
           hlp+="5.ÿ�غϣ�ӵ�е�ÿ��ؿ��ṩ1��ң���ļ���ӣ�10��20��30����ÿ�غ�ά�־��ӣ�2��4��7������Ҫһ�����Ľ�ҡ�\n";
           hlp+="6.�������ߵ�������������һ�����������ϣ����������о��ӽ�����������������Ϊ���¾��ӵȼ�Ϊ����֮�ͣ�������3����������\n";
           hlp+="�������ƶ����ø��ӡ����ø��ӵľ��ӱ��غ�δ�ƶ�������������ľ�����Ȼ���Խ����ƶ���\n";
           hlp+="7.һ������פ������Χ�˸���ͬ�������ڸ��9���У��������Ƶĸ�����Ϊ�������ص����ء�\n";
           hlp+="8.����������Լ��ľ��ڣ��������غϻ�õ����أ������ٽ�1����У����������о����Ͻ��У��϶�Ϊ������\n";
           hlp+="���غ���ı��������ƶ���ֱ�������о������������⡣\n";
           hlp+="9.��һ������ӵ�г���һ����أ���ͬ��صĹ���ֿ����㣬��������غ϶�Ϊһʱ���ϲ���������صĹ��⣬\n";
           hlp+="��һ����ر��ض�ʱ������������������нϴ��һ�飬���һ������������䡣\n";
           hlp+="10.��ĳ����ع����Ǯ������ά������ʱ�����о���ֱ����ʧ������ж����Լ��ĻغϿ�ʼǰ��\n";
           hlp+="11.�����޻�\n\n";

        hlp+="��Ϸģʽ��\n";
            hlp+="1.��������1~5��ai������Ϸ������ai��ˮƽ�ϵͣ�\n";
            hlp+="2.���ˣ�2~6��һ̨�豸���������в���������ѡ����λ�Ƿ���ai���룻\n\n";

            hlp+="��ͼģʽ��\n";
            hlp+="1.�����ͼ��������������ɵ�ͼ��\n";
            hlp+="2.�༭�����������б༭��ͼ��������Ҫ��֤��ͼ��ͨ���ͻ��˲����ж����������ͼ���á�\n";
            hlp+="3.Ԥ�裺����Ϸ������Ԥ��ļ��ŵ�ͼ�����ǲ�ȷ��ƽ���ԡ����滮�У�\n\n";
        
        hlp+="��Ϸ������\n";
            hlp += "������Ҽ�������ӣ����������ƶ���λ���Ҽ�������ؼ�ͷ������þ��ӡ�\n\n";
            //std::cout<<hlp<<endl;
            MessageBox(GetHWnd(), hlp.c_str(), "��Ϸ����", MB_OK);
    return;
}
void RandomPlay()//����и裬��ΪmciSendString���������޷�����ʹ��
{
    if(nmusic==11)  return;//����ģʽ
    clock_t cl=clock();
    //if(tmpp1==100)  cout<<(tmpp1=0,cl)<<" "<<mplay<<endl;
    if(cl-mplay>MusicLen[nmusic]*1000)//����֮��
    {
        int k=rand()%5+2;
        while(k==nmusic)//���ظ�����ͬһ�׸�
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
        case 1:mciSendString("open ./res/�ѷ��С��.wav alias bk",NULL,0,NULL); break;
        case 2:mciSendString("open ./res/1444��С��.wav alias bk",NULL,0,NULL); break;
        case 3:mciSendString("open ./res/1836��С��.wav alias bk",NULL,0,NULL); break;
        case 4:mciSendString("open ./res/1936ͬ�˹���С��.wav alias bk",NULL,0,NULL); break;
        case 5:mciSendString("open ./res/1936��ά����С��.wav alias bk",NULL,0,NULL); break;
        case 6:mciSendString("open ./res/1936ϴͷ�Ф�С��.wav alias bk",NULL,0,NULL); break;
        case 7:mciSendString("open ./res/���Ͽ���ѧ��С��.mp3 alias bk",NULL,0,NULL); break;
        case 8:mciSendString("open ./res/ֻ����̫����С��.wav alias bk",NULL,0,NULL); break;
        case 9://��ȡ���ֳ���
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
    bb=CreateButton(WIN_W/2-30,750,60,30,"����");
    bbc=CreateButton(WIN_W/2-30,705,60,30,"����");
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
        s="��ǰ���ţ�";
        s+=ch[nmusic];
        settextcolor(BLACK);
        //ͳ�Ƴ���
        mciSendString("status bk position", sch, 255,0);//Ŀǰλ��
        lLength=atoi(sch);
        lLength/=1000;
        mciSendString("status bk length", sch, 255,0);//�ܳ�
        len=atoi(sch);
        len/=1000;
        lLength=len-lLength;
        s+="  ";
        if(lLength/3600>0)  Musich=lLength/3600,lLength%=3600,s+=to_string(Musich),s+="��";//�ò�����һ��Сʱ�����ְ�
        if(lLength/60<10)  s+="0";
        Musicm=lLength/60;
        s+=to_string(Musicm);
        s+="��";
        lLength%=60;
        if (lLength % 60 < 10)  s += "0";
        Musics=lLength%60;
        s+=to_string(Musics);

        if(nmusic==11)  s="������";
        outtextxy(25,25,s.c_str());
        s="ע��û���ֶ��������������";
        outtextxy(25,475,s.c_str());
        s="Ϊ���з��湣����������š�";
        outtextxy(25,500,s.c_str());
        s="���Զ������֣��������Ƿ�";
        outtextxy(25,525,s.c_str());
        s="�꣬ÿ6�����и�һ�Ρ�";
        outtextxy(25,550,s.c_str());
        s="�뽫�Զ������ַ�����";
        outtextxy(25,600,s.c_str());
        s="C:/PlayGameMusicĿ¼�£�";
        outtextxy(25,625,s.c_str());
        s="�ļ�����Ϊ��Music1������";
        outtextxy(25,650,s.c_str());
        s="��Music2������ʽΪ.wav"; 
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
����������

�ѷ��С��
1444��С��
1836��С��
1936ͬ�˹���С��
1936��ά����С��
1936ϴͷ�Ф�С��
���Ͽ���ѧ��С��
ֻ����̫����С��

*/

/*
��Ϸ�浵�ṹ��
1.��ͼ��С n,m
2.������˻����� h_p��a_p
3.20*20����ma[i][j]��ͼ����
4.20*20����r[i][j]�������㴦�ڵ���ͨ��״̬
5.20*20����army[i][j]��������Ŀǰ�ľ���
6.10*400���󣬴���cash����
7.10*400���󣬴���q����
8.��ǰ�غ�
У�鹦��ò�Ʋ���Ҫ��
*/