Form words with element abbreviations from the periodic table of elements.
Find ALL POSSIBLE VARIATIONS of each word!

As far as I can tell, this algorithm will create the most exhaustive list
of formed words.  If you can find a list that comes up with more variations of words, 
I would really like to see!!
==============================================================================================
- GENERAL OVERVIEW OF ALGORITHM -

1.  Match symbols to chars in word.
     a. Build a string with equal number of chars to word and fill it in with non alpha char. 
     ( WORD = business -> fill_WORD = ????????)
     b. Place each matching symbol in its own fill-word string
              i.  Ex: WORD = cures -> C???? Cu??? ??Re? ????S ?U???
2.  Ensure each char column is filled
     a. If a column yields 0 it is automatic fail
3.  Check to see if there is capitalized variation in char column
     a. Identify char columns with no variation
     b. Identify char columns with variation
4.  Build bit-wise truth table for word variations
     a. possible variations = 2^n where n is the number of char column variations
5.  Eliminate variation fails
     a.  Two lower case chars in a row = FAIL
     b.  Two upper case chars in a row where first char is not element symbol = FAIL
          ex. LOsS would be an illegal variation because there is no L symbol.
6. Ensure that all variations were not eliminated
    a. If all variations fail then the word can not be formed
    b. Otherwise...store results!!
(**Note**) A much more complicated algorithm is employed when a word
          contains a matching 3-char-symbol.  This happens extremely
          rarely, as there are no English words that contain them.
          However, for completeness' sake, I am in the process of
          writing it.  As of now, words with matching 3-char-symbols
          are identified and matched, but only one variation with
          the 3-char-symbol is identified. If 1 and 2 char symbol
          variations can be formed (for example: uus can be matched
          with U and S so it will always pass a 1 and 2 char symbol
          check), then the normal routine will execute.

================================================================================          
- RANDOM EXAMPLE -

WORD = albinos
Al????? 
?B????? 
?Bi???? 
??I???? 
??In??? 
???N??? 
???No?? 
????O?? 
????Os? 
??????S
- all char columns are filled so we're good so far

Extract data:
- index (letter positions) =    0 1 2 3 4 5 6 7 
- map char variation to index = 1 1 1 2 2 2 2 2
- A, l, B are constants
- (I,i) (N,n) (O,o) (S,s) are variants
- Possible variations = 1x1x1x2x2x2x2 = 2^4 = 16 possible variations

Build "truth table" - constants are ignored:
A l B  
_ _ _ i n o s -->fail: 2 lower          _ _ _ 0 0 0 0(s)
_ _ _ i n o S -->fail: 2 lower          _ _ _ 0 0 0 1(S)
_ _ _ i n O s -->fail: 2 lower          _ _ _ 0 0 1 0(s)
_ _ _ i n O S -->fail: 2 lower          _ _ _ 0 0 1 1(S)
_ _ _ i N o s -->fail: 2 lower          _ _ _ 0 1 0 0(s)
_ _ _ i N o S -->SUCCESS                _ _ _ 0 1 0 1(S)
_ _ _ i N O s -->SUCCESS                etc...
_ _ _ i N O S -->SUCSESS                etc...
_ _ _ I n o s -->fail: 2 lower        
_ _ _ I n o S -->fail: 2 lower
_ _ _ I n O s -->SUCCESS
_ _ _ I n O S -->SUCCESS
_ _ _ I N o s -->fail: 2 lower
_ _ _ I N o S -->SUCCESS
_ _ _ I N O s -->SUCCESS
_ _ _ I N O S -->SUCCESS

8 variations found:
[AlBINOS]
[AlBINOs]
[AlBINoS]
[AlBInOS]
[AlBInOs]
[AlBiNOS]
[AlBiNOs]
[AlBiNoS]
