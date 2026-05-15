#include<iostream>
#include<fstream>
#include<string>
#include<cstdlib>
#include<windows.h>

using namespace std;

// CSV FILE
string filename = "database.csv.txt";

// LOGIN FUNCTION
bool login()
{
    string username, password;

    cout << "========== LOGIN SYSTEM ==========" << endl;

    cout << "Username: ";
    cin >> username;

    cout << "Password: ";
    cin >> password;

    if(username == "admin" && password == "1234")
    {
        cout << "\nLogin Successful!\n";
        return true;
    }
    else
    {
        cout << "\nInvalid Login!\n";
        return false;
    }
}

// CREATE DATABASE IF FILE NOT FOUND
void initializeDatabase()
{
    ifstream file(filename.c_str());

    if(!file)
    {
        ofstream newFile(filename.c_str());

        newFile << "ID,Vehicle,Owner,Status\n";

        newFile.close();
    }

    file.close();
}

// CHECK ESP32 CONNECTION
bool isESP32Connected()
{
    HANDLE hSerial;

    hSerial = CreateFileA(
        "COM4",
        GENERIC_READ | GENERIC_WRITE,
        0,
        0,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        0
    );

    if(hSerial == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    CloseHandle(hSerial);

    return true;
}

// DISPLAY ALL RECORDS
void displayRecords()
{
    ifstream file(filename.c_str());

    string line;

    cout << "\n========== ALL RECORDS ==========\n";

    while(getline(file, line))
    {
        cout << line << endl;
    }

    file.close();
}

// SEARCH RECORD
void searchRecord()
{
    // CHECK ESP32 CONNECTION
    if(!isESP32Connected())
    {
        cout << "\nESP32 Not Connected on COM4!\n";
        return;
    }

    string id;

    cout << "\nEnter ID To Search: ";
    cin >> id;

    ifstream file(filename.c_str());

    string line;

    bool found = false;

    while(getline(file, line))
    {
        if(line.find(id) != string::npos)
        {
            cout << "\n========== RECORD FOUND ==========\n";

            cout << line << endl;

            found = true;

            break;
        }
    }

    if(!found)
    {
        cout << "\nRecord Not Found!\n";
    }

    file.close();
}

// ADD RECORD
void addRecord()
{
    string id, vehicle, owner, status;

    cout << "\nEnter ID: ";
    cin >> id;

    cout << "Enter Vehicle: ";
    cin >> vehicle;

    cout << "Enter Owner: ";
    cin >> owner;

    cout << "Enter Status (IN/OUT): ";
    cin >> status;

    ofstream file(filename.c_str(), ios::app);

    file << id << ","
         << vehicle << ","
         << owner << ","
         << status << endl;

    file.close();

    cout << "\nRecord Added Successfully!\n";
}

// UPDATE RECORD
void updateRecord()
{
    ifstream file(filename.c_str());
    ofstream temp("temp.csv");

    string id;
    string vehicle, owner, status;
    string line;

    bool found = false;

    cout << "\nEnter ID To Update: ";
    cin >> id;

    cout << "Enter New Vehicle: ";
    cin >> vehicle;

    cout << "Enter New Owner: ";
    cin >> owner;

    cout << "Enter New Status: ";
    cin >> status;

    while(getline(file, line))
    {
        if(line.find(id) != string::npos)
        {
            temp << id << ","
                 << vehicle << ","
                 << owner << ","
                 << status << endl;

            found = true;
        }
        else
        {
            temp << line << endl;
        }
    }

    file.close();
    temp.close();

    remove(filename.c_str());
    rename("temp.csv", filename.c_str());

    if(found)
    {
        cout << "\nRecord Updated Successfully!\n";
    }
    else
    {
        cout << "\nRecord Not Found!\n";
    }
}

// DELETE RECORD
void deleteRecord()
{
    ifstream file(filename.c_str());
    ofstream temp("temp.csv");

    string id;
    string line;

    bool found = false;

    cout << "\nEnter ID To Delete: ";
    cin >> id;

    while(getline(file, line))
    {
        if(line.find(id) != string::npos)
        {
            found = true;
        }
        else
        {
            temp << line << endl;
        }
    }

    file.close();
    temp.close();

    remove(filename.c_str());
    rename("temp.csv", filename.c_str());

    if(found)
    {
        cout << "\nRecord Deleted Successfully!\n";
    }
    else
    {
        cout << "\nRecord Not Found!\n";
    }
}

// MAIN FUNCTION
int main()
{
    int choice;

    // LOGIN
    if(!login())
    {
        return 0;
    }

    // CHECK DATABASE
    initializeDatabase();

    do
    {
        cout << "\n\n========== SMART PARKING SYSTEM ==========\n";

        cout << "1. Add Record\n";
        cout << "2. Display All Records\n";
        cout << "3. Search Record\n";
        cout << "4. Update Record\n";
        cout << "5. Delete Record\n";
        cout << "6. Exit\n";

        cout << "\nEnter Choice: ";
        cin >> choice;

        switch(choice)
        {
            case 1:
                addRecord();
                break;

            case 2:
                displayRecords();
                break;

            case 3:
                searchRecord();
                break;

            case 4:
                updateRecord();
                break;

            case 5:
                deleteRecord();
                break;

            case 6:
                cout << "\nProgram Closed Successfully!\n";
                break;

            default:
                cout << "\nInvalid Choice!\n";
        }

    } while(choice != 6);

    return 0;
}
