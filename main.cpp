#include"common.h"
#include"UI.h"
#include"tools.h"

using namespace std;

IMAGE img_army[3];
extern COLORREF _Colors[10];


int h_p,a_p;//h_p���������,a_p:�˻�
int n=7,m=12;//Ĭ�ϵ�ͼ��С
//q=t*k��t=1~6,���ң�kÿ�����ҵĵ�k��صĸ��Ӹ���,���Ϊ0������������ͨ��ϲ�
//r��ÿ������ĸ����ҵĵڼ����
//f:ÿ�������м������
//cash:ÿ����ͨ��Ľ��
//totalmaint  ÿ����ͨ����ÿ�غ�½��ά���ѣ��غϳ�����
//nearltkill���ϻغ��Ʋ��Ĳ���
int newr[20][20],newq[7][400],newf[7],newma[20][20],f[7];
int ma[20][20],r[20][20],q[7][400];
int cash[10][400],totalmaint[10][400];
int army[20][20],nearlykill[20][20];
int fx[5]={0,-1,1,0,0};//�ƶ�
int fy[5]={0,0,0,-1,1};
int dx[10]={0,1,1,1,0,0,0,-1,-1,-1};//�Ź���
int dy[10]={0,-1,0,1,-1,0,1,-1,0,1};
int border[10];//�߽��ж�
int gamemode=1,turn=0;//�غ�������Ϸģʽ

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
void Map_Save();
void Game_Save();
void Map_Load();
void Menu();
void Help();


//������
//--------BEGIN--------
int main()
{
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
    if(!isOurLand(x0,y0,pnum))  return 0;//���Ϸ��ĵص�
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
                if(ma[nx][ny]==ma[x0][y0]&&army[x0][y0]>=level)  return 0;
            }            
        }
        return 1;
    }
}
int ConductCreation(int x0,int y0,int pnum,int dir,int level)//1���� 0����
{
    int nx=x0+fx[dir],ny=y0+fy[dir];
    int flag1=0,maxx=0,maxnum[5]={0};
    if(!isOurLand(nx,ny,pnum))  return 0;
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
        q[ma[nx][ny]][r[nx][ny]]--;
        army[nx][ny]=level;
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
            }
            //cash[ma[nx][ny]][r[nx][ny]]=0;
        }
        //if(newf[ma[nx][ny]]<f[ma[nx][ny]])//����������ͨ�飬ò�Ʋ���Ҫ����
        //{
        //cash[ma[nx][ny]][r[nx][ny]]=0;
        //}
        MaintCounting(pnum);//�Ʒ�
        cash[ma[nx][ny]][r[nx][ny]]=0;
        ma[nx][ny]=pnum;
        ConJudge();
        if(t)  cash[ma[nx+fx[maxnum[t]]][ny+fy[maxnum[t]]]][r[nx+fx[maxnum[t]]][ny+fy[maxnum[t]]]]+=k;
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
    for(int i=1;i<=n;i++)  for(int j=1;j<=n;j++)  if(ma[i][j]==pnum&&army[i][j])  n0.push_back(i),m0.push_back(j);
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
            if(ma[nx][ny]==ma[tx][ty]&&army[n1][m1]<army[tx][ty])  return 1;
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
        for(int i=1;i<=9;i++)
        {
            if(army[n1][m1]==3)
            {break;}
        }        
        army[nx][ny]=army[n1][m1];
        army[n1][m1]=0;
        ma[nx][ny]=ma[n1][m1];
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
            }
        }
        if(t)
        {
            cash[ma[nx][ny]][r[nx][ny]]=0;
            ma[nx][ny]=ma[n1][m1];
            ConJudge();
            cash[ma[nx+fx[maxnum[t]]][ny+fy[maxnum[t]]]][r[nx+fx[maxnum[t]]][ny+fy[maxnum[t]]]]+=k;
            //�뷨������MoneyAndConJud�е�Conjudge���������βΣ���΢�޸Ĵ��뼴�ɣ����ж�f[ma[i][j]]�������һ�£����ı��¿�����ж�q�����ֵ�����½��}
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
                if(t<=2)  army[x0][y0]+=1,cash[pnum][r[x0][y0]]-=10;
            }
            else
            {
                if(3*(totalmaint[pnum][r[x0][y0]]+2)>=cash[pnum][r[x0][y0]]);//��֧��ƽ��
                else//�볬,��1��
                {
                    int t=rand()%5;
                    if(t<=2)  army[x0][y0]+=1,cash[pnum][r[x0][y0]]-=10;

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
                    if(t<=2)  army[x0][y0]+=1,cash[pnum][r[x0][y0]]-=10;
                }
                else//Ǯ��3��
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
            else//��֧�����
            {
                if(3*(totalmaint[pnum][r[x0][y0]]+2)>=cash[pnum][r[x0][y0]]-10);//���
                else if(3*(totalmaint[pnum][r[x0][y0]]+4)>=cash[pnum][r[x0][y0]]-10)//����,����ֻ�����1��
                {
                    int t=rand()%5;
                    if(t<=2)  army[x0][y0]+=1,cash[pnum][r[x0][y0]]-=10;
                }  
                else//����v50
                {
                    int t=rand()%9;
                    if(t<=2)//0,1,2��1��
                    {
                        army[x0][y0]+=1,cash[pnum][r[x0][y0]]-=10;
                    }
                    if(t>=6)//6��7��8��9��2��
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
    int flag = 0, x0, y0, d, r, num = 0;
    bool buttonIsDown[2] = { 0 };

    int curgrade = 1;
    Button* but1 = CreateButton(250, 10, 100, 30, "����1");
    Button* but2 = CreateButton(400, 10, 100, 30, "����2");
    Button* but3 = CreateButton(550, 10, 100, 30, "����3");

    Button* butContinue = CreateButton(0, WIN_H-100, 100, 25, "��һ�غ�");
    Button* butSave = CreateButton(0, WIN_H-50, 100, 25, "����");
    
    ExMessage msg;
    while(1)
    {
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
            //������ڵ�ͼ����
            if (msg.x > 100 & msg.x < 100 + IMG_W * m && msg.y>80 && msg.y < 80 + IMG_H * n) {
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
                    int _x, _y;
                    int r;
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
                            //if (r)  cout << "�ѽ���" << endl;
                            //else  cout << "����ʧ�ܣ�����" << endl;
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

        char s[128] = { 0 };
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


        FlushBatchDraw();
        Sleep(2);
        cleardevice();    
    }
}
void RunGame()//��Ϸ����
{
    turn=0;
    int flag=0,flag1=0;
    while(1){
        turn++;
        flag=0,flag1=0;
        for(int i=1;i<=h_p;i++)
        {
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
            //txtprintChessBoard();///�������
            //system("pause");
            CashCounting(i);
            IniCanMove(i);
            AIMoveStep1(i);
            AICreation(i);
        }
        if(!flag1)//û�л��ŵ������
        {
            Delay(500);
            txtprintChessBoard();
            cout<<endl;
        }
    }
}

void StartRanDomizedGame()
{
    cout<<"������Ҹ������˻�����������֮�Ͳ�����6�Ҵ���1���˻���������Ϊ0����Ҹ�������Ϊ0"<<endl;
    cin>>h_p>>a_p;
    MapGeneration(h_p+a_p);
    ConJudge();
    for(int i=1;i<=h_p+a_p;i++)
    {
        cash[i][1]=10;
    }
    return;
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
            //if(ma[i][j]==pnum)  cout<<cash[ma[i][j]][r[i][j]]<<' ';
            //else  cout<<"? ";
        }
        //cout<<endl;
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
        //cout<<'('<<n0[i]<<','<<m0[i]<<")  ";
        //if(i%5==4)  cout<<endl;
    }
    //cout<<endl;
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

void MapGeneration(int tal)//����tal������
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
    Button* but6 = CreateButton(WIN_W / 2 - 150, 400, 300, 40, "�˳�");
    ExMessage msg;
    while (true) {
        if (peekmessage(&msg)) {
            if (UpdataButton(msg, but1)) {
                gamemode = 1;
                closegraph();
                cout << "�����ͼ��Сn*m:0<n,m<=18" << endl;
                cin >> n >> m;
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

        }

        putimage(0, 0, &img_bk);
        DrawButton(but1);
        DrawButton(but2);
        DrawButton(but3);
        DrawButton(but4);
        DrawButton(but5);
        DrawButton(but6);

        FlushBatchDraw();
        
        cleardevice();
    }

   
    /*
    * int ch;
    while(1)
    {
        cout<<"1:�����ͼ��ʼ��Ϸ\n2:��ȡ��Ϸ�浵������Ϸ\n3:��ȡ��ͼ�浵������Ϸ\n4:�༭��\n5:��Ϸ˵��"<<endl
            <<"9.�˳�"<<endl;
        cin>>ch;
        switch(ch)
        {
        case 1:
            gamemode=1;
            cout<<"�����ͼ��Сn*m:0<n,m<=18"<<endl;
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
            cout<<"��������"<<endl;
            break;
        }
    }
    */
}

void Game_Start()
{
    _InitWin();
    memset(army,0,sizeof(army));
    memset(cash,0,sizeof(cash));
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
        Map_Load();
        RunGame();
    }
    cout<<"��Ϸ������"<<endl;
    int rt=WhoWin();
    cout<<"p"<<rt<<((rt>h_p)?" (�˻�)":" (�������)")<<" ��ʤ����"<<endl;
    system("pause");
}

void Edit()
{
    memset(newma,-1,sizeof(newma));
    int cc;
    cout<<"�Ƿ��ȡ��ͼ�浵�����༭��1����  ��������"<<endl;
    cin>>cc;
    if(cc==1)
    {
        Map_Load();
        for(int i=1;i<=n;i++)  for(int j=1;j<=m;j++)  newma[i][j]=ma[i][j];
        memset(ma,0,sizeof(ma));
    }
    else
    {
        cout<<"�����ͼ��Сn*m:0<n,m<=18"<<endl;
        cin >> n >> m;
    }
    while(n>18||m>18||n<=0||m<=0)  {cout<<"������"<<endl;  cin>>n>>m;}
    int t1,t2,t3;
    cout<<"������Ҹ������˻�����������֮�Ͳ�����6�Ҵ���1���˻���������Ϊ0����Ҹ�������Ϊ0"<<endl;
    while(h_p+a_p>=7||h_p<=0||a_p<0||h_p+a_p<=1)  {cout<<"������"<<endl;cin>>h_p>>a_p;}
    cout<<"�����ͼ��ʽ����ʽΪ��x y t������x,y��ʾ���ꡣt:0��ʾ�յأ�-1��ʾ����ͨ����1-6��ʾ���ң�ϵͳ������Ƿ���ͨ"<<endl;
    while(1)
    {
        cout<<"����114�鿴��ǰ��ͼ������514�˳���,����1919�鿴����������810����һ�������ͼ��������������x,y,z������"<<endl;
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
            cout<<"�����ͼ��ʽ����ʽΪ��x y t������x,y��ʾ���ꡣt:0��ʾ�յأ�-1��ʾ����ͨ����1-6��ʾ����"<<endl
            <<"����114�鿴��ǰ��ͼ������514�˳���,����1919�鿴����������810����һ�������ͼ��"<<endl
            <<"������������x,y,z��������ϵͳ������Ƿ���ͨ"<<endl;
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
                cout<<"���ӳɹ�"<<endl;
            }
            else  cout<<"������"<<endl;
        }
    }
    cout<<"����������1���˳��������棩������2"<<endl;
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
    for(int i=0;i<10;i++)  for(int j=0;j<400;j++)  cin>>cash[i][j];
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
    for(int i=0;i<10;i++)  for(int j=0;j<400;j++)  cin>>cash[i][j];
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
    for(int i=0;i<10;i++)
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
    for(int i=0;i<10;i++)
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

void Help()
{
    cout<<
        "�����Դ��yiotro-antiyoy��ʵ������ǰ��slay��"<<endl<<
        "github:  https://github.com/yiotro/Antiyoy"<<endl<<endl<<

        "��Ϸ����"<<endl<<
        "1.��ͬ��ɫ������ͬ���ҵĹ�������ʤ����Ϊ�������й��ҵĹ���������Ϊ0��"<<endl<<
        "2.���ַ�Ϊ������1���������ֱ���2������̹�ˣ�3�������ֱ��֣�����ֻ��ռ�츽��û�о������ص����أ�"<<endl<<
        "���ֱ�����ռ�츽���в������������ص����أ�ͬʱ�������𲽱���̹�˿����ߵ������κ�һ�����Ӳ���Ӱ�죬��ʹ������̹�ˡ�"<<endl<<
        "3.ÿ�غϣ�ÿ���������ھ����ƶ�2�񣬻�������һ����������ƶ���������������ƶ�1�񵽴�߾����������������ƶ���"<<endl<<
        "4.��ÿ����ƶ���ʽΪ���������ƶ�һ��"<<endl<<
        "5.ÿ�غϣ�ӵ�е�ÿ��ؿ��ṩ1��ң���ļ���ӣ�10��20��30����ÿ�غ�ά�־��ӣ�2��4��7������Ҫһ�����Ľ�ҡ�"<<endl<<
        "6.�������ߵ�������������һ�����������ϣ����������о��ӽ�����������������Ϊ���¾��ӵȼ�Ϊ����֮�ͣ�������3����������"<<endl<<
        "�������ƶ����ø��ӡ����ø��ӵľ��ӱ��غ�δ�ƶ�������������ľ�����Ȼ���Խ����ƶ���"<<endl<<
        "7.һ������פ������Χ�˸���ͬ�������ڸ��9���У��������Ƶĸ�����Ϊ�������ص����ء�"<<endl<<
        "8.����������Լ��ľ��ڣ��������غϻ�õ����أ������ٽ�1����У����������о����Ͻ��У��϶�Ϊ������"<<endl<<
        "���غ���ı��������ƶ���ֱ�������о������������⡣"<<endl<<
        "9.��һ������ӵ�г���һ����أ���ͬ��صĹ���ֿ����㣬��������غ϶�Ϊһʱ���ϲ���������صĹ��⣬"<<endl<<
        "��һ����ر��ض�ʱ������������������нϴ��һ�飬���һ������������䡣"<<endl<<
        "10.��ĳ����ع����Ǯ������ά������ʱ�����о���ֱ����ʧ������ж����Լ��ĻغϿ�ʼǰ��"<<endl<<
        "11.�����޻�"<<endl<<endl<<

        "��Ϸģʽ��"<<endl<<
        "1.��������1~5��ai������Ϸ������ai��ˮƽ�ϵͣ�"<<endl<<
        "2.���ˣ�2~6��һ̨�豸���������в���������ѡ����λ�Ƿ���ai���룻"<<endl<<endl<<

        "��ͼģʽ��"<<endl<<
        "1.�����ͼ��������������ɵ�ͼ��"<<endl<<
        "2.�༭�����������б༭��ͼ��������Ҫ��֤��ͼ��ͨ���ͻ��˲����ж����������ͼ���á�"<<endl<<
        "3.Ԥ�裺����Ϸ������Ԥ��ļ��ŵ�ͼ�����ǲ�ȷ��ƽ���ԡ����滮�У�"<<endl<<endl;
    getch();
    return;
}

/*
��Ϸ�浵�ṹ��
1.��ͼ��С n,m
2.������˻����� h_p��a_p
3.n*m���󣬵�ͼ����
4.n*m����r[i][j]�������㴦�ڵ���ͨ��״̬
5.n*m����army[i][j]��������Ŀǰ�ľ���
6.7*400���󣬴���cash����
�����У�鹦��
*/