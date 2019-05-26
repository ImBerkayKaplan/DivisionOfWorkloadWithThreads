# DivisionOfWorkloadWithThreads

The goal of the program is to demonstrate that partitioning less work to more threads allows the given task to be completed in a shorter time. The program multiplies 2 matrixes that will produce 1200 rows. The work is divided between threads equally. For instance, if there are 6 threads, thread 1 works on rows 0-199, thread 2 works on rows 200-399 etc. The program first performs the multiplication with 1 process and presents the time taken to finish the operation in seconds. Then, it runs the calculation with 2 threads and shows the total time. It does this until it reaches the first command line argument, which is ```<num-of-threads>```.

## Compilation

Use the make file by simply typing ```make``` in the command line window while in the project directory. This directory will contain the “src” folder. ```make``` will generate an executable named main in the project directory. 

## Execution

To run the program, type ```main <num-of-threads>``` in the command line when you are in the project directory. ```<num-of-threads>``` is a positive integer that represents the maximum number of threads that will work on multiplying the matrix.
