#include <iostream>
#include <string>
#include <iomanip>
#include <stdio.h>
#include <time.h>
#include <fstream>
#include <bits/stdc++.h>
#include <vector>
#include <filesystem>
#include <unistd.h>
#include <exception>
#include <windows.h>

namespace fs = std::filesystem;

using namespace std;
using std::filesystem::current_path;
using std::filesystem::directory_iterator;

int passwordAttempts = 3;
string password = "password";
bool flag = false;

int main();

void login()
{
  if(passwordAttempts > 0) {
    string enteredPassword;
    cout<<"\n\n\n\t\t\t\t\t\t   AUTHENTICATION\n";
    cout<<"\n\t\t\t\t\tKindly enter password :  ";
    cin>>enteredPassword;
    if(enteredPassword.compare(password) == 0)
    {
      cout << "\n\t\t\t\t\t\t     Loading...";
      flag = true;
      main();
    }
    else
    {
      system("cls");
      cout << "\n\t\t\t\t\tIncorrect Password. Try again." << endl;
      passwordAttempts--;
      cout << "\n\t\t\t\t\tAttempts left: " << (passwordAttempts) <<"\n";
      main();
    }
  }
  else
  {
    cout << "\n\t\t\t\t\tAttempts limit exceeded. Try later";
    exit(0);
  }

}

void addEntry()
{
  string date;
  cout << "\n\t\t\t\t\tEnter the date in format [YYYY-MM-DD]: ";
  cin >> date;

  string title;
  cout << "\n\t\t\t\t\tEnter the title: ";
  cin >> title;

  string fileName = date;
  fileName = fileName.append("_");
  fileName = fileName.append(title);
  fileName = fileName.append(".txt");

  ifstream ifile;
  ifile.open(fileName);
  if(ifile)
  {
    cout << "\n\t\t\t\t\tEntry already exists.";
  }
  else
  {
    // calculates current date and time on the current system
    time_t now = time(0);
    // convert now to string form
    char* date_time = ctime(&now);
    // cout << "The current date and time is: " << date_time << endl;
    string timestamp = date_time;

    ofstream fout;
    fout.open(fileName, ios::out);
    fout << "-----------------------------------" << endl;
    fout << "Timestamp: " << timestamp;
    fout << "-----------------------------------" << endl;
    fout << "Title: " << title;
    fout << "\n";

    string description;
    cout << "\n\t\t\t\t\tWhat's on your mind? (end with 'EXIT')" << endl;
    cout << "\n\t\t\t\t";
    while(getline(cin,description))
    {
      cout << "\n\t\t\t\t";
      if(description == "EXIT") break;
      fout << description;
    }
    cout << "\n\t\t\t\tEntry saved successfully";
    fout.close();

    char isFav;
    cout<<"\n\n\n\t\t\t\tDo you want to mark this as your favourite entry? [Y/N]: ";
    cin>>isFav;
    if(isFav == 'Y' || isFav == 'y')
    {
      string directoryName = "favorites";
      fs::create_directory(directoryName);
      char tmp[256];
      getcwd(tmp, 256);
      string path = tmp;
      string mainDirectoryFileName = path + "\\" + fileName;
      string favoritePath = path + "\\favorites";
      string newPathFileName = favoritePath + "\\" + fileName;

      // cout << "Current working directory: " << path << endl;

      fs::path sourceFile = mainDirectoryFileName;
      fs::path targetParent = favoritePath;
      auto target = targetParent / sourceFile.filename(); // sourceFile.filename() returns "sourceFile.ext".

      try // If you want to avoid exception handling, then use the error code overload of the following functions.
      {
        fs::create_directories(targetParent); // Recursively create target directory if not existing.
        fs::copy_file(sourceFile, target, fs::copy_options::overwrite_existing);
      }
      catch (std::exception& e) // Not using fs::filesystem_error since std::bad_alloc can throw too.
      {
        cout << e.what();
      }
    }
  }
}

void viewEntries()
{
    char tmp[256];
    getcwd(tmp, 256);
    // cout << "Current working directory: " << tmp << endl;
    string path = tmp;
    string fileExtension = ".txt";
    //recursive_directory_iterator(path) using this statement we can traverse sub directories as well.
    for (auto &p : fs::directory_iterator(path))
    {
      // if body of a loop or if/else if/else condition has only 1 statement, curly braces can be skipped.
      if (p.path().extension() == fileExtension)
      {
        cout<<"\n\t\t\t\t\t\t";
        string fileName = p.path().stem().string();
        if(fileName == "contacts") continue;
        else cout << fileName << "\n";
      }

    }
    string entryName;
    cout<<"\n\t\t\t\t\tSelect the entry you want to view : ";
    cin>>entryName;
    entryName = entryName.append(".txt");

    fstream newfile;
    newfile.open(entryName,ios::in); //open a file to perform read operation using file object
   if (newfile.is_open())
   { //checking whether the file is open
      string tp;
      while(getline(newfile, tp))
      { //read data from file object and put it into string.
         cout <<"\n\t\t\t\t"<< tp << "\n"; //print the data of the string
      }

    }
    else
     {
       cout<<"\n\t\t\t\t\tFile does not exist.";
     }
}

void deleteEntry()
{
  char tmp[256];
  getcwd(tmp, 256);
  // cout << "Current working directory: " << tmp << endl;
  string path = tmp;
  string fileExtension = ".txt";

  for (auto &p : fs::directory_iterator(path))
  {
    if (p.path().extension() == fileExtension)
    {
      cout<<"\n\t\t\t\t\t\t";
      string fileNameCheck = p.path().stem().string();
      if(fileNameCheck == "contacts") continue;
      else cout << p.path().stem().string() << "\n";
    }
  }

  string entryName;
  cout<<"\n\t\t\t\t\tSelect the entry you want to delete : ";
  cin>>entryName;
  entryName = entryName.append(".txt");

  string mainDirectoryEntryName = path + "\\" + entryName;
  string favoritePath = path + "\\favorites";
  string favoritePathEntryName = favoritePath + "\\" + entryName;

  if(filesystem::remove(mainDirectoryEntryName) == 1)
  {
    cout << "\n\t\t\t\t\tEntry " << entryName << " deleted.";
  }
  else
  {
    cout << "\n\t\t\t\t\tEntry " << entryName << " not found.";
  }

  if(filesystem::remove(favoritePathEntryName) == 1)
  {
    cout << "\n\t\t\t\t\tEntry " << entryName << " deleted from favorites too.";
  }
  else
  {
    cout << "\n\t\t\t\t\tEntry " << entryName << " not found in favorites.";
  }

}

void viewFavoriteEntries()
{
  char tmp[256];
  getcwd(tmp, 256);
  // cout << "Current working directory: " << tmp << endl;
  string path = tmp;
  path = path + "\\favorites";
  string fileExtension = ".txt";
  for (auto &p : fs::recursive_directory_iterator(path))
  {
    if (p.path().extension() == fileExtension)
      cout<<"\n\t\t\t\t\t";
      std::cout << p.path().stem().string() << '\n';
  }

  string entryName;
  cout<<"\n\t\t\t\t\tSelect the entry you want to view : ";
  cin>>entryName;
  entryName = entryName.append(".txt");

  fstream newfile;
  newfile.open(entryName,ios::in); //open a file to perform read operation using file object
 if (newfile.is_open())
 { //checking whether the file is open
    string tp;
    while(getline(newfile, tp))
    { //read data from file object and put it into string.
       cout <<"\n\t\t\t\t\t"<< tp << "\n"; //print the data of the string
    }

  }
  else
  {
    cout<<"\n\t\t\t\t\tFile does not exist.";
  }
}

void addContacts()
{
  string contactFileName = "contacts.txt";

  string contactName;
  cout << "\n\t\t\t\t\tEnter contact name: ";
  cin >> contactName;

  long long contactNumber;
  cout << "\n\t\t\t\t\tEnter contact number: ";
  cin >> contactNumber;

  fstream contact;
  contact.open(contactFileName , ios::app);
  contact << contactName ;
  contact << " - ";
  contact << contactNumber ;
  contact << "\n";

}

void viewContacts()
{
  string contactFileName = "contacts.txt";
  int size = 100;
  char arr[size];
  ifstream fin(contactFileName , ios :: in);

  while(fin)
 {
    fin.getline(arr,size);
    cout<<"\n\n\t\t\t\t\t\t";
    cout<<arr;
    //cout<<"\n";
  }
}

void contactsPage()
{
  int choice;
  do
  {
  cout<<endl;
  cout << "\n\t\t\t\t\t\t\tMENU" <<endl;
  cout << "\n\n\t\t\t\t\t\t1] Add Contact" <<endl;
  cout << "\n\t\t\t\t\t\t2] View Contacts" <<endl;
  cout << "\n\t\t\t\t\t\t3] EXIT" << endl;
  cout << "\n\t\t\t\t\t\tEnter your choice: ";
  cin >> choice;
  system("cls");

  switch(choice)
  {
    case 1:
    addContacts();
    break;

    case 2:
    viewContacts();
    break;

    case 3:
    choice = 3;
    break;

    default:
    cout << "\n\t\t\t\t\tIncorrect option selected.";
  }
} while( choice != 3);
}

int main()
{
  if(flag == false)
  {
    login();
  }
  else
  {
    int choice;
    do
    {
    cout << endl;
    cout << "\n\n\n\t\t\t\t\t\t\tMENU" <<endl;
    cout << "\n\t\t\t\t\t\t1] CREATE an entry" <<endl;
    cout << "\n\t\t\t\t\t\t2] VIEW your entries" <<endl;
    cout << "\n\t\t\t\t\t\t3] DELETE an entry" <<endl;
    cout << "\n\t\t\t\t\t\t4] VIEW FAVOURITE entries" <<endl;
    cout << "\n\t\t\t\t\t\t5] Contacts Page" <<endl;
    cout << "\n\t\t\t\t\t\t6] EXIT APPLICATION" <<endl;
    cout << "\n\n\t\t\t\t\t\tENTER YOUR CHOICE: ";
    cin >> choice;
    system("cls");
    //cout << endl;

    switch (choice)
    {
      case 1:
      addEntry();
      break;

      case 2:
      viewEntries();
      break;

      case 3:
      deleteEntry();
      break;

      case 4:
      viewFavoriteEntries();
      break;

      case 5:
      contactsPage();
      break;

      case 6:
      choice = 6;
      break;

      default:
      cout << "\n\t\t\t\t\t\tInvalid Choice." << endl;
      break;
    }
  } while (choice != 6);
  }

  return 0;
}
