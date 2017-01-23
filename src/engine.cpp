#include <iostream>
#include <vector>
#include <string>
#include <string.h>
#include <sstream>
#include <chrono>
#include <iterator>
#include <fstream>
#include "boost/tuple/tuple.hpp"
#include "boost/unordered_map.hpp"
#include <typeinfo>
#include <stdio.h>
#include <regex>
#include "headers.h"
#include <inttypes.h>
typedef int64_t __int64;

using namespace std;

__int64 EPs=0x123456789abc;
__int64 EOs=0x222222222222;
__int64 CPs=0x12345678;
__int64 COs=0x44444444;
__int64 CNs=0x123;
__int64 EP=EPs, EO=EOs, CP=CPs, CO=COs, CN=CNs;
vector < __int64 > slicemask;
vector < int > shiftin;
__int64 EPb, EOb, CPb, COb, CNb;
int temp=0;
vector < string > setups6gen;
vector < string > solutionss1;
vector < string > moves;
__int64 maskEPs1;
__int64 maskEOs1;
__int64 maskCPs1;
__int64 maskCOs1;
__int64 maskCNs1=0xfff;
string currentstep;
vector < string > solutionsLog;
vector < string > solutionsList;
vector < string > rotationsLog;
vector < string > orientationsLog;
vector < vector < string > > wherretf;
bool lbdone=0;
bool rbdone=0;
bool mirrorit=0;
vector < string > customsteps(8);
vector < string > customall;
vector < vector < __int64 > > customdets(8, vector <__int64> (8));

void EOfixer(void);
void CNfixer(void);
void prunes1(int, int, int);
void cleanScrambler(void);
void masks1(int, int);
string moveReverse(string);

string trim(const string& str)
{
    size_t first = str.find_first_not_of(' ');
    if (string::npos == first)
    {
        return str;
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}


void algparser(string str, vector <string> &thisone, int whichset)
{
    string pullin;
    if (whichset==2) //CMLL, roux is 2
    {
        ifstream infile;
        infile.open("CMLL.txt");
        while(getline(infile,pullin))
        {
            bool toggleme=0;
            string tuf;
            string part1="";
            string part2="";
            stringstream ss(pullin);
            while (ss >> tuf)
            {
                if (!toggleme && (tuf!="//"))
                {
                    part1+=(tuf+" ");
                }
                if (tuf=="//")
                {
                    toggleme=1;
                }
            }
            thisone.push_back(str+" "+part1);
        }
        infile.close();
    }

    if (whichset==1 || whichset==3) //petrus,zz
    {
        ifstream infile;
        infile.open("ZBLL.txt");
        while(getline(infile,pullin))
        {
            bool toggleme=0;
            string tuf;
            string part1="";
            string part2="";
            stringstream ss(pullin);
            while (ss >> tuf)
            {
                if (!toggleme && (tuf!="//"))
                {
                    part1+=(tuf+" ");
                }

                if (tuf=="//")
                {
                    toggleme=1;
                }
            }
            thisone.push_back(str+" "+part1);
        }
        infile.close();
    }

    return;
}

bool dosomething(int isscramble, int whatthing, string scr, bool firstdo)
{
    if (isscramble)
    {
        if (wherretf.size()<1)
        {
            vector <string> tempcolumn= {trim(scr),"scramble"};
            wherretf.push_back(tempcolumn);
            usedrotation.clear();
            usedorientation.clear();
        }
        else
        {
            vector <string> tempcolumn= {trim(scr),"moves"};
            wherretf.push_back(tempcolumn);
        }
    }
    else
    {
        if (whatthing>solutionsLog.size()) //valid number
        {
            cout<<"\n   no such solution generated\n\n";
        }
        else
        {
            if (firstdo)
            {
                vector <string> tempcolumn= {trim(rotationsLog[whatthing-1]),"inspection"};
                wherretf.push_back(tempcolumn);
                usedrotation=rotationsLog[whatthing-1];
                usedorientation=orientationsLog[whatthing-1];
            }



            if (currentstep=="lb" && !mirrorit && !rbdone)
            {
                lbdone=1;
                //cout<<"lbdone1";
                currentstep="lb";//
            }
            else if (currentstep=="lb" && mirrorit && !lbdone)
            {
                rbdone=1;
                //cout<<"rbdone1";
                currentstep="rb";//
            }
            else if (currentstep=="rb" && !mirrorit && lbdone)
            {
                rbdone=1;
                //cout<<"rbdone2";
                currentstep="rb";//
            }
            else if (currentstep=="rb" && mirrorit && rbdone)
            {
                lbdone=1;
                //cout<<"lbdone2";
                currentstep="lb";//
            }


            vector <string> tempcolumn= {trim(solutionsLog[whatthing-1]),currentstep};
            wherretf.push_back(tempcolumn);
            cout<<"\n   "<<currentstep<<" #"<<whatthing<<" applied\n\n";


            firstdo=0;
        }

    }
    cleanScrambler();
    return firstdo;
}


string remove_last_word_if(string sentence)
{
    sentence=trim(sentence);

    size_t index = sentence.find_last_of(" ");

    //string lastun=sentence.substr(index+1, sentence.length());
    if (sentence[index+1]=='U')
    {
        sentence.erase(sentence.begin()+index+1,sentence.end());
    }

    return sentence;

    //if (string::npos != pos) sentence.erase(pos, word.length());

    return sentence;
}


template <typename T>
string NumberToString(T pNumber)
{
    ostringstream oOStrStream;
    oOStrStream << pNumber;
    return oOStrStream.str();
}

size_t countWords(std::string s)
{
    string tuf;
    string fin;
    size_t out=0;
    stringstream ss(s);
    while (ss >> tuf)
    {
        if ((tuf[0]!='x')&&(tuf[0]!='y')&&(tuf[0]!='z'))
        {
            out++;
        }
    }
    return out;

    //  return s.empty() ? 0 : std::count_if(std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))), std::unique(s.begin(), s.end()), std::ptr_fun<int, int>(std::isspace)) + !std::isspace(*s.rbegin());
}

string cubeRevert(bool killscrtoo)
{
    if (killscrtoo)
    {
        wherretf.clear();
        lbdone=0;
        rbdone=0;
    }
    EP=EPs;
    EO=EOs;
    CP=CPs;
    CO=COs;
    CN=CNs;

    return "";
}



void showCube()
{
    cout << "\n   EP: 0x" <<std::hex<< EP << "\n"
         "   EO: 0x" << EO << "\n"
         "   CP: 0x" << CP << "\n"
         "   CO: 0x" << CO << "\n"
         "   CN: 0x" << CN << "\n\n";
    return;

}

void applyMove(string themove)
{

    if (themove == "U")
    {
        EP=(EP&0x0000ffffffff)|((EP&0xfff000000000)>>4)|((EP&0x000f00000000)<<12);
        EO=(EO&0x0000ffffffff)|((EO&0xfff000000000)>>4)|((EO&0x000f00000000)<<12);
        CP=(CP&0x0000ffff)|((CP&0xfff00000)>>4)|((CP&0x000f0000)<<12);
        CO=(CO&0x0000ffff)|((CO&0xfff00000)>>4)|((CO&0x000f0000)<<12);
    }
    if (themove == "U2")
    {
        EP=(EP&0x0000ffffffff)|((EP&0xff0000000000)>>8)|((EP&0x00ff00000000)<<8);
        EO=(EO&0x0000ffffffff)|((EO&0xff0000000000)>>8)|((EO&0x00ff00000000)<<8);
        CP=(CP&0x0000ffff)|((CP&0xff000000)>>8)|((CP&0x00ff0000)<<8);
        CO=(CO&0x0000ffff)|((CO&0xff000000)>>8)|((CO&0x00ff0000)<<8);

    }
    if (themove == "U'")
    {
        EP=(EP&0x0000ffffffff)|((EP&0x0fff00000000)<<4)|((EP&0xf00000000000)>>12);
        EO=(EO&0x0000ffffffff)|((EO&0x0fff00000000)<<4)|((EO&0xf00000000000)>>12);
        CP=(CP&0x0000ffff)|((CP&0x0fff0000)<<4)|((CP&0xf0000000)>>12);
        CO=(CO&0x0000ffff)|((CO&0x0fff0000)<<4)|((CO&0xf0000000)>>12);

    }
    if (themove == "D")
    {
        EP=(EP&0xffffffff0000)|((EP&0x000000000fff)<<4)|((EP&0x00000000f000)>>12);
        EO=(EO&0xffffffff0000)|((EO&0x000000000fff)<<4)|((EO&0x00000000f000)>>12);
        CP=(CP&0xffff0000)|((CP&0x00000fff)<<4)|((CP&0x0000f000)>>12);
        CO=(CO&0xffff0000)|((CO&0x00000fff)<<4)|((CO&0x0000f000)>>12);

    }
    if (themove == "D2")
    {
        EP=(EP&0xffffffff0000)|((EP&0x0000000000ff)<<8)|((EP&0x00000000ff00)>>8);
        EO=(EO&0xffffffff0000)|((EO&0x0000000000ff)<<8)|((EO&0x00000000ff00)>>8);
        CP=(CP&0xffff0000)|((CP&0x000000ff)<<8)|((CP&0x0000ff00)>>8);
        CO=(CO&0xffff0000)|((CO&0x000000ff)<<8)|((CO&0x0000ff00)>>8);

    }
    if (themove == "D'")
    {
        EP=(EP&0xffffffff0000)|((EP&0x00000000fff0)>>4)|((EP&0x00000000000f)<<12);
        EO=(EO&0xffffffff0000)|((EO&0x00000000fff0)>>4)|((EO&0x00000000000f)<<12);
        CP=(CP&0xffff0000)|((CP&0x0000fff0)>>4)|((CP&0x0000000f)<<12);
        CO=(CO&0xffff0000)|((CO&0x0000fff0)>>4)|((CO&0x0000000f)<<12);

    }
    if (themove == "R")
    {

        EP=(EP&0xf0fff00ff0ff)|((EP&0x0f000f000000)>>16)|((EP&0x000000f00000)<<20)|((EP&0x000000000f00)<<12);
        EO=(EO&0xf0fff00ff0ff)|((EO&0x0f000f000000)>>16)|((EO&0x000000f00000)<<20)|((EO&0x000000000f00)<<12);
        CP=(CP&0xf00ff00f)|((CP&0x0f000000)>>16)|((CP&0x00f00000)<<4)|((CP&0x00000f00)>>4)|((CP&0x000000f0)<<16);
        CO=(CO&0xf00ff00f)|((CO&0x0f000000)>>16)|((CO&0x00f00000)<<4)|((CO&0x00000f00)>>4)|((CO&0x000000f0)<<16);
        CO+=0x01100110;
        for (int o=28; o>=0; o=o-4)
        {

            if (!((~((CO&(0x7<<o))>>o)&0x7)<<60))
            {
                CO&=~(0xb<<o);
            }
        }
        CO+=0x00100100;
        for (int o=28; o>=0; o=o-4)
        {
            //if (o==24)
            //  {
            // cout<<std::hex<<CO<<endl<<(0xf<<o)<<endl<<(CO&(0xf<<o))<<endl<<((CO&(0xf<<o))>>(o+2))<<endl<<(!((CO&(0xf<<o))>>(o+2)))<<endl;
            //     }

            if (!((CO&(0xf<<o))>>(o+2)))
            {
                CO&=~(0xf<<o);
            }
            else if (!((~((CO&(0x7<<o))>>o)&0x7)<<60))
            {
                CO&=~(0xb<<o);
            }
        }

        // temp=CO2;
        // CO2=CO3+1;
        // CO3=CO7+2;
        // CO7=CO6+1;
        // CO6=temp+2;
    }
    if (themove == "R2")
    {
        EP=(EP&0xf0fff00ff0ff)|((EP&0x0f0000000000)>>32)|((EP&0x000000f00000)<<4)|((EP&0x00000f000000)>>4)|((EP&0x000000000f00)<<32);
        EO=(EO&0xf0fff00ff0ff)|((EO&0x0f0000000000)>>32)|((EO&0x000000f00000)<<4)|((EO&0x00000f000000)>>4)|((EO&0x000000000f00)<<32);
        CP=(CP&0xf00ff00f)|((CP&0x0f000000)>>20)|((CP&0x00f00000)>>12)|((CP&0x00000f00)<<12)|((CP&0x000000f0)<<20);
        CO=(CO&0xf00ff00f)|((CO&0x0f000000)>>20)|((CO&0x00f00000)>>12)|((CO&0x00000f00)<<12)|((CO&0x000000f0)<<20);
        // temp=CO6;
        // CO6=CO3;
        // CO3=temp;
        // temp=CO7;
        //CO7=CO2;
        // CO2=temp;
    }
    if (themove == "R'")
    {
        EP=(EP&0xf0fff00ff0ff)|((EP&0x0f0000000000)>>20)|((EP&0x000000f00000)>>12)|((EP&0x00000f000000)<<16)|((EP&0x000000000f00)<<16);
        EO=(EO&0xf0fff00ff0ff)|((EO&0x0f0000000000)>>20)|((EO&0x000000f00000)>>12)|((EO&0x00000f000000)<<16)|((EO&0x000000000f00)<<16);
        CP=(CP&0xf00ff00f)|((CP&0x0f000000)>>4)|((CP&0x00f00000)>>16)|((CP&0x00000f00)<<16)|((CP&0x000000f0)<<4);
        CO=(CO&0xf00ff00f)|((CO&0x0f000000)>>4)|((CO&0x00f00000)>>16)|((CO&0x00000f00)<<16)|((CO&0x000000f0)<<4);

        CO+=0x01100110;
        for (int o=28; o>=0; o=o-4)
        {
            if (!((~((CO&(0x7<<o))>>o)&0x7)<<60))
            {
                CO&=~(0xb<<o);
            }
        }
        CO+=0x00100100;
        for (int o=28; o>=0; o=o-4)
        {
            if (!((CO&(0xf<<o))>>(o+2)))
            {
                CO&=~(0xf<<o);
            }
            else if (!((~((CO&(0x7<<o))>>o)&0x7)<<60))
            {
                CO&=~(0xb<<o);
            }
        }

        //  temp=CO2;
        // CO2=CO6+1;
        // CO6=CO7+2;
        // CO7=CO3+1;
        // CO3=temp+2;
    }
    if (themove == "L")
    {
        EP=(EP&0xfff00ff0fff0)|((EP&0x000f000f0000)>>16)|((EP&0x0000f0000000)<<4)|((EP&0x00000000000f)<<28);
        EO=(EO&0xfff00ff0fff0)|((EO&0x000f000f0000)>>16)|((EO&0x0000f0000000)<<4)|((EO&0x00000000000f)<<28);
        CP=(CP&0x0ff00ff0)|((CP&0xf0000000)>>12)|((CP&0x000f0000)>>16)|((CP&0x0000f000)<<16)|((CP&0x0000000f)<<12);
        CO=(CO&0x0ff00ff0)|((CO&0xf0000000)>>12)|((CO&0x000f0000)>>16)|((CO&0x0000f000)<<16)|((CO&0x0000000f)<<12);

        CO+=0x10011001;
        for (int o=28; o>=0; o=o-4)
        {
            if (!((~((CO&(0x7<<o))>>o)&0x7)<<60))
            {
                CO&=~(0xb<<o);
            }
        }
        CO+=0x10000001;
        for (int o=28; o>=0; o=o-4)
        {
            if (!((CO&(0xf<<o))>>(o+2)))
            {
                CO&=~(0xf<<o);
            }
            else if (!((~((CO&(0x7<<o))>>o)&0x7)<<60))
            {
                CO&=~(0xb<<o);
            }
        }
        //  temp=CO1;
        //  CO1=CO5+2;
        //  CO5=CO8+1;
        //  CO8=CO4+2;
        // CO4=temp+1;
    }
    if (themove == "L2")
    {
        EP=(EP&0xfff00ff0fff0)|((EP&0x000f00000000)>>32)|((EP&0x0000f0000000)>>12)|((EP&0x00000000000f)<<32)|((EP&0x0000000f0000)<<12);
        EO=(EO&0xfff00ff0fff0)|((EO&0x000f00000000)>>32)|((EO&0x0000f0000000)>>12)|((EO&0x00000000000f)<<32)|((EO&0x0000000f0000)<<12);
        CP=(CP&0x0ff00ff0)|((CP&0xf0000000)>>28)|((CP&0x000f0000)>>4)|((CP&0x0000f000)<<4)|((CP&0x0000000f)<<28);
        CO=(CO&0x0ff00ff0)|((CO&0xf0000000)>>28)|((CO&0x000f0000)>>4)|((CO&0x0000f000)<<4)|((CO&0x0000000f)<<28);

        //  temp=CO1;
        //  CO1=CO8;
        //  CO8=temp;
        //  temp=CO4;
        //  CO4=CO5;
        //  CO5=temp;
    }
    if (themove == "L'")
    {
        EP=(EP&0xfff00ff0fff0)|((EP&0x0000000f000f)<<16)|((EP&0x0000f0000000)>>28)|((EP&0x000f00000000)>>4);
        EO=(EO&0xfff00ff0fff0)|((EO&0x0000000f000f)<<16)|((EO&0x0000f0000000)>>28)|((EO&0x000f00000000)>>4);
        CP=(CP&0x0ff00ff0)|((CP&0xf0000000)>>16)|((CP&0x000f0000)<<12)|((CP&0x0000f000)>>12)|((CP&0x0000000f)<<16);
        CO=(CO&0x0ff00ff0)|((CO&0xf0000000)>>16)|((CO&0x000f0000)<<12)|((CO&0x0000f000)>>12)|((CO&0x0000000f)<<16);

        CO+=0x10011001;
        for (int o=28; o>=0; o=o-4)
        {
            if (!((~((CO&(0x7<<o))>>o)&0x7)<<60))
            {
                CO&=~(0xb<<o);
            }
        }
        CO+=0x10000001;
        for (int o=28; o>=0; o=o-4)
        {
            if (!((CO&(0xf<<o))>>(o+2)))
            {
                CO&=~(0xf<<o);
            }
            else if (!((~((CO&(0x7<<o))>>o)&0x7)<<60))
            {
                CO&=~(0xb<<o);
            }
        }
        //  temp=CO1;
        //  CO1=CO4+2;
        //  CO4=CO8+1;
        //  CO8=CO5+2;
        //  CO5=temp+1;
    }
    if (themove == "F")
    {
        EP=(EP&0xff0fff00ff0f)|((EP&0x00f000f00000)>>16)|((EP&0x0000000f0000)<<20)|((EP&0x0000000000f0)<<12);
        EO=(EO&0xff0fff00ff0f)|((EO&0x00f000f00000)>>16)|((EO&0x0000000f0000)<<20)|((EO&0x0000000000f0)<<12);
        EO=EO^0x001000110010; //1
        CP=(CP&0xff00ff00)|((CP&0x00f00000)>>16)|((CP&0x000f0000)<<4)|((CP&0x000000f0)>>4)|((CP&0x0000000f)<<16);
        CO=(CO&0xff00ff00)|((CO&0x00f00000)>>16)|((CO&0x000f0000)<<4)|((CO&0x000000f0)>>4)|((CO&0x0000000f)<<16);

        CO+=0x00110011;
        for (int o=28; o>=0; o=o-4)
        {
            if (!((~((CO&(0x7<<o))>>o)&0x7)<<60))
            {
                CO&=~(0xb<<o);
            }
        }
        CO+=0x00010010;
        for (int o=28; o>=0; o=o-4)
        {
            if (!((CO&(0xf<<o))>>(o+2)))
            {
                CO&=~(0xf<<o);
            }
            else if (!((~((CO&(0x7<<o))>>o)&0x7)<<60))
            {
                CO&=~(0xb<<o);
            }
        }


        //  temp=CO3;
        //  CO3=CO4+1;
        //  CO4=CO8+2;
        //  CO8=CO7+1;
        //  CO7=temp+2;

    }
    if (themove == "F2")
    {
        EP=(EP&0xff0fff00ff0f)|((EP&0x00f000000000)>>32)|((EP&0x0000000f0000)<<4)|((EP&0x0000000000f0)<<32)|((EP&0x000000f00000)>>4);
        EO=(EO&0xff0fff00ff0f)|((EO&0x00f000000000)>>32)|((EO&0x0000000f0000)<<4)|((EO&0x0000000000f0)<<32)|((EO&0x000000f00000)>>4);
        CP=(CP&0xff00ff00)|((CP&0x00f00000)>>20)|((CP&0x000f0000)>>12)|((CP&0x000000f0)<<12)|((CP&0x0000000f)<<20);
        CO=(CO&0xff00ff00)|((CO&0x00f00000)>>20)|((CO&0x000f0000)>>12)|((CO&0x000000f0)<<12)|((CO&0x0000000f)<<20);
        //  temp=CO3;
        //  CO3=CO8;
        //  CO8=temp;
        //  temp=CO4;
        //  CO4=CO7;
        //  CO7=temp;
    }
    if (themove == "F'")
    {
        EP=(EP&0xff0fff00ff0f)|((EP&0x00f000000000)>>20)|((EP&0x0000000f0000)>>12)|((EP&0x000000f000f0)<<16); //10
        EO=(EO&0xff0fff00ff0f)|((EO&0x00f000000000)>>20)|((EO&0x0000000f0000)>>12)|((EO&0x000000f000f0)<<16);
        EO=EO^0x001000110010; //1
        CP=(CP&0xff00ff00)|((CP&0x00f00000)>>4)|((CP&0x000f0000)>>16)|((CP&0x000000f0)<<16)|((CP&0x0000000f)<<4); //13
        CO=(CO&0xff00ff00)|((CO&0x00f00000)>>4)|((CO&0x000f0000)>>16)|((CO&0x000000f0)<<16)|((CO&0x0000000f)<<4);

        CO+=0x00110011;
        for (int o=28; o>=0; o=o-4)
        {
            if (!((~((CO&(0x7<<o))>>o)&0x7)<<60))
            {
                CO&=~(0xb<<o);
            }
        }
        CO+=0x00010010;
        for (int o=28; o>=0; o=o-4)
        {
            if (!((CO&(0xf<<o))>>(o+2)))
            {
                CO&=~(0xf<<o);
            }
            else if (!((~((CO&(0x7<<o))>>o)&0x7)<<60))
            {
                CO&=~(0xb<<o);
            }
        }
        //  temp=CO3;
        //  CO3=CO7+1;
        //  CO7=CO8+2;
        //  CO8=CO4+1;
        //  CO4=temp+2;
    }
    if (themove == "B")
    {
        EP=(EP&0x0fff00ff0fff)|((EP&0xf000f0000000)>>16)|((EP&0x00000000f000)<<12)|((EP&0x00000f000000)<<20);
        EO=(EO&0x0fff00ff0fff)|((EO&0xf000f0000000)>>16)|((EO&0x00000000f000)<<12)|((EO&0x00000f000000)<<20);
        CP=(CP&0x00ff00ff)|((CP&0xf0000000)>>16)|((CP&0x0f000000)<<4)|((CP&0x0000f000)>>4)|((CP&0x00000f00)<<16);
        CO=(CO&0x00ff00ff)|((CO&0xf0000000)>>16)|((CO&0x0f000000)<<4)|((CO&0x0000f000)>>4)|((CO&0x00000f00)<<16);
        EO=EO^0x100011001000;

        CO+=0x11001100;
        for (int o=28; o>=0; o=o-4)
        {
            if (!((~((CO&(0x7<<o))>>o)&0x7)<<60))
            {
                CO&=~(0xb<<o);
            }
        }
        CO+=0x01001000;
        for (int o=28; o>=0; o=o-4)
        {
            if (!((CO&(0xf<<o))>>(o+2)))
            {
                CO&=~(0xf<<o);
            }
            else if (!((~((CO&(0x7<<o))>>o)&0x7)<<60))
            {
                CO&=~(0xb<<o);
            }
        }
        //   temp=CO1;
        //  CO1=CO2+1;
        //  CO2=CO6+2;
        //  CO6=CO5+1;
        //   CO5=temp+2;
    }
    if (themove == "B2")
    {
        EP=(EP&0x0fff00ff0fff)|((EP&0xf00000000000)>>32)|((EP&0x00000000f000)<<32)|((EP&0x00000f000000)<<4)|((EP&0x0000f0000000)>>4);
        EO=(EO&0x0fff00ff0fff)|((EO&0xf00000000000)>>32)|((EO&0x00000000f000)<<32)|((EO&0x00000f000000)<<4)|((EO&0x0000f0000000)>>4);
        CP=(CP&0x00ff00ff)|((CP&0xf0000000)>>20)|((CP&0x0f000000)>>12)|((CP&0x0000f000)<<12)|((CP&0x00000f00)<<20);
        CO=(CO&0x00ff00ff)|((CO&0xf0000000)>>20)|((CO&0x0f000000)>>12)|((CO&0x0000f000)<<12)|((CO&0x00000f00)<<20);
        //   temp=CO1;
        //   CO1=CO6;
        //  CO6=temp;
        //  temp=CO2;
        //  CO2=CO5;
        //  CO5=temp;

    }
    if (themove == "B'")
    {
        EP=(EP&0x0fff00ff0fff)|((EP&0x0000f000f000)<<16)|((EP&0xf00000000000)>>20)|((EP&0x00000f000000)>>12);
        EO=(EO&0x0fff00ff0fff)|((EO&0x0000f000f000)<<16)|((EO&0xf00000000000)>>20)|((EO&0x00000f000000)>>12);
        CP=(CP&0x00ff00ff)|((CP&0xf0000000)>>4)|((CP&0x0f000000)>>16)|((CP&0x0000f000)<<16)|((CP&0x00000f00)<<4);
        CO=(CO&0x00ff00ff)|((CO&0xf0000000)>>4)|((CO&0x0f000000)>>16)|((CO&0x0000f000)<<16)|((CO&0x00000f00)<<4);
        EO=EO^0x100011001000;

        CO+=0x11001100;
        for (int o=28; o>=0; o=o-4)
        {
            if (!((~((CO&(0x7<<o))>>o)&0x7)<<60))
            {
                CO&=~(0xb<<o);
            }
        }
        CO+=0x01001000;
        for (int o=28; o>=0; o=o-4)
        {
            if (!((CO&(0xf<<o))>>(o+2)))
            {
                CO&=~(0xf<<o);
            }
            else if (!((~((CO&(0x7<<o))>>o)&0x7)<<60))
            {
                CO&=~(0xb<<o);
            }
        }
        //   temp=CO1;
        //   CO1=CO5+1;
        //   CO5=CO6+2;
        //   CO6=CO2+1;
        // CO2=temp+2;
    }
    if (themove == "M")
    {
        EP=(EP&0x0f0fffff0f0f)|((EP&0xf00000000000)>>8)|((EP&0x00f000000000)>>32)|((EP&0x00000000f000)<<32)|((EP&0x0000000000f0)<<8);
        EO=(EO&0x0f0fffff0f0f)|((EO&0xf00000000000)>>8)|((EO&0x00f000000000)>>32)|((EO&0x00000000f000)<<32)|((EO&0x0000000000f0)<<8);

        CN=(CN&0x0f0)|((((CN&0x00f)+2)%6+1)<<8)|((CN&0xf00)>>8);

        slicemask= {0xf00000000000,0x00f000000000,0x00000000f000,0x0000000000f0};
        shiftin= {44,36,12,4};
        for (int i=0; i<slicemask.size(); i++)
        {
            if ((((EP&slicemask[i])>>shiftin[i])!=0x1) & (((EP&slicemask[i])>>shiftin[i])!=0x3) & (((EP&slicemask[i])>>shiftin[i])!=0x9) & (((EP&slicemask[i])>>shiftin[i])!=0xb))
            {
                EO=EO^(0x111111111111&slicemask[i]);
            }
        }

        slicemask= {0x0f0000000000,0x000f00000000,0x0000f0000000,0x00000f000000,0x000000f00000,0x0000000f0000,0x000000000f00,0x00000000000f};
        shiftin= {40,32,28,24,20,16,8,0};
        for (int i=0; i<slicemask.size(); i++)
        {
            if ((((EP&slicemask[i])>>shiftin[i])==0x1) | (((EP&slicemask[i])>>shiftin[i])==0x3) | (((EP&slicemask[i])>>shiftin[i])==0x9) | (((EP&slicemask[i])>>shiftin[i])==0xb))
            {
                EO=EO^(0x111111111111&slicemask[i]);
            }
        }
    }

    if (themove == "M2")
    {
        EP=(EP&0x0f0fffff0f0f)|((EP&0xf00000000000)>>40)|((EP&0x00f000000000)>>24)|((EP&0x00000000f000)<<24)|((EP&0x0000000000f0)<<40);
        EO=(EO&0x0f0fffff0f0f)|((EO&0xf00000000000)>>40)|((EO&0x00f000000000)>>24)|((EO&0x00000000f000)<<24)|((EO&0x0000000000f0)<<40);

        CN=(CN&0x0f0)|(((((CN&0xf00)>>8)+2)%6+1)<<8)|(((CN&0x00f)+2)%6+1);
    }

    if (themove == "M'")
    {
        EP=(EP&0x0f0fffff0f0f)|((EP&0xf00000000000)>>32)|((EP&0x00f000000000)<<8)|((EP&0x00000000f000)>>8)|((EP&0x0000000000f0)<<32);
        EO=(EO&0x0f0fffff0f0f)|((EO&0xf00000000000)>>32)|((EO&0x00f000000000)<<8)|((EO&0x00000000f000)>>8)|((EO&0x0000000000f0)<<32);

        CN=(CN&0x0f0)|((((CN&0xf00)>>8)+2)%6+1)|((CN&0x00f)<<8);

        slicemask= {0xf00000000000,0x00f000000000,0x00000000f000,0x0000000000f0};
        shiftin= {44,36,12,4};
        for (int i=0; i<slicemask.size(); i++)
        {
            if ((((EP&slicemask[i])>>shiftin[i])!=0x1) & (((EP&slicemask[i])>>shiftin[i])!=0x3) & (((EP&slicemask[i])>>shiftin[i])!=0x9) & (((EP&slicemask[i])>>shiftin[i])!=0xb))
            {
                EO=EO^(0x111111111111&slicemask[i]);
            }
        }

        slicemask= {0x0f0000000000,0x000f00000000,0x0000f0000000,0x00000f000000,0x000000f00000,0x0000000f0000,0x000000000f00,0x00000000000f};
        shiftin= {40,32,28,24,20,16,8,0};
        for (int i=0; i<slicemask.size(); i++)
        {
            if ((((EP&slicemask[i])>>shiftin[i])==0x1) | (((EP&slicemask[i])>>shiftin[i])==0x3) | (((EP&slicemask[i])>>shiftin[i])==0x9) | (((EP&slicemask[i])>>shiftin[i])==0xb))
            {
                EO=EO^(0x111111111111&slicemask[i]);
            }
        }
    }

    if (themove == "E")
    {
        EP=(EP&0xffff0000ffff)|((EP&0x00000fff0000)<<4)|((EP&0x0000f0000000)>>12);
        EO=(EO&0xffff0000ffff)|((EO&0x00000fff0000)<<4)|((EO&0x0000f0000000)>>12);

        CN=(CN&0xf00)|((((CN&0x0f0)>>4)+2)%6+1)|((CN&0x00f)<<4);

        slicemask= {0x0000f0000000,0x00000f000000,0x000000f00000,0x0000000f0000};
        shiftin= {28,24,20,16};
        for (int i=0; i<slicemask.size(); i++)
        {
            if ((((EP&slicemask[i])>>shiftin[i])!=0x5) & (((EP&slicemask[i])>>shiftin[i])!=0x6) & (((EP&slicemask[i])>>shiftin[i])!=0x7) & (((EP&slicemask[i])>>shiftin[i])!=0x8))
            {
                EO=EO^(0x111111111111&slicemask[i]);
            }
        }

        slicemask= {0xf00000000000,0x0f0000000000,0x00f000000000,0x000f00000000,0x00000000f000,0x000000000f00,0x0000000000f0,0x00000000000f};
        shiftin= {44,40,36,32,12,8,4,0};
        for (int i=0; i<slicemask.size(); i++)
        {
            if ((((EP&slicemask[i])>>shiftin[i])==0x5) | (((EP&slicemask[i])>>shiftin[i])==0x6) | (((EP&slicemask[i])>>shiftin[i])==0x7) | (((EP&slicemask[i])>>shiftin[i])==0x8))
            {
                EO=EO^(0x111111111111&slicemask[i]);
            }
        }
    }


    if (themove == "E2")
    {
        EP=(EP&0xffff0000ffff)|((EP&0x000000ff0000)<<8)|((EP&0x0000ff000000)>>8);
        EO=(EO&0xffff0000ffff)|((EO&0x000000ff0000)<<8)|((EO&0x0000ff000000)>>8);
        CN=(CN&0xf00)|(((((CN&0x0f0)>>4)+2)%6+1)<<4)|(((CN&0x00f)+2)%6+1);
    }


    if (themove == "E'")
    {
        EP=(EP&0xffff0000ffff)|((EP&0x0000fff00000)>>4)|((EP&0x0000000f0000)<<12);
        EO=(EO&0xffff0000ffff)|((EO&0x0000fff00000)>>4)|((EO&0x0000000f0000)<<12);

        CN=(CN&0xf00)|((((CN&0x00f)+2)%6+1)<<4)|((CN&0x0f0)>>4);

        slicemask= {0x0000f0000000,0x00000f000000,0x000000f00000,0x0000000f0000};
        shiftin= {28,24,20,16};
        for (int i=0; i<slicemask.size(); i++)
        {
            if ((((EP&slicemask[i])>>shiftin[i])!=0x5) & (((EP&slicemask[i])>>shiftin[i])!=0x6) & (((EP&slicemask[i])>>shiftin[i])!=0x7) & (((EP&slicemask[i])>>shiftin[i])!=0x8))
            {
                EO=EO^(0x111111111111&slicemask[i]);
            }
        }

        slicemask= {0xf00000000000,0x0f0000000000,0x00f000000000,0x000f00000000,0x00000000f000,0x000000000f00,0x0000000000f0,0x00000000000f};
        shiftin= {44,40,36,32,12,8,4,0};
        for (int i=0; i<slicemask.size(); i++)
        {
            if ((((EP&slicemask[i])>>shiftin[i])==0x5) | (((EP&slicemask[i])>>shiftin[i])==0x6) | (((EP&slicemask[i])>>shiftin[i])==0x7) | (((EP&slicemask[i])>>shiftin[i])==0x8))
            {
                EO=EO^(0x111111111111&slicemask[i]);
            }
        }
    }



    if (themove == "S")
    {
        EP=(EP&0xf0f0fffff0f0)|((EP&0x0f0000000000)>>32)|((EP&0x000f00000000)<<8)|((EP&0x000000000f00)>>8)|((EP&0x00000000000f)<<32);
        EO=(EO&0xf0f0fffff0f0)|((EO&0x0f0000000000)>>32)|((EO&0x000f00000000)<<8)|((EO&0x000000000f00)>>8)|((EO&0x00000000000f)<<32);
        CN=(CN&0x00f)|(((((CN&0x0f0)>>4)+2)%6+1)<<8)|((CN&0xf00)>>4);

        slicemask= {0x0f0000000000,0x000f00000000,0x000000000f00,0x00000000000f};
        shiftin= {40,32,8,0};
        for (int i=0; i<slicemask.size(); i++)
        {
            if ((((EP&slicemask[i])>>shiftin[i])!=0x2) & (((EP&slicemask[i])>>shiftin[i])!=0x4) & (((EP&slicemask[i])>>shiftin[i])!=0xa) & (((EP&slicemask[i])>>shiftin[i])!=0xc))
            {
                EO=EO^(0x111111111111&slicemask[i]);
            }
        }

        slicemask= {0xf00000000000,0x00f000000000,0x0000f0000000,0x00000f000000,0x000000f00000,0x0000000f0000,0x00000000f000,0x0000000000f0};
        shiftin= {44,36,28,24,20,16,12,4};
        for (int i=0; i<slicemask.size(); i++)
        {
            if ((((EP&slicemask[i])>>shiftin[i])==0x2) | (((EP&slicemask[i])>>shiftin[i])==0x4) | (((EP&slicemask[i])>>shiftin[i])==0xa) | (((EP&slicemask[i])>>shiftin[i])==0xc))
            {
                EO=EO^(0x111111111111&slicemask[i]);
            }
        }
    }

    if (themove == "S2")
    {
        EP=(EP&0xf0f0fffff0f0)|((EP&0x0f0000000000)>>40)|((EP&0x000f00000000)>>24)|((EP&0x000000000f00)<<24)|((EP&0x00000000000f)<<40);
        EO=(EO&0xf0f0fffff0f0)|((EO&0x0f0000000000)>>40)|((EO&0x000f00000000)>>24)|((EO&0x000000000f00)<<24)|((EO&0x00000000000f)<<40);
        CN=(CN&0x00f)|(((((CN&0xf00)>>8)+2)%6+1)<<8)|(((((CN&0x0f0)>>4)+2)%6+1)<<4);

    }

    if (themove == "S'")
    {
        EP=(EP&0xf0f0fffff0f0)|((EP&0x0f0000000000)>>8)|((EP&0x000f00000000)>>32)|((EP&0x000000000f00)<<32)|((EP&0x00000000000f)<<8);
        EO=(EO&0xf0f0fffff0f0)|((EO&0x0f0000000000)>>8)|((EO&0x000f00000000)>>32)|((EO&0x000000000f00)<<32)|((EO&0x00000000000f)<<8);
        CN=(CN&0x00f)|(((((CN&0xf00)>>8)+2)%6+1)<<4)|((CN&0x0f0)<<4);

        slicemask= {0x0f0000000000,0x000f00000000,0x000000000f00,0x00000000000f};
        shiftin= {40,32,8,0};
        for (int i=0; i<slicemask.size(); i++)
        {
            if ((((EP&slicemask[i])>>shiftin[i])!=0x2) & (((EP&slicemask[i])>>shiftin[i])!=0x4) & (((EP&slicemask[i])>>shiftin[i])!=0xa) & (((EP&slicemask[i])>>shiftin[i])!=0xc))
            {
                EO=EO^(0x111111111111&slicemask[i]);
            }
        }

        slicemask= {0xf00000000000,0x00f000000000,0x0000f0000000,0x00000f000000,0x000000f00000,0x0000000f0000,0x00000000f000,0x0000000000f0};
        shiftin= {44,36,28,24,20,16,12,4};
        for (int i=0; i<slicemask.size(); i++)
        {
            if ((((EP&slicemask[i])>>shiftin[i])==0x2) | (((EP&slicemask[i])>>shiftin[i])==0x4) | (((EP&slicemask[i])>>shiftin[i])==0xa) | (((EP&slicemask[i])>>shiftin[i])==0xc))
            {
                EO=EO^(0x111111111111&slicemask[i]);
            }
        }
    }



    if (themove == "x")
    {

        EP=(EP&0xf0fff00ff0ff)|((EP&0x0f000f000000)>>16)|((EP&0x000000f00000)<<20)|((EP&0x000000000f00)<<12);
        EO=(EO&0xf0fff00ff0ff)|((EO&0x0f000f000000)>>16)|((EO&0x000000f00000)<<20)|((EO&0x000000000f00)<<12);
        CP=(CP&0xf00ff00f)|((CP&0x0f000000)>>16)|((CP&0x00f00000)<<4)|((CP&0x00000f00)>>4)|((CP&0x000000f0)<<16);
        CO=(CO&0xf00ff00f)|((CO&0x0f000000)>>16)|((CO&0x00f00000)<<4)|((CO&0x00000f00)>>4)|((CO&0x000000f0)<<16);
        CN=(CN&0x0f0)|((((CN&0xf00)>>8)+2)%6+1)|((CN&0x00f)<<8);
        CO+=0x01100110;
        for (int o=28; o>=0; o=o-4)
        {

            if (!((~((CO&(0x7<<o))>>o)&0x7)<<60))
            {
                CO&=~(0xb<<o);
            }
        }
        CO+=0x00100100;
        for (int o=28; o>=0; o=o-4)
        {
            //if (o==24)
            //  {
            // cout<<std::hex<<CO<<endl<<(0xf<<o)<<endl<<(CO&(0xf<<o))<<endl<<((CO&(0xf<<o))>>(o+2))<<endl<<(!((CO&(0xf<<o))>>(o+2)))<<endl;
            //     }

            if (!((CO&(0xf<<o))>>(o+2)))
            {
                CO&=~(0xf<<o);
            }
            else if (!((~((CO&(0x7<<o))>>o)&0x7)<<60))
            {
                CO&=~(0xb<<o);
            }
        }

        EP=(EP&0x0f0fffff0f0f)|((EP&0xf00000000000)>>32)|((EP&0x00f000000000)<<8)|((EP&0x00000000f000)>>8)|((EP&0x0000000000f0)<<32);
        EO=(EO&0x0f0fffff0f0f)|((EO&0xf00000000000)>>32)|((EO&0x00f000000000)<<8)|((EO&0x00000000f000)>>8)|((EO&0x0000000000f0)<<32);

        slicemask= {0xf00000000000,0x00f000000000,0x00000000f000,0x0000000000f0};
        shiftin= {44,36,12,4};
        for (int i=0; i<slicemask.size(); i++)
        {
            if ((((EP&slicemask[i])>>shiftin[i])!=0x1) & (((EP&slicemask[i])>>shiftin[i])!=0x3) & (((EP&slicemask[i])>>shiftin[i])!=0x9) & (((EP&slicemask[i])>>shiftin[i])!=0xb))
            {
                EO=EO^(0x111111111111&slicemask[i]);
            }
        }

        slicemask= {0x0f0000000000,0x000f00000000,0x0000f0000000,0x00000f000000,0x000000f00000,0x0000000f0000,0x000000000f00,0x00000000000f};
        shiftin= {40,32,28,24,20,16,8,0};
        for (int i=0; i<slicemask.size(); i++)
        {
            if ((((EP&slicemask[i])>>shiftin[i])==0x1) | (((EP&slicemask[i])>>shiftin[i])==0x3) | (((EP&slicemask[i])>>shiftin[i])==0x9) | (((EP&slicemask[i])>>shiftin[i])==0xb))
            {
                EO=EO^(0x111111111111&slicemask[i]);
            }
        }

        EP=(EP&0xfff00ff0fff0)|((EP&0x0000000f000f)<<16)|((EP&0x0000f0000000)>>28)|((EP&0x000f00000000)>>4);
        EO=(EO&0xfff00ff0fff0)|((EO&0x0000000f000f)<<16)|((EO&0x0000f0000000)>>28)|((EO&0x000f00000000)>>4);
        CP=(CP&0x0ff00ff0)|((CP&0xf0000000)>>16)|((CP&0x000f0000)<<12)|((CP&0x0000f000)>>12)|((CP&0x0000000f)<<16);
        CO=(CO&0x0ff00ff0)|((CO&0xf0000000)>>16)|((CO&0x000f0000)<<12)|((CO&0x0000f000)>>12)|((CO&0x0000000f)<<16);

        CO+=0x10011001;
        for (int o=28; o>=0; o=o-4)
        {
            if (!((~((CO&(0x7<<o))>>o)&0x7)<<60))
            {
                CO&=~(0xb<<o);
            }
        }
        CO+=0x10000001;
        for (int o=28; o>=0; o=o-4)
        {
            if (!((CO&(0xf<<o))>>(o+2)))
            {
                CO&=~(0xf<<o);
            }
            else if (!((~((CO&(0x7<<o))>>o)&0x7)<<60))
            {
                CO&=~(0xb<<o);
            }
        }
    }


    if (themove == "x2")
    {

        EP=(EP&0xf0fff00ff0ff)|((EP&0x0f0000000000)>>32)|((EP&0x000000f00000)<<4)|((EP&0x00000f000000)>>4)|((EP&0x000000000f00)<<32);
        EO=(EO&0xf0fff00ff0ff)|((EO&0x0f0000000000)>>32)|((EO&0x000000f00000)<<4)|((EO&0x00000f000000)>>4)|((EO&0x000000000f00)<<32);
        CP=(CP&0xf00ff00f)|((CP&0x0f000000)>>20)|((CP&0x00f00000)>>12)|((CP&0x00000f00)<<12)|((CP&0x000000f0)<<20);
        CO=(CO&0xf00ff00f)|((CO&0x0f000000)>>20)|((CO&0x00f00000)>>12)|((CO&0x00000f00)<<12)|((CO&0x000000f0)<<20);

        EP=(EP&0x0f0fffff0f0f)|((EP&0xf00000000000)>>40)|((EP&0x00f000000000)>>24)|((EP&0x00000000f000)<<24)|((EP&0x0000000000f0)<<40);
        EO=(EO&0x0f0fffff0f0f)|((EO&0xf00000000000)>>40)|((EO&0x00f000000000)>>24)|((EO&0x00000000f000)<<24)|((EO&0x0000000000f0)<<40);
        CN=(CN&0x0f0)|(((((CN&0xf00)>>8)+2)%6+1)<<8)|(((CN&0x00f)+2)%6+1);

        EP=(EP&0xfff00ff0fff0)|((EP&0x000f00000000)>>32)|((EP&0x0000f0000000)>>12)|((EP&0x00000000000f)<<32)|((EP&0x0000000f0000)<<12);
        EO=(EO&0xfff00ff0fff0)|((EO&0x000f00000000)>>32)|((EO&0x0000f0000000)>>12)|((EO&0x00000000000f)<<32)|((EO&0x0000000f0000)<<12);
        CP=(CP&0x0ff00ff0)|((CP&0xf0000000)>>28)|((CP&0x000f0000)>>4)|((CP&0x0000f000)<<4)|((CP&0x0000000f)<<28);
        CO=(CO&0x0ff00ff0)|((CO&0xf0000000)>>28)|((CO&0x000f0000)>>4)|((CO&0x0000f000)<<4)|((CO&0x0000000f)<<28);



    }


    if (themove == "x'")
    {
        EP=(EP&0xf0fff00ff0ff)|((EP&0x0f0000000000)>>20)|((EP&0x000000f00000)>>12)|((EP&0x00000f000000)<<16)|((EP&0x000000000f00)<<16);
        EO=(EO&0xf0fff00ff0ff)|((EO&0x0f0000000000)>>20)|((EO&0x000000f00000)>>12)|((EO&0x00000f000000)<<16)|((EO&0x000000000f00)<<16);
        CP=(CP&0xf00ff00f)|((CP&0x0f000000)>>4)|((CP&0x00f00000)>>16)|((CP&0x00000f00)<<16)|((CP&0x000000f0)<<4);
        CO=(CO&0xf00ff00f)|((CO&0x0f000000)>>4)|((CO&0x00f00000)>>16)|((CO&0x00000f00)<<16)|((CO&0x000000f0)<<4);
        CN=(CN&0x0f0)|((((CN&0x00f)+2)%6+1)<<8)|((CN&0xf00)>>8);

        CO+=0x01100110;
        for (int o=28; o>=0; o=o-4)
        {
            if (!((~((CO&(0x7<<o))>>o)&0x7)<<60))
            {
                CO&=~(0xb<<o);
            }
        }
        CO+=0x00100100;
        for (int o=28; o>=0; o=o-4)
        {
            if (!((CO&(0xf<<o))>>(o+2)))
            {
                CO&=~(0xf<<o);
            }
            else if (!((~((CO&(0x7<<o))>>o)&0x7)<<60))
            {
                CO&=~(0xb<<o);
            }
        }



        EP=(EP&0x0f0fffff0f0f)|((EP&0xf00000000000)>>8)|((EP&0x00f000000000)>>32)|((EP&0x00000000f000)<<32)|((EP&0x0000000000f0)<<8);
        EO=(EO&0x0f0fffff0f0f)|((EO&0xf00000000000)>>8)|((EO&0x00f000000000)>>32)|((EO&0x00000000f000)<<32)|((EO&0x0000000000f0)<<8);

        slicemask= {0xf00000000000,0x00f000000000,0x00000000f000,0x0000000000f0};
        shiftin= {44,36,12,4};
        for (int i=0; i<slicemask.size(); i++)
        {
            if ((((EP&slicemask[i])>>shiftin[i])!=0x1) & (((EP&slicemask[i])>>shiftin[i])!=0x3) & (((EP&slicemask[i])>>shiftin[i])!=0x9) & (((EP&slicemask[i])>>shiftin[i])!=0xb))
            {
                EO=EO^(0x111111111111&slicemask[i]);
            }
        }

        slicemask= {0x0f0000000000,0x000f00000000,0x0000f0000000,0x00000f000000,0x000000f00000,0x0000000f0000,0x000000000f00,0x00000000000f};
        shiftin= {40,32,28,24,20,16,8,0};
        for (int i=0; i<slicemask.size(); i++)
        {
            if ((((EP&slicemask[i])>>shiftin[i])==0x1) | (((EP&slicemask[i])>>shiftin[i])==0x3) | (((EP&slicemask[i])>>shiftin[i])==0x9) | (((EP&slicemask[i])>>shiftin[i])==0xb))
            {
                EO=EO^(0x111111111111&slicemask[i]);
            }
        }
        EP=(EP&0xfff00ff0fff0)|((EP&0x000f000f0000)>>16)|((EP&0x0000f0000000)<<4)|((EP&0x00000000000f)<<28);
        EO=(EO&0xfff00ff0fff0)|((EO&0x000f000f0000)>>16)|((EO&0x0000f0000000)<<4)|((EO&0x00000000000f)<<28);
        CP=(CP&0x0ff00ff0)|((CP&0xf0000000)>>12)|((CP&0x000f0000)>>16)|((CP&0x0000f000)<<16)|((CP&0x0000000f)<<12);
        CO=(CO&0x0ff00ff0)|((CO&0xf0000000)>>12)|((CO&0x000f0000)>>16)|((CO&0x0000f000)<<16)|((CO&0x0000000f)<<12);

        CO+=0x10011001;
        for (int o=28; o>=0; o=o-4)
        {
            if (!((~((CO&(0x7<<o))>>o)&0x7)<<60))
            {
                CO&=~(0xb<<o);
            }
        }
        CO+=0x10000001;
        for (int o=28; o>=0; o=o-4)
        {
            if (!((CO&(0xf<<o))>>(o+2)))
            {
                CO&=~(0xf<<o);
            }
            else if (!((~((CO&(0x7<<o))>>o)&0x7)<<60))
            {
                CO&=~(0xb<<o);
            }
        }

    }

    if (themove == "y")
    {

        EP=(EP&0x0000ffffffff)|((EP&0xfff000000000)>>4)|((EP&0x000f00000000)<<12);
        EO=(EO&0x0000ffffffff)|((EO&0xfff000000000)>>4)|((EO&0x000f00000000)<<12);
        CP=(CP&0x0000ffff)|((CP&0xfff00000)>>4)|((CP&0x000f0000)<<12);
        CO=(CO&0x0000ffff)|((CO&0xfff00000)>>4)|((CO&0x000f0000)<<12);
        CN=(CN&0xf00)|((((CN&0x00f)+2)%6+1)<<4)|((CN&0x0f0)>>4);


        EP=(EP&0xffff0000ffff)|((EP&0x0000fff00000)>>4)|((EP&0x0000000f0000)<<12);
        EO=(EO&0xffff0000ffff)|((EO&0x0000fff00000)>>4)|((EO&0x0000000f0000)<<12);

        slicemask= {0x0000f0000000,0x00000f000000,0x000000f00000,0x0000000f0000};
        shiftin= {28,24,20,16};
        for (int i=0; i<slicemask.size(); i++)
        {
            if ((((EP&slicemask[i])>>shiftin[i])!=0x5) & (((EP&slicemask[i])>>shiftin[i])!=0x6) & (((EP&slicemask[i])>>shiftin[i])!=0x7) & (((EP&slicemask[i])>>shiftin[i])!=0x8))
            {
                EO=EO^(0x111111111111&slicemask[i]);
            }
        }

        slicemask= {0xf00000000000,0x0f0000000000,0x00f000000000,0x000f00000000,0x00000000f000,0x000000000f00,0x0000000000f0,0x00000000000f};
        shiftin= {44,40,36,32,12,8,4,0};
        for (int i=0; i<slicemask.size(); i++)
        {
            if ((((EP&slicemask[i])>>shiftin[i])==0x5) | (((EP&slicemask[i])>>shiftin[i])==0x6) | (((EP&slicemask[i])>>shiftin[i])==0x7) | (((EP&slicemask[i])>>shiftin[i])==0x8))
            {
                EO=EO^(0x111111111111&slicemask[i]);
            }
        }

        EP=(EP&0xffffffff0000)|((EP&0x00000000fff0)>>4)|((EP&0x00000000000f)<<12);
        EO=(EO&0xffffffff0000)|((EO&0x00000000fff0)>>4)|((EO&0x00000000000f)<<12);
        CP=(CP&0xffff0000)|((CP&0x0000fff0)>>4)|((CP&0x0000000f)<<12);
        CO=(CO&0xffff0000)|((CO&0x0000fff0)>>4)|((CO&0x0000000f)<<12);
    }


    if (themove == "y2")
    {
        EP=(EP&0x0000ffffffff)|((EP&0xff0000000000)>>8)|((EP&0x00ff00000000)<<8);
        EO=(EO&0x0000ffffffff)|((EO&0xff0000000000)>>8)|((EO&0x00ff00000000)<<8);
        CP=(CP&0x0000ffff)|((CP&0xff000000)>>8)|((CP&0x00ff0000)<<8);
        CO=(CO&0x0000ffff)|((CO&0xff000000)>>8)|((CO&0x00ff0000)<<8);

        EP=(EP&0xffff0000ffff)|((EP&0x000000ff0000)<<8)|((EP&0x0000ff000000)>>8);
        EO=(EO&0xffff0000ffff)|((EO&0x000000ff0000)<<8)|((EO&0x0000ff000000)>>8);
        CN=(CN&0xf00)|(((((CN&0x0f0)>>4)+2)%6+1)<<4)|(((CN&0x00f)+2)%6+1);

        EP=(EP&0xffffffff0000)|((EP&0x0000000000ff)<<8)|((EP&0x00000000ff00)>>8);
        EO=(EO&0xffffffff0000)|((EO&0x0000000000ff)<<8)|((EO&0x00000000ff00)>>8);
        CP=(CP&0xffff0000)|((CP&0x000000ff)<<8)|((CP&0x0000ff00)>>8);
        CO=(CO&0xffff0000)|((CO&0x000000ff)<<8)|((CO&0x0000ff00)>>8);

    }


    if (themove == "y'")
    {

        EP=(EP&0x0000ffffffff)|((EP&0x0fff00000000)<<4)|((EP&0xf00000000000)>>12);
        EO=(EO&0x0000ffffffff)|((EO&0x0fff00000000)<<4)|((EO&0xf00000000000)>>12);
        CP=(CP&0x0000ffff)|((CP&0x0fff0000)<<4)|((CP&0xf0000000)>>12);
        CO=(CO&0x0000ffff)|((CO&0x0fff0000)<<4)|((CO&0xf0000000)>>12);
        CN=(CN&0xf00)|((((CN&0x0f0)>>4)+2)%6+1)|((CN&0x00f)<<4);

        EP=(EP&0xffff0000ffff)|((EP&0x00000fff0000)<<4)|((EP&0x0000f0000000)>>12);
        EO=(EO&0xffff0000ffff)|((EO&0x00000fff0000)<<4)|((EO&0x0000f0000000)>>12);

        slicemask= {0x0000f0000000,0x00000f000000,0x000000f00000,0x0000000f0000};
        shiftin= {28,24,20,16};
        for (int i=0; i<slicemask.size(); i++)
        {
            if ((((EP&slicemask[i])>>shiftin[i])!=0x5) & (((EP&slicemask[i])>>shiftin[i])!=0x6) & (((EP&slicemask[i])>>shiftin[i])!=0x7) & (((EP&slicemask[i])>>shiftin[i])!=0x8))
            {
                EO=EO^(0x111111111111&slicemask[i]);
            }
        }

        slicemask= {0xf00000000000,0x0f0000000000,0x00f000000000,0x000f00000000,0x00000000f000,0x000000000f00,0x0000000000f0,0x00000000000f};
        shiftin= {44,40,36,32,12,8,4,0};
        for (int i=0; i<slicemask.size(); i++)
        {
            if ((((EP&slicemask[i])>>shiftin[i])==0x5) | (((EP&slicemask[i])>>shiftin[i])==0x6) | (((EP&slicemask[i])>>shiftin[i])==0x7) | (((EP&slicemask[i])>>shiftin[i])==0x8))
            {
                EO=EO^(0x111111111111&slicemask[i]);
            }
        }

        EP=(EP&0xffffffff0000)|((EP&0x000000000fff)<<4)|((EP&0x00000000f000)>>12);
        EO=(EO&0xffffffff0000)|((EO&0x000000000fff)<<4)|((EO&0x00000000f000)>>12);
        CP=(CP&0xffff0000)|((CP&0x00000fff)<<4)|((CP&0x0000f000)>>12);
        CO=(CO&0xffff0000)|((CO&0x00000fff)<<4)|((CO&0x0000f000)>>12);

    }

    if (themove == "z")
    {

        EP=(EP&0xff0fff00ff0f)|((EP&0x00f000f00000)>>16)|((EP&0x0000000f0000)<<20)|((EP&0x0000000000f0)<<12);
        EO=(EO&0xff0fff00ff0f)|((EO&0x00f000f00000)>>16)|((EO&0x0000000f0000)<<20)|((EO&0x0000000000f0)<<12);
        EO=EO^0x001000110010; //1
        CP=(CP&0xff00ff00)|((CP&0x00f00000)>>16)|((CP&0x000f0000)<<4)|((CP&0x000000f0)>>4)|((CP&0x0000000f)<<16);
        CO=(CO&0xff00ff00)|((CO&0x00f00000)>>16)|((CO&0x000f0000)<<4)|((CO&0x000000f0)>>4)|((CO&0x0000000f)<<16);
        CN=(CN&0x00f)|(((((CN&0x0f0)>>4)+2)%6+1)<<8)|((CN&0xf00)>>4);

        CO+=0x00110011;
        for (int o=28; o>=0; o=o-4)
        {
            if (!((~((CO&(0x7<<o))>>o)&0x7)<<60))
            {
                CO&=~(0xb<<o);
            }
        }
        CO+=0x00010010;
        for (int o=28; o>=0; o=o-4)
        {
            if (!((CO&(0xf<<o))>>(o+2)))
            {
                CO&=~(0xf<<o);
            }
            else if (!((~((CO&(0x7<<o))>>o)&0x7)<<60))
            {
                CO&=~(0xb<<o);
            }
        }

        EP=(EP&0xf0f0fffff0f0)|((EP&0x0f0000000000)>>32)|((EP&0x000f00000000)<<8)|((EP&0x000000000f00)>>8)|((EP&0x00000000000f)<<32);
        EO=(EO&0xf0f0fffff0f0)|((EO&0x0f0000000000)>>32)|((EO&0x000f00000000)<<8)|((EO&0x000000000f00)>>8)|((EO&0x00000000000f)<<32);

        slicemask= {0x0f0000000000,0x000f00000000,0x000000000f00,0x00000000000f};
        shiftin= {40,32,8,0};
        for (int i=0; i<slicemask.size(); i++)
        {
            if ((((EP&slicemask[i])>>shiftin[i])!=0x2) & (((EP&slicemask[i])>>shiftin[i])!=0x4) & (((EP&slicemask[i])>>shiftin[i])!=0xa) & (((EP&slicemask[i])>>shiftin[i])!=0xc))
            {
                EO=EO^(0x111111111111&slicemask[i]);
            }
        }

        slicemask= {0xf00000000000,0x00f000000000,0x0000f0000000,0x00000f000000,0x000000f00000,0x0000000f0000,0x00000000f000,0x0000000000f0};
        shiftin= {44,36,28,24,20,16,12,4};
        for (int i=0; i<slicemask.size(); i++)
        {
            if ((((EP&slicemask[i])>>shiftin[i])==0x2) | (((EP&slicemask[i])>>shiftin[i])==0x4) | (((EP&slicemask[i])>>shiftin[i])==0xa) | (((EP&slicemask[i])>>shiftin[i])==0xc))
            {
                EO=EO^(0x111111111111&slicemask[i]);
            }
        }

        EP=(EP&0x0fff00ff0fff)|((EP&0x0000f000f000)<<16)|((EP&0xf00000000000)>>20)|((EP&0x00000f000000)>>12);
        EO=(EO&0x0fff00ff0fff)|((EO&0x0000f000f000)<<16)|((EO&0xf00000000000)>>20)|((EO&0x00000f000000)>>12);
        CP=(CP&0x00ff00ff)|((CP&0xf0000000)>>4)|((CP&0x0f000000)>>16)|((CP&0x0000f000)<<16)|((CP&0x00000f00)<<4);
        CO=(CO&0x00ff00ff)|((CO&0xf0000000)>>4)|((CO&0x0f000000)>>16)|((CO&0x0000f000)<<16)|((CO&0x00000f00)<<4);
        EO=EO^0x100011001000;

        CO+=0x11001100;
        for (int o=28; o>=0; o=o-4)
        {
            if (!((~((CO&(0x7<<o))>>o)&0x7)<<60))
            {
                CO&=~(0xb<<o);
            }
        }
        CO+=0x01001000;
        for (int o=28; o>=0; o=o-4)
        {
            if (!((CO&(0xf<<o))>>(o+2)))
            {
                CO&=~(0xf<<o);
            }
            else if (!((~((CO&(0x7<<o))>>o)&0x7)<<60))
            {
                CO&=~(0xb<<o);
            }
        }


    }


    if (themove == "z2")
    {

        EP=(EP&0xff0fff00ff0f)|((EP&0x00f000000000)>>32)|((EP&0x0000000f0000)<<4)|((EP&0x0000000000f0)<<32)|((EP&0x000000f00000)>>4);
        EO=(EO&0xff0fff00ff0f)|((EO&0x00f000000000)>>32)|((EO&0x0000000f0000)<<4)|((EO&0x0000000000f0)<<32)|((EO&0x000000f00000)>>4);
        CP=(CP&0xff00ff00)|((CP&0x00f00000)>>20)|((CP&0x000f0000)>>12)|((CP&0x000000f0)<<12)|((CP&0x0000000f)<<20);
        CO=(CO&0xff00ff00)|((CO&0x00f00000)>>20)|((CO&0x000f0000)>>12)|((CO&0x000000f0)<<12)|((CO&0x0000000f)<<20);


        EP=(EP&0xf0f0fffff0f0)|((EP&0x0f0000000000)>>40)|((EP&0x000f00000000)>>24)|((EP&0x000000000f00)<<24)|((EP&0x00000000000f)<<40);
        EO=(EO&0xf0f0fffff0f0)|((EO&0x0f0000000000)>>40)|((EO&0x000f00000000)>>24)|((EO&0x000000000f00)<<24)|((EO&0x00000000000f)<<40);
        CN=(CN&0x00f)|(((((CN&0xf00)>>8)+2)%6+1)<<8)|(((((CN&0x0f0)>>4)+2)%6+1)<<4);


        EP=(EP&0x0fff00ff0fff)|((EP&0xf00000000000)>>32)|((EP&0x00000000f000)<<32)|((EP&0x00000f000000)<<4)|((EP&0x0000f0000000)>>4);
        EO=(EO&0x0fff00ff0fff)|((EO&0xf00000000000)>>32)|((EO&0x00000000f000)<<32)|((EO&0x00000f000000)<<4)|((EO&0x0000f0000000)>>4);
        CP=(CP&0x00ff00ff)|((CP&0xf0000000)>>20)|((CP&0x0f000000)>>12)|((CP&0x0000f000)<<12)|((CP&0x00000f00)<<20);
        CO=(CO&0x00ff00ff)|((CO&0xf0000000)>>20)|((CO&0x0f000000)>>12)|((CO&0x0000f000)<<12)|((CO&0x00000f00)<<20);


    }


    if (themove == "z'")
    {


        EP=(EP&0xff0fff00ff0f)|((EP&0x00f000000000)>>20)|((EP&0x0000000f0000)>>12)|((EP&0x000000f000f0)<<16); //10
        EO=(EO&0xff0fff00ff0f)|((EO&0x00f000000000)>>20)|((EO&0x0000000f0000)>>12)|((EO&0x000000f000f0)<<16);
        EO=EO^0x001000110010; //1
        CP=(CP&0xff00ff00)|((CP&0x00f00000)>>4)|((CP&0x000f0000)>>16)|((CP&0x000000f0)<<16)|((CP&0x0000000f)<<4); //13
        CO=(CO&0xff00ff00)|((CO&0x00f00000)>>4)|((CO&0x000f0000)>>16)|((CO&0x000000f0)<<16)|((CO&0x0000000f)<<4);
        CN=(CN&0x00f)|(((((CN&0xf00)>>8)+2)%6+1)<<4)|((CN&0x0f0)<<4);

        CO+=0x00110011;
        for (int o=28; o>=0; o=o-4)
        {
            if (!((~((CO&(0x7<<o))>>o)&0x7)<<60))
            {
                CO&=~(0xb<<o);
            }
        }
        CO+=0x00010010;
        for (int o=28; o>=0; o=o-4)
        {
            if (!((CO&(0xf<<o))>>(o+2)))
            {
                CO&=~(0xf<<o);
            }
            else if (!((~((CO&(0x7<<o))>>o)&0x7)<<60))
            {
                CO&=~(0xb<<o);
            }
        }

        EP=(EP&0xf0f0fffff0f0)|((EP&0x0f0000000000)>>8)|((EP&0x000f00000000)>>32)|((EP&0x000000000f00)<<32)|((EP&0x00000000000f)<<8);
        EO=(EO&0xf0f0fffff0f0)|((EO&0x0f0000000000)>>8)|((EO&0x000f00000000)>>32)|((EO&0x000000000f00)<<32)|((EO&0x00000000000f)<<8);

        slicemask= {0x0f0000000000,0x000f00000000,0x000000000f00,0x00000000000f};
        shiftin= {40,32,8,0};
        for (int i=0; i<slicemask.size(); i++)
        {
            if ((((EP&slicemask[i])>>shiftin[i])!=0x2) & (((EP&slicemask[i])>>shiftin[i])!=0x4) & (((EP&slicemask[i])>>shiftin[i])!=0xa) & (((EP&slicemask[i])>>shiftin[i])!=0xc))
            {
                EO=EO^(0x111111111111&slicemask[i]);
            }
        }

        slicemask= {0xf00000000000,0x00f000000000,0x0000f0000000,0x00000f000000,0x000000f00000,0x0000000f0000,0x00000000f000,0x0000000000f0};
        shiftin= {44,36,28,24,20,16,12,4};
        for (int i=0; i<slicemask.size(); i++)
        {
            if ((((EP&slicemask[i])>>shiftin[i])==0x2) | (((EP&slicemask[i])>>shiftin[i])==0x4) | (((EP&slicemask[i])>>shiftin[i])==0xa) | (((EP&slicemask[i])>>shiftin[i])==0xc))
            {
                EO=EO^(0x111111111111&slicemask[i]);
            }
        }

        EP=(EP&0x0fff00ff0fff)|((EP&0xf000f0000000)>>16)|((EP&0x00000000f000)<<12)|((EP&0x00000f000000)<<20);
        EO=(EO&0x0fff00ff0fff)|((EO&0xf000f0000000)>>16)|((EO&0x00000000f000)<<12)|((EO&0x00000f000000)<<20);
        CP=(CP&0x00ff00ff)|((CP&0xf0000000)>>16)|((CP&0x0f000000)<<4)|((CP&0x0000f000)>>4)|((CP&0x00000f00)<<16);
        CO=(CO&0x00ff00ff)|((CO&0xf0000000)>>16)|((CO&0x0f000000)<<4)|((CO&0x0000f000)>>4)|((CO&0x00000f00)<<16);
        EO=EO^0x100011001000;

        CO+=0x11001100;
        for (int o=28; o>=0; o=o-4)
        {
            if (!((~((CO&(0x7<<o))>>o)&0x7)<<60))
            {
                CO&=~(0xb<<o);
            }
        }
        CO+=0x01001000;
        for (int o=28; o>=0; o=o-4)
        {
            if (!((CO&(0xf<<o))>>(o+2)))
            {
                CO&=~(0xf<<o);
            }
            else if (!((~((CO&(0x7<<o))>>o)&0x7)<<60))
            {
                CO&=~(0xb<<o);
            }
        }

    }


    if (themove == "r")
    {

        EP=(EP&0xf0fff00ff0ff)|((EP&0x0f000f000000)>>16)|((EP&0x000000f00000)<<20)|((EP&0x000000000f00)<<12);
        EO=(EO&0xf0fff00ff0ff)|((EO&0x0f000f000000)>>16)|((EO&0x000000f00000)<<20)|((EO&0x000000000f00)<<12);
        CP=(CP&0xf00ff00f)|((CP&0x0f000000)>>16)|((CP&0x00f00000)<<4)|((CP&0x00000f00)>>4)|((CP&0x000000f0)<<16);
        CO=(CO&0xf00ff00f)|((CO&0x0f000000)>>16)|((CO&0x00f00000)<<4)|((CO&0x00000f00)>>4)|((CO&0x000000f0)<<16);
        CN=(CN&0x0f0)|((((CN&0xf00)>>8)+2)%6+1)|((CN&0x00f)<<8);

        CO+=0x01100110;
        for (int o=28; o>=0; o=o-4)
        {

            if (!((~((CO&(0x7<<o))>>o)&0x7)<<60))
            {
                CO&=~(0xb<<o);
            }
        }
        CO+=0x00100100;
        for (int o=28; o>=0; o=o-4)
        {
            //if (o==24)
            //  {
            // cout<<std::hex<<CO<<endl<<(0xf<<o)<<endl<<(CO&(0xf<<o))<<endl<<((CO&(0xf<<o))>>(o+2))<<endl<<(!((CO&(0xf<<o))>>(o+2)))<<endl;
            //     }

            if (!((CO&(0xf<<o))>>(o+2)))
            {
                CO&=~(0xf<<o);
            }
            else if (!((~((CO&(0x7<<o))>>o)&0x7)<<60))
            {
                CO&=~(0xb<<o);
            }
        }

        EP=(EP&0x0f0fffff0f0f)|((EP&0xf00000000000)>>32)|((EP&0x00f000000000)<<8)|((EP&0x00000000f000)>>8)|((EP&0x0000000000f0)<<32);
        EO=(EO&0x0f0fffff0f0f)|((EO&0xf00000000000)>>32)|((EO&0x00f000000000)<<8)|((EO&0x00000000f000)>>8)|((EO&0x0000000000f0)<<32);

        slicemask= {0xf00000000000,0x00f000000000,0x00000000f000,0x0000000000f0};
        shiftin= {44,36,12,4};
        for (int i=0; i<slicemask.size(); i++)
        {
            if ((((EP&slicemask[i])>>shiftin[i])!=0x1) & (((EP&slicemask[i])>>shiftin[i])!=0x3) & (((EP&slicemask[i])>>shiftin[i])!=0x9) & (((EP&slicemask[i])>>shiftin[i])!=0xb))
            {
                EO=EO^(0x111111111111&slicemask[i]);
            }
        }

        slicemask= {0x0f0000000000,0x000f00000000,0x0000f0000000,0x00000f000000,0x000000f00000,0x0000000f0000,0x000000000f00,0x00000000000f};
        shiftin= {40,32,28,24,20,16,8,0};
        for (int i=0; i<slicemask.size(); i++)
        {
            if ((((EP&slicemask[i])>>shiftin[i])==0x1) | (((EP&slicemask[i])>>shiftin[i])==0x3) | (((EP&slicemask[i])>>shiftin[i])==0x9) | (((EP&slicemask[i])>>shiftin[i])==0xb))
            {
                EO=EO^(0x111111111111&slicemask[i]);
            }
        }


    }


    if (themove == "r2")
    {

        EP=(EP&0xf0fff00ff0ff)|((EP&0x0f0000000000)>>32)|((EP&0x000000f00000)<<4)|((EP&0x00000f000000)>>4)|((EP&0x000000000f00)<<32);
        EO=(EO&0xf0fff00ff0ff)|((EO&0x0f0000000000)>>32)|((EO&0x000000f00000)<<4)|((EO&0x00000f000000)>>4)|((EO&0x000000000f00)<<32);
        CP=(CP&0xf00ff00f)|((CP&0x0f000000)>>20)|((CP&0x00f00000)>>12)|((CP&0x00000f00)<<12)|((CP&0x000000f0)<<20);
        CO=(CO&0xf00ff00f)|((CO&0x0f000000)>>20)|((CO&0x00f00000)>>12)|((CO&0x00000f00)<<12)|((CO&0x000000f0)<<20);
        CN=(CN&0x0f0)|(((((CN&0xf00)>>8)+2)%6+1)<<8)|(((CN&0x00f)+2)%6+1);

        EP=(EP&0x0f0fffff0f0f)|((EP&0xf00000000000)>>40)|((EP&0x00f000000000)>>24)|((EP&0x00000000f000)<<24)|((EP&0x0000000000f0)<<40);
        EO=(EO&0x0f0fffff0f0f)|((EO&0xf00000000000)>>40)|((EO&0x00f000000000)>>24)|((EO&0x00000000f000)<<24)|((EO&0x0000000000f0)<<40);


    }


    if (themove == "r'")
    {
        EP=(EP&0xf0fff00ff0ff)|((EP&0x0f0000000000)>>20)|((EP&0x000000f00000)>>12)|((EP&0x00000f000000)<<16)|((EP&0x000000000f00)<<16);
        EO=(EO&0xf0fff00ff0ff)|((EO&0x0f0000000000)>>20)|((EO&0x000000f00000)>>12)|((EO&0x00000f000000)<<16)|((EO&0x000000000f00)<<16);
        CP=(CP&0xf00ff00f)|((CP&0x0f000000)>>4)|((CP&0x00f00000)>>16)|((CP&0x00000f00)<<16)|((CP&0x000000f0)<<4);
        CO=(CO&0xf00ff00f)|((CO&0x0f000000)>>4)|((CO&0x00f00000)>>16)|((CO&0x00000f00)<<16)|((CO&0x000000f0)<<4);
        CN=(CN&0x0f0)|((((CN&0x00f)+2)%6+1)<<8)|((CN&0xf00)>>8);

        CO+=0x01100110;
        for (int o=28; o>=0; o=o-4)
        {
            if (!((~((CO&(0x7<<o))>>o)&0x7)<<60))
            {
                CO&=~(0xb<<o);
            }
        }
        CO+=0x00100100;
        for (int o=28; o>=0; o=o-4)
        {
            if (!((CO&(0xf<<o))>>(o+2)))
            {
                CO&=~(0xf<<o);
            }
            else if (!((~((CO&(0x7<<o))>>o)&0x7)<<60))
            {
                CO&=~(0xb<<o);
            }
        }



        EP=(EP&0x0f0fffff0f0f)|((EP&0xf00000000000)>>8)|((EP&0x00f000000000)>>32)|((EP&0x00000000f000)<<32)|((EP&0x0000000000f0)<<8);
        EO=(EO&0x0f0fffff0f0f)|((EO&0xf00000000000)>>8)|((EO&0x00f000000000)>>32)|((EO&0x00000000f000)<<32)|((EO&0x0000000000f0)<<8);

        slicemask= {0xf00000000000,0x00f000000000,0x00000000f000,0x0000000000f0};
        shiftin= {44,36,12,4};
        for (int i=0; i<slicemask.size(); i++)
        {
            if ((((EP&slicemask[i])>>shiftin[i])!=0x1) & (((EP&slicemask[i])>>shiftin[i])!=0x3) & (((EP&slicemask[i])>>shiftin[i])!=0x9) & (((EP&slicemask[i])>>shiftin[i])!=0xb))
            {
                EO=EO^(0x111111111111&slicemask[i]);
            }
        }

        slicemask= {0x0f0000000000,0x000f00000000,0x0000f0000000,0x00000f000000,0x000000f00000,0x0000000f0000,0x000000000f00,0x00000000000f};
        shiftin= {40,32,28,24,20,16,8,0};
        for (int i=0; i<slicemask.size(); i++)
        {
            if ((((EP&slicemask[i])>>shiftin[i])==0x1) | (((EP&slicemask[i])>>shiftin[i])==0x3) | (((EP&slicemask[i])>>shiftin[i])==0x9) | (((EP&slicemask[i])>>shiftin[i])==0xb))
            {
                EO=EO^(0x111111111111&slicemask[i]);
            }
        }

    }

    if (themove == "l")
    {

        EP=(EP&0x0f0fffff0f0f)|((EP&0xf00000000000)>>8)|((EP&0x00f000000000)>>32)|((EP&0x00000000f000)<<32)|((EP&0x0000000000f0)<<8);
        EO=(EO&0x0f0fffff0f0f)|((EO&0xf00000000000)>>8)|((EO&0x00f000000000)>>32)|((EO&0x00000000f000)<<32)|((EO&0x0000000000f0)<<8);

        CN=(CN&0x0f0)|((((CN&0x00f)+2)%6+1)<<8)|((CN&0xf00)>>8);

        slicemask= {0xf00000000000,0x00f000000000,0x00000000f000,0x0000000000f0};
        shiftin= {44,36,12,4};
        for (int i=0; i<slicemask.size(); i++)
        {
            if ((((EP&slicemask[i])>>shiftin[i])!=0x1) & (((EP&slicemask[i])>>shiftin[i])!=0x3) & (((EP&slicemask[i])>>shiftin[i])!=0x9) & (((EP&slicemask[i])>>shiftin[i])!=0xb))
            {
                EO=EO^(0x111111111111&slicemask[i]);
            }
        }

        slicemask= {0x0f0000000000,0x000f00000000,0x0000f0000000,0x00000f000000,0x000000f00000,0x0000000f0000,0x000000000f00,0x00000000000f};
        shiftin= {40,32,28,24,20,16,8,0};
        for (int i=0; i<slicemask.size(); i++)
        {
            if ((((EP&slicemask[i])>>shiftin[i])==0x1) | (((EP&slicemask[i])>>shiftin[i])==0x3) | (((EP&slicemask[i])>>shiftin[i])==0x9) | (((EP&slicemask[i])>>shiftin[i])==0xb))
            {
                EO=EO^(0x111111111111&slicemask[i]);
            }
        }

        EP=(EP&0xfff00ff0fff0)|((EP&0x000f000f0000)>>16)|((EP&0x0000f0000000)<<4)|((EP&0x00000000000f)<<28);
        EO=(EO&0xfff00ff0fff0)|((EO&0x000f000f0000)>>16)|((EO&0x0000f0000000)<<4)|((EO&0x00000000000f)<<28);
        CP=(CP&0x0ff00ff0)|((CP&0xf0000000)>>12)|((CP&0x000f0000)>>16)|((CP&0x0000f000)<<16)|((CP&0x0000000f)<<12);
        CO=(CO&0x0ff00ff0)|((CO&0xf0000000)>>12)|((CO&0x000f0000)>>16)|((CO&0x0000f000)<<16)|((CO&0x0000000f)<<12);

        CO+=0x10011001;
        for (int o=28; o>=0; o=o-4)
        {
            if (!((~((CO&(0x7<<o))>>o)&0x7)<<60))
            {
                CO&=~(0xb<<o);
            }
        }
        CO+=0x10000001;
        for (int o=28; o>=0; o=o-4)
        {
            if (!((CO&(0xf<<o))>>(o+2)))
            {
                CO&=~(0xf<<o);
            }
            else if (!((~((CO&(0x7<<o))>>o)&0x7)<<60))
            {
                CO&=~(0xb<<o);
            }
        }
        //  temp=CO1;
        //  CO1=CO5+2;
        //  CO5=CO8+1;
        //  CO8=CO4+2;
        // CO4=temp+1;
    }
    if (themove == "l2")
    {
        EP=(EP&0x0f0fffff0f0f)|((EP&0xf00000000000)>>40)|((EP&0x00f000000000)>>24)|((EP&0x00000000f000)<<24)|((EP&0x0000000000f0)<<40);
        EO=(EO&0x0f0fffff0f0f)|((EO&0xf00000000000)>>40)|((EO&0x00f000000000)>>24)|((EO&0x00000000f000)<<24)|((EO&0x0000000000f0)<<40);

        CN=(CN&0x0f0)|(((((CN&0xf00)>>8)+2)%6+1)<<8)|(((CN&0x00f)+2)%6+1);

        EP=(EP&0xfff00ff0fff0)|((EP&0x000f00000000)>>32)|((EP&0x0000f0000000)>>12)|((EP&0x00000000000f)<<32)|((EP&0x0000000f0000)<<12);
        EO=(EO&0xfff00ff0fff0)|((EO&0x000f00000000)>>32)|((EO&0x0000f0000000)>>12)|((EO&0x00000000000f)<<32)|((EO&0x0000000f0000)<<12);
        CP=(CP&0x0ff00ff0)|((CP&0xf0000000)>>28)|((CP&0x000f0000)>>4)|((CP&0x0000f000)<<4)|((CP&0x0000000f)<<28);
        CO=(CO&0x0ff00ff0)|((CO&0xf0000000)>>28)|((CO&0x000f0000)>>4)|((CO&0x0000f000)<<4)|((CO&0x0000000f)<<28);

        //  temp=CO1;
        //  CO1=CO8;
        //  CO8=temp;
        //  temp=CO4;
        //  CO4=CO5;
        //  CO5=temp;
    }
    if (themove == "l'")
    {
        EP=(EP&0x0f0fffff0f0f)|((EP&0xf00000000000)>>32)|((EP&0x00f000000000)<<8)|((EP&0x00000000f000)>>8)|((EP&0x0000000000f0)<<32);
        EO=(EO&0x0f0fffff0f0f)|((EO&0xf00000000000)>>32)|((EO&0x00f000000000)<<8)|((EO&0x00000000f000)>>8)|((EO&0x0000000000f0)<<32);

        CN=(CN&0x0f0)|((((CN&0xf00)>>8)+2)%6+1)|((CN&0x00f)<<8);

        slicemask= {0xf00000000000,0x00f000000000,0x00000000f000,0x0000000000f0};
        shiftin= {44,36,12,4};
        for (int i=0; i<slicemask.size(); i++)
        {
            if ((((EP&slicemask[i])>>shiftin[i])!=0x1) & (((EP&slicemask[i])>>shiftin[i])!=0x3) & (((EP&slicemask[i])>>shiftin[i])!=0x9) & (((EP&slicemask[i])>>shiftin[i])!=0xb))
            {
                EO=EO^(0x111111111111&slicemask[i]);
            }
        }

        slicemask= {0x0f0000000000,0x000f00000000,0x0000f0000000,0x00000f000000,0x000000f00000,0x0000000f0000,0x000000000f00,0x00000000000f};
        shiftin= {40,32,28,24,20,16,8,0};
        for (int i=0; i<slicemask.size(); i++)
        {
            if ((((EP&slicemask[i])>>shiftin[i])==0x1) | (((EP&slicemask[i])>>shiftin[i])==0x3) | (((EP&slicemask[i])>>shiftin[i])==0x9) | (((EP&slicemask[i])>>shiftin[i])==0xb))
            {
                EO=EO^(0x111111111111&slicemask[i]);
            }
        }


        EP=(EP&0xfff00ff0fff0)|((EP&0x0000000f000f)<<16)|((EP&0x0000f0000000)>>28)|((EP&0x000f00000000)>>4);
        EO=(EO&0xfff00ff0fff0)|((EO&0x0000000f000f)<<16)|((EO&0x0000f0000000)>>28)|((EO&0x000f00000000)>>4);
        CP=(CP&0x0ff00ff0)|((CP&0xf0000000)>>16)|((CP&0x000f0000)<<12)|((CP&0x0000f000)>>12)|((CP&0x0000000f)<<16);
        CO=(CO&0x0ff00ff0)|((CO&0xf0000000)>>16)|((CO&0x000f0000)<<12)|((CO&0x0000f000)>>12)|((CO&0x0000000f)<<16);

        CO+=0x10011001;
        for (int o=28; o>=0; o=o-4)
        {
            if (!((~((CO&(0x7<<o))>>o)&0x7)<<60))
            {
                CO&=~(0xb<<o);
            }
        }
        CO+=0x10000001;
        for (int o=28; o>=0; o=o-4)
        {
            if (!((CO&(0xf<<o))>>(o+2)))
            {
                CO&=~(0xf<<o);
            }
            else if (!((~((CO&(0x7<<o))>>o)&0x7)<<60))
            {
                CO&=~(0xb<<o);
            }
        }
        //  temp=CO1;
        //  CO1=CO4+2;
        //  CO4=CO8+1;
        //  CO8=CO5+2;
        //  CO5=temp+1;
    }

    if (themove == "u'")
    {

        EP=(EP&0x0000ffffffff)|((EP&0x0fff00000000)<<4)|((EP&0xf00000000000)>>12);
        EO=(EO&0x0000ffffffff)|((EO&0x0fff00000000)<<4)|((EO&0xf00000000000)>>12);
        CP=(CP&0x0000ffff)|((CP&0x0fff0000)<<4)|((CP&0xf0000000)>>12);
        CO=(CO&0x0000ffff)|((CO&0x0fff0000)<<4)|((CO&0xf0000000)>>12);
        EP=(EP&0xffff0000ffff)|((EP&0x00000fff0000)<<4)|((EP&0x0000f0000000)>>12);
        EO=(EO&0xffff0000ffff)|((EO&0x00000fff0000)<<4)|((EO&0x0000f0000000)>>12);

        CN=(CN&0xf00)|((((CN&0x0f0)>>4)+2)%6+1)|((CN&0x00f)<<4);

        slicemask= {0x0000f0000000,0x00000f000000,0x000000f00000,0x0000000f0000};
        shiftin= {28,24,20,16};
        for (int i=0; i<slicemask.size(); i++)
        {
            if ((((EP&slicemask[i])>>shiftin[i])!=0x5) & (((EP&slicemask[i])>>shiftin[i])!=0x6) & (((EP&slicemask[i])>>shiftin[i])!=0x7) & (((EP&slicemask[i])>>shiftin[i])!=0x8))
            {
                EO=EO^(0x111111111111&slicemask[i]);
            }
        }

        slicemask= {0xf00000000000,0x0f0000000000,0x00f000000000,0x000f00000000,0x00000000f000,0x000000000f00,0x0000000000f0,0x00000000000f};
        shiftin= {44,40,36,32,12,8,4,0};
        for (int i=0; i<slicemask.size(); i++)
        {
            if ((((EP&slicemask[i])>>shiftin[i])==0x5) | (((EP&slicemask[i])>>shiftin[i])==0x6) | (((EP&slicemask[i])>>shiftin[i])==0x7) | (((EP&slicemask[i])>>shiftin[i])==0x8))
            {
                EO=EO^(0x111111111111&slicemask[i]);
            }
        }
    }


    if (themove == "u2")
    {

        EP=(EP&0x0000ffffffff)|((EP&0xff0000000000)>>8)|((EP&0x00ff00000000)<<8);
        EO=(EO&0x0000ffffffff)|((EO&0xff0000000000)>>8)|((EO&0x00ff00000000)<<8);
        CP=(CP&0x0000ffff)|((CP&0xff000000)>>8)|((CP&0x00ff0000)<<8);
        CO=(CO&0x0000ffff)|((CO&0xff000000)>>8)|((CO&0x00ff0000)<<8);
        EP=(EP&0xffff0000ffff)|((EP&0x000000ff0000)<<8)|((EP&0x0000ff000000)>>8);
        EO=(EO&0xffff0000ffff)|((EO&0x000000ff0000)<<8)|((EO&0x0000ff000000)>>8);
        CN=(CN&0xf00)|(((((CN&0x0f0)>>4)+2)%6+1)<<4)|(((CN&0x00f)+2)%6+1);
    }


    if (themove == "u")
    {
        EP=(EP&0x0000ffffffff)|((EP&0xfff000000000)>>4)|((EP&0x000f00000000)<<12);
        EO=(EO&0x0000ffffffff)|((EO&0xfff000000000)>>4)|((EO&0x000f00000000)<<12);
        CP=(CP&0x0000ffff)|((CP&0xfff00000)>>4)|((CP&0x000f0000)<<12);
        CO=(CO&0x0000ffff)|((CO&0xfff00000)>>4)|((CO&0x000f0000)<<12);

        EP=(EP&0xffff0000ffff)|((EP&0x0000fff00000)>>4)|((EP&0x0000000f0000)<<12);
        EO=(EO&0xffff0000ffff)|((EO&0x0000fff00000)>>4)|((EO&0x0000000f0000)<<12);

        CN=(CN&0xf00)|((((CN&0x00f)+2)%6+1)<<4)|((CN&0x0f0)>>4);

        slicemask= {0x0000f0000000,0x00000f000000,0x000000f00000,0x0000000f0000};
        shiftin= {28,24,20,16};
        for (int i=0; i<slicemask.size(); i++)
        {
            if ((((EP&slicemask[i])>>shiftin[i])!=0x5) & (((EP&slicemask[i])>>shiftin[i])!=0x6) & (((EP&slicemask[i])>>shiftin[i])!=0x7) & (((EP&slicemask[i])>>shiftin[i])!=0x8))
            {
                EO=EO^(0x111111111111&slicemask[i]);
            }
        }

        slicemask= {0xf00000000000,0x0f0000000000,0x00f000000000,0x000f00000000,0x00000000f000,0x000000000f00,0x0000000000f0,0x00000000000f};
        shiftin= {44,40,36,32,12,8,4,0};
        for (int i=0; i<slicemask.size(); i++)
        {
            if ((((EP&slicemask[i])>>shiftin[i])==0x5) | (((EP&slicemask[i])>>shiftin[i])==0x6) | (((EP&slicemask[i])>>shiftin[i])==0x7) | (((EP&slicemask[i])>>shiftin[i])==0x8))
            {
                EO=EO^(0x111111111111&slicemask[i]);
            }
        }
    }

    return;
}


string y2rotator(string initial)
{
    vector<string> words;
    string tuf;
    stringstream ss(initial);
    while (ss >> tuf)
    {
        words.push_back(tuf+" ");
    }
    for (int q=0; q<words.size(); q++)
    {
        if (words[q][0]=='R')
        {
            words[q][0]='L';
        }
        else if (words[q][0]=='L')
        {
            words[q][0]='R';
        }
        /*  else if (words[q][0]=='D' || words[q][0]=='U')
          {
              if (words[q][1]=='\'')
              {
                  words[q][1]==' ';
              }
               if (words[q][1]==' ')
              {
                  words[q][1]=='\'';
              }

          }*/
    }

    string goout;
    for (const auto &piece : words) goout += piece + " ";
    goout = regex_replace(goout, std::regex(" +"), " ");
    return goout;
}


string solutionreducer(string initial)
{
    vector<string> words;
    vector<int> clearthese= {};
    string tuf;
    stringstream ss(initial);
    while (ss >> tuf)
    {
        words.push_back(tuf+" ");
    }

    for (int q=(words.size()-1); q>0; q--)
    {
        if ((words[q][0]=='U' && words[q-1][0]=='U') || (words[q][0]=='D' && words[q-1][0]=='D') || (words[q][0]=='R' && words[q-1][0]=='R') || (words[q][0]=='L' && words[q-1][0]=='L') || (words[q][0]=='F' && words[q-1][0]=='F') || (words[q][0]=='B' && words[q-1][0]=='B') || (words[q][0]=='M' && words[q-1][0]=='M') || (words[q][0]=='S' && words[q-1][0]=='S') || (words[q][0]=='E' && words[q-1][0]=='E') || (words[q][0]=='r' && words[q-1][0]=='r'))
        {
            if ( (words[q][1]==' ' && words[q-1][1]==' ') || (words[q][1]=='\'' && words[q-1][1]=='\'') )
            {
                words[q-1][1]='2';
                clearthese.push_back(q);
            }
            else if ( (words[q][1]=='2' && words[q-1][1]==' ') || (words[q][1]==' ' && words[q-1][1]=='2') )
            {
                words[q-1][1]='\'';
                clearthese.push_back(q);
            }
            else if ( (words[q][1]=='2' && words[q-1][1]=='\'') || (words[q][1]=='\'' && words[q-1][1]=='2') )
            {
                words[q-1][1]=' ';
                clearthese.push_back(q);
            }
            else if ( (words[q][1]=='2' && words[q-1][1]=='2') || (words[q][1]=='\'' && words[q-1][1]==' ') || (words[q][1]==' ' && words[q-1][1]=='\'') )
            {
                clearthese.push_back(q);
                clearthese.push_back(q-1);
            }
        }
    }

    for (int q=(words.size()-1); q>0; q--)
    {
        if ((words[q][0]=='R' && words[q-1][0]=='L') || (words[q][0]=='U' && words[q-1][0]=='D') || (words[q][0]=='F' && words[q-1][0]=='B'))
        {
            string temp1=words[q-1];
            words[q-1]=words[q];
            words[q]=temp1;
        }
    }

    sort( clearthese.begin(), clearthese.end() );
    clearthese.erase( unique( clearthese.begin(), clearthese.end() ), clearthese.end() );


    for (int i=(clearthese.size()-1); i>=0; i--)
    {
        words.erase(words.begin()+clearthese[i]);
    }

    string goout;

    for (const auto &piece : words) goout += piece + " ";

    goout = regex_replace(goout, std::regex(" +"), " ");

    return goout;

}


string moveReverse(string regular)
{
    string reversed=" "; // added an extra space here
    string tuf;
    string temp;
    stringstream ss(regular);
    while (ss >> tuf)
    {
        if (tuf=="U")
        {
            temp="U' ";
            reversed=temp.append(reversed);
        }
        else if (tuf=="U'")
        {
            temp="U ";
            reversed=temp.append(reversed);
        }
        else if (tuf=="U2")
        {
            temp="U2 ";
            reversed=temp.append(reversed);
        }
        else if (tuf=="D")
        {
            temp="D' ";
            reversed=temp.append(reversed);
        }
        else if (tuf=="D'")
        {
            temp="D ";
            reversed=temp.append(reversed);
        }
        else if (tuf=="D2")
        {
            temp="D2 ";
            reversed=temp.append(reversed);
        }
        else if (tuf=="R")
        {
            temp="R' ";
            reversed=temp.append(reversed);
        }
        else if (tuf=="R'")
        {
            temp="R ";
            reversed=temp.append(reversed);
        }
        else if (tuf=="R2")
        {
            temp="R2 ";
            reversed=temp.append(reversed);
        }
        else if (tuf=="L")
        {
            temp="L' ";
            reversed=temp.append(reversed);
        }
        else if (tuf=="L'")
        {
            temp="L ";
            reversed=temp.append(reversed);
        }
        else if (tuf=="L2")
        {
            temp="L2 ";
            reversed=temp.append(reversed);
        }
        else if (tuf=="F")
        {
            temp="F' ";
            reversed=temp.append(reversed);
        }
        else if (tuf=="F'")
        {
            temp="F ";
            reversed=temp.append(reversed);
        }
        else if (tuf=="F2")
        {
            temp="F2 ";
            reversed=temp.append(reversed);
        }
        else if (tuf=="B")
        {
            temp="B' ";
            reversed=temp.append(reversed);
        }
        else if (tuf=="B'")
        {
            temp="B ";
            reversed=temp.append(reversed);
        }
        else if (tuf=="B2")
        {
            temp="B2 ";
            reversed=temp.append(reversed);
        }
        else if (tuf=="x")
        {
            temp="x' ";
            reversed=temp.append(reversed);
        }
        else if (tuf=="x'")
        {
            temp="x ";
            reversed=temp.append(reversed);
        }
        else if (tuf=="x2")
        {
            temp="x2 ";
            reversed=temp.append(reversed);
        }
        else if (tuf=="y")
        {
            temp="y' ";
            reversed=temp.append(reversed);
        }
        else if (tuf=="y'")
        {
            temp="y ";
            reversed=temp.append(reversed);
        }
        else if (tuf=="y2")
        {
            temp="y2 ";
            reversed=temp.append(reversed);
        }
        else if (tuf=="z")
        {
            temp="z' ";
            reversed=temp.append(reversed);
        }
        else if (tuf=="z'")
        {
            temp="z ";
            reversed=temp.append(reversed);
        }
        else if (tuf=="z2")
        {
            temp="z2 ";
            reversed=temp.append(reversed);
        }
        else if (tuf=="E")
        {
            temp="E' ";
            reversed=temp.append(reversed);
        }
        else if (tuf=="E'")
        {
            temp="E ";
            reversed=temp.append(reversed);
        }
        else if (tuf=="E2")
        {
            temp="E2 ";
            reversed=temp.append(reversed);
        }
        else if (tuf=="S")
        {
            temp="S' ";
            reversed=temp.append(reversed);
        }
        else if (tuf=="S'")
        {
            temp="S ";
            reversed=temp.append(reversed);
        }
        else if (tuf=="S2")
        {
            temp="S2 ";
            reversed=temp.append(reversed);
        }
        else if (tuf=="M")
        {
            temp="M' ";
            reversed=temp.append(reversed);
        }
        else if (tuf=="M'")
        {
            temp="M ";
            reversed=temp.append(reversed);
        }
        else if (tuf=="M2")
        {
            temp="M2 ";
            reversed=temp.append(reversed);
        }
        else if (tuf=="r")
        {
            temp="r' ";
            reversed=temp.append(reversed);
        }
        else if (tuf=="r'")
        {
            temp="r ";
            reversed=temp.append(reversed);
        }
        else if (tuf=="r2")
        {
            temp="r2 ";
            reversed=temp.append(reversed);
        }
        else if (tuf=="l")
        {
            temp="l' ";
            reversed=temp.append(reversed);
        }
        else if (tuf=="l'")
        {
            temp="l ";
            reversed=temp.append(reversed);
        }
        else if (tuf=="l2")
        {
            temp="l2 ";
            reversed=temp.append(reversed);
        }
        else if (tuf=="u")
        {
            temp="u' ";
            reversed=temp.append(reversed);
        }
        else if (tuf=="u'")
        {
            temp="u ";
            reversed=temp.append(reversed);
        }
        else if (tuf=="u2")
        {
            temp="u2 ";
            reversed=temp.append(reversed);
        }
        else if (tuf==" ")
        {
            temp=" ";
            reversed=temp.append(reversed);
        }
    }
    return reversed;
}




string concat_strings(const vector<string> &elements, const string &separator)
{
    if (!elements.empty())
    {
        std::stringstream ss;
        auto it = elements.cbegin();
        while (true)
        {
            ss << *it++;
            if (it != elements.cend())
                ss << separator;
            else
                return ss.str();
        }
    }
    return "";
}

typedef boost::tuples::tuple<__int64,__int64,__int64,__int64,__int64> Edge;

struct ihash
    : std::unary_function<Edge, std::size_t>
{
    std::size_t operator()(Edge const& e) const
    {
        std::size_t seed = 0;
        boost::hash_combine( seed, e.get<0>() );
        boost::hash_combine( seed, e.get<1>() );
        boost::hash_combine( seed, e.get<2>() ); // only up to here
        boost::hash_combine( seed, e.get<3>() );
        boost::hash_combine( seed, e.get<4>() );
        //  boost::hash_combine( seed, e.get<3>() );
        return seed;
    }
};

struct iequal_to
    : std::binary_function<Edge, Edge, bool>
{
    bool operator()(Edge const& x, Edge const& y) const
    {
        return ( x.get<0>()==y.get<0>() &&
                 x.get<1>()==y.get<1>() &&
                 x.get<2>()==y.get<2>()   //);// &&
                 //           x.get<3>()==y.get<3>());

                 && x.get<3>()==y.get<3>()
                 && x.get<4>()==y.get<4>());
    }
};

typedef boost::unordered_multimap< Edge, string, ihash, iequal_to > EdgeMap;
EdgeMap mapp0;
EdgeMap mapp1;
EdgeMap mapp2;
EdgeMap mapp3;
EdgeMap mapp5;
EdgeMap mapp6;
EdgeMap mapp7;
EdgeMap mapp8;


EdgeMap mapp4; // reserved for alg sets



void testtuple(string thescramble) // OBSOLETE TEST FUNCTION DELETE LOL
{

    //       prunes1(5,1,1);

    cout << "pruned"<<endl;

    cubeRevert(0);
    // Edge key (EPs, EOs, CPs);
    // map[key] = "hi";

    masks1(1,1);

    EP=EPs&maskEPs1;
    EO=EOs&maskEOs1;
    CP=CPs&maskCPs1;
    CO=COs&maskCOs1;
    CN=CNs&maskCNs1;
    showCube();
    string buf;
    stringstream ss(thescramble);
    while (ss >> buf)
    {
        applyMove(buf);
    }
    EOfixer();
    CNfixer();
    showCube();
    //cout<<mapp0.size();
    Edge key (EP, EO, CP, CO, CN);


    // cout<<moveReverse(mapp0[key])<<endl;

}

void createmoves()
{
    moves.push_back(" U");
    moves.push_back(" U2");
    moves.push_back(" U'");
    moves.push_back(" D");
    moves.push_back(" D2");
    moves.push_back(" D'");
    moves.push_back(" R");
    moves.push_back(" R2");
    moves.push_back(" R'");
    moves.push_back(" L");
    moves.push_back(" L2");
    moves.push_back(" L'");
    moves.push_back(" F");
    moves.push_back(" F2");
    moves.push_back(" F'");
    moves.push_back(" B");
    moves.push_back(" B2");
    moves.push_back(" B'");
    moves.push_back(" M");
    moves.push_back(" M2");
    moves.push_back(" M'");
    moves.push_back(" r");
    moves.push_back(" r2");
    moves.push_back(" r'");
}


void addLayers(string str, vector <string> &thisone, int allowedmoves)
{
    // allowedmoves: 1=6 gen, 2=6gen+Mr, 3=RUMr, 4=RUD, 5=RULD, 6=FURB, 7=MU, 8=U (algs), 9=MUD, 10=LUD;
    string str1=str;
    int axis;
    bool hasdouble=0;
    bool previousR=0;
    bool previousL=0;
    bool previousU=0;
    bool previousF=0;
    bool previousB=0;
    bool previousm=0;
    bool previousr=0;
    bool previousD=0;
    stringstream sstr(str1);
    vector<string> vstr;
    while(sstr >> str1)
    {
        vstr.push_back(str1);
    }

    if (vstr[vstr.size()-1][0]=='U')
    {
        axis=0;
        if (vstr.size() >= 2 && vstr[vstr.size()-2][0]=='D')
        {
            hasdouble=1;
            previousD=1;
        }
    }
    if (vstr[vstr.size()-1][0]=='D')
    {
        axis=1;
        if (vstr.size() >= 2 && vstr[vstr.size()-2][0]=='U')
        {
            hasdouble=1;
            previousU=1;
        }
    }
    if (vstr[vstr.size()-1][0]=='R')
    {
        axis=2;
        if (vstr.size() >= 2 && vstr[vstr.size()-2][0]=='L')
        {
            hasdouble=1;
            previousL=1;
        }
    }
    if (vstr[vstr.size()-1][0]=='L')
    {
        axis=3;
        if (vstr.size() >= 2 && vstr[vstr.size()-2][0]=='R')
        {
            hasdouble=1;
            previousR=1;
        }
    }
    if (vstr[vstr.size()-1][0]=='F')
    {
        axis=4;
        if (vstr.size() >= 2 && vstr[vstr.size()-2][0]=='B')
        {
            hasdouble=1;
            previousB=1;
        }
    }
    if (vstr[vstr.size()-1][0]=='B')
    {
        axis=5;
        if (vstr.size() >= 2 && vstr[vstr.size()-2][0]=='F')
        {
            hasdouble=1;
            previousF=1;
        }
    }

    if (allowedmoves==2 || allowedmoves==3)
    {
        if (vstr[vstr.size()-1][0]=='M')
        {
            axis=6; // whatever
            if (vstr.size() >= 2 && vstr[vstr.size()-2][0]=='M')
            {
                previousm=1;
            }
        }
        if (vstr[vstr.size()-1][0]=='r')
        {
            axis=7; // whatever
            if (vstr.size() >= 2 && vstr[vstr.size()-2][0]=='r')
            {
                previousr=1;
            }
        }
    }

    if (allowedmoves==7)
    {
        if (vstr[vstr.size()-1][0]=='M')
        {
            axis=6; // whatever
        }
        else if (vstr[vstr.size()-1][0]=='U')
        {
            axis=0; // whatever
        }
    }

    if (allowedmoves==1 || allowedmoves==2)
    {

        for (int j=0; j<18; j++)
        {
            if (j/3!=axis)
            {
                if (!hasdouble || j/3!=(axis+1-2*(axis%2)))
                {
                    thisone.push_back(str+moves[j]);

                }
            }
        }
    }

    if (allowedmoves==3 || allowedmoves==4 || allowedmoves==5 || allowedmoves==6) // RU
    {
        for (int j=0; j<3; j++)
        {
            if (j/3!=axis && !previousU)
            {
                thisone.push_back(str+moves[j]);
            }
        }
        for (int j=6; j<9; j++)
        {
            if (j/3!=axis && !previousR)
            {
                thisone.push_back(str+moves[j]);
            }
        }
    }

    if (allowedmoves==6) // FB of RUFB
    {
        for (int j=12; j<15; j++)
        {
            if (j/3!=axis && !previousF)
            {
                thisone.push_back(str+moves[j]);
            }
        }
        for (int j=15; j<18; j++)
        {
            if (j/3!=axis && !previousB)
            {
                thisone.push_back(str+moves[j]);
            }
        }
    }


    if (allowedmoves==7 || allowedmoves==9) // MU
    {
        for (int j=0; j<3; j++)
        {
            if (j/3!=axis)
            {
                thisone.push_back(str+moves[j]);
            }
        }
        for (int j=18; j<21; j++)
        {
            if (j/3!=axis)
            {
                thisone.push_back(str+moves[j]);
            }
        }
    }


    if (allowedmoves==4 || allowedmoves==5 || allowedmoves==10 || allowedmoves==9)     // D
    {
        for (int j=3; j<6; j++)
        {
            if (j/3!=axis && !previousD)
            {
                thisone.push_back(str+moves[j]);
            }
        }

    }

    if (allowedmoves==5 || allowedmoves==10)     // L
    {
        for (int j=9; j<12; j++)
        {
            if (j/3!=axis && !previousL)
            {
                thisone.push_back(str+moves[j]);
            }
        }

    }

    if (allowedmoves==10)     // U
    {
        for (int j=0; j<3; j++)
        {
            if (j/3!=axis)
            {
                thisone.push_back(str+moves[j]);
            }
        }
    }


    if (allowedmoves==2 || allowedmoves==3) // M r
    {
        for (int j=18; j<24; j++)
        {
            if (!previousm && j/3==6 && j/3!=axis)
            {
                thisone.push_back(str+moves[j]);
            }
            if (!previousr && j/3==7 && j/3!=axis)
            {
                thisone.push_back(str+moves[j]);
            }
        }
    }


}

void EOfixer(void)
{
    // showCube();
    for (int o=44; o>=0; o=o-4)
    {
        //cout<<std::hex<<EO<<endl<<(EO>>o)<<endl<<((EO>>o)&0x2)<<endl<<(!((EO>>o)&0x2))<<endl<<endl;
        if (!((EO>>o)&0x02))
        {
            // cout<<"i'm in"<<endl<<(0xf<<o)<<endl<<~(0xf<<o)<<endl<<endl;
            EO&=~((__int64) (0xf)<<o);
        }
        // getchar();
    }
    // showCube();
}
void CNfixer(void)
{
    // showCube();
    CN&=maskCNs1;
    // showCube();
}


void cleanScrambler(void)
{
    cubeRevert(0);
    string buf;
    for (int i=0; i<wherretf.size(); i++)
    {
        stringstream ss(wherretf[i][0]);
        while (ss >> buf)
        {
            applyMove(buf);
        }
    }
    EOfixer();
    CNfixer();
}


void unpruner(void)
{
    mapp0.clear();
    mapp1.clear();
    mapp2.clear();
    mapp3.clear();
    mapp4.clear();
    mapp5.clear();
    mapp6.clear();
    mapp7.clear();
    mapp8.clear();
    customsteps.clear();
    customdets.clear();
    customall.clear();
    return;
}


void masks1(int method, int step)
{
    if (method==0) // cfop
    {
        if (step==0) // cross on D
        {
            currentstep="cross";
            maskEPs1=0x00000000ffff;
            maskEOs1=0x00000000ffff;
            maskCPs1=0x00000000;
            maskCOs1=0x00000000;
            maskCNs1=0x000;
        }
        if (step==1) // xcross on D, block at DFR
        {
            currentstep="xcross";
            maskEPs1=0x000000f0ffff;
            maskEOs1=0x000000f0ffff;
            maskCPs1=0x000000f0;
            maskCOs1=0x000000f0;
            maskCNs1=0x000;
        }
    }

    if (method==1) // petrus
    {
        if (step==0) // 2x2x2 at DLB
        {
            currentstep="2x2x2";
            maskEPs1=0x0000f000f00f;
            maskEOs1=0x0000f000f00f;
            maskCPs1=0x0000f000;
            maskCOs1=0x0000f000;
            maskCNs1=0xfff;
        }
        if (step==1) // 3x2x2 at DL
        {
            currentstep="3x2x2";
            maskEPs1=0x0000f00ff0ff;
            maskEOs1=0x0000f00ff0ff;
            maskCPs1=0x0000f00f;
            maskCOs1=0x0000f00f;
            maskCNs1=0xfff;
        }
        if (step==2) // eo
        {
            currentstep="eo";
            maskEPs1=0x0000f00ff0ff;
            maskEOs1=0xffffffffffff;
            maskCPs1=0x0000f00f;
            maskCOs1=0x0000f00f;
            maskCNs1=0xfff;
        }
        if (step==3) // f2l
        {
            currentstep="f2l";
            maskEPs1=0x0000ffffffff;
            maskEOs1=0xffffffffffff;
            maskCPs1=0x0000ffff;
            maskCOs1=0x0000ffff;
            maskCNs1=0xfff;
        }
        if (step==4) // zbll
        {
            currentstep="zbll";
            maskEPs1=0xffffffffffff;
            maskEOs1=0xffffffffffff;
            maskCPs1=0xffffffff;
            maskCOs1=0xffffffff;
            maskCNs1=0xfff;
        }
    }

    if (method==2) // roux
    {
        if (step==0) // fb at DL
        {
            currentstep="fb";
            maskEPs1=0x0000f00f000f;
            maskEOs1=0x0000f00f000f;
            maskCPs1=0x0000f00f;
            maskCOs1=0x0000f00f;
            maskCNs1=0x0f0;
        }
        if (step==1) // sb at DR
        {
            currentstep="sb";
            maskEPs1=0x0000ffff0f0f;
            maskEOs1=0x0000ffff0f0f;
            maskCPs1=0x0000ffff;
            maskCOs1=0x0000ffff;
            maskCNs1=0x0f0;
        }
        if (step==3) // fs at DLB
        {
            currentstep="fs";
            maskEPs1=0x0000f000000f;
            maskEOs1=0x0000f000000f;
            maskCPs1=0x0000f000;
            maskCOs1=0x0000f000;
            maskCNs1=0x0f0;
        }

        if (step==4) //
        {
            currentstep="cmll";
            maskEPs1=0x0000ffff0f0f;
            maskEOs1=0x0000ffff0f0f;
            maskCPs1=0xffffffff;
            maskCOs1=0xffffffff;
            maskCNs1=0x0f0;
        }


        if (step==5) // LSE
        {
            currentstep="lse";
            maskEPs1=0xffffffffffff;
            maskEOs1=0xffffffffffff;
            maskCPs1=0xffffffff;
            maskCOs1=0xffffffff;
            maskCNs1=0xfff;
        }
    }

    if (method==3) // zz
    {

        if (step==0) // eoline at D
        {
            currentstep="eoline";
            maskEPs1=0x00000000f0f0;
            maskEOs1=0xffffffffffff;
            maskCPs1=0x00000000;
            maskCOs1=0x00000000;
            maskCNs1=0xfff;
        }
        if (step==1) // eo3x2x2 at D
        {
            currentstep="eo3x2x2";
            maskEPs1=0x0000f00ff0ff;
            maskEOs1=0xffffffffffff;
            maskCPs1=0x0000f00f;
            maskCOs1=0x0000f00f;
            maskCNs1=0xfff;
        }
        //     if (azzblockdone==0)
        //   {
        if (step==2) // lb
        {
            currentstep="lb";
            maskEPs1=0x0000f00ff0ff;
            maskEOs1=0xffffffffffff;
            maskCPs1=0x0000f00f;
            maskCOs1=0x0000f00f;
            maskCNs1=0xfff;
        }

        if (step==3) // rb
        {
            currentstep="rb";
            maskEPs1=0x0000ffffffff;
            maskEOs1=0xffffffffffff;
            maskCPs1=0x0000ffff;
            maskCOs1=0x0000ffff;
            maskCNs1=0xfff;
        }


        if (step==4) // zbll
        {
            currentstep="zbll";
            maskEPs1=0xffffffffffff;
            maskEOs1=0xffffffffffff;
            maskCPs1=0xffffffff;
            maskCOs1=0xffffffff;
            maskCNs1=0xfff;
        }
        //  }

    }

    if (method==4) // custom method
    {
        currentstep=customsteps[step];
        maskEPs1=customdets[step][3];
        maskEOs1=customdets[step][4];
        maskCPs1=customdets[step][5];
        maskCOs1=customdets[step][6];
        maskCNs1=customdets[step][7];
    }
}

void prunes1 (int depth, int method, int step, int allowedmoves)
{
    createmoves();
    bool isalgset=0;
    setups6gen.clear();
    setups6gen.push_back(" ");

    if (allowedmoves==1)
    {
        setups6gen.push_back("U");
        setups6gen.push_back("U2");
        setups6gen.push_back("U'");
        setups6gen.push_back("D");
        setups6gen.push_back("D2");
        setups6gen.push_back("D'");
        setups6gen.push_back("R");
        setups6gen.push_back("R2");
        setups6gen.push_back("R'");
        setups6gen.push_back("L");
        setups6gen.push_back("L2");
        setups6gen.push_back("L'");
        setups6gen.push_back("F");
        setups6gen.push_back("F2");
        setups6gen.push_back("F'");
        setups6gen.push_back("B");
        setups6gen.push_back("B2");
        setups6gen.push_back("B'");
    }

    if (allowedmoves==2)
    {
        setups6gen.push_back("U");
        setups6gen.push_back("U2");
        setups6gen.push_back("U'");
        setups6gen.push_back("D");
        setups6gen.push_back("D2");
        setups6gen.push_back("D'");
        setups6gen.push_back("R");
        setups6gen.push_back("R2");
        setups6gen.push_back("R'");
        setups6gen.push_back("L");
        setups6gen.push_back("L2");
        setups6gen.push_back("L'");
        setups6gen.push_back("F");
        setups6gen.push_back("F2");
        setups6gen.push_back("F'");
        setups6gen.push_back("B");
        setups6gen.push_back("B2");
        setups6gen.push_back("B'");
        setups6gen.push_back("M");
        setups6gen.push_back("M2");
        setups6gen.push_back("M'");
        setups6gen.push_back("r");
        setups6gen.push_back("r2");
        setups6gen.push_back("r'");
    }

    if (allowedmoves==3)
    {
        setups6gen.push_back("U");
        setups6gen.push_back("U2");
        setups6gen.push_back("U'");
        setups6gen.push_back("R");
        setups6gen.push_back("R2");
        setups6gen.push_back("R'");
        setups6gen.push_back("M");
        setups6gen.push_back("M2");
        setups6gen.push_back("M'");
        setups6gen.push_back("r");
        setups6gen.push_back("r2");
        setups6gen.push_back("r'");
    }

    if (allowedmoves==4)
    {
        setups6gen.push_back("U");
        setups6gen.push_back("U2");
        setups6gen.push_back("U'");
        setups6gen.push_back("D");
        setups6gen.push_back("D2");
        setups6gen.push_back("D'");
        setups6gen.push_back("R");
        setups6gen.push_back("R2");
        setups6gen.push_back("R'");
    }

    if (allowedmoves==5)
    {
        setups6gen.push_back("U");
        setups6gen.push_back("U2");
        setups6gen.push_back("U'");
        setups6gen.push_back("D");
        setups6gen.push_back("D2");
        setups6gen.push_back("D'");
        setups6gen.push_back("R");
        setups6gen.push_back("R2");
        setups6gen.push_back("R'");
        setups6gen.push_back("L");
        setups6gen.push_back("L2");
        setups6gen.push_back("L'");
    }

    if (allowedmoves==6)
    {
        setups6gen.push_back("U");
        setups6gen.push_back("U2");
        setups6gen.push_back("U'");
        setups6gen.push_back("R");
        setups6gen.push_back("R2");
        setups6gen.push_back("R'");
        setups6gen.push_back("F");
        setups6gen.push_back("F2");
        setups6gen.push_back("F'");
        setups6gen.push_back("B");
        setups6gen.push_back("B2");
        setups6gen.push_back("B'");
    }


    if (allowedmoves==7)
    {
        setups6gen.push_back("U");
        setups6gen.push_back("U2");
        setups6gen.push_back("U'");
        setups6gen.push_back("M");
        setups6gen.push_back("M2");
        setups6gen.push_back("M'");
    }

    if (allowedmoves==8)
    {
        setups6gen.push_back("U");
        setups6gen.push_back("U2");
        setups6gen.push_back("U'");
        isalgset=1;
    }
    if (allowedmoves==9)
    {
        setups6gen.push_back("U");
        setups6gen.push_back("U2");
        setups6gen.push_back("U'");
        setups6gen.push_back("M");
        setups6gen.push_back("M2");
        setups6gen.push_back("M'");
        setups6gen.push_back("D");
        setups6gen.push_back("D2");
        setups6gen.push_back("D'");
    }
    if (allowedmoves==10)
    {
        setups6gen.push_back("U");
        setups6gen.push_back("U2");
        setups6gen.push_back("U'");
        setups6gen.push_back("L");
        setups6gen.push_back("L2");
        setups6gen.push_back("L'");
        setups6gen.push_back("D");
        setups6gen.push_back("D2");
        setups6gen.push_back("D'");
    }

    if (!isalgset)
    {
        int startat=1;
        for (int ii=1; ii<depth; ii++)
        {
            int endat=setups6gen.size();
            for (int i=startat; i<endat; i++)
            {
                addLayers(setups6gen[i], setups6gen, allowedmoves);
            }
//cout<<ii<<" "<<startat<<" "<<endat<<" "<<setupss1[startat]<<" "<<setupss1[endat]<<" "<<endl;
            startat=endat;
        }
    }
    else if (isalgset)
    {
        /* for (int i=0; i<4; i++)
         {
         algparser(setups6gen[i], setups6gen, method);
         }*/
    }

//for (auto const& c : setupss1)
//    std::cout << c << '\n';


    //for (int i=setups6gen.size()-1; i>=0; i--) // UNCOMMENT THIS WHEN DONE !!!!!!
    for (int i=0; i<setups6gen.size(); i++)
    {
        cubeRevert(0);
        masks1(method, step);

        EP=EPs&maskEPs1;
        EO=EOs&maskEOs1;
        CP=CPs&maskCPs1;
        CO=COs&maskCOs1;
        CN=CNs&maskCNs1;

        string buf;
        stringstream ss(setups6gen[i]);
        while (ss >> buf)
        {
            applyMove(buf);

        }


        EOfixer();
        CNfixer();

        Edge key (EP, EO, CP, CO, CN);// uncomment this

        if (step==0)
        {
            mapp0.insert(make_pair(key,setups6gen[i]));
            // mapp0[key] = setups6gen[i];
        }
        if (step==1)
        {
            mapp1.insert(make_pair(key,setups6gen[i]));
        }
        if (step==2)
        {
            mapp2.insert(make_pair(key,setups6gen[i]));
        }
        if (step==3)
        {
            mapp3.insert(make_pair(key,setups6gen[i]));
        }
        if (step==4)
        {
            mapp4.insert(make_pair(key,setups6gen[i]));
        }
        if (step==5)
        {
            mapp5.insert(make_pair(key,setups6gen[i]));
        }
        if (step==6)
        {
            mapp6.insert(make_pair(key,setups6gen[i]));
        }
        if (step==7)
        {
            mapp7.insert(make_pair(key,setups6gen[i]));
        }
        if (step==8)
        {
            mapp8.insert(make_pair(key,setups6gen[i]));
        }
    }

    cubeRevert(0);

}

void solves1(int depth, int method, int step, int nom, vector < string > oris, vector < string > rots, int allowedmoves)
{
    solutionsLog.clear();
    rotationsLog.clear();
    orientationsLog.clear();
    string thescramble;
    bool isfirst=1;
    bool isalgset=0;
    mirrorit=0;

    for (int i=0; i<wherretf.size(); i++)
    {
        if (1)
        {
            thescramble=thescramble+wherretf[i][0]+" ";
        }
    }

    if (!(usedrotation.empty()))
    {
        rots.clear();
        rots.push_back(moveReverse(usedrotation));
        oris.clear();
        oris.push_back(usedorientation);
        isfirst=0;
    }
    for (int orr=0; orr<rots.size(); orr++)
    {
        bool nomore=0;
        string thenewscramble;
        solutionsList.clear();
        if (isfirst)
        {
            thenewscramble=rots[orr]+" "+thescramble+" "+moveReverse(rots[orr]);
        }
        else
        {
            thenewscramble=rots[orr]+" "+thescramble+" ";
        }

//cout<<lbdone<<" "<<rbdone<<" "<<method<<" "<<step<<endl;
        if (!lbdone && !rbdone && method==3 && step==3)
        {
            step=2;
            mirrorit=1;
            //cout<<"first block mirrored";
            thenewscramble= "y2 " + thenewscramble + " y2";

        }

        if (!lbdone && rbdone && method==3 && step==2)
        {
            step=3;
            //cout<<"second block mirrored";
            mirrorit=1;
            thenewscramble= "y2 " + thenewscramble + " y2";
        }



        cout<<"\n"<<oris[orr]<<":"<<endl;

        cubeRevert(0);
        masks1(method,step);

        /*  if (mirrorit)
              {
                  if (currentstep=="lb")
                  {
                      currentstep="rb";
                  }
                  else if (currentstep=="rb")
                  {
                      currentstep="lb";
                  }
              }*/


        EP=EPs&maskEPs1;
        EO=EOs&maskEOs1;
        CP=CPs&maskCPs1;
        CO=COs&maskCOs1;
        CN=CNs&maskCNs1;

        //mask the unscrambled cube
        string buf;
        stringstream ss(thenewscramble);
        while (ss >> buf)
        {
            applyMove(buf);
        }
        EOfixer();
        CNfixer();
        EPb=EP;
        EOb=EO;
        CPb=CP;
        COb=CO;
        CNb=CN;


        solutionss1.clear();

        solutionss1.push_back(" ");

        if (allowedmoves==1)
        {
            solutionss1.push_back("U");
            solutionss1.push_back("U2");
            solutionss1.push_back("U'");
            solutionss1.push_back("D");
            solutionss1.push_back("D2");
            solutionss1.push_back("D'");
            solutionss1.push_back("R");
            solutionss1.push_back("R2");
            solutionss1.push_back("R'");
            solutionss1.push_back("L");
            solutionss1.push_back("L2");
            solutionss1.push_back("L'");
            solutionss1.push_back("F");
            solutionss1.push_back("F2");
            solutionss1.push_back("F'");
            solutionss1.push_back("B");
            solutionss1.push_back("B2");
            solutionss1.push_back("B'");
        }

        if (allowedmoves==2)
        {
            solutionss1.push_back("U");
            solutionss1.push_back("U2");
            solutionss1.push_back("U'");
            solutionss1.push_back("D");
            solutionss1.push_back("D2");
            solutionss1.push_back("D'");
            solutionss1.push_back("R");
            solutionss1.push_back("R2");
            solutionss1.push_back("R'");
            solutionss1.push_back("L");
            solutionss1.push_back("L2");
            solutionss1.push_back("L'");
            solutionss1.push_back("F");
            solutionss1.push_back("F2");
            solutionss1.push_back("F'");
            solutionss1.push_back("B");
            solutionss1.push_back("B2");
            solutionss1.push_back("B'");
            solutionss1.push_back("M");
            solutionss1.push_back("M2");
            solutionss1.push_back("M'");
            solutionss1.push_back("r");
            solutionss1.push_back("r2");
            solutionss1.push_back("r'");
        }

        if (allowedmoves==3)
        {
            solutionss1.push_back("U");
            solutionss1.push_back("U2");
            solutionss1.push_back("U'");
            solutionss1.push_back("R");
            solutionss1.push_back("R2");
            solutionss1.push_back("R'");
            solutionss1.push_back("M");
            solutionss1.push_back("M2");
            solutionss1.push_back("M'");
            solutionss1.push_back("r");
            solutionss1.push_back("r2");
            solutionss1.push_back("r'");
        }

        if (allowedmoves==4)
        {
            solutionss1.push_back("U");
            solutionss1.push_back("U2");
            solutionss1.push_back("U'");
            solutionss1.push_back("D");
            solutionss1.push_back("D2");
            solutionss1.push_back("D'");
            solutionss1.push_back("R");
            solutionss1.push_back("R2");
            solutionss1.push_back("R'");
        }

        if (allowedmoves==5)
        {
            solutionss1.push_back("U");
            solutionss1.push_back("U2");
            solutionss1.push_back("U'");
            solutionss1.push_back("D");
            solutionss1.push_back("D2");
            solutionss1.push_back("D'");
            solutionss1.push_back("R");
            solutionss1.push_back("R2");
            solutionss1.push_back("R'");
            solutionss1.push_back("L");
            solutionss1.push_back("L2");
            solutionss1.push_back("L'");
        }

        if (allowedmoves==6)
        {
            solutionss1.push_back("U");
            solutionss1.push_back("U2");
            solutionss1.push_back("U'");
            solutionss1.push_back("R");
            solutionss1.push_back("R2");
            solutionss1.push_back("R'");
            solutionss1.push_back("F");
            solutionss1.push_back("F2");
            solutionss1.push_back("F'");
            solutionss1.push_back("B");
            solutionss1.push_back("B2");
            solutionss1.push_back("B'");
        }

        if (allowedmoves==7)
        {
            solutionss1.push_back("U");
            solutionss1.push_back("U2");
            solutionss1.push_back("U'");
            solutionss1.push_back("M");
            solutionss1.push_back("M2");
            solutionss1.push_back("M'");
        }
        if (allowedmoves==8)
        {
            solutionss1.push_back("U");
            solutionss1.push_back("U2");
            solutionss1.push_back("U'");
            nom=1;
            isalgset=1;
        }
        if (allowedmoves==9)
        {
            solutionss1.push_back("U");
            solutionss1.push_back("U2");
            solutionss1.push_back("U'");
            solutionss1.push_back("M");
            solutionss1.push_back("M2");
            solutionss1.push_back("M'");
            solutionss1.push_back("D");
            solutionss1.push_back("D2");
            solutionss1.push_back("D'");
        }
        if (allowedmoves==10)
        {
            solutionss1.push_back("U");
            solutionss1.push_back("U2");
            solutionss1.push_back("U'");
            solutionss1.push_back("L");
            solutionss1.push_back("L2");
            solutionss1.push_back("L'");
            solutionss1.push_back("D");
            solutionss1.push_back("D2");
            solutionss1.push_back("D'");
        }


        if (!isalgset)
        {
            int startat=1;
            for (int ii=1; ii<depth; ii++)
            {
                int endat=solutionss1.size();
                for (int i=startat; i<endat; i++)
                {
                    addLayers(solutionss1[i],solutionss1, allowedmoves);
                }

                startat=endat;
            }
        }
        else if (isalgset)
        {
            for (int i=0; i<4; i++)
            {
                algparser(solutionss1[i],solutionss1, method);
            }

        }


        int foundsolutions=0;
        for (int i=0; i<solutionss1.size(); i++)
        {
            EP=EPb;
            EO=EOb;
            CP=CPb;
            CO=COb;
            CN=CNb;


            string buf;
            stringstream ss(solutionss1[i]);
            while (ss >> buf)
            {
                applyMove(buf);
            }

//check the prune table to see if that sequence puts you on it

            EOfixer();
            CNfixer();

            Edge key (EP, EO, CP, CO, CN);


            if (step==0)
            {

                if (mapp0.find(key) != mapp0.end())
                {

                    pair <boost::unordered_multimap<Edge,string>::iterator,boost::unordered_multimap<Edge,string>::iterator> ret = mapp0.equal_range(key);
                    boost::unordered_multimap<Edge,string>::iterator it;
                    for (it=ret.first; it!=ret.second; ++it)
                    {

                        string solfound=(*it).second;

                        if (std::find(solutionsList.begin(), solutionsList.end(), solutionreducer(solutionss1[i]+" "+moveReverse(solfound))) == solutionsList.end())
                        {
                            string soln=solutionreducer(solutionss1[i]+" "+moveReverse(solfound));
                            if (mirrorit)
                            {
                                soln=y2rotator(soln);
                            }

                            if (isfirst)
                            {
                                rotationsLog.push_back(moveReverse(rots[orr])+" ");
                                orientationsLog.push_back(oris[orr]);
                                solutionsLog.push_back(soln);
                                solutionsList.push_back(soln);
                                foundsolutions++;
                                cout<<"\n   " <<solutionsLog.size()<<".  "<<moveReverse(rots[orr])<< soln <<" ("<<std::dec<<countWords(solutionss1[i])+countWords(solfound)<<")\n";

                            }
                            else
                            {
                                solutionsLog.push_back(soln);
                                solutionsList.push_back(soln);
                                foundsolutions++;
                                cout<<"\n   " <<solutionsLog.size()<<".  "<< soln <<" ("<<std::dec<<countWords(solutionss1[i])+countWords(solfound)<<")\n";
                            }
                            if (foundsolutions==nom)
                            {
                                nomore=1;
                                break;   // make it so it doesnt do more than the lowest amount of moves
                            }
                        }
                    }
                }
            }

            if (step==1)
            {

                if (mapp1.find(key) != mapp1.end())
                {

                    pair <boost::unordered_multimap<Edge,string>::iterator,boost::unordered_multimap<Edge,string>::iterator> ret = mapp1.equal_range(key);
                    boost::unordered_multimap<Edge,string>::iterator it;
                    for (it=ret.first; it!=ret.second; ++it)
                    {

                        string solfound=(*it).second;

                        if (std::find(solutionsList.begin(), solutionsList.end(), solutionreducer(solutionss1[i]+" "+moveReverse(solfound))) == solutionsList.end())
                        {
                            string soln=solutionreducer(solutionss1[i]+" "+moveReverse(solfound));
                            if (mirrorit)
                            {
                                soln=y2rotator(soln);
                            }

                            if (isfirst)
                            {
                                rotationsLog.push_back(moveReverse(rots[orr])+" ");
                                orientationsLog.push_back(oris[orr]);
                                solutionsLog.push_back(soln);
                                solutionsList.push_back(soln);
                                foundsolutions++;
                                cout<<"\n   " <<solutionsLog.size()<<".  "<<moveReverse(rots[orr])<< soln <<" ("<<std::dec<<countWords(solutionss1[i])+countWords(solfound)<<")\n";

                            }
                            else
                            {
                                solutionsLog.push_back(soln);
                                solutionsList.push_back(soln);
                                foundsolutions++;
                                cout<<"\n   " <<solutionsLog.size()<<".  "<< soln <<" ("<<std::dec<<countWords(solutionss1[i])+countWords(solfound)<<")\n";
                            }
                            if (foundsolutions==nom)
                            {
                                nomore=1;
                                break;   // make it so it doesnt do more than the lowest amount of moves
                            }
                        }
                    }
                }
            }


            if (step==2)
            {

                if (mapp2.find(key) != mapp2.end())
                {

                    pair <boost::unordered_multimap<Edge,string>::iterator,boost::unordered_multimap<Edge,string>::iterator> ret = mapp2.equal_range(key);
                    boost::unordered_multimap<Edge,string>::iterator it;
                    for (it=ret.first; it!=ret.second; ++it)
                    {

                        string solfound=(*it).second;

                        if (std::find(solutionsList.begin(), solutionsList.end(), solutionreducer(solutionss1[i]+" "+moveReverse(solfound))) == solutionsList.end())
                        {
                            string soln=solutionreducer(solutionss1[i]+" "+moveReverse(solfound));
                            if (mirrorit)
                            {
                                soln=y2rotator(soln);
                            }

                            if (isfirst)
                            {
                                rotationsLog.push_back(moveReverse(rots[orr])+" ");
                                orientationsLog.push_back(oris[orr]);
                                solutionsLog.push_back(soln);
                                solutionsList.push_back(soln);
                                foundsolutions++;
                                cout<<"\n   " <<solutionsLog.size()<<".  "<<moveReverse(rots[orr])<< soln <<" ("<<std::dec<<countWords(solutionss1[i])+countWords(solfound)<<")\n";

                            }
                            else
                            {
                                solutionsLog.push_back(soln);
                                solutionsList.push_back(soln);
                                foundsolutions++;
                                cout<<"\n   " <<solutionsLog.size()<<".  "<< soln <<" ("<<std::dec<<countWords(solutionss1[i])+countWords(solfound)<<")\n";
                            }
                            if (foundsolutions==nom)
                            {
                                nomore=1;
                                break;   // make it so it doesnt do more than the lowest amount of moves
                            }
                        }
                    }
                }
            }


            if (step==3)
            {

                if (mapp3.find(key) != mapp3.end())
                {

                    pair <boost::unordered_multimap<Edge,string>::iterator,boost::unordered_multimap<Edge,string>::iterator> ret = mapp3.equal_range(key);
                    boost::unordered_multimap<Edge,string>::iterator it;
                    for (it=ret.first; it!=ret.second; ++it)
                    {

                        string solfound=(*it).second;

                        if (std::find(solutionsList.begin(), solutionsList.end(), solutionreducer(solutionss1[i]+" "+moveReverse(solfound))) == solutionsList.end())
                        {
                            string soln=solutionreducer(solutionss1[i]+" "+moveReverse(solfound));
                            if (mirrorit)
                            {
                                soln=y2rotator(soln);
                            }

                            if (isfirst)
                            {
                                rotationsLog.push_back(moveReverse(rots[orr])+" ");
                                orientationsLog.push_back(oris[orr]);
                                solutionsLog.push_back(soln);
                                solutionsList.push_back(soln);
                                foundsolutions++;
                                cout<<"\n   " <<solutionsLog.size()<<".  "<<moveReverse(rots[orr])<< soln <<" ("<<std::dec<<countWords(solutionss1[i])+countWords(solfound)<<")\n";

                            }
                            else
                            {
                                solutionsLog.push_back(soln);
                                solutionsList.push_back(soln);
                                foundsolutions++;
                                cout<<"\n   " <<solutionsLog.size()<<".  "<< soln <<" ("<<std::dec<<countWords(solutionss1[i])+countWords(solfound)<<")\n";
                            }
                            if (foundsolutions==nom)
                            {
                                nomore=1;
                                break;   // make it so it doesnt do more than the lowest amount of moves
                            }
                        }
                    }
                }
            }


            if (step==4)
            {
                if (mapp4.find(key) != mapp4.end())
                {
                    pair <boost::unordered_multimap<Edge,string>::iterator,boost::unordered_multimap<Edge,string>::iterator> ret = mapp4.equal_range(key);
                    boost::unordered_multimap<Edge,string>::iterator it;
                    for (it=ret.first; it!=ret.second; ++it)
                    {

                        string solfound=(*it).second;

                        if (method==2) //remove correctional AUF if it's roux method
                        {
                            solfound=remove_last_word_if(solfound);
                        }


                        if (std::find(solutionsList.begin(), solutionsList.end(), solutionreducer(solutionss1[i]+" "+moveReverse(solfound))) == solutionsList.end())
                        {
                            string soln=solutionreducer(solutionss1[i]+" "+moveReverse(solfound));
                            if (mirrorit)
                            {
                                soln=y2rotator(soln);
                            }

                            if (isfirst)
                            {
                                rotationsLog.push_back(moveReverse(rots[orr])+" ");
                                orientationsLog.push_back(oris[orr]);
                                solutionsLog.push_back(soln);
                                solutionsList.push_back(soln);
                                foundsolutions++;
                                cout<<"\n   " <<solutionsLog.size()<<".  "<<moveReverse(rots[orr])<< soln <<" ("<<std::dec<<countWords(solutionss1[i])+countWords(solfound)<<")\n";

                            }
                            else
                            {
                                solutionsLog.push_back(soln);
                                solutionsList.push_back(soln);
                                foundsolutions++;
                                cout<<"\n   " <<solutionsLog.size()<<".  "<< soln <<" ("<<std::dec<<countWords(solutionss1[i])+countWords(solfound)<<")\n";
                            }
                            if (foundsolutions==nom)
                            {
                                nomore=1;
                                break;   // make it so it doesnt do more than the lowest amount of moves
                            }
                        }
                    }
                }
            }





            if (step==5)
            {

                if (mapp5.find(key) != mapp5.end())
                {

                    pair <boost::unordered_multimap<Edge,string>::iterator,boost::unordered_multimap<Edge,string>::iterator> ret = mapp5.equal_range(key);
                    boost::unordered_multimap<Edge,string>::iterator it;
                    for (it=ret.first; it!=ret.second; ++it)
                    {

                        string solfound=(*it).second;

                        if (std::find(solutionsList.begin(), solutionsList.end(), solutionreducer(solutionss1[i]+" "+moveReverse(solfound))) == solutionsList.end())
                        {
                            string soln=solutionreducer(solutionss1[i]+" "+moveReverse(solfound));
                            if (mirrorit)
                            {
                                soln=y2rotator(soln);
                            }

                            if (isfirst)
                            {
                                rotationsLog.push_back(moveReverse(rots[orr])+" ");
                                orientationsLog.push_back(oris[orr]);
                                solutionsLog.push_back(soln);
                                solutionsList.push_back(soln);
                                foundsolutions++;
                                cout<<"\n   " <<solutionsLog.size()<<".  "<<moveReverse(rots[orr])<< soln <<" ("<<std::dec<<countWords(solutionss1[i])+countWords(solfound)<<")\n";

                            }
                            else
                            {
                                solutionsLog.push_back(soln);
                                solutionsList.push_back(soln);
                                foundsolutions++;
                                cout<<"\n   " <<solutionsLog.size()<<".  "<< soln <<" ("<<std::dec<<countWords(solutionss1[i])+countWords(solfound)<<")\n";
                            }
                            if (foundsolutions==nom)
                            {
                                nomore=1;
                                break;   // make it so it doesnt do more than the lowest amount of moves
                            }
                        }
                    }
                }
            }

            if (step==6)
            {

                if (mapp6.find(key) != mapp6.end())
                {

                    pair <boost::unordered_multimap<Edge,string>::iterator,boost::unordered_multimap<Edge,string>::iterator> ret = mapp6.equal_range(key);
                    boost::unordered_multimap<Edge,string>::iterator it;
                    for (it=ret.first; it!=ret.second; ++it)
                    {

                        string solfound=(*it).second;

                        if (std::find(solutionsList.begin(), solutionsList.end(), solutionreducer(solutionss1[i]+" "+moveReverse(solfound))) == solutionsList.end())
                        {
                            string soln=solutionreducer(solutionss1[i]+" "+moveReverse(solfound));
                            if (mirrorit)
                            {
                                soln=y2rotator(soln);
                            }

                            if (isfirst)
                            {
                                rotationsLog.push_back(moveReverse(rots[orr])+" ");
                                orientationsLog.push_back(oris[orr]);
                                solutionsLog.push_back(soln);
                                solutionsList.push_back(soln);
                                foundsolutions++;
                                cout<<"\n   " <<solutionsLog.size()<<".  "<<moveReverse(rots[orr])<< soln <<" ("<<std::dec<<countWords(solutionss1[i])+countWords(solfound)<<")\n";

                            }
                            else
                            {
                                solutionsLog.push_back(soln);
                                solutionsList.push_back(soln);
                                foundsolutions++;
                                cout<<"\n   " <<solutionsLog.size()<<".  "<< soln <<" ("<<std::dec<<countWords(solutionss1[i])+countWords(solfound)<<")\n";
                            }
                            if (foundsolutions==nom)
                            {
                                nomore=1;
                                break;   // make it so it doesnt do more than the lowest amount of moves
                            }
                        }
                    }
                }
            }


            if (step==7)
            {

                if (mapp7.find(key) != mapp7.end())
                {

                    pair <boost::unordered_multimap<Edge,string>::iterator,boost::unordered_multimap<Edge,string>::iterator> ret = mapp7.equal_range(key);
                    boost::unordered_multimap<Edge,string>::iterator it;
                    for (it=ret.first; it!=ret.second; ++it)
                    {

                        string solfound=(*it).second;

                        if (std::find(solutionsList.begin(), solutionsList.end(), solutionreducer(solutionss1[i]+" "+moveReverse(solfound))) == solutionsList.end())
                        {
                            string soln=solutionreducer(solutionss1[i]+" "+moveReverse(solfound));
                            if (mirrorit)
                            {
                                soln=y2rotator(soln);
                            }

                            if (isfirst)
                            {
                                rotationsLog.push_back(moveReverse(rots[orr])+" ");
                                orientationsLog.push_back(oris[orr]);
                                solutionsLog.push_back(soln);
                                solutionsList.push_back(soln);
                                foundsolutions++;
                                cout<<"\n   " <<solutionsLog.size()<<".  "<<moveReverse(rots[orr])<< soln <<" ("<<std::dec<<countWords(solutionss1[i])+countWords(solfound)<<")\n";

                            }
                            else
                            {
                                solutionsLog.push_back(soln);
                                solutionsList.push_back(soln);
                                foundsolutions++;
                                cout<<"\n   " <<solutionsLog.size()<<".  "<< soln <<" ("<<std::dec<<countWords(solutionss1[i])+countWords(solfound)<<")\n";
                            }
                            if (foundsolutions==nom)
                            {
                                nomore=1;
                                break;   // make it so it doesnt do more than the lowest amount of moves
                            }
                        }
                    }
                }
            }


            if (step==8)
            {

                if (mapp8.find(key) != mapp8.end())
                {

                    pair <boost::unordered_multimap<Edge,string>::iterator,boost::unordered_multimap<Edge,string>::iterator> ret = mapp8.equal_range(key);
                    boost::unordered_multimap<Edge,string>::iterator it;
                    for (it=ret.first; it!=ret.second; ++it)
                    {

                        string solfound=(*it).second;

                        if (std::find(solutionsList.begin(), solutionsList.end(), solutionreducer(solutionss1[i]+" "+moveReverse(solfound))) == solutionsList.end())
                        {
                            string soln=solutionreducer(solutionss1[i]+" "+moveReverse(solfound));
                            if (mirrorit)
                            {
                                soln=y2rotator(soln);
                            }

                            if (isfirst)
                            {
                                rotationsLog.push_back(moveReverse(rots[orr])+" ");
                                orientationsLog.push_back(oris[orr]);
                                solutionsLog.push_back(soln);
                                solutionsList.push_back(soln);
                                foundsolutions++;
                                cout<<"\n   " <<solutionsLog.size()<<".  "<<moveReverse(rots[orr])<< soln <<" ("<<std::dec<<countWords(solutionss1[i])+countWords(solfound)<<")\n";

                            }
                            else
                            {
                                solutionsLog.push_back(soln);
                                solutionsList.push_back(soln);
                                foundsolutions++;
                                cout<<"\n   " <<solutionsLog.size()<<".  "<< soln <<" ("<<std::dec<<countWords(solutionss1[i])+countWords(solfound)<<")\n";
                            }
                            if (foundsolutions==nom)
                            {
                                nomore=1;
                                break;   // make it so it doesnt do more than the lowest amount of moves
                            }
                        }
                    }
                }
            }



            if (nomore)
            {
                break;
            }


        }
        if (!foundsolutions)
        {
            cout<<"\n   no solutions found within search depth in prescribed movegroup\n";
        }
        else if (foundsolutions<nom)
        {
            cout<<"\n   only "<<foundsolutions<<" solution(s) found.\n";
        }
        cout<<endl;

    }

    cleanScrambler();
}

void customparser(void)
{
    ifstream infile("custom.txt");
    int stepnumber=-1;
    for( string line; getline( infile, line ); )
    {
        string sss;
        bool isstep=0;
        bool ismg=0;
        bool ispd=0;
        bool issd=0;
        bool isep=0;
        bool iseo=0;
        bool iscp=0;
        bool isco=0;
        bool iscn=0;
        bool isall=0;
        int commandnumber=1;
        stringstream comma(line);
        while (comma >> sss)
        {
            if (isall)
            {
                customall.push_back(sss.c_str());
            }
            if (commandnumber==2 && isall==0)
            {
                if (isstep)
                {
                    customsteps[stepnumber]=sss.c_str();
                    isstep=0;
                }
                else
                {
                    __int64 x;
                    stringstream tohex;
                    tohex<<std::hex<<sss;
                    tohex>>x;
                    if (ismg)
                    {
                        customdets[stepnumber][0]=x;
                        ismg=0;
                    }
                    if (ispd)
                    {
                        customdets[stepnumber][1]=x;
                        ispd=0;
                    }
                    if (issd)
                    {
                        customdets[stepnumber][2]=x;
                        issd=0;
                    }
                    if (isep)
                    {
                        customdets[stepnumber][3]=x;
                        isep=0;
                    }
                    if (iseo)
                    {
                        customdets[stepnumber][4]=x;
                        iseo=0;
                    }
                    if (iscp)
                    {
                        customdets[stepnumber][5]=x;
                        iscp=0;
                    }
                    if (isco)
                    {
                        customdets[stepnumber][6]=x;
                        isco=0;
                    }
                    if (iscn)
                    {
                        customdets[stepnumber][7]=x;
                        iscn=0;
                    }
                    commandnumber++;
                }
            }
            if (commandnumber==1)
            {
                if (sss=="step")
                {
                    isstep=1;
                    stepnumber++;
                }
                if (sss=="movegroup")
                {
                    ismg=1;
                }
                if (sss=="prunedepth")
                {
                    ispd=1;
                }
                if (sss=="searchdepth")
                {
                    issd=1;
                }
                if (sss=="epmask")
                {
                    isep=1;
                }
                if (sss=="eomask")
                {
                    iseo=1;
                }
                if (sss=="cpmask")
                {
                    iscp=1;
                }
                if (sss=="comask")
                {
                    isco=1;
                }
                if (sss=="cnmask")
                {
                    iscn=1;
                }
                if (sss=="all")
                {
                    isall=1;
                }
                commandnumber++;
            }
        }
    }
    cout<<"\n   Loading";
    for (int stepgo=0; stepgo<customsteps.size(); stepgo++)
    {
        if (customsteps[stepgo]!="")
        {
            cout<<" "<<customsteps[stepgo]<<",";
            prunes1(customdets[stepgo][1],4,stepgo,customdets[stepgo][0]);
        }
    }

    cout<<" Ready.\n\n";
}


bool executecommand(string comma, int nom, vector < string > oris, vector < string > rots)
{
    bool firstdo=1;
    if (comma=="all")
    {
        for (int j=0; j<customall.size(); j++)
        {
            for (int i=0; i<customsteps.size(); i++)
            {
                if (customall[j]==customsteps[i])
                {
                    solves1(customdets[i][2],4,i,1,oris,rots,customdets[i][0]);
                    firstdo=dosomething(0,1," ",firstdo);
                }
            }
        }
        return 0;
    }
    else
    {
        for (int i=0; i<customsteps.size(); i++)
        {
            if (comma==customsteps[i])
            {
                solves1(customdets[i][2],4,i,nom,oris,rots,customdets[i][0]);
                return 0;
            }
        }
    }
    return 1;
}
