# Matrix-Multiplication-Using-MPI

The aim of this project was to use to parallelism using MPI to multiply 3x3 and 4x4 Matrix.

### Executing the cpp file

- Open the Terminal
- Compile the .cpp file
    - Method 1 
        - g++ -o matrix_multiplication matrix_multiplication.cpp
    - Method 2 (Better Approach)
        - mpicxx -o matrix_multiplication matrix_multiplication.cpp
- Execution
    - mpirun -np number_of_cores ./matrix_multiplication
