#include "gtest/gtest.h"
#include <iostream>
#include <iomanip>
#include "../../includes/sql/sql.h"
using namespace std;

bool test_stub(bool debug = false)
{
  if (debug){
    cout << "testB:: test-sub() entering test_sub" << endl;
  }
  return true;
}

bool test_sql(bool debug = false)
{
  if (debug){
    cout << "testB:: test-sql() entering test_sql" << endl;
    string s1 = "make table student fields fname, lname, major, age"; 
    string s2 = "insert into student values Flo, Yao, CS, 20"; 
    string s3 = "insert into student values \"Flo\", \"Jackson\", Math,21"; 
    string s4 = "insert into student values Calvin, Woo, Physics,22"; 
    string s5 = "insert into student values \"Anna Grace\", \"Del Rio\", CS, 22"; 
    vector<string> commands; 
    commands.push_back(s1);  
    commands.push_back(s2); 
    commands.push_back(s3); 
    commands.push_back(s4); 
    commands.push_back(s5); 

    /*
    0                      Flo                      Yao                       CS                       20
    1                     Flo                  Jackson                     Math                        21
    2                   Calvin                      Woo                  Physics                       22
    3               Anna Grace                  Del Rio                       CS                       22
    */

   //203
    SQL sql; 
    for(int i = 0; i < commands.size(); i++){
      sql.command(commands[i]); 
    }

    string command = "select * from student"; 
    cout<<sql.command(command)<<endl; 
    sql.select_recnos(); 

    command = "select * from student where fname = \"Flo\" and lname = \"Yao\""; 
    cout<<"command: "<<command<<endl; 
    cout<<sql.command(command)<<endl; 
    cout << sql.select_recnos(); 

  }
  return true;
}


bool test_logical(bool debug = false){
  vector<string> commands{
                              "make table student fields age, major, fname, lname",
                              "insert into student values 15, CS, Flo, Yao",
                              "insert into student values 17, Math, Flo, Jackson",
                              "insert into student values 20, Physics, Calvin, Woo",
                              "insert into student values 13, Communications, Lilli, Sutton",
                              "insert into student values 11, Communications, Ricong, Huang"
                         
    };
    SQL sql;

    for(int i = 0; i < commands.size(); i++){
      sql.command(commands[i]);
    }
  if(!debug){
    cout << "full table\n";
    cout << sql.command("select * from student") << endl;
    string command;

    command = "select * from student where age < 16";
    cout<<"command: "<<command<<endl;
    cout<<sql.command(command)<<endl;
    cout << sql.select_recnos() << endl;

    command = "select * from student where age < 17";
    cout<<"command: "<<command<<endl;
    cout<<sql.command(command)<<endl;
    cout << sql.select_recnos() << endl;

    command = "select * from student where age > 11 and lname > Jackson";
    cout<<"command: "<<command<<endl;
    cout<<sql.command(command)<<endl;
    cout << sql.select_recnos() << endl;


  }

  if(debug){
    cout << "full table\n";
    cout << sql.command("select * from student") << endl;
    string command;

    // command = "select * from student where age <= 10";
    // cout<<"command: "<<command<<endl;
    // cout<<sql.command(command)<<endl;
    // cout << sql.select_recnos() << endl;

    // command = "select * from student where age >= 10";
    // cout<<"command: "<<command<<endl;
    // cout<<sql.command(command)<<endl;
    // cout << sql.select_recnos() << endl;

    // command = "select * from student where age > 10";
    // cout<<"command: "<<command<<endl;
    // cout<<sql.command(command)<<endl;
    // cout << sql.select_recnos() << endl;

    // command = "select * from student where age < 10";
    // cout<<"command: "<<command<<endl;
    // cout<<sql.command(command)<<endl;
    // cout << sql.select_recnos() << endl;

    command = "select * from student where age > 17";
    cout<<"command: "<<command<<endl;
    cout<<sql.command(command)<<endl;
    cout << sql.select_recnos() << endl;
    // command = "select * from student where age >= 12";
    // cout<<"command: "<<command<<endl;
    // cout<<sql.command(command)<<endl;
    // cout << sql.select_recnos() << endl;
    // command = "select * from student where age > 12";
    // cout<<"command: "<<command<<endl;
    // cout<<sql.command(command)<<endl;
    // cout << sql.select_recnos() << endl;
    // command = "select * from student where age < 12";
    // cout<<"command: "<<command<<endl;
    // cout<<sql.command(command)<<endl;
    // cout << sql.select_recnos() << endl;

    
  }
  return true;
}


bool test_parentheses(bool debug){
  string s1 = "make table student fields fname, lname, major, age"; 
    string s2 = "insert into student values Flo, Yao, CS, 20"; 
    string s3 = "insert into student values \"Flo\", \"Jackson\", Math,21"; 
    string s4 = "insert into student values Calvin, Woo, Physics,22"; 
    string s5 = "insert into student values \"Anna Grace\", \"Del Rio\", CS, 22"; 
    string s6 = "insert into student values \"Anna Kat\", \"The Bat\", Art, 33"; 
    string s7 = "insert into student values Lilli, Sutton, Communications, 34"; 
    string s8 = "insert into student values Oliver, Knox, Math, 25"; 
    string s9 = "insert into student values Johnathan, Salinas, Biology, 18";
    string s10 = "insert into student values Justin, Wang, History, 19"; 

    vector<string> commands; 
    commands.push_back(s1);  

    //insertions
    commands.push_back(s2); 
    commands.push_back(s3); 
    commands.push_back(s4); 
    commands.push_back(s5);
    commands.push_back(s6); 
    commands.push_back(s7);
    commands.push_back(s8); 

    SQL sql; 
    for(int i = 0; i < commands.size(); i++){
      sql.command(commands[i]); 

    }

  if(debug){
    cout<<"full table\n"; 
    cout<<sql.command("select * from student")<<endl; 

    string command; 
    command = "select * from student where age < 30 and ((major = CS or major = Physics) and (lname < Y))";
    cout<<"command: "<<command<<endl; 
    cout<<sql.command(command)<<endl; 
    cout << sql.select_recnos() << endl; 

    command = "select * from student where (fname = \"Anna Grace\" and lname = \"Del Rio\")) or (age >= 22))";
    cout<<"command: "<<command<<endl; 
    cout<<sql.command(command)<<endl; 
    cout << sql.select_recnos() << endl; 


    //professor tests
    command = "select * from student where (age < 30 and age > 20) and ((major = CS or major = Physics) or (lname < M))";
    cout<<"command: "<<command<<endl; 
    cout<<sql.command(command)<<endl; 
    cout << sql.select_recnos() << endl; 

    command = "select * from student where ((age < 30 and age > 20) and major = Communications) or ((major = CS or major = Physics))";
    cout<<"command: "<<command<<endl; 
    cout<<sql.command(command)<<endl; 
    cout << sql.select_recnos() << endl; 

    command = "select * from student where ((age < 25 and age > 20) or major = Communications) or ((lname > D and lname <= H))";
    cout<<"command: "<<command<<endl; 
    cout<<sql.command(command)<<endl; 
    cout << sql.select_recnos() << endl; 

    command = "select * from student where age < 25 or (age > 20 and major = Communications)";
    cout<<"command: "<<command<<endl; 
    cout<<sql.command(command)<<endl; 
    cout << sql.select_recnos() << endl; 

    command = "select * from student where (age < 25 or age > 20) and major = Communications"; 
    cout<<"command: "<<command<<endl; 
    cout<<sql.command(command)<<endl; 
    cout << sql.select_recnos() << endl; 

    command = "select * from student where (age < 25 and age > 20) and major = Communications"; //will give non-existent
    cout<<"command: "<<command<<endl; 
    cout<<sql.command(command)<<endl; 
    cout << sql.select_recnos() << endl; 

    command = "select * from student where ((age < 25 and age > 20) or major = Communications) or ((lname > D and lname <= N))";
    cout<<"command: "<<command<<endl; 
    cout<<sql.command(command)<<endl; 
    cout << sql.select_recnos() << endl; 

    command = "select * from student where (age < 25 and age > 20) and (lname > D and lname <= N) or (major = Communications)";
    cout<<"command: "<<command<<endl; 
    cout<<sql.command(command)<<endl; 
    cout << sql.select_recnos();

    command = "select * from student where (age < 25 and age > 20) and (lname > J) or (major = Math)";
    cout<<"command: "<<command<<endl; 
    cout<<sql.command(command)<<endl; 
    cout << sql.select_recnos();

    command = "select * from student where ((age < 20 and age > 30)) and ((major = Art) or (major = Math))";
    cout<<"command: "<<command<<endl; 
    cout<<sql.command(command)<<endl; 
    cout << sql.select_recnos();

  }

  if(debug){
    cout<<"full table\n";
    cout << sql.command("select * from student") << endl;
    string command;
    command = "select * from student where age >= 33";
    cout<<"command: "<<command<<endl; 
    cout<<sql.command(command)<<endl; 
    cout << sql.select_recnos() << endl; 

  }
  return true;
}

bool test_map_iterators(bool debug = false)
{
  Map<string, int> map;
  vector<string> teams = {"12B", "16AA", "18AA", "10A", "10BB"};
  vector<int> numPlayers = {13, 18, 20, 16, 15};
  for(int i = 0; i < teams.size(); i++)
    map.insert(teams[i], numPlayers[i]);
  if(debug)
    cout << "EMPIRE:\n" << map;
  typename Map<string, int>::Iterator it;
  if(debug)
  {
    for(it = map.begin(); it != map.end(); ++it)
      cout << *it << " ";
    cout << endl;
  }
  //testing lower bound
  vector<string> boundNames = {"09A", "10A", "10B", "12B", "12BB", "16AA", "17A", "18AA", "19A"};
  for(int i = 0; i < boundNames.size(); i++)
  {
    it = map.lower_bound(boundNames[i]);
    if(debug && i < boundNames.size() - 1)
      cout << "Lower bound for " << boundNames[i] << ": " << *it << endl;
  }
  cout << "\n____\n";
  for(int i = 0; i < boundNames.size(); i++)
  {
    it = map.upper_bound(boundNames[i]);
    if(debug && i < boundNames.size() - 2)
      cout << "Upper bound for " << boundNames[i] << ": " << *it << endl;
  }
  return true;
}

TEST(TEST_STUB, TestStub) {
  
  //EXPECT_EQ(0, <your individual test functions are called here>);

  EXPECT_EQ(1, test_stub(false));
}

TEST(TEST_SQL, TestSQL) {
  
  //EXPECT_EQ(0, <your individual test functions are called here>);

  EXPECT_EQ(1, test_sql(0));
  EXPECT_EQ(1, test_logical(0));
  EXPECT_EQ(1, test_parentheses(0));
}

TEST(TEST_MAP_ITERATORS, TestMapIterators) {
  
  EXPECT_EQ(1, test_map_iterators(1));
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  std::cout<<"\n\n----------running testB.cpp---------\n\n"<<std::endl;
  return RUN_ALL_TESTS();
}

