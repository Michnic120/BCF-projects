#include<iostream>
#include<string>
#include<vector>
#include<windows.h>
#include<conio.h>
#include<algorithm>

using namespace std;

struct Event
{
    int minute, hour, day, month, year;
    string name, desc;

    Event(int mi, int ho, int d, int m, int y, string n, string des)
        : minute(mi), hour(ho), day(d), month(m), year(y), name(n), desc(des) {}
};

typedef vector<Event> Vector;

void Enter(Vector &Vec)
{
    int mminute, mhour, mday, mmonth, myear;
    string mname, mdesc;

    {do{cout<<"Put hour: ";      cin>> mhour;
          if(mhour<0 || mhour>23)cout<<"Invalid hour entered! ";}
           while(mhour<0 || mhour>23);

         do{cout<<"Put minutes: ";      cin>> mminute;
         if(mminute<0 || mminute>59)cout<<"Invalid minutes entered! ";}
           while(mminute<0 || mminute>59);

         do{cout<<"Put day: ";      cin>> mday;
         if(mday<1 || mday>31)cout<<"Invalid day entered! ";}
           while(mday<1 || mday>31);

         do{cout<<"Put month: ";    cin>> mmonth;
         if(mmonth<1 || mmonth>12)cout<<"Invalid month entered! ";}
           while(mmonth<1 || mmonth>12);

         do{cout<<"Put year: ";     cin>> myear;
         if(myear<2017 || myear>2090)cout<<"Invalid year entered! ";}
           while(myear<2017 || myear>2090);

         do{cout<<"Put title: ";     cin>> mname; int w = mname.length();
         if(w>15)cout<<"Entered title is too long! (max.15 characters) ";
         if(w<15)for(int i=0;i<15-w;i++)mname+=" ";}
           while(mname.length()>15);

         cout<<"Put description: "; cin>> mdesc;

         Vec.push_back(Event(mminute,mhour,mday,mmonth,myear,mname,mdesc));}
}

void Print(Vector Vec)
{
   system("cls");
   if(!Vec.empty())
   {
    sort(Vec.begin(),Vec.end(), [](const Event&lhs, const Event&rhs){return lhs.minute < rhs.minute;});
    sort(Vec.begin(),Vec.end(), [](const Event&lhs, const Event&rhs){return lhs.hour   < rhs.hour;});
    sort(Vec.begin(),Vec.end(), [](const Event&lhs, const Event&rhs){return lhs.day    < rhs.day;});
    sort(Vec.begin(),Vec.end(), [](const Event&lhs, const Event&rhs){return lhs.month  < rhs.month;});
    sort(Vec.begin(),Vec.end(), [](const Event&lhs, const Event&rhs){return lhs.year   < rhs.year;});

    cout<<"Hour:  "<<"Date:         "<<"Title:            "<<"Description:   "<<endl;
    for(int i=0; i<Vec.size(); i++)
     {if(Vec[i].hour<10)   cout<<"0"; cout<<Vec[i].hour<<":";
      if(Vec[i].minute<10) cout<<"0"; cout<<Vec[i].minute<<"  ";
      if(Vec[i].day<10)    cout<<"0"; cout<<Vec[i].day<<"/";
      if(Vec[i].month<10)  cout<<"0"; cout<<Vec[i].month<<"/"<<Vec[i].year;
      cout<<"    "<<Vec[i].name<<"   "<<Vec[i].desc<<endl;}
    cout<<endl<<"Press any key to continue.";}

   else
     cout<<"There are no events added yet! Press any key to continue.";
   getch();
}

int main()
{
    int sign;
    Vector V;

    do
    {
      system("cls");
      cout<<"1 - add event, 2 - print all events, 3 - exit  "; cin>>sign;
      switch(sign)
      {
      case 1:
        Enter(V);
        break;
      case 2:
        Print(V);
        break;
      case 3:
        break;
      default:
        cout<<"Try again...";
        getch();
        break;
      }
    }
    while(sign!=3);

    return 0;
}
