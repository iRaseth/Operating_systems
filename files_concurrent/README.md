## About

Two processes work concurrently (child and parent). Child is writting 100 randomly generated numbers to the file, 
parent is reading them and printing only divisible by 3. Child is using original file descriptor, parent is using copy. 
