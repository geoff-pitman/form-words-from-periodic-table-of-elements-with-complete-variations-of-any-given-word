Form words with element abbreviations from the periodic table of elements. 
Find ALL POSSIBLE VARIATIONS of each word!

This algorithm will create the absolute most exhaustive list of formed 
word variations.  If anyone can find or knows of an alternative algorithm
for finding every variation of a given word, I'm very interested to see how
it is implemented...especially if it is faster!! Currently runs in nlogn if 
the pass/fail check is implemented in the same loop that the variations are
formed.  Since words are of a finite length, n <= longest word in dictionary.
Even a million word dictionary will complete in under 10 seconds on an
everyday dual core machine (in C++ and Java anyway).  The English dictionary
is around 100,000.

Inspiration derived from Jeff Thompson
[http://www.jeffreythompson.org/blog/2013/12/04/every-word-in-the-periodic-table/]

===============================================================================================

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
(**Note**) A much more complicated algorithm is executed when a word
          contains a matching 3-char-symbol.  This happens extremely
          rarely, as there are no English words that contain one AND
          can also be formed.  However, for completeness' sake, I
          decided to write it in anyway.
================================================================================          
- RANDOM EXAMPLE -

WORD = albinos
Al????? 
??B???? 
??Bi??? 
???I??? 
???In?? 
????N?? 
????No? 
?????O? 
?????Os 
??????S
- all char columns are filled so we're good so far

Map char variations to letter position:
- index (letter positions) =    0 1 2 3 4 5 6 
- map char variation to index = 1 1 1 2 2 2 2
- Possible variations = 1x1x1x2x2x2x2 = 2^4 = 16
- (a, l, b) are constants -> (A, l, B)
- (i, n, o, s) are variants -> [(I,i) (N,n) (O,o) (S,s)]

Build word variation "truth table": |   (analagous to building a truth table for n-bit input)
A l B                               |    (x = constant, 0 = lower, 1 = upper):  
      i n o s -->fail: 2 lower      |     x x x 0(i) 0(n) 0(o) 0(s)
      i n o S -->fail: 2 lower      |     x x x 0(i) 0(n) 0(o) 1(S)
      i n O s -->fail: 2 lower      |     x x x 0(i) 0(n) 1(O) 0(s)
      i n O S -->fail: 2 lower      |     x x x 0(i) 0(n) 1(O) 1(S)
      i N o s -->fail: 2 lower      |     x x x 0(i) 1(N) 0(o) 0(s)
      i N o S -->SUCCESS            |     x x x 0(i) 1(N) 0(o) 1(S)
      i N O s -->SUCCESS            |     etc...
      i N O S -->SUCSESS            |     etc...
      I n o s -->fail: 2 lower        
      I n o S -->fail: 2 lower
      I n O s -->SUCCESS
      I n O S -->SUCCESS
      I N o s -->fail: 2 lower
      I N o S -->SUCCESS
      I N O s -->SUCCESS
      I N O S -->SUCCESS

8 variations found:
[AlBINOS]
[AlBINOs]
[AlBINoS]
[AlBInOS]
[AlBInOs]
[AlBiNOS]
[AlBiNOs]
[AlBiNoS]
