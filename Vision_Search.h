#include "initial.h"

void mutation(vector<int> &P,int ind,int region_len)//隨機選取一點做調換
{
     int c=rand()%ind;
     while(c<region_len)
     {
         c=rand()%ind;
     }
    if(P[c]==0)
        P[c]=1;
    else
        P[c]=0;
}
void crossover(vector<vector<int> > &P,vector<vector<int> > P1,vector<vector<int> > P2,int P1pop,int P2pop,int ind,int region_len)
{
    int i=0;
    int y=0;
    while(i<P2pop){

        for(int j=0;j<P1pop;j++)
        {
            int c1=rand()%ind;
            for(int k=0;k<ind;k++)
            {
                if(k<region_len)
                {
                    P[y][k]=P2[i][k];
                }
                else{
                    if(k>c1)
                    {
                        P[y][k]=P2[i][k];
                    } 
                    else{
                        P[y][k]=P1[j][k]; 
                    }
                }
                
            }
            float x = (float) rand() / (RAND_MAX + 1.0);
            if(x<MR)
            {
                mutation(P[y],ind,region_len);
            }
            y++;
          
            
        }
        i++;
    }
}
vector<int> Fitness_Calculate(vector<vector<int> > &goods,vector<vector<int> >sampleV,int Region)//計算sampleV的Fitness
{
    vector<int> fitness(sampleV.size(),0);
    for(int i=0;i<sampleV.size();i++)
    {
        for(int j=0;j<sampleV[0].size();j++)
        {
            if(sampleV[i][j]==1)
                fitness[i]++;
        }
    }
    
    return fitness;
}
vector<double> Region_fitness(vector<vector<int> > &goods,vector<vector<int> >sampleV,vector<int> Sample_Fitness,vector<vector<int> >&Region_Fitness,int region,int Each_Region_Quantity)
{
    int i=0;
    vector<vector<int> > Region_index;
    vector<double> SampleV_Region_Fitness(region,0);
    Region_index.resize(Region_Fitness.size());
    for(int z=0;z<Region_index.size();z++) {Region_index[z].resize(Region_Fitness[0].size());}//每個region最好的幾個的Index
    for(int j=0;j<Region_Fitness.size();j++)
    {
        for(int k=0;k<Region_Fitness[0].size();k++)
        {
            Region_Fitness[j][k]=0;
        }
    }
    while(i<Sample_Fitness.size())
    {
        int k=i/Each_Region_Quantity;
        SampleV_Region_Fitness[k]+=Sample_Fitness[i];//把所有同region的fitness加起來
        for(int y=0;y<Region_Fitness[0].size();y++)   
        {
            if(Sample_Fitness[i] > Region_Fitness[k][y])
            {
                Region_Fitness[k][y] = Sample_Fitness[i] ;
                Region_index[k][y] = i;
                break;
            }
         }
         i++;
    }
    int index=0;
     for(int j=0;j<Region_index.size();j++)
    {
        for(int k=0;k<Region_index[0].size();k++)
        {
             int ind=Region_index[j][k];
            // int temp=0;
             for(int y=0;y<sampleV[0].size();y++)
             {
                
                 goods[index][y]=sampleV[ind][y];
                //  cout<<goods[index][y]<<' ';
                //  if(goods[index][y]==1)
                //     temp++;
             }
             index++;
            //  cout<<temp<<' '<<endl;
        }
    }
    return SampleV_Region_Fitness;
}
int  Sum_Region(vector<vector<int> > fitness)
{
    int sum=0;
    for(int i=0;i<fitness.size();i++)
    {
        for(int j=0;j<fitness[0].size();j++)
        {
            sum+=fitness[i][j];
        }
    }
    return sum;
}
vector<double> Vision_Search_Expected_Value_1(vector<int> T_visit,vector<int> T_not_Visit)
{
    vector<double> V(T_visit.size());
    for(int i=0;i<T_visit.size();i++)
    {
        V[i]=(double)T_not_Visit[i]/T_visit[i];
    }
    return V;
}
void Vision_Search_Expected_Value_2(vector<double> &SampleV_Region_Fitness,int SampleV_quantity)
{
    for(int i=0;i<SampleV_Region_Fitness.size();i++)
    {
        SampleV_Region_Fitness[i]=SampleV_Region_Fitness[i]/SampleV_quantity;
    }
}
vector<double> Vision_Search_Expected_Value_3(vector<vector<int> >Region_fitness,int Fitness_Sum)//紀錄區域中最佳的 Fitness / 加總所有 Sample 的 Fitness
{
    vector<double>Best_Region_fitness(Region_fitness.size(),0);
    for(int i=0;i<Region_fitness.size();i++)
    {
        Best_Region_fitness[i]=(double)Region_fitness[i][0]/Fitness_Sum;
    }
    return Best_Region_fitness;
}
vector<double> Vision_Search_Expected_Value_Final(vector<double> f1,vector<double> f2,vector<double> f3)
{
    vector<double> f(f1.size());
    for(int i=0;i<f1.size();i++)
    {
        f[i]=f1[i]*f2[i]*f3[i];
    }
    return f;
}