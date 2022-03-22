#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <bits/stdc++.h>
#include <vector>
#include <list>
#include <sstream>
#include <map>
#include "src/Item.hpp"
#include "src/Tool.hpp"
#include "src/NonTool.hpp"
#include "src/Recipe.hpp"
#include "src/GameState.hpp"
using namespace std;
string clean(string s) {
    // sometimes the file can have CRLF ending, so we need to clean the string first before using it.
    if (s.length() == 0) return s;
    if (s[s.length()-1] != '\x0D') return s;
    return s.substr(0, s.length()-1);
}

vector<string> getRecipeMatrix(vector<string> eachRecipe, int row, int col) {
  vector<string> res(9);
  int appendmanyRow = 3 - row;
  int idx = 0;
  for (int i = 0; i <= 3; i++) {
    if (i < col) {
      res[i] = eachRecipe[idx];
      idx++;
    } else {
      res[i] ="-";
    }
  }
  
  if (appendmanyRow == 1) {
    for (int i = 3; i <= 5; i++) {
      if (i < col + 3) {
        res[i] = eachRecipe[idx];
        idx++;
      } else {
        res[i] = "-";
      }
    }

    for (int i = 6; i <= 8; i++) {
      res[i] = "-";
    }
  } else if (appendmanyRow == 2) {
    for (int i = 3; i <= 8; i++) {
      res[i] = "-";
    }
  } else {
    for (int i = 3; i <= 5; i++) {
      if (i < col + 3) {
        res[i] = eachRecipe[idx];
        idx++;
      } else {
        res[i] = "-";
      }
    }

    for (int i = 6; i <= 8; i++) {
      if (i < col + 6) {
        res[i] = eachRecipe[idx];
        idx++;
      } else {
        res[i] = "-";
      }
    }
  }
  return res;
}

int main() {
  //add welcome message

  // /* SETUP */
  cout << "setting up..." << endl;
  string configPath = "./config";
  string itemConfigPath = configPath + "/item.txt";

  // CREATE LIST OF LEGAL ITEM
  list<Item*> legalItem;

  //buat ngebantu bikin recipe, aku butuh sebuah item, ketika diketahui nama item.
  map<string, Item*> ItemNameToItemObject; 
  map<string, bool> isItemATool;
   
  //read from config 
  ifstream itemConfigFile(itemConfigPath);
  for (string line; getline(itemConfigFile, line);) {
    cout << line << endl;

    //spliting line
    stringstream ss(line);
    string currentWord;
    int count = 0;
    string id;
    string name;
    string nonToolClass;
    string toolnontool;
    while(ss >> currentWord){
      if(count == 0){
        id = currentWord;
      }
      else if(count == 1){ //
        name = currentWord;
      }
      else if(count == 2){ //
        nonToolClass = currentWord;
      }
      else if(count == 3){ //
        toolnontool = currentWord;
      }
      count++;
      //cout << count++ << ". " << currentWord << endl;
    }

    /* FOR DEBUGGING PURPOSE */
    // cout << "\tid = " << id << endl;
    // cout << "\tname = "<< name << endl;
    // cout << "\tclass = "<< nonToolClass << endl;
    // cout << "\ttoolnontool = " << toolnontool << endl;
    /* FOR DEBUGGING PURPOSE */

   


    //add to legal list
    if(toolnontool == "TOOL"){
      //tambahkan tool ke legal item
      Tool* temp = new Tool(stoi(id) ,name, 10);
      // temp->printInfo();
      legalItem.push_back(temp);

      //add to recipe helper
      ItemNameToItemObject.insert(pair<string,Item*> (name,temp));
      isItemATool.insert(pair<string,bool> (name, true));
    }
    else{
      //tambahakn nontool ke legal item
      NonTool* temp = new NonTool(stoi(id), name, nonToolClass,1); // quantity disini harusnya gaperna disentuh. aku kasih 1. kenapa gk dikasih 0 aja? takutnya nti bikin behaviour yg aneh
      legalItem.push_back(temp);

            //add to recipe helper

      ItemNameToItemObject.insert(pair<string,Item*> (name,temp));
      isItemATool.insert(pair<string,bool> (name, false));

    }
  }

  /* FOR DEBUGGING PURPOSE */
  // list<Item*>::iterator it = legalItem.begin();
  // for(it = legalItem.begin(); it != legalItem.end();it++){ //
  //   (*it)->printInfo();
  // }
  /* FOR DEBUGGING PURPOSE */

  vector<Recipe*> legalRecipe;

  // read recipes
  for (const auto &entry :filesystem::directory_iterator(configPath + "/recipe")) {
    cout << "====== batas ====" << endl;
    cout << entry.path() << endl;
    
    // create list of legal recipe
    ifstream eachRecipeFile(entry.path());

    //changing text inside into one long string
    string currentRecipeString;
    for (string line; getline(eachRecipeFile, line);) {
      // line = clean(line);

      //each line inside a recipe
      //cout << "\t" << line << endl;
      currentRecipeString.append(line);
      currentRecipeString.append(" ");
    }

    cout <<"INI ISI FILENYA:" << currentRecipeString << endl;
    

    //spliting long string into each variable needed to make a recipe
    string currentWordRecipe;
    int count = 0;
    int row = 0;
    int col = 0; 
    vector<string> eachRecipe;
    string itemName;
    int resultquantity = 0;

    stringstream ssrecipe(currentRecipeString);
    //assigning into the correct variable
    while(ssrecipe >> currentWordRecipe) {
      count++;
      cout <<"\t\t" << count << ". " << currentWordRecipe << endl;
      if(count == 1){
        row = stoi(currentWordRecipe);
      }
      else if(count == 2){ //
        col = stoi(currentWordRecipe);
      }
      else if(count > 2 && count <= ((row*col) + 2)){
        //add to vector recipe
        eachRecipe.push_back(currentWordRecipe);
      }
      else if(count == ((row*col) + 3)){ //
        itemName = currentWordRecipe;
        cout << "INI ITEM NAMENYA " << currentWordRecipe << endl;
      }
      else if(count == ((row*col) + 4)){ //
        resultquantity = stoi(currentWordRecipe);
        cout << "INI jumlah yg kebikin " << currentWordRecipe << endl;
      }
    }

    vector<string> addToRecipe = getRecipeMatrix(eachRecipe, row, col);
    for (auto it = addToRecipe.begin(); it != addToRecipe.end(); it++) {
      cout << *it << endl;
    }

    //creating Recipe Object
    if(isItemATool[itemName]){
    // //   //recipe made for tool

      int tempid = ItemNameToItemObject[itemName]->getID();
      string tempname = ItemNameToItemObject[itemName]->getName();
      int tempdurability = ItemNameToItemObject[itemName]->getDurability();
      Recipe *temp = new Recipe(row, col, addToRecipe, new Tool(tempid, tempname, tempdurability),resultquantity);
      legalRecipe.push_back(temp);

    }
    else{
    //   //recipe made for nontool
      int tempid = ItemNameToItemObject[itemName]->getID();
      string tempname = ItemNameToItemObject[itemName]->getName();
      string nontoolClass = ItemNameToItemObject[itemName]->getNonToolClass();
      int quantity = ItemNameToItemObject[itemName]->getQuantity();
      Recipe *temp = new Recipe(row, col, addToRecipe, new NonTool(tempid, tempname, nontoolClass, quantity),resultquantity);
      legalRecipe.push_back(temp);
    }
  }
  /*SETUP DONE*/

  //baris dibawah ini(yg buat ngecek recipe), gk jalan kalo recipe legalRecipe dibuat sebagai vector<Recipe>. kalo dibikin vector<Recipe*>, isa. mungkin karena Recipe gapunya cctor?
  cout << "================================================================" << endl;
  //CHECKING LEGAL RECIPE
  vector<Recipe*>::iterator ptr;
  for(ptr = legalRecipe.begin(); ptr != legalRecipe.end();ptr++){
    cout << (*ptr)->getItem()->getName() << endl;
    cout << (*ptr)->getRow() << (*ptr)->getCol() << (*ptr)->getQuantityResult();
  }
  cout << "================================================================" << endl;
  //CHEKING LEGAL ITEM
  list<Item*>::iterator it;
  for (it = legalItem.begin(); it != legalItem.end(); it++){
    cout << (*it)->getName() << endl;
  }


  //creating gamestate
  //vector<Recipe> temp;
  GameState *GS = new GameState(legalItem, legalRecipe);



  cout << "game is ready!" << endl;
  // add more glorified welcome message





  /* START */
  string command;
  command = "HELP";

  
  while (command != "EXIT") {
    cout << "input your command!"<< endl;
    cout << ">";
    cin >> command;
    if(command == "SHOW"){ //COMMAND SHOW
      cout << "SHOW command is picked" << endl;
      GS->SHOW();
    } 
    else if (command == "GIVE") { //COMMAND GIVE
      cout << "GIVE command is picked" << endl;
      string itemName;
      int itemQty;
      cin >> itemName >> itemQty;
      cout << "main done, lempar ke gamestate" << endl;
      GS->GIVE(itemName, itemQty);
    }
    else if (command == "DISCARD"){ //COMMAND DISCARD
      cout << "DISCARD command is picked" << endl;
      string i_id;
      int itemQty;
      cin >> i_id >> itemQty;
      cout << "main done, lempar ke gamestate" << endl;

      GS->DISCARD(i_id, itemQty);

    }
    else if (command == "MOVE") { //COMMAND MOVE
      cout << "MOVE command is picked" << endl;
      //MOVE ada 3 macem
      //pertama dan kedua, diawali sama i_id, angka, sama tujuan akhir. keknya ini digabungin jadi 1, soalnya di gamestate cuman ada 1 buah method

      //ketiga diawali sama c_id
      string src;
      int N;
      string tempN;
      cin >> src >> tempN;
      N = stoi(tempN);
      if(src.substr(0,1) == "I"){
        cout << "DEPANNYA I" << endl;
        vector<string> dest;
        for(int i = 0 ; i < N ; i++){
          string temp;
          cin >> temp; 
          dest.push_back(temp);
        }
        cout << "main done, lempar ke gamestate" << endl;

        GS->MOVE(src, N, dest);
      }
      else{
        string dest;
        cin >> dest;
      cout << "main done, lempar ke gamestate" << endl;

        GS->MOVE(src, dest);
      }
    }
    else if(command == "USE"){ //COMMAND USE
      cout << "USE command is picked" << endl;
      string i_id;
      cin >> i_id;
      cout << "main done, lempar ke gamestate" << endl;

      GS->USE(i_id);
    } 
    else if (command == "CRAFT") { //COMMAND CRAFT
      cout << "CRAFT command is picked" << endl;

      //ntah kenapa dari code dari templatenya kek gini. padahal di spek, craft gk nerima argumen apapun
      // string slotSrc;
      // int slotQty;
      // string slotDest;
      // // need to handle multiple destinations
      // cin >> slotSrc >> slotQty >> slotDest;
      cout << "main done, lempar ke gamestate" << endl;
      
      GS->CRAFT();
    } 
    else if (command == "EXPORT") { //COMMAND EXPORT

      //template export, mangga di reuse di GameState
      // cout << "EXPORT command is picked" << endl;
      string outputPath;
      cin >> outputPath;
      // ofstream outputFile(outputPath);

      // // hardcode for first test case
      // outputFile << "21:10" << endl;
      // outputFile << "6:1" << endl;
      // for (int i = 2; i < 27; i++) {
      //   outputFile << "0:0" << endl;
      // }

      // cout << "Exported" << endl;
      // todo
      cout << "main done, lempar ke gamestate" << endl;

      GS->EXPORT(outputPath);
    }

    // COMMAND TAMBAHAN
    else if(command == "HELP"){
      cout << "this is your help lol" << endl;
    }
    else if(command == "EXIT"){
      cout << "do you want to export your state first? (y/n)" << endl;
      char yn;
      cin >> yn;
      if(yn == 'y'){
        //do export
      }
      else if(yn == 'n'){
        //say goodbye
      }
      else{
        //print invalid input. we take it as a 'n', so bye bye
      }
    }
    else {
      cout << "Invalid command. try \"HELP\" " << endl;
    }
    cout << "COMMAND DONE, NEXT COMMAND" << endl;
  }

  // add closing screen?
  return 0;
}
