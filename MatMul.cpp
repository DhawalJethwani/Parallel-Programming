#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <time.h>

#define n 600
#define notrd 6

using namespace std;

int matA[n][n];
int matB[n][n];
int matC[n][n];

void makemat(string mat)
{
    ofstream fmat;
    fmat.open(mat.c_str());
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<n;j++)
        {
            fmat << rand()%10+1 << " ";
        }
        fmat << endl;
    }
    fmat.close();
}

void creatematfile(string a,string b)
{
    ifstream file1(a.c_str());
    if(!file1)
    {
        makemat(a);
    }
    ifstream file2(b.c_str());
    if(!file2)
    {
        makemat(b);
    }
}

void f2m(string a,string b)
{
    ifstream file1(a.c_str());
    string line1;
    int i=0;
    while(getline(file1,line1))
    {
        char *s1=new char [line1.length()+1];
        strcpy(s1,line1.c_str());
        char *pch1;
        int j=0;
        pch1 = strtok (s1," ");
        while (pch1!=NULL)
        {
            matA[i][j]=atoi(pch1);
            pch1 = strtok(NULL," ");
            j++;
        }
        i++;
    }
    file1.close();
    ifstream file2(b.c_str());
    string line2;
    i=0;
    while(getline(file2,line2))
    {
        char *s2=new char[line2.length()+1];
        strcpy(s2,line2.c_str());
        char *pch2;
        int j=0;
        pch2 = strtok (s2," ");
        while (pch2 != NULL)
        {
            matB[i][j]=atoi(pch2);
            pch2 = strtok (NULL, " ");
            j++;
        }
        i++;
    }
    file2.close();
}

void* matrix_multi(void *k)
{
    int tnum = (int)k;
    int x=(int)n/(notrd-1);
    if(tnum!=notrd-1)
    {
        for(int i = tnum*x; i < (tnum+1)*x; i++)
        {
            for(int j = 0; j < n; j++)
            {
                matC[i][j] = 0;
                for(int k = 0; k < n; k++)
                {
                    matC[i][j] += matA[i][k] * matB[k][j];
                }
            }
        }
    }
    else
    {
        for(int i = tnum*x; i < n; i++)
        {
            for(int j = 0; j < n; j++)
            {
                matC[i][j] = 0;
                for(int k = 0; k < n; k++)
                {
                    matC[i][j] += matA[i][k] * matB[k][j];
                }
            }
        }
    }
    pthread_exit(0);
}

void* mat_mul()
{
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < n; j++)
        {
            matC[i][j] = 0;
            for(int k = 0; k < n; k++)
            {
                matC[i][j] += matA[i][k] * matB[k][j];
            }
        }
    }
}

int main()
{
    pthread_t thread[notrd];
    creatematfile("matA.txt","matB.txt");
    f2m("matA.txt","matB.txt");
    clock_t t;
    t = clock();
    for(int i = 0; i < notrd; i++)
    {
        pthread_create(&thread[i],NULL,matrix_multi,(void *)i);
    }
    for(int i = 0; i < notrd; i++)
    {
        pthread_join(thread[i],NULL);
    }
    t = clock() - t;
    double time_taken1 = ((double)t)/CLOCKS_PER_SEC;
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<n;j++)
        {
            cout << matC[i][j] << " ";
        }
        cout << endl;
    }

    cout << "Time taken to execute matrix multiplication using threads " << time_taken1 << endl;
    t=clock();
    mat_mul();
    t=clock()-t;
    double time_taken2 = ((double)t)/CLOCKS_PER_SEC;
    cout << "Time taken to execute matrix multiplication without using threads " << time_taken2 << endl;
    return 0;
}
