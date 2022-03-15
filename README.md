# CountUnique
Homework
Implemented under Linux but can be run under Windows OS as well.

A console program that counts the number of distinct unique words in a file whose name is passed as an argument to a program.

Consider following information:
- For example, given the file content "a horse and a dog" the program must output "4" (the word 'a' appears twice but only accounts for one distinct unique occurrence).
- The input text is guaranteed to contain only 'a'..'z' and space characters in ASCII encoding.
- The program should be able to handle large inputs (e.g. 32 GiB)
- We can assume that all unique words fit into memory when using the data structure of your choice.
- The solution must utilize all available CPU resources.

Prepare/Setup test text-file (Linux):
Generate a text file that contains the combination of letter a-z and a space
tr -dc 'a-z ' </dev/urandom | head -c <size in Bytes> > <Path to text file>
For example, to generate a four GB file:
  tr -dc 'a-z ' </dev/urandom | head -c 4294967296 > projects/test.txt
  
Compile the source
  g++ -O3 -std=c++14 -lpthread CountUnique.cpp -o CountUnique
  
Run the program:
  ./CountUnique projects/test.txt
