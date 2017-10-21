#include<windows.h>
#include<iostream>
#include<string>
#include<conio.h>

using namespace std;

int Move()
{
    int j=0;
    string str1, str2,str_name;

    system("cls");
    cout<<"Enter source of the file"; cin>>str1;
    cout<<"Enter destination path";   cin>>str2;
    system("cls");

try
{
    if(!MoveFileA(str1.c_str(),str2.c_str())) throw 1;
    cout<<str1<<" file moved successfully."<<endl;
}
    catch(...)
    {
      cerr<<"Could not move "<<str1<<" file."<<endl;
    }

return GetLastError();
}

void ErrorList(int error)
{
    switch(error)
    {
    case 0:
    break;
    case 2:
    cout<<"File not found. Try again."<<endl;
    break;
    }
    getch();
}

 int main ()
{
int e;
do
{
    e = Move();
    ErrorList(e);
}
while(e!=0);

return 0;
}
