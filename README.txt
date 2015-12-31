- GENERAL OVERVIEW OF ALGORITHM -
  
1.  Match symbols to chars in word.
     a. Build a string with equal number of chars to word and fill it in with non alpha char. 
     ( word = element -> fill-word = ???????)
     b. Place each matching symbol in its own fill-word string
              i.  Ex: word = cures -> C???? Cu??? ??Re? ????S ?U???
2.  Ensure each char column is filled
     a. If a column yields 0 it is automatic fail
3.  Check to see if there is capitalized variation in char column
     a. Identify char columns with no variation
     b. Identify char columns with variation
4.  Build bit-wise truth table for word variations
     a. possible variations = 2^n where n is the number of char column variations
5.  Eliminate fail cases
     a.  Two lower case chars in a row
     b.  Two upper case chars in a row where first char is not element symbol
          >let's assume Ti and In are only possible symbols
          >word = tin -> Ti In -> Tin=fail , TIn=fail  
          >even though matches are found and length is filled, it still fails
6. Ensure that all cases were not eliminated
    a. If all cases fail it is a fail
    b. Otherwise...store results!!
(**Note**) A much more complicated algorithm is employed when a word
          contains a matching 3-char-symbol.  This happens extremely
          rarely, as there are no English words that contain them.
          However, for completeness' sake, I am in the process of
          writing it.  As of now, words with matching 3-char-symbols
          are identified and matched, but only one variation with
          the 3-char-symbol is identified, if the word can not
          be formed with 1 and 2 char symbols.

- EXAMPLE -
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
-A is always upper, l is always lower, B is always upper
-3 char constants, 4 chars that vary.  only 2 possible variations, upper or lower,
-so we can create truth table!!!
-2x2x2x2 = 2^4 = 16 possible variations
-a, l, b are constants

A l B  
_ _ _ i n o s -->fail: 2 lower 
      i n o S -->fail: 2 lower 
      i n O s -->fail: 2 lower
      i n O S -->fail: 2 lower
      i N o s -->fail: 2 lower
      i N o S -->SUCCESS
      i N O s -->SUCCESS
      i N O S -->SUCSESS
      I n o s -->fail: 2 lower
      I n o S -->fail: 2 lower
      I n O s -->SUCCESS
      I n O S -->SUCCESS
      I N o s -->fail: 2 lower
      I N o S -->SUCCESS
      I N O s -->SUCCESS
      I N O S -->SUCCESS
      
Simple two step check to eliminate illegal variations
1. Two lower cases in a row
2. If two upper cases, check to make sure previous upper has a matching 1 char symbol
   ex. LOsS would be an illegal variation because there is no L symbol.

8 variations found:
[AlBINOS]
[AlBINOs]
[AlBINoS]
[AlBInOS]
[AlBInOs]
[AlBiNOS]
[AlBiNOs]
[AlBiNoS]
