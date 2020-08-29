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
class SE_Init { 
public: 
	vector<vector<int> > Searcher ;//分類
    vector<vector<int> > Good ;//分類
    vector<vector<int> > SampleV ;//vision search產生的解
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
void Goods_update(vector<vector<int> >goods,vector<vector<int> >sampleV)
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
int main(int argc, const char * argv[]) {
    srand((unsigned int)time(NULL));
    int ITE = atoi(argv[1]);//iteration
    int run = atoi(argv[2]);
    int Bit = atoi(argv[3]);
    int Region = atoi(argv[4]);//how many regions
    int Searcher_Quan = atoi(argv[5]);//goods quantity(each region )
    int Good_Quan = atoi(argv[6]);//searcher qunatity(each reigon)
    SE_Init SE_ini;
    SE_ini.Searcher.resize(Region*Searcher_Quan,vector<int>(Bit));//幾個searcher and length is bits
    SE_ini.Good.resize( Region*Good_Quan ,vector<int>(Bit));
    SE_ini.SampleV.resize(Searcher_Quan*Region*Region*Good_Quan,vector<int>(Bit));
    cout<<SE_ini.Good.size()<<endl;
    random_zero_or_one(SE_ini.Searcher);
    random_zero_or_one(SE_ini.Good);
    int Region_len = CUT_Region(SE_ini.Good,Region,Region*Good_Quan);
    Region_len  = CUT_Region(SE_ini.Searcher,Region,Region*Searcher_Quan);
    crossover(SE_ini.SampleV,SE_ini.Searcher,SE_ini.Good,Region*Searcher_Quan,(Region*Good_Quan),Bit,Region_len);//crossover完成，儲存到sampleV
    
    // for(int i=0;i<SE_ini.Good.size();i++)
    // {
    //     cout<<i<<"   :  ";
    //     for(int j=0;j<Bit;j++)
    //     {
    //         cout<<SE_ini.Good[i][j]<<' ';
    //     }
    //     cout<<endl;
    // }
    Goods_update(SE_ini.Good,SE_ini.SampleV);
    for(int i=0;i<SE_ini.SampleV.size();i++)
    {
        cout<<i<<"   :  ";
        for(int j=0;j<Bit;j++)
        {
            cout<<SE_ini.SampleV[i][j]<<' ';
        }
        cout<<endl;
    }
}