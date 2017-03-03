#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <time.h>
#include <string>
#include <algorithm>
using namespace std;

const long DataNum = 2000000000;
const long SubDataNum = 10000000;
// const long DataNum = 10000000;
// const long SubDataNum = 500000;

// show excute time
time_t rawtime;
struct tm * timeinfo;

double arr[10000005];  //sort 1000W numbers each time
int K;  // divide to K sub file
char Filename[10] = {"sub"};

char* GetFileName(int num) {
   sprintf(Filename + 3, "%d", num);
   return Filename;
}

// create 16G data (2*10^9 double)
void CreateData() {
   time(&rawtime);
   timeinfo = localtime(&rawtime);
   cout << "Start creating 16G data:  " << asctime(timeinfo);

   srand((unsigned int)time(NULL));
   ofstream out("RandomData", ios::binary | ios::trunc);
   double randomNum = 0;
   for (long i = 0; i < DataNum; ++i) {
        randomNum = rand() * 1.0 / RAND_MAX;
        out.write((char*)(&randomNum), 8);
   }
   out.close();

   time(&rawtime);
   timeinfo = localtime(&rawtime);
   cout << "Finish creating 16G data: " << asctime(timeinfo);
}

// each sub file has SubDataNum numbers
int DivideToKFile() {
   ifstream data("RandomData", ios::binary);
   int i, Kfile;

   for (Kfile = 0; data.read((char*)(&arr[0]), 8);) {

      ++Kfile;
      for (i = 1; i < SubDataNum && data.read((char*)(&arr[i]), 8); ++i) {}
      sort(arr, arr + i);

      ofstream subFile(GetFileName(Kfile), ios::binary | ios::trunc);
      for (int j = 0; j < i; ++j) {
         subFile.write((char*)(&arr[j]), 8);
      }
      subFile.close();

      if (i != SubDataNum)
         break;
   }
   data.close();
   return Kfile;
}

void MergeTwoFile(ifstream & Sub1, ifstream & Sub2) {
   // get the length of two sub file
   Sub1.seekg(0, ios::end);
   Sub2.seekg(0, ios::end);
   long len1 = Sub1.tellg() >> 3, len2 = Sub2.tellg() >> 3;
   Sub1.seekg(0, ios::beg);
   Sub2.seekg(0, ios::beg);

   ++K;
   ofstream Merge(GetFileName(K), ios::binary | ios::trunc);
   double a, b;
   Sub1.read((char*)(&a), 8);
   Sub2.read((char*)(&b), 8);
   long i = 0, j = 0;

   for (; i < len1 && j < len2;) {
      if (a <= b) {
         Merge.write((char*)(&a), 8);
         ++i;
         if (i < len1) {
            Sub1.read((char*)(&a), 8);
         }
      }
      else {
         Merge.write((char*)(&b), 8);
         ++j;
         if (j < len2) {
            Sub2.read((char*)(&b), 8);
         }
      }
   }

   while (i < len1) {
      Merge.write((char*)(&a), 8);
      ++i;
      if (i < len1) {
         Sub1.read((char*)(&a), 8);
      }
   }

   while (j < len2) {
      Merge.write((char*)(&b), 8);
      ++j;
      if (j < len2) {
         Sub2.read((char*)(&b), 8);
      }
   }

   Merge.close();
}

void mergeSort() {
   time(&rawtime);
   timeinfo = localtime(&rawtime);
   cout << "\nStart mergesorting:  " << asctime(timeinfo);

   K = DivideToKFile();

   // merge two file each time until only one file
   for (int i = 1; i < K; i += 2) {
      ifstream Sub1(GetFileName(i), ios::binary);
      ifstream Sub2(GetFileName(i + 1), ios::binary);
      if (Sub1.is_open() && Sub2.is_open()) {
         MergeTwoFile(Sub1, Sub2);
         Sub1.close();
         Sub2.close();
         remove(GetFileName(i));
         remove(GetFileName(i + 1));
      }
      else {
         Sub1.close();
      }
   }
   rename(GetFileName(K), "SortedData");

   time(&rawtime);
   timeinfo = localtime(&rawtime);
   cout << "Finish mergesorting: " << asctime(timeinfo);
}

void isSorted(const char* filename) {
   time(&rawtime);
   timeinfo = localtime(&rawtime);
   cout << "\nStart checking:  " << asctime(timeinfo);

   ifstream CheckFile(filename, ios::binary);
   if (CheckFile.is_open() == false) {
      cout << "Fail to open " << filename << "\n";
      return;
   }

   double a, b;
   long i = 1;
   CheckFile.read((char*)&a, 8);
   while (CheckFile.read((char*)&b, 8)) {
      if (a > b) {
         cout << i << "th number is wrong\n";
         return;
      }
      else {
         a = b;
         ++i;
      }
   }

   CheckFile.close();
   time(&rawtime);
   timeinfo = localtime(&rawtime);
   cout << "Finish checking: " << asctime(timeinfo);
}

int main() {
   CreateData();
   mergeSort();
   isSorted("SortedData");
   return 0;

   // test 1000W
   /*
   ifstream data("RandomData", ios::binary);
   int i = 0;
   while (data.read((char*)(&arr[i]), 8)) {++i;}
   data.close();
   sort(arr, arr + i);

   ofstream Sort("Sort", ios::binary | ios::trunc);
   for (int j = 0; j < i; ++j) {
   Sort.write((char*)(&arr[j]), 8);
   }
   Sort.close();
   isSorted("Sort");
   */
}
