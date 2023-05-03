//Unverified
#include<bits/stdc++.h>
using namespace std;
int ma[20][20],r[20][20],army[20][20],cash[7][400];
int isOurLand(int x0,int y0,int pnum)//1可以 0不可以
{
    if(x0<=0||y0<=0||x0>n||y0>m)  return 0;
    if(ma[x0][y0]==pnum)  return 1;
    if(ma[x0-1][y0]==pnum)  return 1;
    if(ma[x0][y0-1]==pnum)  return 1;
    if(ma[x0][y0+1]==pnum)  return 1;
    if(ma[x0-1][y0]==pnum)  return 1;
    return 0;
}
int dx[10]={0,1,1,1,0,0,0,-1,-1,-1};
int dy[10]={0,-1,0,1,-1,0,1,-1,0,1};
int CanArmyBeCreated(int x0,int y0,int pnum,int level)//1可以 0不可以
{
    if(!isOurLand(x0,y0,pnum))  return 0;
    if(ma[x0][y0]==pnum)
    {
        if(army[x0][y0]+level<=3)  return 1;
        return 0;
    }
    else
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
int ConductCreation(int x0,int y0,int pnum,int level)//1可以 0不行
{
    if(!isOurLand(x0,y0,pnum))  return 0;
    if(CanArmyBeCreated(x0,y0,pnum,level))  return 0;
    cash[r[x0][y0]][level]-=10*level;
    if(ma[x0][y0]==pnum)  army[x0][y0]+=level;
    else army[x0][y0]=level;
    //重新计算是否连通，重新分配金币
    return 1;
}
int main()
{

}