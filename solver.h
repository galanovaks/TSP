double **create_list(int n,FILE *f)
{
    double **list;
    int i;
    list=malloc(n*sizeof(double*));
    for (i=0;i<n;i++)
    {
        list[i]=malloc(5*sizeof(double));
        fscanf(f,"%lf %lf",&list[i][0],&list[i][1]);
        list[i][2]=0;
        list[i][3]=-1;
        list[i][4]=0;
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

double distance(double **l,int i,int j)
{
    return sqrt(pow(l[j][0]-l[i][0],2)+pow(l[j][1]-l[i][1],2));
}
/*
double **create_graph(double **l, int n)
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
            graph[i][j]=distance(l,i,j);
            graph[j][i]=graph[i][j];
        }
    return graph;
}
*/
void max_count(int **note,double **l,int n,int c)
{
    double d,dist=-1;
    int i;
    for (i=0;i<n;i++)
        if ((l[i][2]!=2)&&(i!=c)&&(l[c][3]!=i)&&(note[c][0]!=i))
        {
            d=distance(l,c,i);
            if (dist<d)
                dist=d;                                                      }
    l[c][4]=dist;
}

int search_min(int **note,double **l,int n,int c,int var,double *len)
{
    int i,res=-2;
    double d,dist=-1;
    for (i=0;i<n;i++)
    {   
        if ((l[i][2]==var+1)||(l[c][3]==i))
            continue;
        if (c!=i)
        {
            d=distance(l,c,i);
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
    if (res!=-2)
    {
        if (l[res][4]==dist)
            max_count(note,l,n,res);
        if (l[c][4]==dist)
            max_count(note,l,n,c);
        *len+=dist;
//        printf("%.3lf\n",dist);
        l[c][2]++;
        l[res][2]++;
        if (var==0)
        {
            l[c][3]=res;
            l[res][3]=c;
        }
    }
    return res;
}

int search_start(double **l,int n,int var)
{
    int i,j,res=-2;
    double d,dist=-1;
    for (i=0;i<n;i++)
    {   
        if (l[i][2]==var+1)
            continue;
        if (dist<l[i][4])
            {
                dist=l[i][4];
                res=i;
            }
    }
//    printf("\n|%d %.3lf|\n",res,dist);
    return res;
}

void swipe(int **note,int i,int c1,int c2)
{
    if (note[i][0]==c1)
        note[i][0]=c2;
    else
        note[i][1]=c2;
}

int triangle(int **note,double **l, int i,double *len)
{
    int c1,c2,c3,res=0;
    double r1,r2;
    c1=note[i][0];
    c3=note[i][1];
    if (note[c1][0]==i)
        c2=note[c1][1];
    else
        c2=note[c1][0];
    r1=distance(l,i,c3)+distance(l,c1,c2);
    r2=distance(l,i,c2)+distance(l,c1,c3);
    if (r2<r1)
    {
        *len+=r2;
        *len-=r1;
        swipe(note,i,c3,c2);
        swipe(note,c3,i,c1);
        swipe(note,c1,c2,c3);
        swipe(note,c2,c1,i);
        res=1;
    }
    c1=note[i][1];
    c3=note[i][0];
    if (note[c1][0]==i)
        c2=note[c1][1];
    else
        c2=note[c1][0];
    r1=distance(l,i,c3)+distance(l,c1,c2);
    r2=distance(l,i,c2)+distance(l,c1,c3);
    if (r2<r1)
    {
        *len+=r2;
        *len-=r1;
        swipe(note,i,c3,c2);
        swipe(note,c3,i,c1);
        swipe(note,c1,c2,c3);
        swipe(note,c2,c1,i);
        res=1;
    }
    return res;
}

int optimization(int **note,double **l,int n,double *len,int var,int var2)
{
    int i,j,r1,r2,r3,r4,res=0;
    double R1,R2;
    for (i=var-1;i<n;i++)
    {   
        if (note[i][1]==-1)
            continue;
        for (j=0;j<n;j++)
        {
            if ((note[j][1]==-1)||(i==j))
                continue;
            if ((note[i][0]==j)||(note[i][1]==j))
            {
                if (note[i][0]==j)
                    r1=note[i][1];
                else
                    r1=note[i][0];
                if (note[j][0]==i)
                    r2=note[j][1];
                else
                    r2=note[j][0];
                R1=distance(l,i,r1)+distance(l,j,r2);
                R2=distance(l,i,r2)+distance(l,j,r1);
                if (R2<R1)
                {
                    *len+=R2;
                    *len-=R1;
                    swipe(note,i,r1,r2);
                    swipe(note,j,r2,r1);
                    swipe(note,r1,i,j);
                    swipe(note,r2,j,i);
                    res++;
                    if (var2==0)
                        return res;
                }
            }
            else
            {
                r1=note[i][0];
                r2=note[i][1];
                r3=note[j][0];
                r4=note[j][1];
                R1=distance(l,i,r3)+distance(l,i,r4)
                +distance(l,j,r1)+distance(l,j,r2);
                R2=distance(l,i,r1)+distance(l,i,r2)
                +distance(l,j,r3)+distance(l,j,r4);
                if (R2>R1)
                {   
                    
                    *len+=R1;
                    *len-=R2;
                    swipe(note,i,r1,r3);
                    swipe(note,i,r2,r4);
                    swipe(note,j,r3,r1);
                    swipe(note,j,r4,r2);
                    if ((note[r1][0]!=j)&&(note[r1][1]!=j))
                        swipe(note,r1,i,j);
                    if ((note[r2][0]!=j)&&(note[r2][1]!=j))
                        swipe(note,r2,i,j);
                    if ((note[r3][0]!=i)&&(note[r3][1]!=i))
                        swipe(note,r3,j,i);
                    if ((note[r4][0]!=i)&&(note[r4][1]!=i))
                        swipe(note,r4,j,i);
                    res++;
                    if (var2==0)
                        return res;
                }
            }
            if (var2==2)
            {
                res+=triangle(note,l,i,len);
//                if ((res!=0)&&(var!=1))
//                    return res;
            }
        }
    }
    return res;
}

void solution(double **l,int n)
{
    int n2=n,c,st,fin,i,j,contr=1;
    int **note;
    double res,r,*len=malloc(sizeof(int));
    *len=0;
    note=malloc(n*sizeof(int*));
    for (i=0;i<n;i++)
    {
        note[i]=malloc(2*sizeof(int));
        note[i][0]=-1;
        note[i][1]=-1;
    }
    for (i=0;i<n;i++)
    {
        if (l[i][2]==0)
            note[i][0]=0;
        max_count(note,l,n,i);
    }
    for (i=0;i<n2/2;i++)
    {        
        c=search_start(l,n,0);
//        printf("\nis%d ",c);
        fin=search_min(note,l,n,c,0,len);
        note[c][0]=fin;
        note[fin][0]=c;
//        printf("v%d\n",fin);
    }
    if (n%2!=0)
    {
        for (i=0;i<n;i++)
            if (l[i][2]==0)
                c=i;
        fin=search_min(note,l,n,c,1,len);
        note[c][0]=fin;
        note[fin][1]=c;
//        printf("is %d v %d\n",c,fin);
        fin=l[fin][3];
        l[c][3]=fin;
        l[fin][3]=c;
        max_count(note,l,n,fin);
        max_count(note,l,n,fin);
    }
    n2=n/2;
    for (i=0;i<n2-1;i++)
    {
        c=search_start(l,n,1);
        contr=c;
        fin=search_min(note,l,n,c,1,len);
        note[c][1]=fin;
        note[fin][1]=c;
        c=l[c][3];
        fin=l[fin][3];
        l[c][3]=fin;
        l[fin][3]=c;
        max_count(note,l,n,fin);
        max_count(note,l,n,c);
        if (n-i<500)
            optimization(note,l,n,len,contr+1,0);
    }
    c=-3;
    fin=-3;
    for (i=0;i<n;i++)
    {
        if (note[i][1]==-1)
        {
            if (c==-3)
                c=i;
            else 
            {
                fin=i;
                note[c][1]=fin;
                note[fin][1]=c;
                *len+=distance(l,c,fin);
//                printf("%.3lf\n",*len);
            }
        }
    }
//    printf("%.3lf\n",*len);
    fin=optimization(note,l,n,len,1,2);
/*    c=1;
    while ((c<10)&&(fin!=0))
    {
        fin=optimization(note,l,n,len,(fin+1)%n+1,2);
        c++;
    }*/
    printf("%.3lf\n",*len);
/*    for (i=0;i<n;i++)
        printf("%d-%d,%d\n",i,note[i][0],note[i][1]);
  */  c=0;
    fin=note[0][1];
//    printf("%d ",c);
    i=1;
    while (i!=n)
    {
        if (note[fin][1]==c)
        {
            c=fin;
            fin=note[fin][0];
        }
        else
        {   
            c=fin;
            fin=note[fin][1];
        }
//        printf("%d ",c);
        i++;
    } 
    if (c==note[0][0])
        printf("OK\n");
    else
        printf("FATAL ERROR\n");
    delete_list(l,n);
}
