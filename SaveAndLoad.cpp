//Verified
#include<bits/stdc++.h>
using namespace std;
int ma[20][20],r[20][20],cash[7][400],q[7][400],army[20][20];
int n,m,k;
void save()
{
    //cout代指文件操作
    cout<<n<<" "<<m<<" "<<k<<endl;
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
    for(int i=0;i<=3;i++)
    {
        for(int j=0;j<=3;j++)
        {
            cout<<cash[i][j]<<" ";
        }
        cout<<endl;
    }
    cout<<'E'<<endl;
}
void load()
{
    //cin代指文件操作
    cin>>n>>m>>k;
    for(int i=1;i<=n;i++)  for(int j=1;j<=m;j++)  cin>>ma[i][j];
    for(int i=1;i<=n;i++)  for(int j=1;j<=m;j++)
    {
        cin>>r[i][j];
        q[ma[i][j]][r[i][j]]++;
    }
    for(int i=1;i<=n;i++)  for(int j=1;j<=m;j++)  cin>>army[i][j];
    for(int i=0;i<=3;i++)  for(int j=0;j<=3;j++)  cin>>cash[i][j];
    char ch;
    while(ch=getchar())
    {
        if(ch=='E')  return;
    }
    return;
}
int main()
{
    freopen("Save.txt","w",stdout);
    freopen("Load.txt","r",stdin);
    load();
    save();
}
/*
游戏存档结构：
1.地图大小 n,m
2.国家数量 k
3.n*m矩阵，地图归属
4.n*m矩阵，r[i][j]，即各点处于的连通块状态
5.n*m矩阵，army[i][j]，即各点目前的军队
6.7*400矩阵，存下cash数组
拟加入校验功能
*/
