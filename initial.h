
#include <stdio.h>
#include<stdlib.h>
#include<iostream>
#include<fstream>
#include<sstream>
#include <string.h>
#include<time.h>
#include<math.h>
#include<vector>
#define MR 0.15
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
class RUN{
    public:
        vector<int> AVG_Iter_Searcher_Fitness;
        int AVG_Best_Search_Fitness;
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
int  CUT_Region(vector<vector<int> >&P,int Quan,int pop,int Each_Region_Quantity)
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
    vector<vector<int> >Region_Cut;
    Region_Cut.resize(Quan);
    for(int i=0;i<Quan;i++){Region_Cut[i].resize(len);}
    while(k<Quan)
    {
        int check=-1;
        temp=0;
        for(int i=len-1;i>=0;i--)
        {
            if(check==1)
            {
                Region_Cut[k][i]=Region_Cut[k-1][i];
            }
            else{
                if(Region_Cut[k-1][i]==0 && temp==1)
                {
                    Region_Cut[k][i]=1;
                    temp=0;
                    check=1;
                }
            
                else if(Region_Cut[k-1][i]==0 && temp==0)
                {
                    Region_Cut[k][i]=1;
                    check=1;
                }
                else if(Region_Cut[k-1][i]==1 && temp==0)
                {
                    Region_Cut[k][i]=0;
                    temp=1;
                }
                else if(Region_Cut[k-1][i]==1 && temp==1)
                {
                    Region_Cut[k][i]=0;
                    temp=1;
                }
            }
            
            
        }
        k++;
    }
 
    for(int i=0;i<P.size();i++)
    {
        int temp=i/Each_Region_Quantity;
        for(int j=0;j<Region_Cut[0].size();j++)
        {
            P[i][j] = Region_Cut[temp][j];
        }
    }
   return len;
}
void AVG(vector<int> &P,int len){
    for(int i=0;i<P.size();i++)
    {
        P[i]=P[i]/len;
    }
}
void InformationOutput(int run,int bit,int region,int searcher,int sample,int Evaluation,int AVG_Fitness,vector<int> &Avg_Iter_Fitness)
{
    fstream file;
    file.open("SE.txt",ios::app);
    file<<"number of runs : "<<run<<endl;
    file<<"number of Evaluation : "<<Evaluation<<endl;
    file<<"number of Bit : "<<bit<<endl;
    file<<"number of regions : "<<region<<endl;
    file<<"number of Searcher : "<<searcher<<endl;
    file<<"number of Sample : "<<sample<<endl;
    file<<"Average Fitness : "<<AVG_Fitness<<endl;
    for(int i=0;i<Avg_Iter_Fitness.size();i++)
    {
        file<<i+1<<' '<<Avg_Iter_Fitness[i]<<endl;
    }
}