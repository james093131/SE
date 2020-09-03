#include "Marketing_Research.h"

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

    RUN Runresult;
    Runresult.AVG_Iter_Searcher_Fitness.resize(ITE);
    Runresult.AVG_Best_Search_Fitness=0;

    double START,END;
    int r=0;
    START=clock();
    while(r<run)
    {
        FIN_INF.Best_Searcher_Fitness=0;
        random_zero_or_one(SE_ini.Searcher);
        random_zero_or_one(SE_ini.Good);
        //Searcher and Good random start finish
        
        int Region_len = CUT_Region(SE_ini.Good,Region,Region*Good_Quan,Good_Quan);
        Region_len  = CUT_Region(SE_ini.Searcher,Region,Region*Searcher_Quan,Searcher_Quan);
        //Region cut finish

        int iter=0;
        while(iter<ITE)
        {   
        crossover(SE_ini.SampleV,SE_ini.Searcher,SE_ini.Good,Region*Searcher_Quan,(Region*Good_Quan),Bit,Region_len);//crossover完成，儲存到sampleV
        SE_ini.SampleV_Fitness = Fitness_Calculate(SE_ini.Good,SE_ini.SampleV,Region);
        // for(int i=0;i<SE_ini.SampleV_Fitness.size();i++)
        // {
        //     cout<<i<<' '<<SE_ini.SampleV_Fitness[i]<<endl;
        // }
        //SampleV finish(crossover + mutation)

        //Vision Search -Expected Value start
        CLC_Expected_Value.SampleV_Region_Fitness = Region_fitness(SE_ini.Good,SE_ini.SampleV,SE_ini.SampleV_Fitness,SE_ini.Region_Fitness,Region,Good_Quan*Region);
        // for(int i=0;i<SE_ini.Region_Fitness.size();i++)
        // {
        //     for(int j=0;j<SE_ini.Region_Fitness[0].size();j++)
        //     {
        //         cout<<SE_ini.Region_Fitness[i][j]<<' ';
        //     }
        //     cout<<endl;
        // }
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
        // for(int i=0;i<Region;i++)
        // {
        //     cout<<CLC_Expected_Value.Expected_FitnessValue[i]<<' ';
        // }
        // cout<<endl;
        // for(int i=0;i<SE_ini.Region_Fitness.size();i++)
        // {
        //     cout<<i<<" : ";
        //     for(int j=0;j<SE_ini.Region_Fitness[0].size();j++)
        //     {
        //         cout<<SE_ini.Region_Fitness[i][j]<<' ';
        //     }
        //     cout<<endl;
        // }
        // cout<<"SampleV"<<endl;
        // for(int i=0;i<SE_ini.Good.size();i++)
        // {
        //     cout<<i<<endl;
        //     for(int j=0;j<SE_ini.Good[0].size();j++)
        //     {
        //         cout<<SE_ini.Good[i][j]<<' ';
        //     }
        //     cout<<endl;
        // }
        Marketing_Research(SE_ini.Searcher,CLC_Expected_Value.Expected_FitnessValue,SE_ini.Good,SE_ini.T_Visit,SE_ini.T_Not_Visit);
        Find_Best_Searcher(SE_ini.Searcher,FIN_INF.Best_Searcher_Fitness,FIN_INF.Best_Searcher);
        cout<<"Run"<<r+1<<'_'<<"Iteration"<<iter<<": "<<FIN_INF.Best_Searcher_Fitness<<endl;
        Runresult.AVG_Iter_Searcher_Fitness[iter]+=FIN_INF.Best_Searcher_Fitness;
        iter++;
        }
        Runresult.AVG_Best_Search_Fitness += FIN_INF.Best_Searcher_Fitness;
        r++;
    }
    END=clock();
    AVG(Runresult.AVG_Iter_Searcher_Fitness,run);
    Runresult.AVG_Best_Search_Fitness = Runresult.AVG_Best_Search_Fitness/run;
    InformationOutput(run,Bit,Region,Searcher_Quan*Region,Good_Quan*Region,ITE*Region*Searcher_Quan*Good_Quan,Runresult.AVG_Best_Search_Fitness,Runresult.AVG_Iter_Searcher_Fitness,START,END);

}