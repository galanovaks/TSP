double **create_list(int n,FILE *f)
{
    double **list;
    int i;
    list=malloc(n*sizeof(double*));
    for (i=0;i<n;i++)
    {
        list[i]=malloc(8*sizeof(double));
        fscanf(f,"%lf %lf",&list[i][0],&list[i][1]);
        list[i][3]=-1;
        list[i][4]=0;
        list[i][5]=-1;
        list[i][6]=-1;
        list[i][7]=-1;
    }
    return list;
}

void delete_list(double **l,int n)
{
    double **aux=l;
    int i;
    for (i=0;i<n;i++)
        free(l[i]);
    free(aux);
}

void delete_n(int **note,int n)
{
    int **aux=note;
    int i;
    for (i=0;i<n;i++)
        free(note[i]);
    free(aux);
}

double distance(double **l,int i,int j)
{
    return sqrt(pow(l[j][0]-l[i][0],2)+pow(l[j][1]-l[i][1],2));
}

void max_count(int **note,double **l,int n,int c)
{
    double d,dist=0,dm=-1,p7=-1;
    int i;
    if (l[c][4]==2)
        return;
    for (i=0;i<n;i++)
        if ((i!=c)&&(l[i][4]!=2)&&(note[c][0]!=i))
        {
            d=distance(l,c,i);
            dist+=d;
            if ((dm==-1)||(d>dm))
            {
                dm=d;
                p7=i;
            }
        }
    l[c][3]=dist;
    l[c][5]=dm;
    l[c][7]=p7;
}

int search_start(double **l,int n,int var)
{
    int i,res=-2;
    double dist=-1;
    for (i=n-1;i>=0;i--)
    {   
        if ((l[i][4]==2)||((var==0)&&(l[i][4]==1)))
            continue;
        if ((dist<l[i][3])||((dist==l[i][3])
        &&(l[res][5]<l[i][5])))
        {
            dist=l[i][3];
            res=i;
        }
    }
    return res;
}

int check_cycle(int **note,double **l,int n,int c1,int c)
{
    int k=c,p=c1;
    if ((l[c][4]!=1)||(l[c1][4]!=1))
        return 0;
    l[c1][4]++;
    l[c][4]++;
    note[c][1]=c1;
    note[c1][1]=c;
    while (k!=c1)
    {
        if (l[k][4]==1)
            break;
        if (note[k][0]==p)
        {
            p=k;
            k=note[k][1];
        }
        else
        {
            p=k;
            k=note[k][0];
        }
    }
    note[c][1]=-1;
    note[c1][1]=-1;
    l[c1][4]--;
    l[c][4]--;
    if (k==c1)
        return 1;
    else
        return 0;
}

int search_min(int **note,double **l,int n,int c,double *len)
{
    int i,res=-2;
    double d,dist=-1;
    for (i=0;i<n;i++)
    {
        if ((i==c)||(l[i][4]==2)||(note[c][0]==i))
            continue;
        d=distance(l,c,i);
        if (((dist==-1)||(dist>d)||((dist==d)&&(l[i][3]>l[res][3])))
        &&(check_cycle(note,l,n,i,c)==0))
        {
            dist=d;
            res=i;
        }
    }
    if (res!=-2)
    {
        *len+=dist;
        l[c][4]++;
        l[res][4]++;
        if (l[res][4]==1)
            note[res][0]=c;
        else
            note[res][1]=c;
        if (l[c][4]==1)
            note[c][0]=res;
        else
            note[c][1]=res;
        l[c][3]-=dist;
        l[res][3]-=dist;
        if (n<10000)
        {
            max_count(note,l,n,res);
            max_count(note,l,n,c);
        }
        if ((l[c][4]==2)||(l[res][4]==2))
            for (i=0;i<n;i++)
            {
                if (l[i][4]==2)
                    continue;
                if ((n<10000)&&((l[i][7]==c)||(l[i][7]==res)))
                    max_count(note,l,n,i);
                if  ((l[c][4]==2)&&(note[c][0]!=i)&&(note[c][1]!=i))
                    l[i][3]-=distance(l,i,c);
                if  ((l[res][4]==2)&&(note[res][0]!=i)&&(note[res][1]!=i))
                    l[i][3]-=distance(l,i,res);
            }
    }
    return res;
}

void swipe(int **note,int i,int c1,int c2)
{
    if (note[i][0]==c1)
        note[i][0]=c2;
    else
    note[i][1]=c2;
}

void triangle(int **note,double **l,int i,int var,double *len)
{
    int c1,c2,c3,var2;
    double r1,r2;
    if (l[i][4]!=2)
        return;
    if (var==0)
        var2=1;
    else
        var2=0;
    c1=note[i][var];
    c3=note[i][var2];
    if (l[c1][4]!=2)
        return;
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
    }
}

void optimization1(int **note,double **l,int n,double *len,int i)
{
    int j,r1,r2,r3,r4,res=0;
    double R1,R2;
    if (l[i][4]!=2)
            return;
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
                if (n<10000)
                    optimization1(note,l,n,len,i);
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
                if (n<10000)
                    optimization1(note,l,n,len,i);
            }
        }
    }
}

void optimization(int **note,double **l,int n,double *len,int i)
{
    int j,r1,r2,r3,r4,res=0;
    double R1,R2;
    if (l[i][4]!=2)
        return;
    for (j=n-1;j>=0;j--)
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
            if (R2==R1)
            {
                *len+=R2;
                *len-=R1;
                swipe(note,i,r1,r2);
                swipe(note,j,r2,r1);
                swipe(note,r1,i,j);
                swipe(note,r2,j,i);
                res++;
                if (n<10000)
                    optimization1(note,l,n,len,j);
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
            if (R2==R1)
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
                if (n<10000)
                    optimization1(note,l,n,len,j);
            }
        }
    }
}

void solution(double **l,int n)
{
    int i,c,j,fin;
    int **note;
    double *len=malloc(sizeof(double));
    *len=0;
    note=malloc(n*sizeof(int*));
    for (i=0;i<n;i++)
    {
        note[i]=malloc(2*sizeof(int));
        note[i][0]=-1;
        note[i][1]=-1;
    }
    for (i=0;i<n;i++)
        max_count(note,l,n,i);
    i=1;
    c=search_start(l,n,i);
    while ((c!=-2)&&((fin=search_min(note,l,n,c,len))!=-2))
    {
        optimization1(note,l,n,len,c);
        optimization1(note,l,n,len,fin);
/*        triangle(note,l,c,0,len);
        triangle(note,l,c,1,len);
        triangle(note,l,fin,0,len);
        triangle(note,l,fin,1,len);
        fin=search_min(note,l,n,c,len);*/
        c=search_start(l,n,i);
        if (c==-2)
        {
//            for (i=0;i<n;i++)
//                printf("%d-%d,%d\n ",i,note[i][0],note[i][1]);
            i++;
            c=search_start(l,n,i);
        }
    }
//    for (i=0;i<n;i++)
//        printf("%d-%d,%d\n ",i,note[i][0],note[i][1]);
    c=-1;
    j=-1;
    for (i=0;i<n;i++)
    {
        if (note[i][1]==-1)
        {
            if (c==-1)
                c=i;
            else
            {
                j=i;
                break;
            }
        }
    }
    *len+=distance(l,c,j);
    l[c][4]++;
    l[j][4]++;
    note[c][1]=j;
    note[j][1]=c;
    optimization1(note,l,n,len,c);
    optimization1(note,l,n,len,j);
    if (n<4000)
    {
        for (fin=0;fin<2;fin++)
            for (i=0;i<n;i++)
            {
                optimization1(note,l,n,len,i);
                triangle(note,l,i,0,len);
                triangle(note,l,i,1,len);
                optimization(note,l,n,len,i);
            }
        for (i=n-1;i>=0;i--)
        {
            optimization1(note,l,n,len,i);
            triangle(note,l,i,0,len);
            triangle(note,l,i,1,len);
            optimization(note,l,n,len,i);
        }
    }
    printf("%.3lf\n",*len);
    c=0;
    fin=note[0][1];
    printf("%d ",c);
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
        printf("%d ",c);
        i++;
    }
    printf("%d\n",fin);
/*    if (check_cycle(note,l,n,c,j)!=1)
        printf ("FATAL ERROR\n");
//    else
//        printf("OK\n");
    l[c][4]++;
    l[j][4]++;
    note[c][1]=j;
    note[j][1]=c;*/
//    for (i=0;i<n;i++)
//    {
//        printf("%d-%d,%d\n ",i,note[i][0],note[i][1]);
//    }
    delete_list(l,n);
    delete_n(note,n);
    free(len);
}

