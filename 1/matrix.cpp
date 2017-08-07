#include<iostream>
#include<windows.h>
#include<conio.h>

using namespace std;

class Matrix
{
public:
    int rows, cols;
    double** mat;

Matrix(int s1 =1, int s2=1)
    : rows(s1), cols(s2)
    {
    mat = new double*[rows];
     for(int i=0;i<rows;i++)
      mat[i] = new double[cols];

    cout<<"Enter the numbers into matrix:  ";

    for(int i=0;i<rows;i++)
     for(int j=0;j<cols;j++)
      cin>>mat[i][j];
    system("cls");
    }

void Print()
{
for(int i=0;i<rows;i++)
 {cout<<"|";
  for(int j=0;j<cols;j++)
    {cout<<mat[i][j]; if(j+1!=cols) cout<<" ";}
  cout<<"|"<<endl;
 }
}

~Matrix()
    {
        for(int i=0;i<rows;i++)
            delete[] mat[i];
        delete[] mat;
    }
};

void AddSub( Matrix O1, Matrix O2, int sign, int s1, int s2)
{

/*int** mat = new int*[s1];
     for(int i=0;i<s1;i++)
      mat[i] = new int[s2];
*/

if(sign==3)
{
for(int i=0;i<s1;i++)
 for(int j=0;j<s2;j++)
    O1.mat[i][j] =  O1.mat[i][j] + O2.mat[i][j];   //mat[i][j] =  O1.mat[i][j] + O2.mat[i][j];
system("cls");
}
if(sign==4)
{
for(int i=0;i<s1;i++)
 for(int j=0;j<s2;j++)
  O1.mat[i][j] = O1.mat[i][j] - O2.mat[i][j];    //mat[i][j] = O1.mat[i][j] - O2.mat[i][j];
system("cls");
}

for(int i=0;i<s1;i++)
 {cout<<"|";
  for(int j=0;j<s2;j++)
    {cout<<O1.mat[i][j]; if(j+1!=s2) cout<<" ";}     //cout<<mat[i][j]; if(j+1!=s2) cout<<" ";
  cout<<"|"<<endl;
 }

/*for(int i=0;i<s1;i++)
    delete[] mat[i];
     delete[] mat;*/
}

void Mul(Matrix O1, Matrix O2, int r1, int c1, int r2, int c2)
{
int s1 = (r1<r2) ? r1 : r2, s2 = (c1<c2) ? c1 : c2;

double** mat = new double*[s1];
      for(int i=0;i<s1;i++)
       mat[i] = new double[s2];

for(int i=0;i<s1;i++)
 for(int j=0;j<s2;j++)
  mat[i][j] =0;

for(int i=0;i<s1;i++)
 for(int j=0;j<c2;j++)
  for(int k=0;k<c1;k++)
    mat[i][j] += O1.mat[i][k]*O2.mat[k][j];

system("cls");

for(int i=0;i<s1;i++)
 {cout<<"|";
  for(int j=0;j<s2;j++)
    {cout<<mat[i][j]; if(j+1!=s2) cout<<" ";}
  cout<<"|"<<endl;
 }

for(int i=0;i<s1;i++)
    delete[] mat[i];
     delete[] mat;
}

int main()
{
int rows, rows2, cols, cols2, iSign;
char cSign;

do
{
    cout<<"Enter size of your matrices. Do you want them to have same size? Enter Y/N"<<endl<<endl;
    cout<<"Note that if you will enter different sizes for each matrix,"<<endl<<"the addition and subtraction operations will not be able to be executed."<<endl;
    cin>> cSign;
    system("cls");

    switch(cSign)
   {
    case 'Y':
        cout<<"Length: "; cin>>cols;
        cout<<"Width:  "; cin>>rows;
        rows2=rows; cols2=cols;
        break;
    case 'N':
        cout<<"First matrix: "<<endl;
        cout<<"  Length: "; cin>>cols;
        cout<<"  Width:  "; cin>>rows;
        cout<<"Second matrix: "<<endl;
        cout<<"  Length: "; cin>>cols2;
        cout<<"  Width:  "; cin>>rows2;
        break;
    }
    system("cls");
    if(rows<=0 || cols<=0 || rows2<=0 || cols2<=0)cout<<"Size of matrix can't be negative or zero! Enter again."<<endl;
    if(rows!=cols2 && rows2!=cols){cout<<"Column of first matrix not equal to row of second or"<<endl;
                                   cout<<"row of first matrix not equal to column of second"<<endl;}

}
while(!(rows>0 && cols>0 && rows2>0 && cols2>0) || !(rows==cols2 && rows2==cols) );

Matrix matrix1(rows, cols), matrix2(rows2, cols2);

system("cls");

cout<<"What do you want to do with matrices?"<<endl<<"1 - printing,  2 - multiplication"; if(cSign=='Y') cout<<",  3 - addition,  4 - subtraction "<<endl;
cin>>iSign;

switch(iSign)
{case 1:
    matrix1.Print(); cout<<endl; matrix2.Print();
    break;
 case 2:
    Mul(matrix1, matrix2, rows, cols, rows2, cols2);
    break;
 case 3:
    if(cSign=='N') cout<<"Addition can't be executed!"<<endl;
    AddSub(matrix1, matrix2, iSign, rows, cols);
    break;
 case 4:
    if(cSign=='N') cout<<"Subtraction can't be executed!"<<endl;
    AddSub(matrix1, matrix2, iSign, rows, cols);
    break;
}

getch();
return 0;
}
