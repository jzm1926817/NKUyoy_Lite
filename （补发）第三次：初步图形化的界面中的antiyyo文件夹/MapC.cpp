//生成地图
#include"MapC.h"

using namespace std;
void MapGeneration(int tal)//生成tal个国家
{
    int n=12,m=16,tot=0;
    struct node
    {
        int x,y;
    } a[400];
    int maxtt=rand()%50+80;
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
    for(int i=1;i<=tal;i++)
    {
        for(int j=1;j<=3;j++)
        {
            while(ma[x][y]!=0)  int x=rand()%n+1,y=rand()%m+1;
            ma[x][y]=i;
            for(int k=1;k<114514;k++);//刷时间种子
        }
    }
    return;
}