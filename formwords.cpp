#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cmath>
#include <cctype>
#include <sys/time.h>
#include <cstdio>

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
    vector<string> els1, els2, els3;
    ifstream dict("dict.txt");
    ifstream el1("el1.txt");
    ifstream el2("el2.txt");
    ifstream el3("el3.txt"); 
    
    while (el1 >> temp)
        els1.push_back(temp);
     
    while (el2 >> temp)
        els2.push_back(temp);
        
    //while (el3 >> temp)
    //{
    //    els3.push_back(temp);
    //    //cout << els3[els3.size()-1] << endl;
    //}
    
    while (dict >> word)
    {
        int truthtable = 1, tablecount = 0;
        bool fail = false;
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
//  3 char symbols
        for (int idx = 0; idx < els3.size(); ++idx)
        {
            for (int idx2 = 0; idx2 < word.length()-1; ++idx2)
            {
                if (els3[idx][0] == toupper(word[idx2]) && els3[idx][1] == tolower(word[idx2+1]) && els3[idx][2] == tolower(word[idx2+2]))
                {
                    temp = fill;
                    temp[idx2] = els3[idx][0];
                    temp[idx2+1] = els3[idx][1];
                    temp[idx2+2] = els3[idx][2];
                    idx2 += 2;
                }
            } 
        }
*/  
    
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
                    
                    if (lower >= 2)
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
