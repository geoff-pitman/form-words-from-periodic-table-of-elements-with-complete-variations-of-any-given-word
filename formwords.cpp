#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cmath>
#include <cctype>
#include <sys/time.h>

using namespace std;

float gettime_ms(struct timeval start, struct timeval end)
{
    float secs  = end.tv_sec  - start.tv_sec;
    float usecs = end.tv_usec - start.tv_usec;
    float mtime = ((secs)*1000 + usecs/1000) + .0005;
    
    return mtime;
}  

int main()
{
    struct timeval start, end;
    float stopwatch;
    gettimeofday(&start, NULL); 
    
    string word, temp;
    vector<string> els1, els2;
    ifstream dict("dict.txt");
    ifstream el1("el1.txt");
    ifstream el2("el2.txt");
    
    while (el1 >> temp)
        els1.push_back(temp);
     
    while (el2 >> temp)
        els2.push_back(temp);
    
    while (dict >> word)
    {
        int truthtable = 1, tablecount = 0;
        bool fail = false, ucheck = false;
        string conts, fill;
        vector<bool> captrack;
        vector<int> variations;
        vector<string> wordtable, results;
 
        for(int idx = 0; idx < word.length(); ++idx)
            variations.push_back(0);
    
        for(int idx = 0; idx < word.length(); ++idx)
            fill += "?";
    
        conts = fill;
    
 
////////////////////////////////////////////////////////////////////////////////
// FIND MATCHING LETTERS
//////////////////////////////////////////////////////////////////////////////// 
// 1 char symbols
        for (int idx = 0; idx < els1.size(); ++idx)
        {
            for (int idx2 = 0; idx2 < word.length(); ++idx2)
            {
                if (els1[idx][0] == toupper(word[idx2]))
                {
                    temp = fill;
                    temp[idx2] = els1[idx][0];
                    ++variations[idx2];   
                }
            }        
        }
 
// 2 char symbols 
        for (int idx = 0; idx < els2.size(); ++idx)
        {
            for (int idx2 = 0; idx2 < word.length()-1; ++idx2)
            {
                if (els2[idx][0] == toupper(word[idx2]) && els2[idx][1] == tolower(word[idx2+1]))
                {
                    temp = fill;
                    temp[idx2] = els2[idx][0];
                    temp[idx2+1] = els2[idx][1];
                
                    if (variations[idx2] == 0)
                        ++variations[idx2];
                           
                    ++idx2;
                    ++variations[idx2];
                
                    if (variations[idx2] == 1)
                        conts[idx2] = els2[idx][1];
                    else if (variations[idx2] > 1)
                        conts[idx2] = '?';
                }
            } 
        }


 /*  
specific 3 char symbol check
Uuo: O)safe
Uup: P)a,d,m,r,t
Uus: S)e,g,m,r
Uut: *)Ta,Te,Tl,Tm
*/
//////////////////////////////////////////////////////////
/////////  !!!! NOT FULLY TESTED !!!  ////////////////////
        for (int idx = 0; idx < word.length()-2; ++idx)
        {
            if (word[idx] == 'u' && word[idx+1] == 'u')
            {
                if (word[idx+2] == 'o')
                {
                    ucheck = true;
                    ++variations[idx+1];
                    ++variations[idx+2];
                }
                else if (word[idx+2] == 'p')
                {
                    ucheck = true;
                    ++variations[idx+1];
                    
                    if (idx < word.length()-3 && variations [idx+2] < 2)
                    {
                        if (word[idx+3] == 'a')
                            ++variations[idx+2];
                        else if (word[idx+3] == 'd')
                            ++variations[idx+2];
                        else if (word[idx+3] == 'm')
                            ++variations[idx+2];
                        else if (word[idx+3] == 'r')
                            ++variations[idx+2];
                        else if (word[idx+3] == 't')
                            ++variations[idx+2];    
                    }
                }
                else if (word[idx+2] == 's')
                {
                    ucheck = true;
                    ++variations[idx+1];
                    
                    if (idx == word.length()-3)
                        ++variations[idx+2];
                    else if (idx < word.length()-3 && variations[idx+2] < 2)
                    {
                        if (word[idx+3] == 'e')
                            ++variations[idx+2];
                        else if (word[idx+3] == 'g')
                            ++variations[idx+2];
                        else if (word[idx+3] == 'm')
                            ++variations[idx+2];
                        else if (word[idx+3] == 'r')
                            ++variations[idx+2];
                    }
                }
                else if (word[idx+2] == 't')
                {
                    ucheck = true;
                    ++variations[idx+1];
                   
                    if (idx == word.length()-3 && variations[idx+2] == 0)
                        ++variations[idx+2];
                }
            }
        }
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////         
         
        for(int idx = 0; idx < word.length(); ++idx)
        {
            if (variations[idx] == 0)
            {
                cout << word << ": ***FAIL***" << endl << endl;
                fail = true;
                
                break;
            }
            else if (variations[idx] == 2)
            {
                truthtable *= 2;
                ++tablecount;
            }
        }
        if (fail)
        {
            fail = false;
            
            continue;
        }

        for (int idx = 0; idx < tablecount; ++idx)
            captrack.push_back(false);
   
        for (int idx = 0; idx < truthtable; ++idx)
        {
            int trackexp = 0;
            wordtable.push_back(fill);
            
            for (int idx2 = word.length()-1; idx2 >= 0; --idx2)
            {
                if (variations[idx2] == 1)
                {
                    if (conts[idx2] == '?')
                        wordtable[idx][idx2] = toupper(word[idx2]);
                    else
                        wordtable[idx][idx2] = tolower(word[idx2]);         
                }
                else
                {
                    if (captrack[trackexp] == false)
                    {
                        wordtable[idx][idx2] = tolower(word[idx2]);
                        
                        if ((idx % (int)(pow(2, trackexp))) == 0)
                            captrack[trackexp] = true;
                    }
                    else if (captrack[trackexp] == true)
                    {
                        wordtable[idx][idx2] = toupper(word[idx2]);
                        
                        if ((idx % (int)(pow(2, trackexp))) == 0)
                            captrack[trackexp] = false;  
                    }      
               
                    ++trackexp;
                }      
            }
        } 
        
        
        for (int idx = 0; idx < truthtable; ++idx)
        {
            int lower = 0;
            bool lowercheck = true;
            bool elscheck = true;
            
            for (int idx2 = 0; idx2 < word.length(); ++idx2)
            {
                if (islower(wordtable[idx][idx2]))
                {
                    ++lower;
                    
                    if (ucheck && lower >= 2 && idx2 < word.length()-2 && wordtable[idx][idx2-2] == 'U' && wordtable[idx][idx2-1] == 'u')
                        lower = 0;
                    else if (lower >= 2)
                    {
                        lowercheck = false;
                        
                        break;
                    }
                }
                
                if (isupper(wordtable[idx][idx2]))
                    lower = 0;
                
                if (isupper(wordtable[idx][idx2]) && idx2 < word.length()-1 && isupper(wordtable[idx][idx2+1]))
                {   
                    elscheck = false;
                   
                    for (int idx3 = 0; idx3 < els1.size(); ++idx3)
                    {
                        if (els1[idx3][0] == wordtable[idx][idx2])
                        {
                            elscheck = true;
                            
                            continue;
                        }
                    }
                    
                    if (elscheck == false)
                        break;
                }
            }
           
            if (elscheck && lowercheck) 
                results.push_back(wordtable[idx]);
        }
    
        if (results.size() == 0)
            cout << word << ": ***FAIL***" << endl << endl;
        else 
        {
            cout << word << ": " << results.size() << " variation(s) found... \n";
         
            for (int idx = 0; idx < results.size(); ++idx)
                cout << "[" << results[idx] << "]\n";
        
            cout << endl;
        } 
    }
  
    gettimeofday(&end, NULL);
    stopwatch = gettime_ms(start, end);
    printf("Runtime: %.3f ms\n", stopwatch);
    
    return 0;
}
