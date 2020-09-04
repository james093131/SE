#include "Vision_Search.h"
void Marketing_Research_Visit_Choice(vector<int>&Which_Region,vector<int>&Visit_Time,int size,vector<double> Expected_Value,int region)
{
    int i=0;
    while(i< size)
    {
        int c1=rand()%region;
        int c2=rand()%region;
        int c3=rand()%region;
        if(region >1)
        {
             while(c1==c2)
            {
                c2=rand()%region;
            }
        }
       
        if(region > 3)
        {
            while(c3==c2 || c3==c1)
            {
                c3=rand()%region;
            }  
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
void Marketing_Research(vector <vector<int> > &Searcher,vector<double> Expected_Value,vector<vector<int> > good,vector<int> &T_Visit,vector<int> &T_Not_Visit,int region)
{
    
    vector<int> Which_Region(Searcher.size());//Searcher要拿哪個Region
    vector<int> Visit_Time(region,0);
    Marketing_Research_Visit_Choice(Which_Region,Visit_Time,Searcher.size(),Expected_Value,region);
    for(int i=0;i<Searcher.size();i++)
    {
        // cout<<Which_Region[i]<<endl;
        int ind=Which_Region[i]*good.size()/region;
        // cout<<ind<<endl;
        for(int j=0;j<Searcher[0].size();j++)
        {
            Searcher[i][j]=good[ind][j];
        }
    }
    for(int i=0;i<Visit_Time.size();i++)//更新Visit or not Visit
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
void Find_Best_Searcher(vector<vector<int> >Searcher,int &BestOne_Fitness,vector<int> &Best_Searcher)
{
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
    if(BestOne > BestOne_Fitness)
    {
        BestOne_Fitness=BestOne;
        for(int i=0;i<Searcher[0].size();i++)
        {
            Best_Searcher[i]=Searcher[BestOne_ind][i];
        }
    }
}