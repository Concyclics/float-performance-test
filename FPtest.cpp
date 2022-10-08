/*
 * Copyright (c) SCUT. 2022. All rights reserved.
 * Float Performance Benchmark
 * 
 * This program is a benchmark for floating point performance. It is
 * designed to be run on both single and double precision systems.
 * In multi-core systems, it is designed to be run on omp.
 * 
 * To compile: -O3 -fopenmp
 * Since in O3 and higher mode, the compiler will auto vectorize the code.
 * 
 * For macOS with x86, use: g++ -Ofast -Xpreprocessor -fopenmp -I/usr/local/include -L/usr/local/lib -lomp -mavx2 FPtest.cpp -o FPtest
 * For macOS with ARM, use: g++ -Ofast -Xpreprocessor -fopenmp -I/opt/local/include -L/opt/local/lib -lomp FPtest.cpp -o FPtest
 * 
 * For Linux with x86, use: g++ -Ofast -fopenmp -mavx2 FPtest.cpp -o FPtest
 * For Linux with ARM, use: g++ -Ofast -fopenmp FPtest.cpp -o FPtest
 * 
 * For Windows with x86, use: g++ -Ofast -fopenmp -mavx2 FPtest.cpp -o FPtest.exe
 * For Windows with ARM, use: g++ -Ofast -fopenmp FPtest.cpp -o FPtest.exe
 * 
 * Author:  CHEN Han
 * Date:    2022-10-06
 * Version: 1.2
 * update:  2022-10-08
 */

//#define MACOSX

#include <iostream>
#include <chrono>
#include <time.h>
#ifdef MACOSX
#include <malloc/malloc.h>
#else
#include <malloc.h>
#endif
#define uint64_t unsigned long long
#define uint32_t unsigned int
#define int64_t long long
#define int32_t int

using namespace std;

template <typename T>
double flopsTest(const int64_t TestSize, const int threads)
{
    
    time_t begin, end;
    double time_spent;
    const int each = 512/sizeof(T);
    //count in milliseconds
    begin = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
    #pragma omp parallel for num_threads(threads)
    for (int thread_no = 0; thread_no < threads; thread_no++)
    {
        T a[each], b[each];
        for (int64_t i = 0; i < TestSize/threads; i+=each)
        {
            #pragma omp simd
            for (int j = 0; j < each; j++)
            {
                a[j] *= b[j];
            }
        }
    }
    end = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
    time_spent = (double)(end - begin);
    return time_spent;
}

int main(int argc, char *argv[])
{
    int64_t TestSize = 1<<30;
    int threads = 1;
    int times = 10;
    if(argc <= 1)
    {
        puts("Usage: ./flops [TestSize] [threads] [times]");
    }
    if (argc > 1)
    {
        TestSize = atoll(argv[1]);
    }
    if (argc > 2)
    {
        threads = atoi(argv[2]);
    }
    if (argc > 3)
    {
        times = atoi(argv[3]);
    }


    double FP32_1core = 0.0,
           FP64_1core = 0.0,
           FP32_omp = 0.0,
           FP64_omp = 0.0;

    for (int i = 0; i < times; i++)
    {
        FP32_1core += flopsTest<float>(TestSize, 1);
        FP64_1core += flopsTest<double>(TestSize, 1);
        FP32_omp += flopsTest<float>(TestSize, threads);
        FP64_omp += flopsTest<double>(TestSize, threads);
    }

    FP32_1core /= times;
    FP64_1core /= times;
    FP32_omp /= times;
    FP64_omp /= times;

    cout << "----------------------FP32------------------------" << endl;
    cout << "FP32 single: " << FP32_1core << "ms" << endl;
    cout << "FP32 "<<threads<<" threads: " << FP32_omp << "ms" << endl;
    cout << "FP32 single performance: " << TestSize / FP32_1core / 1e6 << "GFLOPS" << endl;
    cout << "FP32 "<<threads<<" threads performance: " << TestSize / FP32_omp / 1e6 << "GFLOPS" << endl;
    cout << "Multi-core speedup of FP32: " << FP32_1core / FP32_omp << endl;
    cout << "Multi-core efficiency of FP32: " << FP32_1core / FP32_omp / threads * 100 << "%" << endl;
    double ParallelRate = (threads/(FP32_1core / FP32_omp)-threads)/(1-threads);
    cout << "Parallel rate of FP32: " << ParallelRate << endl;
    cout << "--------------------------------------------------" << endl << endl;

    cout << "----------------------FP64------------------------" << endl;
    cout << "FP64 single: " << FP64_1core << "ms" << endl;
    cout << "FP64 "<<threads<<" threads: " << FP64_omp << "ms" << endl;
    cout << "FP64 single performance: " << TestSize / FP64_1core / 1e6 << "GFLOPS" << endl;
    cout << "FP64 "<<threads<<" threads performance: " << TestSize / FP64_omp / 1e6 << "GFLOPS" << endl;
    cout << "Multi-core speedup of FP64: " << FP64_1core / FP64_omp << endl;
    cout << "Multi-core efficiency of FP64: " << FP64_1core / FP64_omp / threads * 100 << "%" << endl;
    ParallelRate = (threads/(FP64_1core / FP64_omp)-threads)/(1-threads);
    cout << "Parallel rate of FP64: " << ParallelRate << endl;
    cout << "--------------------------------------------------" << endl;

    return 0;
}