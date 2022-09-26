double **create_list(int n,FILE *f)
{
    double **list;
    int i;
    list=malloc(n*sizeof(double*));
    for (i=0;i<n;i++)
    {
        list[i]=malloc(4*sizeof(double));
        fscanf(f,"%lf %lf",&list[i][0],&list[i][1]);
        list[i][2]=0;
        list[i][3]=-1;
    }
    return list;
}

void output_list(double **list,int n)
{
    int i;
    for (i=0;i<n;i++)
        printf("%d|%.3lf,%.3lf&&  %.0lf,  %lf\n",i,list[i][0],list[i][1],list[i][2],list[i][3]);
}

void delete_list(double **l,int n)
{
    double **aux=l;
    int i;
    for (i=0;i<n;i++)
        free(l[i]);
    free(aux);
}
/*
double **create_graph(double **list,int n)
{
    double **graph;
    int i,j;
    graph=malloc(n*sizeof(double*));
    for (i=0;i<n;i++)
    {
        graph[i]=malloc(n*sizeof(double));
        graph[i][i]=-1;
    }
    for (i=0;i<n-1;i++)
        for (j=i+1;j<n;j++)
        {
            graph[i][j]=sqrt(pow(list[i][0]-list[j][0],2)
            +pow(list[i][1]-list[j][1],2));
            graph[j][i]=graph[i][j];
        }
    return graph;
}

void output_graph(double **g, int n)
{
    int i,j;
    for (i=0;i<n;i++)
    {
        for (j=0;j<n;j++)
            printf("%.3lf ",g[i][j]);
        printf("\n");
    }
}
*/
/*
void block_cycle(double **l,double **g, int n, int c)
{
    int i,c1=-1,c2=-1;
    double a,b;
    for (i=0;i<n;i++)
    {
        if ((i!=c)&&(g[c][i]==-1))
        {
            if (c1==-1)
                c1=i;
            else
            {
                c2=i;
                break;
            }
        }
    }
    g[c1][c2]=-1;
    g[c2][c1]=-1;
}

void distance (double **l,double **g,int n,int k,int *count)
{
    int i,j,c1,c2,f=-1,ch=0;
    double dist=-1;
//    printf("\nITER%d\n",k);
    for (i=0;i<n;i++)
    {
        if (l[i][2]==2)
            continue;
        if (k==0)
            dist=-1;
        for (j=0;j<n;j++)
        {
            if (l[j][2]==2)
                continue;
            if (g[i][j]!=-1)
            {
                if ((dist==-1)||(dist>=g[i][j]))
                {   
                    if (dist==g[i][j])
                        f=1;
                    else
                    {
                        dist=g[i][j];
                        c1=i;
                        c2=j;
                        f=0;
  //                      printf("\nCAND%d-%d\n",c1,c2);
                    }
                }
             }
        }
        if ((f==0)&&(dist!=-1))
        {
            l[c1][2]++;
            l[c2][2]++;
            l[c1][3]+=g[c1][c2];
            g[c1][c2]=-1;
            g[c2][c1]=-1;
            (*count)++;
            ch=1;
//            printf("\nPARA %d,%d\n",c1,c2);
            if (l[c1][2]==2)
                block_cycle(l,g,n,c1);
            if (l[c2][2]==2)
                block_cycle(l,g,n,c2);
        }
    }
    if (((*count)<n)&&(ch!=0))
        distance(l,g,n,0,count);
}
*/
int search_min(double **l,int n,int c,double *len)
{
    int i,j,res;
    double d,dist=-1;
    for (i=0;i<n;i++)
    {   
        if ((l[i][2]==2)||(l[c][3]==i)||
        ((l[i][3]==l[c][3])&&(l[i][3]!=-1)))
            continue;
        if (c!=i)
        {
            d=sqrt(pow(l[c][0]-l[i][0],2)+pow(l[c][1]-l[i][1],2));
            if (dist==-1)
            {
                dist=d;
                res=i;
            }
            else
            {
                if (dist>d)
                {
                    dist=d;
                    res=i;
                }
            }
        }
    }
    *len+=dist;
    l[c][2]++;
    l[res][2]++;
    l[c][3]=res;
    l[res][3]=c;
    return res;
}

int search_start(double **l,int n,int var)
{
    int i,j,res=-2;
    double d,dist=-1;
    for (i=0;i<n;i++)
    {   
        if (l[i][2]!=var)
            continue;
        for (j=0;j<n;j++)
            if ((i!=j)&&(l[i][3]!=j))
            {
                d=sqrt(pow(l[i][0]-l[j][0],2)
                +pow(l[i][1]-l[j][1],2));
                if (dist==-1)
                {
                    dist=d;
                    res=i;
                }
                else
                {
                    if (dist<d)
                    {
                        dist=d;
                        res=i;
                    }
                }
//                printf("\n|%d %.3lf|\n",res,dist);
            }
    }
    return res;
}

void solution(double **l, int n)
{
    int c,st,fin,i,contr=-2;
    double *len=malloc(sizeof(int));
//    double **g=create_graph(l,n),res=0;
//    int i,min=search_min(l,g,n);
//    printf("%d\n",min);
//    output_list(l,n);
    *len=0;
//    st=search_start(l,n);
//    c=st;
    c=search_start(l,n,0);
    while(c!=-2)
    {        
//        printf("\nis%d ",c);
        fin=search_min(l,n,c,len);
        c=search_start(l,n,0);
//        printf("v%d\n",fin);
    }
    c=search_start(l,n,1);
    while(c!=-2)
    {
        fin=search_min(l,n,c,len);
        c=search_start(l,n,1);
    }
//    c=search_min(l,n,fin,len);
//    printf("v %d",search_start(l,n));
//    *len+=sqrt(pow(l[st][0]-l[c][0],2)+pow(l[st][1]-l[c][1],2));
//    l[st][2]++;
//    l[c][2]++;
//    printf("%d\n",c);
    printf("%.3lf\n",*len);
    for (i=0;i<n;i++)
        if (l[i][2]!=2)
            contr++;
    if (contr==-2)
        printf("OK\n");
    else
    {
        contr+=2;
        printf("FATAL ERROR %d\n",contr);
    }
    delete_list(l,n);
//  delete_list(g,n);
//    output_graph(g,n);
}
