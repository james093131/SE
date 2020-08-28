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
};
void random_zero_or_one(vector<vector<int> > &P)
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
void mutation(vector<int>&P,int ind,int category,vector<int>lock)//隨機選取一點做調換
{
    int c=rand()%ind;
    if(lock[c]==0)
    {
        int a=rand()%category;
        P[c]=a;
    }
}
void crossover(vector<vector<int> > &P1,vector<vector<int> > &P2,int pop,int ind)
{
    int i=0;
    while(i<pop){
        int c1=rand()%ind;
        float t1 = (float) rand() / (RAND_MAX + 1.0);
        for(int k=0;k<ind;k++)
        {
           
                if(k>c1)
                {
                    int temp=P1[i][k];
                    P1[i][k]=P2[i+1][k];
                    P2[i+1][k]=temp;
                } 
                else{
                    
                    P1[i][k]=P1[i][k];
                    P2[i+1][k]=P2[i+1][k];
                }
            
           
        }
        float x = (float) rand() / (RAND_MAX + 1.0);
        float y = (float) rand() / (RAND_MAX + 1.0);
        if(x<MR)
        {
            mutation(P[i],ind,category,lock);
        }
        if(y<MR)
        {
             mutation(P[i+1],ind,category,lock);
        }
         i+=2;
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
    cout<<SE_ini.Searcher.size()<<' '<<endl;
    cout<<SE_ini.Searcher[0].size()<<' '<<endl;
    cout<<SE_ini.Good.size()<<' '<<endl;
    cout<<SE_ini.Good[0].size()<<' '<<endl;
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


}