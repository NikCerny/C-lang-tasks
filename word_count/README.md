## Problem Description:

### Most common words

List a given number (user-selectable) of the most common words found in the specified text document.

The user provides all of his inputs exclusively as program parameters at run time. It is not possible to make any assumptions about the length of the input files, therefore it is necessary to use dynamic data structures (dynamically allocated array, linked list, tree)

Test the created programs on large data sets, for example, for text tasks you can use the English text of the Bible (size approx. 4 MB)


## Solution:

### Usage CLI:

args: program_name input_name.txt [word_count_output]

example:

`./a.out tests/in_2.txt 5` -> 5 most common words

`./a.out tests/in_2.txt` -> 10 most common words(default)