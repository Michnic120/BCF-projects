#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>

using namespace std;

int main()
{
  srand( time( NULL ) );

  vector<char> code(8), guess(4), hints(4);
  vector<bool> hintBool(4,0);
  int m, g, n = 0;
  char c;

  for(int i=0; i<4; i++)
  {
    switch(rand() % 6)
    {
        case 0: code[i] = 'o'; break;
        case 1: code[i] = 'r'; break;
        case 2: code[i] = 'b'; break;
        case 3: code[i] = 'g'; break;
        case 4: code[i] = 'y'; break;
        case 5: code[i] = 'v'; break;
    }
  }

  for(int i = 0; i < 4; i++)
     code[i+4] = code[i];

  cout << "\nEnter the color\nr - red\nb - blue\ng - green\ny - yellow\nv - violet\no - orange\n";

  while(n < 10)
    {
      m = 0;
      for(int i = 0; i<4; )
      {
        cin >> c;

        switch(c)
        {
          case 'r': guess[i] = c; i++; break;
          case 'b': guess[i] = c; i++; break;
          case 'g': guess[i] = c; i++; break;
          case 'y': guess[i] = c; i++; break;
          case 'v': guess[i] = c; i++; break;
          case 'o': guess[i] = c; i++; break;
          default: cout << "Wrong color!\n";
        }
      }

      for(int i = 0; i < 4; i++)
        {
          if(guess[i] == code[i])
            {
              hints[m] ='O';
              hintBool[i] = true;
              m++;
            }
        }

      if(m == 4) break;  g = m;

      for(int i = 0; i < 4; i++)
          if(hintBool[i] == 0)
            {
              for(int j=0;j<4;j++)
                if(guess[i]==code[j] && hintBool[j]==0)
                  {
                    hints[g]='o';
                    code[j]='.';
                    g++;
                  }
            }

        for(int i = 0; i < 4; i++)
            {
                if(hints[i]!= 'o' && hints[i]!= 'O')
                {
                  hints[i]='.';
                }

                cout << hints[i];
                hints[i] = '.';
                hintBool[i] = 0;
                code[i] = code[i+4];
            }
        cout << "\nTry again\n";
        n++;
    }
}
