//Verified
#include<bits/stdc++.h>
using namespace std;
const int n=5,m=5;
int ma[20][20],r[20][20],q[7][400],f[7];
//q=t*k，t=1~6,国家；k每个国家的第k块地的格子个数,如果为0代表被其他连通块合并
//r：每块地是哪个国家的第几块地
//f:每个国家有几块领地
//cash:每个连通块的金币
//totalmaint  每个连通块上每回合陆军维护费，回合初更新
//nearltkill：上回合破产的部队
int cash[10][400],totalmaint[10][400];
int army[20][20],nearlykill[20][20];
void ConJudge()//并查集判定连通块
{
    for(int i=1;i<=n;i++)//枚举每一个点
    {
        for(int j=1;j<=m;j++)
        {
            if(ma[i][j]>0)//如果有颜色
            {
                    if(ma[i-1][j]==ma[i][j])//这点的上面有
                    {
                        if(q[ma[i][j]][r[i][j]])
                        {
                            if(q[ma[i][j]][r[i][j]]==1)//最后一块了
                            {
                                cash[ma[i-1][j]][r[i-1][j]]+=cash[ma[i][j]][r[i][j]];
                                cash[ma[i][j]][r[i][j]]=0;
                                f[ma[i][j]]--;
                            }
                            q[ma[i][j]][r[i][j]]--;
                        }
                        r[i][j]=r[i-1][j];
                        q[ma[i][j]][r[i][j]]++;//加入上面
                        if(ma[i][j]==ma[i][j-1]&&r[i][j]!=r[i][j-1])//上面和左边都有且原来的编号不一致，涉及重新连通
                        {
                            cash[ma[i][j]][r[i][j]]+=cash[ma[i][j]][r[i][j-1]];//更新金币
                            cash[ma[i][j]][r[i][j-1]]=0;
                            int tmp=j-1;
                            f[ma[i][j]]--;
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
                                cash[ma[i][j]][r[i][j]]=0;
                                f[ma[i][j]]--;
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
                //}
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
void CashCounting(int pnum)//更新金币,num:第k个国家
{
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
    for(int i=1;i<=200;i++)//计算国库与破产清算
    {
        if(q[t][i])//计算合法块的金币
        {
            g++;
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
int main()
{
    memset(r,0,sizeof(r));
    //memset(totalcount,0,sizeof(totalcount));
    memset(f,0,sizeof(f));
    memset(q,0,sizeof(q));
    memset(totalmaint,0,sizeof(totalmaint));
    //for(int i=1;i<=n;i++)  for(int j=1;j<=m;j++)  cin>>ma[i][j];
    //for(int i=1;i<=n;i++)  for(int j=1;j<=m;j++)  cin>>army[i][j];
    while(1)
    {
        for(int i=1;i<=n;i++)  for(int j=1;j<=m;j++)  cin>>ma[i][j];
        //for(int i=1;i<=n;i++)  for(int j=1;j<=m;j++)  cin>>army[i][j];
        ConJudge();
        for(int i=1;i<=n;i++)  
        {
            for(int j=1;j<=m;j++)  cout<<r[i][j]<<" ";
            cout<<endl;
        }
        cout<<endl;
        CashCounting(2);
        for(int i=1;i<=n;i++)
        {
            for(int j=1;j<=n;j++)
            {
                cout<<army[i][j];
            }
            cout<<endl;
        }
        cout<<endl;
    }

    return 0;
}
