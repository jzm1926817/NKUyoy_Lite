void ConJudge()//并查集判定连通块
{
    for(int i=1;i<=n;i++)
    {
        for(int j=1;j<=m;j++)
        {
            newq[i][j]=q[i][j],newr[i][j]=r[i][j];
        }
    }
    for(int i=1;i<=n;i++)
    {
        for(int j=1;j<=m;j++)
        {
            if(ma[i][j]>0)
            {
                if(ma[i-1][j]==ma[i][j])
                {
                    if(newq[ma[i][j]][newr[i][j]])  newq[ma[i][j]][newr[i][j]]--;
                    newr[i][j]=newr[i-1][j];
                    newq[ma[i][j]][newr[i][j]]++;
                    if(ma[i][j]==ma[i][j-1]&&newr[i][j]!=newr[i][j-1])
                    {
                        int tmp=j-1;
                        for(int l=1;l<=n;l++)
                        {
                            for(int k=1;k<=m;k++)
                            {
                                if(ma[l][k]==ma[i][j]&&newr[l][k]==newr[i][tmp])
                                {
                                    newq[ma[i][j]][newr[l][k]]--;
                                    newr[l][k]=newr[i][j];
                                    newq[ma[i][j]][newr[i][j]]++;
                                }
                            }
                        }
                    }
                }
                else if(ma[i][j]==ma[i][j-1])
                {
                    if(newq[ma[i][j]][newr[i][j]])  newq[ma[i][j]][newr[i][j]]--;
                    newr[i][j]=newr[i][j-1];
                    newq[ma[i][j]][newr[i][j]]++;
                }
                else
                {
                    int tmp=0,t;
                    if(newq[ma[i][j]][newr[i][j]])  newq[ma[i][j]][newr[i][j]]--;
                    f[ma[i][j]]++;
                    for(int l=1;l<=200;l++)
                    {
                        if(!newq[ma[i][j]][l])
                        {
                            t=l;
                            break;
                        }
                    }
                    newq[ma[i][j]][t]++;
                    newr[i][j]=t;
                }
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