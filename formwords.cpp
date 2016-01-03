/*   
Author: Geoffrey Pitman
Creation Date: Dec 27, 2015

Form words with element abbreviations from periodic table of elements.
Find ALL POSSIBLE VARIATIONS of each word!
*/
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cmath>
#include <cctype>
#include <sys/time.h> // *****time header for nix machine*******

using namespace std;


// convert time struct values to milliseconds
float gettime_ms(struct timeval start, struct timeval end)
{
    float secs  = end.tv_sec  - start.tv_sec;
    float usecs = end.tv_usec - start.tv_usec;
    float mtime = ((secs)*1000 + usecs/1000) + .0005;
    
    return mtime;
}  

int main()
{
    int totalvars = 0, totalwords = 0, success = 0, maxvar = 0;
        biggestablesize = 0, varscheck = 0, aftervar = 0;
    string word, temp, uuxtrack, maxword, tableword;
    vector<string> els1, els2, els3, maxvars, bigtable;
    vector<int> stat, biggestable;
    ifstream dict("dictionaryEng.txt");
    ifstream el1("el1.txt");
    ifstream el2("el2.txt");
    
    // load 1 char symbols
    while (el1 >> temp)
        els1.push_back(temp);
    // load 2 char symbols
    while (el2 >> temp)
        els2.push_back(temp); 

//////////////////////////////////////////    
    // used to get time stats
    struct timeval start, end;
    float stopwatch;
    gettimeofday(&start, NULL); 
////////////////////////////////////////  


// BEGIN MAIN PROCESSING ROUTINE 
    while (dict >> word)   // get next word from dictionary
    {
        int truthtable = 1, tablecount = 0;
        bool fail = false, ucheck = false, upass = false, 
             twofail = false, checkstat = false;
        string conts, fill;
        vector<bool> captrack;
        vector<int> variations;
        vector<string> wordtable, results;
        
        ++totalwords;   // for program stats
        
        // check for single char words
        if (word.length() == 1)
        {
            for (int idx = 0; idx < els1.size(); ++idx)
            {
                if (word[0] == tolower(els1[idx][0]))        
                {
                    ++totalvars;
                    ++success;
                    cout << word << ": 1 variation(s) found... \n"
                         << "[" << toupper(word[0]) << "]\n";

                    continue;   // success, go on to next word
                }
            }  

            //cout << word << ": ***FAIL***" << endl << endl;
            
            continue;  // fail, go on to next word
        }
             
        // init char symbol variations map
        for(int idx = 0; idx < word.length(); ++idx)
            variations.push_back(0);
    
        // init the place holder fill var with non alpha chars
        for(int idx = 0; idx < word.length(); ++idx)
            fill += "?";
    
        conts = fill;  // used to keep track of chars with no variation
        uuxtrack = fill;  // used to keep track of possible special 3 char case
    
////////////////////////////////////////////////////////////////////////////////
//                     FIND MATCHING LETTERS
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
                    uuxtrack[idx2] = els1[idx][0];
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
                    uuxtrack[idx2] = els2[idx][0];
                    temp[idx2+1] = els2[idx][1];
                    uuxtrack[idx2+1] = els2[idx][1];
                
                    if (variations[idx2] == 0)
                        ++variations[idx2];
                           
                    ++idx2;
                    ++variations[idx2];
                
                    // check for char symbol constants
                    if (variations[idx2] == 1)
                        conts[idx2] = els2[idx][1];  
                    else if (variations[idx2] > 1)
                        conts[idx2] = '?';
                }
            } 
        }
/*
!!!! special case 3-char-symbol check  !!! 
  Happens rarely, so check is used to make sure it matches.
  If a match is found for one of the 3 char symbols, the rest
  of the process is initiated.  
Uuo: O)safe
Uup: P)a,d,m,r,t
Uus: S)e,g,m,r
Uut: *)Ta,Te,Tl,Tm
*/
        for (int idx = 0; idx < word.length()-2; ++idx)
        {
            if (word[idx] == 'u' && word[idx+1] == 'u')
            {
                if (word[idx+2] == 'o')
                    ucheck = true;
                else if (word[idx+2] == 'p')
                    ucheck = true;
                else if (word[idx+2] == 's')
                    ucheck = true;
                else if (word[idx+2] == 't')
                    ucheck = true;
            }
        }

        if (ucheck)
        {
           ifstream el3("el3.txt");
           
            while (el3 >> temp)
                els3.push_back(temp);
            
            // check to see if word can be formed without 3-char-symbol
            for (int idx = 0; idx < word.length(); ++idx)
            {
                if (uuxtrack[idx] == '?')
                    twofail = true;
            }        
                
            for (int idx = 0; idx < els3.size(); ++idx)
            {   
                for (int idx2 = 0; idx2 < word.length()-2; ++idx2)
                {
                    if ((els3[idx][0] == toupper(word[idx2])) && (els3[idx][1] == tolower(word[idx2+1]))
                                                              && (els3[idx][2] == tolower(word[idx2+2])))
                    {
                        uuxtrack[idx2] = els3[idx][0];
                        uuxtrack[idx2+1] = els3[idx][1];
                        uuxtrack[idx2+2] = els3[idx][2];
                        idx2 += 3;
                    }
                }
            } 
            
            upass = true;
            for (int idx = 0; idx < word.length(); ++idx)
            {
                if (uuxtrack[idx] == '?')
                {
                    upass = false;  
                    
                    break;    // fail, go on to 1 and 2 char symbol check
                }
                else if (idx < word.length() - 3 && uuxtrack[idx] == 'U' && uuxtrack[idx+1] == 'u'
                                            && islower(uuxtrack[idx+2]) && islower(uuxtrack[idx+3]))
                {
                    upass = false;
                    for (int idx2 = 0; idx2 < els1.size(); ++idx2)
                    {
                        if (tolower(uuxtrack[idx+3]) == tolower(els1[idx2][0]))
                        {
                            uuxtrack[idx+3] = toupper(els1[idx2][0]);
                            upass = true;
                        }
                    }
                    if (!upass)
                        break;   // fail, go on to 1 and 2 char symbol check
                }
            }
            
            // if UuX check fails, continue to normal routine
            if (upass)
            {
                results.push_back(uuxtrack);
                
               // if word can only be formed with 3-char-symbol then spit out results
               if (twofail)
               {                  
                    totalvars += results.size();
                    ++success;                    
                    cout << word << ": " << results.size() << " variation(s) found... \n";
         
                    for (int idx = 0; idx < results.size(); ++idx)
                        cout << "[" << results[idx] << "]\n";
                    
                    cout << endl;
                
                    continue;  // success, go on to next word
               }
            }
            else if (twofail)
            {
                //cout << word << ": ***FAIL***" << endl << endl;
            
                continue;   // fail, go on to next word
            }
        }      
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
      
///////////////////////////////////////////////////////////////////////
//      DETERMINE IF AT LEAST ONE VARIATION OF WORD CAN BE FORMED     
///////////////////////////////////////////////////////////////////////         
        // make sure there is a char in each column
        for(int idx = 0; idx < word.length(); ++idx)
        {
            if (variations[idx] == 0)
            {
                //cout << word << ": ***FAIL***" << endl << endl;
                fail = true;
                
                break;  // fail, break out of for loop
            }
            else if (variations[idx] == 2)
            {
                truthtable *= 2;   // total variations of word is factor of 2
                ++tablecount;      // keep track of 2's power  (2^tablecount)
                                   // i.e the times there are 2 variations of a char
            }
        }
        if (fail)
        {
            //cout << word << ": ***FAIL***" << endl << endl;
            
            continue;  // fail, go on to next word
        }
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//                      BUILD VARIATIONS TABLE
//////////////////////////////////////////////////////////////////////////
        // init map for keeping track of bit flips (uppers/lowers)
        for (int idx = 0; idx < tablecount; ++idx)
            captrack.push_back(false);
 
        // build all possible variations of formed word (truth table)
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
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////
//     ELIMINATE ERRONEOUS VARIATIONS / STORE RESULTS  
/////////////////////////////////////////////////////////////////////////
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
                        
                        break; //  variation failed, continue processing variations
                    }
                }
                else if (isupper(wordtable[idx][idx2]))
                    lower = 0;
                
                if (isupper(wordtable[idx][idx2]) && idx2 < word.length()-1 && isupper(wordtable[idx][idx2+1]))
                {   
                    elscheck = false;
                   
                    for (int idx3 = 0; idx3 < els1.size(); ++idx3)
                    {
                        if (els1[idx3][0] == wordtable[idx][idx2])
                        {
                            elscheck = true;
                            
                            break;   // we're okay, continue processing variations
                        }
                    }
                    
                    if (elscheck == false)
                        break;     // variation failed, continue processing variations
                }
            }
           
           // store successful variations
            if (elscheck && lowercheck) 
                results.push_back(wordtable[idx]);
        }
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//                       OUTPUT RESULTS
////////////////////////////////////////////////////////////////////////////////
        if (results.size() > 0)
        {
            totalvars += results.size();
            ++success;            
            cout << word << ": " << results.size() << " variation(s) found... \n";
         
            for (int idx = 0; idx < results.size(); ++idx)
                cout << "[" << results[idx] << "]\n";
        
            cout << endl;
        } 
        //else
            //cout << word << ": ***FAIL***" << endl << endl;
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                       GET AND PRINT PROGRAM STATS                  
//////////////////////////////////////////////////////////////////////////////////
        // get program stats
        checkstat = false;
        for (int idx = 0; idx < biggestable.size(); ++idx)
        {
            if (wordtable.size() == biggestable[idx])
               checkstat = true;
           
            if (wordtable.size() > biggestablesize)
            {
                biggestablesize = wordtable.size();
                tableword = word;
                aftervar = results.size();
            }    
        }  
        if (!checkstat)
            biggestable.push_back(wordtable.size());
        
        checkstat = false;
        for (int idx = 0; idx < stat.size(); ++idx)
        {
            if (results.size() == stat[idx])
               checkstat = true;
           
            if (results.size() > maxvar)
            {
                maxvar = results.size();
                maxword = word;
                varscheck = wordtable.size();
            }
        }
        if (!checkstat)
            stat.push_back(results.size());
    
 ////////////////////////////////////////////////////////////////////////////////////////////////////   
    } //  end main processing while loop
  
    // output program stats
    gettimeofday(&end, NULL);
    stopwatch = gettime_ms(start, end);
    cout << "Total words checked: " << totalwords << endl
         << "Total words formed: " << success << endl
         << "Total variations: " << totalvars << endl;
    printf("Runtime: %.3f ms\n", stopwatch);
    cout << endl << "Most variations after elimnination: " << maxword << " = " << maxvar << endl;
    cout << "Variations before elimination: " << varscheck << endl << endl;
    cout << "Most variations before elimination: " << tableword << " = " << biggestablesize << endl;
    cout << "Variations after elimination: " << aftervar << endl;
 ////////////////////////////////////////////////////////////////////////////////////////////////////
 ////////////////////////////////////////////////////////////////////////////////////////////////////
 
    cout << endl;
    return 0;
}
