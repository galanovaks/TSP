#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "solver.h"

int main()
{
    int N,j;
    double **list;
    char files[74][12]={"tsp_100_1","tsp_100_2","tsp_100_3", 
    "tsp_100_4","tsp_100_5","tsp_100_6","tsp_1000_1","tsp_101_1", 
    "tsp_105_1","tsp_1060_1","tsp_107_1","tsp_1084_1","tsp_1173_1", 
    "tsp_11849_1","tsp_124_1","tsp_127_1","tsp_1291_1","tsp_1304_1", 
    "tsp_1323_1","tsp_136_1","tsp_1379_1","tsp_1400_1","tsp_14051_1",
    "tsp_1432_1","tsp_144_1","tsp_150_1","tsp_150_2","tsp_152_1", 
    "tsp_1577_1","tsp_159_1","tsp_1655_1","tsp_1748_1","tsp_1817_1", 
    "tsp_18512_1","tsp_1889_1","tsp_195_1","tsp_198_1","tsp_200_1", 
    "tsp_200_2","tsp_2103_1","tsp_2152_1","tsp_225_1","tsp_226_1", 
    "tsp_2319_1","tsp_2392_1","tsp_262_1","tsp_264_1","tsp_299_1", 
    "tsp_3038_1","tsp_318_1","tsp_318_2","tsp_33810_1","tsp_3795_1", 
    "tsp_400_1","tsp_417_1","tsp_439_1","tsp_442_1", "tsp_4461_1", 
    "tsp_493_1","tsp_5_1","tsp_51_1","tsp_574_1","tsp_575_1", 
    "tsp_5915_1","tsp_5934_1","tsp_654_1","tsp_657_1","tsp_70_1", 
    "tsp_724_1","tsp_7397_1","tsp_76_1","tsp_76_2","tsp_783_1", 
    "tsp_99_1"};
    FILE *f;
    for(j=0;j<74;j++)
    {
        f=fopen(files[j],"r");
        fscanf(f,"%d",&N);
        list=create_list(N,f);
//        printf("%s ",files[27]);
        fclose(f);
        solution(list,N);
    }
    return 0;
}
