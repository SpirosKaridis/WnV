#include <iostream>
#include <string>
#include <ctime>
#include <random>
#include <vector>

#define STANDARD_B "\033[31;0m"             // FOREGROUND: system's, BACKGROUND: system's
#define GREEN_B "\033[37;42m"               // FOREGROUND: white, BACKGROUND: green
#define BLUE_B "\033[37;44m"                // FOREGROUND: white, BACKGROUND: blue
#define OUTLINE "\033[0;41m"                // FORGROUD: system prefrence, Background: system prefrence
#define TREES "\e[4;32m\e[1;32m\e[42;31m"

using namespace std;

class Characters {
     public:
          int HP, DEF, ATT, LVL, POTIONS,x,y;
          bool Team, HealPeriod;

          Characters (int, int, int, int, int,int,int, bool, bool); // parent class Characters default constructor
          Characters() : HP(1), DEF(1), ATT(1), LVL(1), POTIONS(0) , Team(0), HealPeriod(0){};      //We use an Initialiser List for the object

          ~Characters()  //Destructor of parent class
          {};

          virtual int DealDamage(int EnemyDEF)
          {
               return EnemyDEF - ATT;
          }

          virtual int TakeDamage(int EnemyATT)
          {
               return DEF - EnemyATT;
          }

          virtual int DrinkPotion(void)
          {
               return HP + 1;
          }
};

class NPCs : public Characters {
     public:
          int HP,DEF,ATT,LVL,POTIONS,x,y;
          bool Team, HealPeriod;               // 1 for vampires and 0 for werewolves and  Game time during which a team can heal. 1 for vampires (day) and 0 for werewolves (night) 
 
           NPCs(int,int,int,int,int,int,int,bool,bool);  //default constructor
           NPCs() : HP(1), DEF(1), ATT(1), LVL(1), POTIONS(1), Team(0), HealPeriod(0), x(1), y(1){};

           virtual ~NPCs(){};

           int DealDamage(int EnemyDEF)
           {
               return EnemyDEF - ATT;
          }

          int TakeDamage(int EnemyATT)
          {
               return DEF - EnemyATT;
          }

          int DrinkPotion(void)
          {
               return HP + 1;
          }
};


class Player : public Characters {
     public:
          int RedemptionPotions,x,y;
          bool Team;
          
          Player(int,bool);   //default constructor
          Player() : RedemptionPotions(1), Team(1), x(0), y(0) {};

          virtual ~Player() {};
};






void Pause(int VampireTeam, int WerewolfTeam, int RedemptionPotions)      // Function used for Printing the desired data when the user Pauses the game
{
     int ch;
     bool flag = 0;
     cout << "Number of Vampires Alive: " << VampireTeam << "\n" << "Number of Werewolves Alive: " << WerewolfTeam << "\n" << "Your available Mass Redemption Potions: " << RedemptionPotions;
     cout << "\nPress Any key AND Enter to Continue";
     while (flag == 0)
     {
          cin >> ch;
          flag = 1;
     }
     system("clear");
}

int Randomizer(int minValue, int maxValue)
{
     random_device rd;     //random_device obtains a random number from hardware (DELETE IF COMPILER HAS PROBLEM)
     mt19937 gen(rd());   //we seed the generator | mt19937 is a Mersenne Twister pseudo-random generator based on the mersene-twister-engine for 32-bit numbers with a size of 19937 bits
     uniform_int_distribution<int> distr(minValue, maxValue);        //a discrete distribution of random numbers in the range of minValue-maxValue (or whatever we use in it)

     return distr(gen);
}

bool Wait(const unsigned long &Time)    // a function to add delay
{
     clock_t Tick = clock_t(float(clock()) / float(CLOCKS_PER_SEC) * 1000.f);
     if (Tick < 0)
     {
          return 0;
     }
     clock_t Now = clock_t(float(clock()) / float(CLOCKS_PER_SEC) * 1000.f);
     if (Now < 0)
     {
          return 0;
     }
     while ((Now - Tick) < Time)
     {
          Now = clock_t(float(clock()) / float(CLOCKS_PER_SEC) * 1000.f);
          if (Now < 0)
               return 0;
     }
     return 1;
}

// * * * * * * * * * *  M A I N   F U N C T I O N  * * * * * * * * * * //
int main()
{
     
     int i, j, x, movement, oldX, oldY, dimensionsX, dimensionsY,n,Dayperiod;      // we initialize the size of both teams to 0 (will be increase once team members are generated)
     char alliegiance, playerentry;
     bool coinflip;

     cout << "Insert the length and the height of the map respectively.\n" << endl;
     cin >> i >> j;      // i is width j is heigth
     dimensionsX = i;    // a value storage since I use i and j afterwards, honestly a cheap fix (not memory wise) 
     dimensionsY = j;


     int map[i][j];


     cout << " \t\tChoose Team ";
     cout << "\n";
     cout << " Enter W for Werewolf Team and V for Vampire Team\n ";

     cin >> alliegiance;

     system("clear");

     Player playercharacter; //(int (1),bool (alliegiance),int (0), int (0)); //creating the player object
     playercharacter.RedemptionPotions = 1;
     playercharacter.Team = alliegiance;
     playercharacter.x = Randomizer(1,dimensionsX);
     playercharacter.y = Randomizer(1, dimensionsY);

     bool flag = 0;      //used to fault check team entry
     playercharacter.RedemptionPotions = 1;       //player starts with 1 Mass Heal Potion

     while (flag == 0) {      // assigning player team
          if (alliegiance == 'W' || 'w')
          {
               playercharacter.Team = 0;
               flag = 1;
          } else if (alliegiance == 'V' || 'v') 
          {
               playercharacter.Team = 1;
               flag = 1;
          } else
          {
               cout << "Wrong Input. Please Enter either V or W\n";
               cin >> alliegiance;
          }
     }


     cout << "You can move by using WASD and pressing ENTER to confirm your choice\n\n";
     
     //***********************************************//
     // * * *  P R I N T I N G   T H E   M A P  * * * //
     for (i = 0; i < dimensionsX + 2; i++)     // Top map border printing
          cout << OUTLINE "##" STANDARD_B;

          cout << endl;
          
     srand((int)time(0));      //We're using srand() to add a level of randomness in rand()    

     for (i = 0; i < dimensionsY; i++)
     {
          cout << OUTLINE "##";
          for (j = 0;j < dimensionsX; j++)
          {
               n = rand() % 10;          // According to "rand()"'s value we print either green, blue, or trees
              if (n >= 3) {    // We've assigned more values to green, in order to be more possible to be printed
                    cout << GREEN_B "  " STANDARD_B;
                    map[j][i]  = 1;
              } else if (n == 0) {
                    cout << BLUE_B "  " STANDARD_B;
                    map[j][i] = 0;
              } else if (n == 2)
              {
               int tree = rand() % 4;
               cout << TREES "/*" STANDARD_B;
               map[j][i] = 2;
              }
          
          }
          cout << OUTLINE "##" STANDARD_B << endl;
     }

     for (i = 0; i < dimensionsX + 2; i++) // Bottom map border printing
          cout << OUTLINE "##" STANDARD_B;
     
     cout << "\n";

     while (map[playercharacter.x][playercharacter.y] != 1)
     {
          playercharacter.x = Randomizer(1,dimensionsX);
          playercharacter.y = Randomizer(1,dimensionsY);
     }
     map[playercharacter.x][playercharacter.y] = 5;
     Wait(10000);
     
     
     // Creating the team NPCs
     x = Randomizer(1, ((i * j) / 15));

     vector<NPCs> Vampires;
     Vampires.reserve(x);
     
     for (i = 0; i < x; i++)
     {
          NPCs vampire;
          Vampires.push_back(vampire);
          Vampires[i].HP = 3;
          Vampires[i].ATT = Randomizer(1,3);
          Vampires[i].DEF = Randomizer(1,2);
          Vampires[i].LVL = Randomizer(1,3);
          Vampires[i].POTIONS = Randomizer(0,2);
          Vampires[i].Team = 1;
          Vampires[i].HealPeriod = 1;
          Vampires[i].x = Randomizer(1,dimensionsX);
          Vampires[i].y = Randomizer(1, dimensionsY);
          while (map[Vampires[i].x][Vampires[i].y] != 1)
          {
              Vampires[i].x = Randomizer(1, dimensionsX);
              Vampires[i].y = Randomizer(1, dimensionsY);
          }
          map[Vampires[i].x][Vampires[i].y] = 3;
     }

     vector<NPCs> Werewolves;
     Werewolves.reserve(x);

     for (i = 0; i < x; i++)
     {
          NPCs werewolf;
          Werewolves.push_back(werewolf);
          Werewolves[i].HP = 3;
          Werewolves[i].ATT = Randomizer(1, 3);
          Werewolves[i].DEF = Randomizer(1, 2);
          Werewolves[i].LVL = Randomizer(1, 3);
          Werewolves[i].POTIONS = Randomizer(0, 2);
          Werewolves[i].Team = 0;
          Werewolves[i].HealPeriod = 0;
          Werewolves[i].x = Randomizer(1, dimensionsX);
          Werewolves[i].y = Randomizer(1, dimensionsY);
          while (map[Werewolves[i].x][Werewolves[i].y] != 1)
          {
              Werewolves[i].x = Randomizer(1, dimensionsX);
              Werewolves[i].y = Randomizer(1, dimensionsY);
          }
          map[Werewolves[i].x][Werewolves[i].y] = 4;
     }




cout << "Press E at any time to quit the game\t\n";

Wait(6000);

while (!Vampires.empty() || !Werewolves.empty() || (playerentry != 'e' && playerentry != 'E')) {
     coinflip = Randomizer(0,1);
     if (playerentry == 'W' || playerentry == 'w')
     {
          if (map[playercharacter.x][playercharacter.y - 1] == 1)
          {
               oldX = playercharacter.x;
               oldY = playercharacter.y;
               playercharacter.y -= 1;
               map[playercharacter.x][playercharacter.y] = 5;
               map[oldX][oldY] = 1;
          }
     }
     if (playerentry == 'S' || playerentry == 's')
     {
          if (map[playercharacter.x][playercharacter.y + 1] == 1)
          {
               oldX = playercharacter.x;
               oldY = playercharacter.y;
               playercharacter.y += 1;
               map[playercharacter.x][playercharacter.y] = 5;
               map[oldX][oldY] = 1;
          }
     }
     if (playerentry == 'A' || playerentry == 'a')
     {
          if (map[playercharacter.x - 1][playercharacter.y] == 1)
          {
               oldX = playercharacter.x;
               oldY = playercharacter.y;
               playercharacter.x -= 1;
               map[playercharacter.x][playercharacter.y] = 5;
               map[oldX][oldY] = 1;
          }
     }
     if (playerentry == 'D' || playerentry == 'd')
     {
          if (map[playercharacter.x + 1][playercharacter.y] == 1)
          {
               oldX = playercharacter.x;
               oldY = playercharacter.y;
               playercharacter.x  += 1;
               map[playercharacter.x][playercharacter.y] = 5;
               map[oldX][oldY] = 1; 
          }
     }
     if (playerentry == 'e' || playerentry == 'E')
     {
          cout << "\nYou have ended the Game\n";
          break;
     }
     
     if (playerentry == 'H' || playerentry == 'h')
     {
          if (playercharacter.Team == 0 && Dayperiod == 0 && playercharacter.RedemptionPotions > 0)
          {
               for (i = 0; i < x; i++) 
                    Werewolves[i].HP += 1;
               playercharacter.RedemptionPotions -= 1;
          } else if (Dayperiod == 1 && playercharacter.RedemptionPotions > 0)
          {
               for (i = 0; i < x; i++)
                    Vampires[i].HP += 1;
               playercharacter.RedemptionPotions -= 1;
          }
     } 

     if (playerentry == 'P' || playerentry == 'p')
     {
          system("clear");
          Pause(Vampires.size(),Werewolves.size(), playercharacter.RedemptionPotions);
     }

     if (coinflip == 0)  // first go the werewolves
     {
          for (i = 0; i < x; i++)
          {
               movement = Randomizer(1,4);
               if (movement == 1 && map[Werewolves[i].x + 1][Werewolves[i].y] == 1)
               {
                    oldX = Werewolves[i].x;
                    oldY = Werewolves[i].y;
                    Werewolves[i].x += 1;
                    map[Werewolves[i].x][Werewolves[i].y] = 4;
                    map[oldX][oldY] = 1;
               } else if (movement == 2 && map[Werewolves[i].x - 1][Werewolves[i].y] == 1)
               {
                    oldX = Werewolves[i].x;
                    oldY = Werewolves[i].y;
                    Werewolves[i].x -= 1;
                    map[Werewolves[i].x][Werewolves[i].y] = 4;
                    map[oldX][oldY] = 1;
               } else if (movement == 3 && map[Werewolves[i].x][Werewolves[i].y + 1] == 1)
               {
                    oldX = Werewolves[i].x;
                    oldY = Werewolves[i].y;
                    Werewolves[i].y += 1;
                    map[Werewolves[i].x][Werewolves[i].y] = 4;
                    map[oldX][oldY] = 1;
               } else if (movement == 4 && map[Werewolves[i].x][Werewolves[i].y - 1] == 1)
               {
                    oldX = Werewolves[i].x;
                    oldY = Werewolves[i].y;
                    Werewolves[i].y -= 1;
                    map[Werewolves[i].x][Werewolves[i].y] = 4;
                    map[oldX][oldY] = 1;
               }
          }

          for (i = 0; i < x; i++) 
          {
               movement = Randomizer(1, 4);
               if (movement == 1 && map[Vampires[i].x + 1][Vampires[i].y] == 1)
               {
                    oldX = Vampires[i].x;
                    oldY = Vampires[i].y;
                    Vampires[i].x += 1;
                    map[Vampires[i].x][Vampires[i].y] = 3;
                    map[oldX][oldY] = 1;
               } else if (movement == 2 && map[Vampires[i].x - 1][Vampires[i].y] == 1)
               {
                    oldX = Vampires[i].x;
                    oldY = Vampires[i].y;
                    Vampires[i].x -= 1;
                    map[Vampires[i].x][Vampires[i].y] = 3;
                    map[oldX][oldY] = 1;
               } else if (movement == 3 && map[Vampires[i].x][Vampires[i].y + 1] == 1)
               {
                    oldX = Vampires[i].x;
                    oldY = Vampires[i].y;
                    Vampires[i].y += 1;
                    map[Vampires[i].x][Vampires[i].y] = 3;
                    map[oldX][oldY] = 1;
               } else if (movement == 4 && map[Vampires[i].x][Vampires[i].y - 1] == 1)
               {
                    oldX = Vampires[i].x;
                    oldY = Vampires[i].y;
                    Vampires[i].y -= 1;
                    map[Vampires[i].x][Vampires[i].y] = 3;
                    map[oldX][oldY] = 1;
               }
          }
     } else 
     {
          for (i = 0; i < x; i++)
          {
               movement = Randomizer(1, 4);
               if (movement == 1 && map[Vampires[i].x + 1][Vampires[i].y] == 1)
               {
                    oldX = Vampires[i].x;
                    oldY = Vampires[i].y;
                    Vampires[i].x += 1;
                    map[Vampires[i].x][Vampires[i].y] = 3;
                    map[oldX][oldY] = 1;
               } else if (movement == 2 && map[Vampires[i].x - 1][Vampires[i].y] == 1)
               {
                    oldX = Vampires[i].x;
                    oldY = Vampires[i].y;
                    Vampires[i].x -= 1;
                    map[Vampires[i].x][Vampires[i].y] = 3;
                    map[oldX][oldY] = 1;
               } else if (movement == 3 && map[Vampires[i].x][Vampires[i].y + 1] == 1)
               {
                    oldX = Vampires[i].x;
                    oldY = Vampires[i].y;
                    Vampires[i].y += 1;
                    map[Vampires[i].x][Vampires[i].y] = 3;
                    map[oldX][oldY] = 1;
               } else if (movement == 4 && map[Vampires[i].x][Vampires[i].y - 1] == 1)
               {
                    oldX = Vampires[i].x;
                    oldY = Vampires[i].y;
                    Vampires[i].y -= 1;
                    map[Vampires[i].x][Vampires[i].y] = 3;
                    map[oldX][oldY] = 1;

               }
          }

          for (i = 0; i < x; i++)
          {
               movement = Randomizer(1,4);
               if (movement == 1 && map[Werewolves[i].x + 1][Werewolves[i].y] == 1 )
               {
                    oldX = Werewolves[i].x;
                    oldY = Werewolves[i].y;
                    Werewolves[i].x += 1;
                    map[Werewolves[i].x][Werewolves[i].y] = 4;
                    map[oldX][oldY] = 1;
               } else if (movement == 2 && map[Werewolves[i].x - 1][Werewolves[i].y] == 1)
               {
                    oldX = Werewolves[i].x;
                    oldY = Werewolves[i].y;
                    Werewolves[i].x -= 1;
                    map[Werewolves[i].x][Werewolves[i].y] = 4;
                    map[oldX][oldY] = 1;
               } else if (movement == 3 && map[Werewolves[i].x][Werewolves[i].y + 1] == 1)
               {
                    oldX = Werewolves[i].x;
                    oldY = Werewolves[i].y;
                    Werewolves[i].y += 1;
                    map[Werewolves[i].x][Werewolves[i].y] = 4;
                    map[oldX][oldY] = 1;
               } else if (movement == 4 && map[Werewolves[i].x][Werewolves[i].y - 1] == 1)
               {
                    oldX = Werewolves[i].x;
                    oldY = Werewolves[i].y;
                    Werewolves[i].y -= 1;
                    map[Werewolves[i].x][Werewolves[i].y] = 4;
                    map[oldX][oldY] = 1;
               }
          }
     }
     system("clear");

     for (i = 0; i < dimensionsX + 2; i++) // Top map border printing
          cout << OUTLINE "##" STANDARD_B;

     cout << endl;
     for (i = 0; i < dimensionsY; i++)
     {
          cout << OUTLINE "##";
          for (j = 0; j < dimensionsX;j++)
          {
               if (map[j][i] ==  1)     //map has grass
                    cout << GREEN_B "  " STANDARD_B;
               if (map[j][i] == 0)
                    cout << BLUE_B "  " STANDARD_B;    //map has water
               if (map[j][i] == 2)    //map has trees
                    cout << TREES "/*" STANDARD_B;
               if (map[j][i] == 3)    //map has vampire
                    cout << GREEN_B "V " STANDARD_B;
               if (map[j][i] == 4)    //map has werewolf
                    cout << GREEN_B "W " STANDARD_B;
               if (map[j][i] == 5) // playerlocation
                    cout << GREEN_B "P " STANDARD_B;
          }
          cout << OUTLINE "##" STANDARD_B << endl;
     }

     for (i = 0; i < dimensionsX + 2; i++)  // Bottom map border printing
     { 
          cout << OUTLINE "##" STANDARD_B;
     }

     cout << "\n";
     Wait(1000);

     cout << "Your turn to play\n";
     cin >> playerentry;
}

return 0;
}
