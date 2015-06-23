#include <iostream>
#include <fstream>
#include <vector>
#include <time.h>
#include <algorithm>
#include <string>
#include <Windows.h>


using namespace std;

class data
{
public:
    int machineNum;
    int time;

    data(int machineNumx, int timex)
	{
		machineNum=machineNumx;
		time=timex;
	}
};

class algorithmINSA
{
public:
    int tasks;
    int machines;

    int tab[20001][10];
    int tabI[101][101];

    vector <data> sortByTime;
    int cmax;
    algorithmINSA() {} ///nie wiem czy tego nie moge wywalic



    void fillZeroI()
    {
        for(int i = 0 ; i <= tasks ; i++)
            for(int j = 0 ; j <= tasks ; j++)
                tabI[i][j] = 0;
    }

    void fillZero()
    {
        for(int i = 0 ; i <= tasks * machines ; i++)
            for (int j = 0 ; j < 10 ; j++)
                tab[i][j] = 0;
    }

    int sortByQ(int i)
 	{
		int nextColumn=0;
		int nextMachine=0;
		if(tab[i][9]!=0) return tab[i][9];
		else
		{
			if(tab[i][3]==0&&tab[i][5]==0)
				return tab[i][9]=tab[i][2];
			else
			{
                if(tab[i][5]!=0)
					nextMachine=sortByQ(tab[i][5]);
				if(tab[i][3]!=0)
					nextColumn=sortByQ(tab[i][3]);
				return tab[i][9]= nextMachine>nextColumn ? nextMachine+tab[i][2] : nextColumn+tab[i][2];
			}
        }
    }

    int sortByR(int i)
	{
		int previousColumn=0;
		int previousMachine=0;
		if(tab[i][8]!=0)
			return tab[i][8];
		else
		{
			if(tab[i][4]==0&&tab[i][6]==0)
				return tab[i][8]=tab[i][2];
			else
			{
				if(tab[i][4]!=0)
					previousColumn=sortByR(tab[i][4]);
				if(tab[i][6]!=0)
					previousMachine=sortByR(tab[i][6]);
				return tab[i][8]= (previousMachine>previousColumn) ? previousMachine+tab[i][2] : previousColumn+tab[i][2];
			}
		}
	}

    int max(int a, int b)
	{
        return (a<b) ? b : a;
    }

    	void tabData()
	{
		int machineNum = 0;
		for(int i =tabI[0][0] ; i <= machines*tasks ; i++)
		{
			machineNum = sortByTime[i].machineNum;
			int machine = tab[machineNum][1];
			tabI[machine][++tabI[machine][0]] = machineNum;
			tabI[0][0]++;
			int size=tabI[tab[machineNum][1]][0];
			int minimumTime=0;
			int timei=0;
			int min_i=0;

			if(size>1)
			{
				minimumTime=set_queue(0,machineNum,tabI[machine][1]);
				min_i=0;
				for(int i = 1; i<size-1;i++)
				{
					timei=set_queue(tabI[machine][i],machineNum,tabI[machine][i+1]);
					if(timei<minimumTime)
					{
						minimumTime=timei;
						min_i=i;
					}
				}
				timei=set_queue(tabI[machine][size-1],machineNum,0);
				if(timei<minimumTime)
				{
					minimumTime=timei;
					min_i=size-1;
				}
                CMax(minimumTime);
				if(min_i==0)
					kolejka(0,machineNum,tabI[machine][min_i+1]);
				else if(min_i==size-1)
					kolejka(tabI[machine][min_i],machineNum,0);
				else
					kolejka(tabI[machine][min_i],machineNum,tabI[machine][min_i+1]);
				//ustawienie poprawnej kolejnosci
				int temp=0;
				for(int i = min_i+1 ; i<size ; i++)
				{
					temp=tabI[machine][i];
					tabI[machine][i]=machineNum;
					machineNum=temp;
				}
				tabI[machine][size]=machineNum;
				return;
			}
		}
	}

		int set_queue(int poprzednik_kolejn, int srodek, int nastepnik_kolejn)
	{
		int czas = tab[srodek][2];
		czas+= (tab[poprzednik_kolejn][8]>tab[tab[srodek][4]][8]) ? tab[poprzednik_kolejn][8] : tab[tab[srodek][4]][8];
		czas+= (tab[nastepnik_kolejn][9]>tab[tab[srodek][3]][9]) ? tab[nastepnik_kolejn][9] : tab[tab[srodek][3]][9];
		return czas;
	}

    void kolejka(int l, int sr, int pr)
	{
		if(l) tab[l][5]=sr;
		tab[sr][5]=pr;
		tab[sr][6]=l;
		if(pr) tab[pr][6]=sr;
	}

     void CMax(int di)
    {
        cmax = max (cmax, di);
	}

};

int main()
{
    cout<<" INSA: "<<endl;

    int w;
    algorithmINSA insa;

    fstream plik_in;
    fstream plik_out;
    int temp_numer, temp_czas;

    plik_in.open("data.txt",ios::in);


   // for(int i=0 ; i<80 ; i++)
	//{
     // Wczytywanie instancji z pliku
     insa.cmax=0;
	 insa.sortByTime.clear();

	 insa.fillZero();
	 insa.fillZeroI();

	 char znak;
	 plik_in>>znak>>znak>>znak>>znak;
	 plik_in>>insa.tasks;
	 plik_in>>insa.machines;
     for(int i=1; i<=insa.tasks*insa.machines; i++)
	 {
	  insa.tab[i][0]=i;
	  plik_in>>insa.tab[i][1]>>insa.tab[i][2];
	  insa.sortByTime.push_back(data(i,insa.tab[i][2]));
     }
     plik_in.clear();
     //

	 for(int i=0; i <=insa.machines*insa.tasks ; i++)
	 {
 	  if(i%insa.machines!=0) insa.tab[i][3]=i+1; //nastepnik
	  if(i%insa.machines!=1) insa.tab[i][4]=i-1; //poprzednik z kol z indexem 4
	 }

	 for(int i = insa.machines*insa.tasks ; i>0 ; i--)
	 {
	  insa.sortByR(i);
	  insa.sortByQ(insa.machines*insa.tasks+1-i);
     }

     for (int i = 0; i<insa.sortByTime.size(); i++)
      for (int j=0; j<insa.sortByTime.size()-1; j++)
      {
       if (insa.sortByTime[j].time < insa.sortByTime[j+1].time)
        {
         temp_numer = insa.sortByTime[j+1].machineNum;
         temp_czas = insa.sortByTime[j+1].time;
         insa.sortByTime[j+1].machineNum = insa.sortByTime[j].machineNum;
         insa.sortByTime[j+1].time = insa.sortByTime[j].time;
         insa.sortByTime[j].time=temp_czas;
         insa.sortByTime[j].machineNum=temp_numer;
        }
       }

		while(insa.tabI[0][0]<insa.machines*insa.tasks)
		{
		 insa.tabData();
		 // Zerowanie RQ
		 for(int i = 1 ; i < insa.machines*insa.tasks+1 ; i++)
		 {
		  insa.tab[i][8]=0;
		  insa.tab[i][9]=0;
		 }
         //Wyliczanie RQ
         for(int i = insa.machines*insa.tasks ; i>0 ; i--)
	     {
	      insa.sortByR(i);
	      insa.sortByQ(insa.machines*insa.tasks+1-i);
         }
		}

    cout << "Obliczone cmax: " << insa.cmax << endl;

	//}



	system("pause");
	return 0;


}
