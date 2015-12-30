#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cmath>
#include <cctype>

using namespace std;

int main()
{
    ifstream el1("el1.txt");
    ifstream el2("el2.txt");
    ifstream el3("el3.txt");
    string word = "businesses";
    int trackexp = 0; 
    vector<bool> captrack;
    string conts = "";
    //string word = "alfhkoklnkbhjgtftesrwrytfgyiuijpoiouyttesdhvb";
    string fill = "";
    string temp, list1, list2, list3;
    vector<string> els1, els2, els3, wl, wl1, wl2, wl3;
    vector<int> variations;
    int truthtable = 1;
    int tablecount = 0;
    vector<string> wordtable;
    vector<string> results;
 
    for(int idx = 0; idx < word.length(); ++idx)
        variations.push_back(0);
    
    for(int idx = 0; idx < word.length(); ++idx)
        fill += "?";
    
    conts = fill;
    
    while (el1 >> temp)
    {
        els1.push_back(temp);
        //cout << els1[els1.size()-1] << endl;
    }
     
    while (el2 >> temp)
    {
        els2.push_back(temp);
        //cout << els2[els2.size()-1] << endl;
    }
        
    while (el3 >> temp)
    {
        els3.push_back(temp);
        //cout << els3[els3.size()-1] << endl;
    }
 
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
                wl.push_back(temp);
                wl1.push_back(temp);
                cout << wl1[wl1.size()-1] << endl;
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
                    
                
                wl.push_back(temp);
                wl2.push_back(temp);
                cout << wl2[wl2.size()-1] << endl;
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
                wl.push_back(temp);
                wl3.push_back(temp);
                cout << wl3[wl3.size()-1] << endl;
                
            }
        } 
    }
*/  
    
    for(int idx = 0; idx < word.length(); ++idx)
    {
        cout << variations[idx] << " ";
        
        if (variations[idx] == 0)
        {
            cout << "FAIL: No possible word formations" << endl;
            return 0;
        }
        else if (variations[idx] == 2)
        {
            truthtable *= 2;
            ++tablecount;
        }
    }
    cout << "\nChecking for binary information: \n"
         << "Total binary values: " << tablecount << endl
         << "Total possible permutations: 2^" << tablecount << " = " << truthtable << endl;
 
    for (int idx = 0; idx < tablecount; ++idx)
       captrack.push_back(false);
   
    for (int idx = 0; idx < truthtable; ++idx)
    {
        trackexp = 0;
        wordtable.push_back(fill);
        for (int idx2 = word.length()-1; idx2 >= 0; --idx2)
        {
            // need to grab variations.  can't throw away list.  use albino to see
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
            {
                results.push_back(wordtable[idx]);
                wordtable[idx] += ": SUCCESS";
            }
            else
            {
                wordtable[idx] += ": FAIL";
            }
            
           cout << wordtable[idx] << endl; 
    }
    
    cout << "Results calculated: \n"
         << "Possible - actual = " << truthtable - results.size() << " possibilities removed\n"
         << "Variations found = " << results.size() << endl;
         
    for (int idx = 0; idx < results.size(); ++idx)
        cout << results[idx] << endl;
    
    cout << endl;
    return 0;
}
