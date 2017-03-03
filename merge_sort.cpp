#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <time.h>
#include <string>
#include <algorithm>
using namespace std;

// show excute time
time_t rawtime;
struct tm * timeinfo;

// merge two file
void merge(fstream & from, long len1, long len2, fstream & into) {
   long i = 1, j = 1, pos = from.tellg();
   double a, b;
   from.read((char*)&a, 8);
   from.seekg(((len1 - 1) << 3), ios::cur);
   from.read((char*)&b, 8);
   for (; i <= len1 || j <= len2; ) {
      if (i <= len1 && j <= len2) {
         if (a < b) {
           into.write((char*)&a, 8);
            if (i < len1) {
               from.seekg(pos + (i << 3) - from.tellg(), ios::cur);
               from.read((char*)&a, 8);
               ++i;
            }
            else {
               from.seekg(pos + ((len1 + j) << 3) - from.tellg(), ios::cur);
               ++i;
            }
        }
        else {
           into.write((char*)&b, 8);
            if (j < len2) {
               from.seekg(pos + ((len1 + j) << 3) - from.tellg(), ios::cur);
               from.read((char*)&b, 8);
               ++j;
            }
            else {
               from.seekg(pos + (i << 3) - from.tellg(), ios::cur);
               ++j;
            }
        }
      }
      else if (i <= len1) {
         into.write((char*)&a, 8);
         ++i;
         if (i <= len1) {
            from.read((char*)&a, 8);
         }
      }
      else if (j <= len2){
         into.write((char*)&b, 8);
         ++j;
         if (j <= len2) {
            from.read((char*)&b, 8);
         }
      }
   }
}

void mergeSort(fstream& in, fstream& temp, long len, bool resultInData) {
   if (len == 1) {
      if (! resultInData) {
         double num;
         in.read((char*)&num, 8);
         temp.write((char*)&num, 8);
      }
      return;
   }
   long len1 = len / 2, len2 = len - len1,
      inpos = in.tellg(), tempos = temp.tellg();

   mergeSort(in, temp, len1, !resultInData);
   in.seekg(inpos + (len1 << 3) - in.tellg(), ios::cur);
   temp.seekg(tempos + (len1 << 3) - temp.tellg(), ios::cur);

   mergeSort(in, temp, len2, !resultInData);
   in.seekg(inpos - in.tellg(), ios::cur);
   temp.seekg(tempos - temp.tellg(), ios::cur);

   if (resultInData) {
      merge(temp, len1, len2, in);
   }
   else {
      merge(in, len1, len2, temp);
   }

   in.seekg(inpos - in.tellg(), ios::cur);
   temp.seekg(tempos - temp.tellg(), ios::cur);
}

void mergeSort() {
   time(&rawtime);
   timeinfo = localtime(&rawtime);
   cout << "\nStart mergesorting:  " << asctime(timeinfo);

   fstream in("data.bin", ios::in | ios::out | ios::binary);
   fstream temp("temp.bin", ios::in | ios::out |ios::binary | ios::trunc);
   mergeSort(in, temp, 2000000000L, true);
   in.close();
   temp.close();

   time(&rawtime);
   timeinfo = localtime(&rawtime);
   cout << "Finish mergesorting: " << asctime(timeinfo);
}

// create 16G data (2*10^9 double)
void create_data() {
   time(&rawtime);
   timeinfo = localtime(&rawtime);
   cout << "Start creating 16G data:  " << asctime(timeinfo);

   srand((unsigned int)time(NULL));
   ofstream out("data.bin", ios::binary | ios::trunc);

   double randomNum;
   for (long i = 0; i < 2000000000L; ++i) {
      randomNum = rand();
      out.write((char*)(&randomNum), 8);
   }
   out.close();

   time(&rawtime);
   timeinfo = localtime(&rawtime);
   cout << "Finish creating 16G data: " << asctime(timeinfo);
}

void isSorted() {
   time(&rawtime);
   timeinfo = localtime(&rawtime);
   cout << "\nStart checking:  " << asctime(timeinfo);

   ifstream check("data.bin", ios::binary);
   double a, b;
   check.read((char*)&a, 8);
   for (int i = 0; i < 1999999999; ++i) {
      check.read((char*)&b, 8);
      if (a > b) {
         cout << i << "th number is wrong\n";
         return;
      }
      else
         a = b;
   }

   time(&rawtime);
   timeinfo = localtime(&rawtime);
   cout << "Finish checking: " << asctime(timeinfo);
}

int main() {
   create_data();
   mergeSort();
   isSorted();
}
