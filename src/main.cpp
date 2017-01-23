#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <stdlib.h>
#include <fstream>
#include "headers.h"
#include <emscripten.h>

using namespace std;

string cubeRevert(bool);
void applyMove(string themove);
void showCube(void);
void prunes1(int depth, int, int, int);
void solves1(int depth, int, int, int, vector <string>, vector <string>, int);
void unpruner(void);
string remove_last_word_if(string sentence);
//void algparser(int);
size_t countWords(std::string s);
string solutionreducer(string);
string scramble;
// string inputWCA;
bool firstWordWCA=0;
bool isScramble2=0;
bool isSolver2=0;
bool isCheck=0;
bool isDepth=0;
int number=3;
int oldnumber=0;
int tempnumber;
bool setOutput=1;
bool prunedyet=0;
int prunedmethod=99;
bool dosomething(int, int, string, bool);
vector < string > orientations= {"UF"};
vector < string > rotations{" "};
bool firstdo=1;
string usedrotation;
string usedorientation;
void customparser(void);
bool executecommand(string comma, int nom, vector < string > oris, vector < string > rots);


void introWCA()
{
    cout <<
         "      ooooo   ooooo       .o.       ooooooooo.     .oooooo.    .oooooo..o \n"
         "      `888'   `888'      .888.      `888   `Y88.  d8P'  `Y8b  d8P'    `Y8 \n"
         "       888     888      .88888.      888   .d88' 888          Y88bo.      \n"
         "       888ooooo888     .8' `888.     888ooo88P'  888           `8888o.  \n"
         "       888     888    .88ooo8888.    888`88b.    888               `88b \n"
         "       888     888   .8'     `888.   888  `88b.  `88b    ooo  oo     .d8P \n"
         "      o888o   o888o o88o     o8888o o888o  o888o  `Y8bood8P'  `8888888P' \n\n\n"
         "                       Here's A Replacement Cube Solver\n\n\n\n\n\n\n";
}
bool is_number(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

string ToString(size_t val)
{
    stringstream stream;
    stream << val;
    return stream.str();
}

string replaceStrChar(string str, const string& replace, char ch) {

size_t found = str.find_first_of(replace);

  while (found != string::npos)
    {
    str[found] = ch;
    found = str.find_first_of(replace, found+1);
  }

  return str; // return our new string.
}

void showHelpWCA()
{
    cout <<"\n   info\t\tdisplay version information\n"
         "   cls\t\tclear the screen\n"
         "   apply * #\tapply a scramble/moves to current cube, ending with #\n\t\t   (ex: apply R U2 B' L ... #)\n"
         "   revert\trevert cube to solved state\n"
         "   number *\tset desired count of substep solutions (default: 3)\n"
         "   state\tshow current hexadecimal cubestate representation\n"
         "   view\t\topen current cube on alg.cubing.net\n"
         "   print\tprint applied sequences on current cube\n"
         "   save\t\tappend applied sequences to text file in directory\n"
         "   style * #\tset solution orientations \n\t\t   (ex: style UF DB #) (default: UF) CN and DCN are options\n"
         "   cfop\t\tprunes for cross (D) and xcross (D,DFR)\n"
         "   petrus\tprunes for 2x2x2 (DLB), 3x2x2 (DL), eo, f2l, and zbll\n"
         "   roux\t\tprunes for fs (DLB), fb (DL), sb (DR), cmll, and lse\n"
         "   zz\t\tprunes for eoline (D), eo3x2x2 (DL), lb, rb, and zbll\n"
         "   custom\tprunes for the method defined in custom.txt\n"
         "   all\t\tsolves entire cube with set method in single orientation\n"
         "\n   To solve a substep for a pruned method, type the step, as written above."
         "\n   To apply a generated substep solution, type the number next to it.\n\n"
         ;

}
int getInputWCA(string inputWCA)
{
        firstWordWCA=0;
        // getline(cin, inputWCA);
        string buf;
        stringstream ss(inputWCA);
        while (ss >> buf)
        {  bool notrecognized=1;
            if (buf=="test")
            {
                firstWordWCA=1;
                cout<<remove_last_word_if("R U B L U2 D R F' U' ");
                //algparser(1);
            }

            if (buf=="custom")
            {
                firstWordWCA=1;
                if (prunedmethod==4)
                {
                    cout<<"\n   custom method already loaded.\n\n";
                }
                else
                {
                    prunedmethod=4;
                    if (prunedyet)
                    {
                        unpruner();

                        cout<<"\n   Clearing old tables.\n";
                    }

                    customparser();


                    prunedyet=1;
                    prunedmethod=4;
                }
            }

            if (buf=="petrus")
            {
                firstWordWCA=1;
                if (prunedmethod==1)
                {
                    cout<<"\n   petrus already loaded.\n\n";
                }
                else
                {
                    prunedmethod=1;
                    if (prunedyet)
                    {
                        unpruner();
                        cout<<"\n   Clearing old tables.\n";
                    }
                    cout<<"\n   Loading 2x2x2,";
                    prunes1(4,1,0,1);
                    cout<<" Loading 3x2x2,";
                    prunes1(5,1,1,1);
                    cout<<" Loading eo,";
                    prunes1(4,1,2,6);
                    cout<<" Loading f2l,";
                    prunes1(6,1,3,4);
                    cout<<" Loading zbll,";
                    prunes1(1,1,4,8);
                    cout<<" Ready.\n\n";
                    prunedyet=1;
                    prunedmethod=1;
                }
            }

            if (buf=="cfop")
            {
                firstWordWCA=1;
                if (prunedmethod==0)
                {
                    cout<<"\n   cfop already loaded.\n\n";
                }
                else
                {
                    if (prunedyet)
                    {
                        unpruner();
                        cout<<"\n   Clearing old tables.\n";
                    }
                    cout<<"\n   Loading cross,";
                    prunes1(4,0,0,1);
                    cout<<" Loading xcross,";
                    prunes1(5,0,1,1);
                    cout<<" Ready.\n\n";
                    prunedyet=1;
                    prunedmethod=0;
                }
            }

            if (buf=="zz")
            {
                firstWordWCA=1;
                if (prunedmethod==3)
                {
                    cout<<"\n   zz already loaded.\n\n";
                }
                else
                {
                    if (prunedyet)
                    {
                        unpruner();
                        cout<<"\n   Clearing old tables.\n";
                    }
                    cout<<"\n   Loading eoline,";
                    prunes1(4,3,0,1);
                    cout<<" Loading eo3x2x2,";
                    prunes1(5,3,1,1);
                    cout<<" Loading lb,";
                    prunes1(6,3,2,5);
                    cout<<" Loading rb,";
                    prunes1(6,3,3,5);
                    cout<<" Loading zbll,";
                    prunes1(1,3,4,8);
                    cout<<" Ready.\n\n";
                    prunedyet=1;
                    prunedmethod=3;
                }
            }
            if (buf=="roux")
            {
                firstWordWCA=1;
                if (prunedmethod==2)
                {
                    cout<<"\n   roux already loaded.\n\n";
                }
                else
                {
                    if (prunedyet)
                    {
                        unpruner();
                        cout<<"\n   Clearing old tables.\n";
                    }
                    cout<<"\n   Loading fs,";
                    prunes1(4,2,3,2);
                    cout<<" Loading fb,";
                    prunes1(4,2,0,2);
                    cout<<" Loading sb,";
                    prunes1(6,2,1,3);
                    cout<<" Loading cmll,";
                    prunes1(1,2,4,8);
                    cout<<" Loading lse,";
                    prunes1(8,2,5,7);
                    cout<<" Ready.\n\n";
                    prunedyet=1;
                    prunedmethod=2;
                }
            }

        if (prunedmethod==4)
            {
                notrecognized=executecommand(buf,number,orientations,rotations);
            }

        else
            {
            if (buf=="all")
            {
                oldnumber=number;
                number=1;
                firstWordWCA=1;
                if (prunedmethod!=1&&prunedmethod!=2&&prunedmethod!=3)
                {
                    cout<<"\n   must be pruned for petrus/roux/zz\n\n";
                }
                else if (orientations.size()==1)
                {

                    if (prunedmethod==1) // petrus
                    {
                    solves1(5,1,1,number,orientations,rotations,1);
                    firstdo=dosomething(0,1," ",firstdo);
                    solves1(5,1,2,number,orientations,rotations,6);
                    firstdo=dosomething(0,1," ",firstdo);
                    solves1(5,1,3,number,orientations,rotations,4);
                    firstdo=dosomething(0,1," ",firstdo);
                    solves1(1,1,4,number,orientations,rotations,8);
                    firstdo=dosomething(0,1," ",firstdo);
                    cout<<"\n   complete petrus solution applied\n\n";
                    }

                    if (prunedmethod==2) // roux
                    {
                    solves1(5,2,0,number,orientations,rotations,2);
                    firstdo=dosomething(0,1," ",firstdo);
                    solves1(6,2,1,number,orientations,rotations,3);
                    firstdo=dosomething(0,1," ",firstdo);
                    solves1(1,2,4,number,orientations,rotations,8);
                    firstdo=dosomething(0,1," ",firstdo);
                    solves1(8,2,5,number,orientations,rotations,7);
                    firstdo=dosomething(0,1," ",firstdo);
                    cout<<"\n   complete roux solution applied\n\n";
                    }

                    if (prunedmethod==3) // zz
                    {
                    solves1(5,3,0,number,orientations,rotations,1);
                    firstdo=dosomething(0,1," ",firstdo);
                    solves1(6,3,2,number,orientations,rotations,5);
                    firstdo=dosomething(0,1," ",firstdo);
                    solves1(6,3,3,number,orientations,rotations,5);
                    firstdo=dosomething(0,1," ",firstdo);
                    solves1(1,1,4,number,orientations,rotations,8);
                    firstdo=dosomething(0,1," ",firstdo);
                    cout<<"\n   complete zz solution applied\n\n";
                    }

                }
                else
                {
                    cout<<"\n   specify a single orientation\n\n";
                }
                number=oldnumber;
            }



            if (buf=="eoline")
            {
                firstWordWCA=1;
                if (prunedmethod!=3)
                {
                    cout<<"\n   zz is not currently loaded.\n\n";
                }
                else
                {
                    solves1(5,3,0,number,orientations,rotations,1);
                }
            }

            if (buf=="eo3x2x2")
            {
                firstWordWCA=1;
                if (prunedmethod!=3)
                {
                    cout<<"\n   zz is not currently loaded.\n\n";
                }
                else
                {
                    solves1(6,3,1,number,orientations,rotations,1);
                }
            }

            if (buf=="lb")
            {
                firstWordWCA=1;
                if (prunedmethod!=3)
                {
                    cout<<"\n   zz is not currently loaded.\n\n";
                }
                else
                {
                    solves1(6,3,2,number,orientations,rotations,5);
                }
            }

             if (buf=="rb")
            {
                firstWordWCA=1;
                if (prunedmethod!=3)
                {
                    cout<<"\n   zz is not currently loaded.\n\n";
                }
                else
                {
                    solves1(6,3,3,number,orientations,rotations,5);
                }
            }

             if (buf=="zbll")
            {
                firstWordWCA=1;
                if (prunedmethod!=3 && prunedmethod!=1)
                {
                    cout<<"\n   neither zz nor petrus is currently loaded.\n\n";
                }
                else
                {
                    solves1(1,1,4,number,orientations,rotations,8);
                }
            }

            if (buf=="fs")
            {
                firstWordWCA=1;
                if (prunedmethod!=2)
                {
                    cout<<"\n   roux is not currently loaded.\n\n";
                }
                else
                {
                    solves1(4,2,3,number,orientations,rotations,2);
                }
            }


            if (buf=="fb")
            {
                firstWordWCA=1;
                if (prunedmethod!=2)
                {
                    cout<<"\n   roux is not currently loaded.\n\n";
                }
                else
                {
                    solves1(5,2,0,number,orientations,rotations,2);
                }
            }

           if (buf=="sb")
            {
                firstWordWCA=1;
                if (prunedmethod!=2)
                {
                    cout<<"\n   roux is not currently loaded.\n\n";
                }
                else
                {
                    solves1(6,2,1,number,orientations,rotations,3);
                }
            }


           if (buf=="cmll")
            {
                firstWordWCA=1;
                if (prunedmethod!=2)
                {
                    cout<<"\n   roux is not currently loaded.\n\n";
                }
                else
                {
                    solves1(1,2,4,number,orientations,rotations,8);
                }
            }


            if (buf=="lse")
            {
                firstWordWCA=1;
                if (prunedmethod!=2)
                {
                    cout<<"\n   roux is not currently loaded.\n\n";
                }
                else
                {
                    solves1(8,2,5,number,orientations,rotations,7);
                }
            }


            if (buf=="cross")
            {
                firstWordWCA=1;
                if (prunedmethod!=0)
                {
                    cout<<"\n   cfop is not currently loaded.\n\n";
                }
                else
                {
                    solves1(5,0,0,number,orientations,rotations,1);
                }
            }

            if (buf=="xcross")
            {
                firstWordWCA=1;
                if (prunedmethod!=0)
                {
                    cout<<"\n   cfop is not currently loaded.\n\n";
                }
                else
                {
                    solves1(5,0,1,number,orientations,rotations,1);
                }
            }

            if (buf=="2x2x2")
            {
                firstWordWCA=1;
                if (prunedmethod!=1)
                {
                    cout<<"\n   petrus is not currently loaded.\n\n";
                }
                else
                {
                    solves1(5,1,0,number,orientations,rotations,1);
                }
            }

            if (buf=="3x2x2")
            {
                firstWordWCA=1;
                if (prunedmethod!=1)
                {
                    cout<<"\n   petrus is not currently loaded.\n\n";
                }
                else
                {
                    solves1(5,1,1,number,orientations,rotations,1);
                }
            }

            if (buf=="eo")
            {
                firstWordWCA=1;
                if (prunedmethod!=1)
                {
                    cout<<"\n   petrus is not currently loaded.\n\n";
                }
                else
                {
                    solves1(5,1,2,number,orientations,rotations,6);
                }
            }

            if (buf=="f2l")
            {
                firstWordWCA=1;
                if (prunedmethod!=1)
                {
                    cout<<"\n   petrus is not currently loaded.\n\n";
                }
                else
                {
                    solves1(5,1,3,number,orientations,rotations,4);
                }
            }

        }
            if (buf=="illuminati")
            {
                firstWordWCA=1;
                cout<<"\n   Confirmed.\n\n";
            }
            if (buf=="help")
            {
                firstWordWCA=1;
                showHelpWCA();
            }
            else if (buf=="cls")
            {
                firstWordWCA=1;
                cout << string(400, '\n');
            }
            else if (buf=="info")
            {
                firstWordWCA=1;
                cout << "\n   HARCS v0.6 : 01-17-2017 : Matt DiPalma : USA\n\n";
                cout << "\n   web port by Kirjava\n\n";
            }
            else if (buf=="number")
            {
                firstWordWCA=1;
                isDepth=1;
            }
            else if (buf=="revert")
            {
                firstWordWCA=1;
                scramble=cubeRevert(1);
                usedrotation.clear();
                firstdo=1;
                cout << "\n   Reset to solved.\n\n";
            }
            else if (buf=="state")
            {
                firstWordWCA=1;
                showCube();
            }
            else if (buf=="print")
            {
                firstWordWCA=1;
                if (!wherretf.empty())
                {
                    for (int j=0; j<wherretf.size(); j++)
                    {
                        cout <<std::dec<< "\n   "<<wherretf[j][0]<<" // "<<wherretf[j][1];
                        if (j>1)
                        {cout<<" ("<<countWords(wherretf[j][0])<<")";}
                        cout<<endl;
                    }
                    cout<<endl;
                }
                else
                {
                    cout<<"\n   no sequences applied yet\n\n";
                }
            }

            else if (buf=="save")
            {
                firstWordWCA=1;
                ofstream outfile;
                outfile.open("solutions.txt",ios_base::app);
                if (!wherretf.empty())
                {
                    //outfile<<endl;
                    for (int j=0; j<wherretf.size(); j++)
                    {
                        outfile << "\n"<<wherretf[j][0]<<" // "<<wherretf[j][1];
                        if (j>1)
                        {outfile<<" ("<<countWords(wherretf[j][0])<<")";}
                       // outfile<<endl;
                    }
                    outfile<<"\n---"<<endl;
                    cout<<"\n   sequences saved to solutions.txt\n\n";
                }
                else
                {
                    cout<<"\n   no sequences applied yet\n\n";
                }
                outfile.close();
            }


            else if (buf=="view")
            {
                firstWordWCA=1;
                if (!wherretf.empty())
                {
                string url="https://alg.cubing.net?";
                string useit;
                    for (int j=0; j<wherretf.size(); j++)
                    {
                        useit=replaceStrChar(wherretf[j][0], "'", '-');
                        useit=replaceStrChar(useit, " ", '_');
                        if (j==0)
                        {
                            url+=("setup="+useit);
                        }
                        else if (j==1)
                        {
                            url+=("&alg="+useit+"_%2F%2F_"+wherretf[j][1]);
                        }
                        else if (j>1)
                        {
                            url+=("%0A"+useit+"_%2F%2F_"+wherretf[j][1]+"_("+ToString(countWords(wherretf[j][0]))+")");
                        }
                     }
                    cout<<"\n   opening alg.cubing.net in default browser\n\n";
                    cout<<"__url_" << url << '\n';
                    // ShellExecute(NULL, "open", url.c_str(), NULL, NULL , SW_SHOWNORMAL );

                }

                else
                {
                    cout<<"\n   no sequences applied yet\n\n";
                }


             // https://alg.cubing.net?setup=R-_U-_L_D_F2&alg=y_%2F%2F_inspectionR_U_B_L2_M_%2F%2F_fb
            }


            else if (buf == "style")
            {
                firstWordWCA=1;
                isCheck=1;
                orientations.clear();
                rotations.clear();

            }
            else if (isCheck)
            {
                if (buf=="UB")
                {
                    orientations.push_back("UB");
                    rotations.push_back("y2");
                }
                else if (buf=="UR")
                {
                    orientations.push_back("UR");
                    rotations.push_back("y'");
                }
                else if (buf=="UF")
                {
                    orientations.push_back("UF");
                    rotations.push_back(" ");
                }
                else if (buf=="UL")
                {
                    orientations.push_back("UL");
                    rotations.push_back("y");
                }
                else if (buf=="BL")
                {
                    orientations.push_back("BL");
                    rotations.push_back("z' y");
                }
                else if (buf=="BR")
                {
                    orientations.push_back("BR");
                    rotations.push_back("z y'");
                }
                else if (buf=="FR")
                {
                    orientations.push_back("FR");
                    rotations.push_back("z' y'");
                }
                else if (buf=="FL")
                {
                    orientations.push_back("FL");
                    rotations.push_back("z y");
                }
                else if (buf=="DB")
                {
                    orientations.push_back("DB");
                    rotations.push_back("x2");
                }
                else if (buf=="DR")
                {
                    orientations.push_back("DR");
                    rotations.push_back("y z2");
                }
                else if (buf=="DF")
                {
                    orientations.push_back("DF");
                    rotations.push_back("z2");
                }
                else if (buf=="DL")
                {
                    orientations.push_back("DL");
                    rotations.push_back("y' z2");
                }
                else if (buf=="BU")
                {
                    orientations.push_back("BU");
                    rotations.push_back("x");
                }
                else if (buf=="RU")
                {
                    orientations.push_back("RU");
                    rotations.push_back("x y");
                }
                else if (buf=="FU")
                {
                    orientations.push_back("FU");
                    rotations.push_back("x y2");
                }
                else if (buf=="LU")
                {
                    orientations.push_back("LU");
                    rotations.push_back("x y'");
                }
                else if (buf=="LB")
                {
                    orientations.push_back("LB");
                    rotations.push_back("z y2");
                }
                else if (buf=="RB")
                {
                    orientations.push_back("RB");
                    rotations.push_back("z' y2");
                }
                else if (buf=="RF")
                {
                    orientations.push_back("RF");
                    rotations.push_back("z");
                }
                else if (buf=="LF")
                {
                    orientations.push_back("LF");
                    rotations.push_back("z'");
                }
                else if (buf=="BD")
                {
                    orientations.push_back("BD");
                    rotations.push_back("x' y2");
                }
                else if (buf=="RD")
                {
                    orientations.push_back("RD");
                    rotations.push_back("x' y'");
                }
                else if (buf=="FD")
                {
                    orientations.push_back("FD");
                    rotations.push_back("x'");
                }
                else if (buf=="LD")
                {
                    orientations.push_back("LD");
                    rotations.push_back("x' y");
                }

                else if (buf=="DCN")
                {
                    orientations.push_back("UB");
                    rotations.push_back("y2");

                    orientations.push_back("UR");
                    rotations.push_back("y'");

                    orientations.push_back("UF");
                    rotations.push_back("");

                    orientations.push_back("UL");
                    rotations.push_back("y");

                    orientations.push_back("DB");
                    rotations.push_back("x2");

                    orientations.push_back("DR");
                    rotations.push_back("y z2");

                    orientations.push_back("DF");
                    rotations.push_back("z2");

                    orientations.push_back("DL");
                    rotations.push_back("y' z2");
                }

                else if (buf=="CN")
                {
                    orientations.push_back("UB");
                    rotations.push_back("y2");

                    orientations.push_back("UR");
                    rotations.push_back("y'");

                    orientations.push_back("UF");
                    rotations.push_back(" ");

                    orientations.push_back("UL");
                    rotations.push_back("y");

                    orientations.push_back("BL");
                    rotations.push_back("z' y");

                    orientations.push_back("BR");
                    rotations.push_back("z y'");

                    orientations.push_back("FR");
                    rotations.push_back("z' y'");

                    orientations.push_back("FL");
                    rotations.push_back("z y");

                    orientations.push_back("DB");
                    rotations.push_back("x2");

                    orientations.push_back("DR");
                    rotations.push_back("y z2");

                    orientations.push_back("DF");
                    rotations.push_back("z2");

                    orientations.push_back("DL");
                    rotations.push_back("y' z2");

                    orientations.push_back("BU");
                    rotations.push_back("x");

                    orientations.push_back("RU");
                    rotations.push_back("x y");

                    orientations.push_back("FU");
                    rotations.push_back("x y2");

                    orientations.push_back("LU");
                    rotations.push_back("x y'");

                    orientations.push_back("LB");
                    rotations.push_back("z y2");

                    orientations.push_back("RB");
                    rotations.push_back("z' y2");

                    orientations.push_back("RF");
                    rotations.push_back("z");

                    orientations.push_back("LF");
                    rotations.push_back("z'");

                    orientations.push_back("BD");
                    rotations.push_back("x' y2");

                    orientations.push_back("RD");
                    rotations.push_back("x' y'");

                    orientations.push_back("FD");
                    rotations.push_back("x'");

                    orientations.push_back("LD");
                    rotations.push_back("x' y");
                }

                else if (buf=="#")
                {
                    cout<<"\n   orientations set to: ";
                    isCheck=0;
                    for (int i=0; i<orientations.size(); i++)
                    {
                        cout<<orientations[i]<<" ";
                    }
                    cout<<"\n\n";
                }
                else
                    cout << "\n   invalid orientation specified.\n\n";

            }



            else if (buf=="apply")
            {
                isScramble2=1;
                cout << "\n   sequence applied\n\n";
            }
            else if (isDepth==1)
            {
                istringstream convert(buf);
                if ( (convert >> tempnumber) )
                {
                    number=tempnumber;
                    cout << "\n   max count set to: " << number << "\n\n";
                    isDepth=0;
                }
                else
                {
                    cout << "\n   invalid count specified; count remains: "<<number<<"\n\n";
                    isDepth=0;
                }

            }

            else if (isScramble2==1)
            {
            if (buf=="#")
            {
                if (isScramble2)
                {
                    isScramble2=0;
                    dosomething(1,0,scramble,firstdo);
                    scramble="";
                   }

            }
            else
            {
                applyMove(buf);
                scramble.append(buf);
                scramble.append(" ");
            }
            }
             else if (buf=="#")
            {

                if (isCheck)
                {
                    cout<<"\n   orientations set to: ";
                    isCheck=0;
                    for (int i=0; i<orientations.size(); i++)
                    {
                        cout<<orientations[i]<<" ";
                    }
                    cout<<"\n\n";
                }
            }
            else if (firstWordWCA==1)
            {
                firstWordWCA=0;
            }

            else if (is_number(buf))
            {
                firstdo=dosomething(0,atoi(buf.c_str())," ",firstdo);
            }

            else
            {
                if (notrecognized)
                {
                cout << "\n   command not recognized\n\n";
                break;
                }
            }
        }

    return 0;
}

int main()
{
    introWCA();
    return 0;
}


extern "C" {
    void cmd(char* pos) {
      getInputWCA((string) pos);
    }
}