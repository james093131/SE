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
void crossover(vector<vector<int> > &P,vector<vector<int> > P1,vector<vector<int> > P2,int P1pop,int P2pop,int ind)
{
    int i=0;
    int y=0;
    while(i<P1pop){

        for(int j=0;j<P2pop;j++)
        {
            int c1=rand()%ind;
            for(int k=0;k<ind;k++)
            {
           
                if(k>c1)
                {
                    P[y][k]=P2[j][k];
                } 
                else{
                    P[y][k]=P1[i][k]; 
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
int main(int argc, const char * argv[]) {
    srand((unsigned int)time(NULL));
    int ITE = atoi(argv[1]);//iteration
    int run = atoi(argv[2]);
    int Bit = atoi(argv[3]);
    int SE_Quan = atoi(argv[4]);//searcher qunatity
    int Good_Quan = atoi(argv[5]);//goods quantity
    SE_Init SE_ini;
    SE_ini.Searcher.resize(SE_Quan,vector<int>(Bit));//幾個searcher and length is bits
    SE_ini.Good.resize(SE_Quan*Good_Quan,vector<int>(Bit));
    SE_ini.SampleV.resize(SE_Quan*SE_Quan*Good_Quan,vector<int>(Bit));
   
    random_zero_or_one(SE_ini.Searcher);
    random_zero_or_one(SE_ini.Good);
    // for(int i=0;i<SE_ini.Good.size();i++)
    // {
    //     for(int j=0;j<SE_ini.Good[0].size();j++)
    //     {
    //         cout<<SE_ini.Good[i][j]<<' ';
    //     }
    //     cout<<endl;
    // }
    crossover(SE_ini.SampleV,SE_ini.Searcher,SE_ini.Good,SE_Quan,(SE_Quan*Good_Quan),Bit);
    for(int i=0;i<SE_Quan*SE_Quan*Good_Quan;i++)
    {
        cout<<i<<"   :  ";
        for(int j=0;j<Bit;j++)
        {
            cout<<SE_ini.SampleV[i][j]<<' ';
        }
        cout<<endl;
    }
}