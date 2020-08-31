#include <stdio.h>
#include<stdlib.h>
#include<iostream>
#include<fstream>
#include<sstream>
#include <string.h>
#include<time.h>
#include<math.h>
#include<vector>
#define MR 0.1
using namespace std;
class Final {
    public:
    vector<int> Best_Searcher;
    
    int Best_Searcher_Fitness;
};
class SE_Init { 
public: 
	vector<vector<int> > Searcher ;//分類
    vector<vector<int> > Good ;//分類
    vector<vector<int> > SampleV ;//vision search產生的解
    vector<int> SampleV_Fitness;
    vector<vector<int> >Region_Fitness;
    vector<int>T_Visit;
    vector<int>T_Not_Visit;
    
};
class Vision_Search{
    public:
        vector<double >SampleV_Region_Fitness;
        vector<double >Region_Best_Fitness;
        vector<double >Visit_Or_Not;
        vector<double >Expected_FitnessValue;
        int Fitness_Sum;
};
void random_zero_or_one(vector<vector<int> > &P)//隨機起始01
{
    int a=P.size();
    int b=P[0].size();
    for(int i=0;i<a;i++)
    {
        for(int j=0;j<b;j++)
        {
            int ran=rand()%2;
            P[i][j]=ran;
        }
    }
}
void mutation(vector<int> &P,int ind)//隨機選取一點做調換
{
     int c=rand()%ind;
    if(P[c]==0)
        P[c]=1;
    else
        P[c]=0;
}
void crossover(vector<vector<int> > &P,vector<vector<int> > P1,vector<vector<int> > P2,int P1pop,int P2pop,int ind,int region_len)
{
    int i=0;
    int y=0;
    while(i<P1pop){

        for(int j=0;j<P2pop;j++)
        {
            int c1=rand()%ind;
            for(int k=0;k<ind;k++)
            {
                if(k<region_len)
                {
                    P[y][k]=P2[j][k];
                }
                else{
                    if(k>c1)
                    {
                        P[y][k]=P2[j][k];
                    } 
                    else{
                        P[y][k]=P1[i][k]; 
                    }
                }
                
            }
            float x = (float) rand() / (RAND_MAX + 1.0);
            if(x<MR)
            {
                mutation(P[y],ind);
            }
            y++;
          
            
        }
        i++;
    }
}
vector<int> Fitness_Calculate(vector<vector<int> > &goods,vector<vector<int> >sampleV,int Region)
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
vector<double> Region_fitness(vector<vector<int> > &goods,vector<vector<int> >sampleV,vector<int> Sample_Fitness,vector<vector<int> >&Region_Fitness,int region)
{
    int i=0;
    vector<vector<int> > Region_index;
    vector<double> SampleV_Region_Fitness(region,0);
    Region_index.resize(Region_Fitness.size());
    for(int z=0;z<Region_index.size();z++) {Region_index[z].resize(Region_Fitness[0].size());}
    while(i<Sample_Fitness.size())
    {
        int k=i%region;
        SampleV_Region_Fitness[k]+=Sample_Fitness[i];
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
            int temp=0;
             for(int y=0;y<sampleV[0].size();y++)
             {
                
                 goods[index][y]=sampleV[ind][y];
                //  cout<<goods[index][y]<<' ';
                 if(goods[index][y]==1)
                    temp++;
             }
             index++;
            //  cout<<temp<<' '<<endl;
        }
    }
    return SampleV_Region_Fitness;
}
int  CUT_Region(vector<vector<int> >&P,int Quan,int pop)
{
    int temp=0;
    int k=1;
    int test=Quan;
    int len=0;
    while(test>=2)
    {
        test=test/2;
        len++;
    }
    for(int j=0;j<pop;j++)
    {
        for(int i=0;i<len;i++)
        {
        P[j][i]=0;
        }
    }
    
    while(k<pop)
    {
        int check=-1;
        temp=0;
        for(int i=len-1;i>=0;i--)
        {
            if(check==1)
            {
                P[k][i]=P[k-1][i];
            }
            else{
                if(P[k-1][i]==0 && temp==1)
                {
                    P[k][i]=1;
                    temp=0;
                    check=1;
                }
            
                else if(P[k-1][i]==0 && temp==0)
                {
                    P[k][i]=1;
                    check=1;
                }
                else if(P[k-1][i]==1 && temp==0)
                {
                    P[k][i]=0;
                    temp=1;
                }
                else if(P[k-1][i]==1 && temp==1)
                {
                    P[k][i]=0;
                    temp=1;
                }
            }
            
            
        }
        k++;
    }
   return len;
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
void Marketing_Research_Visit_Choice(vector<int>&Which_Region,vector<int>&Visit_Time,int size,vector<double> Expected_Value)
{
    int i=0;
    while(i< size)
    {
        int c1=rand()%Expected_Value.size();
        int c2=rand()%Expected_Value.size();
        int c3=rand()%Expected_Value.size();
        while(c1==c2)
        {
            c2=rand()%Expected_Value.size();
        }
        while(c3==c2 || c3==c1)
        {
            c3=rand()%Expected_Value.size();
        }       
        if(Expected_Value[c1] > Expected_Value[c2])
        {
            if(Expected_Value[c1] >Expected_Value[c3])
            {
                Which_Region[i]=c1;
                Visit_Time[c1]++;
            }
            else{
                Which_Region[i] = c3;
                Visit_Time[c3]++;
            }
        }
        else if(Expected_Value[c2] > Expected_Value[c1])
        {
            if( Expected_Value[c2] > Expected_Value[c3])
            {
                Which_Region[i]=c2;
                Visit_Time[c2]++;
            }
            else{
                Which_Region[i]=c3;
                Visit_Time[c3]++;
            }
        }
        i++;
    }
}
void Marketing_Research(vector <vector<int> > &Searcher,vector<double> Expected_Value,vector<vector<int> > good,vector<int> &T_Visit,vector<int> &T_Not_Visit)
{
    
    vector<int> Which_Region(Searcher.size());
    vector<int> Visit_Time(Expected_Value.size(),0);
    Marketing_Research_Visit_Choice(Which_Region,Visit_Time,Searcher.size(),Expected_Value);
    for(int i=0;i<Searcher.size();i++)
    {
        int ind=Which_Region[i]*4;
        // cout<<ind<<endl;
        for(int j=0;j<Searcher[0].size();j++)
        {
            Searcher[i][j]=good[ind][j];
        }
    }
    for(int i=0;i<Visit_Time.size();i++)
    {
        if(Visit_Time[i]==0)
        {
            T_Visit[i]=1;
            T_Not_Visit[i]++;
        }
        else {
            T_Visit[i]++;
            T_Not_Visit[i]=1;
        }
    }
}
vector<int> Find_Best_Searcher(vector<vector<int> >Searcher,int &BestOne_Fitness)
{
    vector<int> Best_Searcher(Searcher[0].size());
    vector<int> Fitness(Searcher.size());
    int BestOne_ind;
    int BestOne=-100;
    for(int i=0;i<Searcher.size();i++)
    {
        for(int j=0;j<Searcher[0].size();j++)
        {
            if(Searcher[i][j]==1)
                Fitness[i]++;
        }
        if(Fitness[i]>BestOne)
        {
            BestOne=Fitness[i];   
            BestOne_ind=i;
        }
    }
    BestOne_Fitness=BestOne;
    for(int i=0;i<Searcher[0].size();i++)
    {
        Best_Searcher[i]=Searcher[BestOne_ind][i];
    }
    return Best_Searcher;
}
int main(int argc, const char * argv[]) {
    
    srand((unsigned int)time(NULL));
    int ITE = atoi(argv[1]);//iteration
    int run = atoi(argv[2]);
    int Bit = atoi(argv[3]);
    int Region = atoi(argv[4]);//how many regions
    int Searcher_Quan = atoi(argv[5]);//goods quantity(each region )
    int Good_Quan = atoi(argv[6]);//searcher qunatity(each reigon)

    //input finish

    SE_Init SE_ini;
    SE_ini.Searcher.resize(Region*Searcher_Quan,vector<int>(Bit));//幾個searcher and length is bits
    SE_ini.Good.resize( Region*Good_Quan ,vector<int>(Bit));//幾個Good and length is bits 
    SE_ini.SampleV.resize(Searcher_Quan*Region*Region*Good_Quan,vector<int>(Bit));
    SE_ini.SampleV_Fitness.resize(Searcher_Quan*Region*Region*Good_Quan);
    SE_ini.Region_Fitness.resize(Region,vector<int>(Good_Quan,-100));
    SE_ini.T_Visit.resize(Region,1);
    SE_ini.T_Not_Visit.resize(Region,1);
    //SE_ini initial

    Vision_Search CLC_Expected_Value;
    CLC_Expected_Value.SampleV_Region_Fitness.resize(Region);
    CLC_Expected_Value.Region_Best_Fitness.resize(Region);
    CLC_Expected_Value.Visit_Or_Not.resize(Region);
    CLC_Expected_Value.Expected_FitnessValue.resize(Region);
    //CLC_Expected_Value initial 

    Final FIN_INF;
    FIN_INF.Best_Searcher.resize(Bit);
    //Final information initial


    random_zero_or_one(SE_ini.Searcher);
    random_zero_or_one(SE_ini.Good);
    //Searcher and Good random start finish

    int Region_len = CUT_Region(SE_ini.Good,Region,Region*Good_Quan);
    Region_len  = CUT_Region(SE_ini.Searcher,Region,Region*Searcher_Quan);
    //Region cut finish

    crossover(SE_ini.SampleV,SE_ini.Searcher,SE_ini.Good,Region*Searcher_Quan,(Region*Good_Quan),Bit,Region_len);//crossover完成，儲存到sampleV
    SE_ini.SampleV_Fitness = Fitness_Calculate(SE_ini.Good,SE_ini.SampleV,Region);
    //SampleV finish(crossover + mutation)

    //Vision Search -Expected Value start
    CLC_Expected_Value.SampleV_Region_Fitness = Region_fitness(SE_ini.Good,SE_ini.SampleV,SE_ini.SampleV_Fitness,SE_ini.Region_Fitness,Region);
    CLC_Expected_Value.Fitness_Sum = Sum_Region(SE_ini.Region_Fitness);
    CLC_Expected_Value.Visit_Or_Not =  Vision_Search_Expected_Value_1(SE_ini.T_Visit,SE_ini.T_Not_Visit);
    // for(int i=0;i<Region;i++)
    // {
    //     cout<<CLC_Expected_Value.Visit_Or_Not[i]<<' ';
    // }
    // cout<<endl;
    Vision_Search_Expected_Value_2(CLC_Expected_Value.SampleV_Region_Fitness,SE_ini.SampleV.size());
    // for(int i=0;i<CLC_Expected_Value.SampleV_Region_Fitness.size();i++)
    // {
    //     cout<<CLC_Expected_Value.SampleV_Region_Fitness[i]<<' ';
    // }
    // cout<<endl;



    CLC_Expected_Value.Region_Best_Fitness =  Vision_Search_Expected_Value_3(SE_ini.Region_Fitness,CLC_Expected_Value.Fitness_Sum);
    
    // for(int i=0;i<CLC_Expected_Value.Region_Best_Fitness.size();i++)
    // {
    //     cout<<CLC_Expected_Value.Region_Best_Fitness[i]<<' ';
    // }
    // cout<<endl;

    CLC_Expected_Value.Expected_FitnessValue = Vision_Search_Expected_Value_Final(CLC_Expected_Value.Visit_Or_Not,CLC_Expected_Value.SampleV_Region_Fitness,CLC_Expected_Value.Region_Best_Fitness);
    for(int i=0;i<Region;i++)
    {
        cout<<CLC_Expected_Value.Expected_FitnessValue[i]<<' ';
    }
    cout<<endl;
    for(int i=0;i<SE_ini.Region_Fitness.size();i++)
    {
        cout<<i<<" : ";
        for(int j=0;j<SE_ini.Region_Fitness[0].size();j++)
        {
            cout<<SE_ini.Region_Fitness[i][j]<<' ';
        }
        cout<<endl;
    }
    // for(int i=0;i<SE_ini.Good.size();i++)
    // {
    //     for(int j=0;j<SE_ini.Good[0].size();j++)
    //     {
    //         cout<<SE_ini.Good[i][j]<<' ';
    //     }
    //     cout<<endl;
    // }
    Marketing_Research(SE_ini.Searcher,CLC_Expected_Value.Expected_FitnessValue,SE_ini.Good,SE_ini.T_Visit,SE_ini.T_Not_Visit);
    FIN_INF.Best_Searcher =Find_Best_Searcher(SE_ini.Searcher,FIN_INF.Best_Searcher_Fitness);
    cout<<FIN_INF.Best_Searcher_Fitness<<endl;
}