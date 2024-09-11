#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <map>
#include <stdexcept>
#include <numeric>

using namespace std;
namespace fs = filesystem;

template<typename T>
T GetInput(string inputMessage);
int GetChoice(string inputMessage, vector<string> options);
vector<string> MakeList(string strList);
string MakeString(vector<string> list);
vector<string> GetFiles(int folderIndex);
vector<string> OpenFile(int folderIndex, string fileName);
void SaveChanges(vector<string> credentials, int folderIndex, string fileName);
int Menu();
bool CreateAccount();
bool SignIn();
bool SignOut();
int AccountInfo();
void AccountBalance();
void DepositMoney();
void WithdrawMoney();
int MovieOptions();
void GetRoomID(int movieIndex);
void ViewBookings();
void AddBooking();
void RemoveBooking();
int StaffMovieOptions();
void AddMovie();
void RemoveMovie();
vector<string> MakeLine(vector<string> list, int start, int end);
void RoomAscii();
vector<string> MakeSeatQueue(int seatAmount);
void AddSeats(vector<string> seatQueue);
void RemoveSeats(vector<string> seatQueue);
bool Finish();

string username, fName, lName, eMail, password, movieTitle;
float money, price;
bool staff;
int roomID, seatTotal;
vector<string> userMovies, bookedSeats;
map <int, vector<string>> userSeats = { { 1, {}}, { 2, {}}, { 3, {}} };
vector<string> folderPaths = { "Account\\", "Room\\", "Movie\\" };
vector<string> accountList = GetFiles(0);
vector<string> roomList = GetFiles(1);
vector<string> movieList = GetFiles(2);

int main()
{
    bool verified = false;
    int menuChoice = Menu();
    if (menuChoice == 1)
    {
        verified = CreateAccount();
    }
    else
    {
        verified = SignIn();
    }
    while (verified == true)
    {
        int accountChoice = AccountInfo();
        if (staff == false)
        {
            if (accountChoice == 1)
            {
                AccountBalance();
            }
            else if (accountChoice == 2)
            {
                DepositMoney();
            }
            else if (accountChoice == 3)
            {
                WithdrawMoney();
            }
            else if (accountChoice == 4)
            {
                bool exit = false;
                while (exit == false)
                {
                    int movieMenuChoice = MovieOptions();
                    if (movieMenuChoice == 1)
                    {
                        ViewBookings();
                    }
                    else if (movieMenuChoice == 2)
                    {
                        AddBooking();
                    }
                    else if (movieMenuChoice == 3)
                    {
                        RemoveBooking();
                    }
                    else if (movieMenuChoice == 4)
                    {
                        exit = true;
                    }
                }
            }
            else if (accountChoice == 5)
            {
                bool signOut = SignOut();
                if (signOut == true)
                {
                    verified = false;
                }
            }
        }
        else if (staff == true)
        {
            if (accountChoice == 1)
            {
                bool exit = false;
                while (exit == false)
                {
                    bool staffMenuChoice = StaffMovieOptions();
                    if (staffMenuChoice == 1)
                    {
                        AddMovie();
                    }
                    else if (staffMenuChoice == 2)
                    {
                        RemoveMovie();
                    }
                    else if (staffMenuChoice == 3)
                    {
                        exit = true;
                    }
                }
            }
            else
            {
                bool signOut = SignOut();
                if (signOut == true)
                {
                    verified = false;
                }
            }
        }
    }
}
template<typename T>
T GetInput(string inputMessage)
{
    T userInput;
    while (true)
    {
        cout << inputMessage << endl;
        string rawInput;
        getline(cin, rawInput);
        try
        {
            istringstream iss(rawInput);
            iss >> userInput;
            if (iss.fail() || iss.peek() != EOF)
            {
                throw new exception;
            }
            break;
        }
        catch (bad_cast e)
        {
            cerr << "This input is of the wrong data type" << endl;
        }
    }
    return userInput;
}
int GetChoice(string inputMessage, vector<string> options)
{
    int userInput = 0;
    while (true)
    {
        for (int x = 0; x < options.size(); x++)
        {
            cout << endl << x + 1 << " ---> " << options[x] << endl;
        }
        cout << inputMessage << endl;
        string rawInput;
        getline(cin, rawInput);
        try
        {
            userInput = stoi(rawInput);
            if (userInput < 1)
            {
                throw out_of_range("Your input cannot be less than 1");
            }
            else if (userInput > options.size())
            {
                throw out_of_range("Your input cannot be greater than the options available");
            }
            break;
        }
        catch (const out_of_range& e)
        {
            cerr << e.what() << endl;
        }
        catch (invalid_argument)
        {
            cerr << "This input is of the wrong data type" << endl;
        }
    }
    return userInput;
}
vector<string> MakeList(string strList)
{
    vector<char> stocl(strList.begin(), strList.end());
    vector<string> list;
    istringstream iss(strList);
    string word;
    while (iss >> word)
    {
        for (char c : word)
        {
            if (c == ',')
            {
                c = NULL;
            }
        }
        list.push_back(word);
    }
    return list;
}
string MakeString(vector<string> list)
{
    ostringstream oss;
    for (const string& item : list)
    {
        if (&item != &list.front())
        {
            oss << " ";
        }
        oss << item;
    }
    string strList = oss.str();
    return strList;
}
vector<string> GetFiles(int folderIndex)
{
    vector<string> filesList;
    for (const auto& file : fs::directory_iterator(folderPaths[folderIndex]))
    {
        string newFile;
        string tempFile = file.path().filename().string();
        newFile = tempFile.erase(tempFile.length() - 4, 4);
        filesList.push_back(newFile);
    }
    return filesList;
}
vector<string> OpenFile(int folderIndex, string fileName)
{
    ifstream linesList(folderPaths[folderIndex] + fileName + ".txt");
    vector<string>lines;
    string line;
    while (getline(linesList, line))
    {
        lines.push_back(line);
    }
    linesList.close();
    if (folderIndex == 0)
    {
        fName = lines[0];
        lName = lines[1];
        eMail = lines[2];
        password = lines[3];
        money = stof(lines[4]);
        istringstream iss(lines[5]);
        iss >> staff;
        userMovies = MakeList(lines[6]);
        userSeats[1] = MakeList(lines[7]);
        userSeats[2] = MakeList(lines[8]);
        userSeats[3] = MakeList(lines[9]);
    }
    else if (folderIndex == 1)
    {
        movieTitle = lines[0];
        bookedSeats = MakeList(lines[1]);
        seatTotal = stoi(lines[2]);
    }
    else if (folderIndex == 2)
    {
        movieTitle = lines[0];
        price = stof(lines[1]);
        roomID = stoi(lines[2]);
    }
    return lines;
}
void SaveChanges(vector<string> credentials, int folderIndex, string fileName)
{
    ofstream outFile(folderPaths[folderIndex] + username + ".txt");
    for (string cred : credentials)
    {
        outFile << cred << endl;
    }
    outFile.close();
}
int Menu()
{
    vector<string> options = { "Create Account", "Sign-In" };
    int choice = GetChoice("What would you like to do?", options);
    return choice;
}
bool CreateAccount()
{
    bool verified = false;
    bool x = true;
    while (x == true)
    {
        string usernameTemp = GetInput<string>("Please enter a username");
        if (accountList.size() == 0)
        {
            username = usernameTemp;
            x = false;
        }
        else
        {
            for(string account : accountList)
            {
                if (account != usernameTemp)
                {
                    username = usernameTemp;
                    x = false;
                }
                else
                {
                    cout << "This username already exists" << endl;
                    x = true;
                    break;
                }
            }
        }
    }
    fName = GetInput<string>("What is your first name?");
    lName = GetInput<string>("What is your last name?");
    while (true)
    {
        bool isAt = false;
        bool isDot = false;
        eMail = GetInput<string>("Please enter your E-mail");
        for (char c : eMail)
        {
            if (c == '@')
            {
                isAt = true;
            }
            else if (c == '.')
            {
                isDot = true;
            }
        }
        if (isAt == true && isDot == true)
        {
            break;
        }
        else
        {
            cout << "E-Mail does not have a valid domain" << endl;
        }
    }
    while (true)
    {
        int digits = 0;
        int special = 0;
        int capital = 0;
        cout << "Password must contain:" << endl <<  "- 12 Characters" << endl << "- At least 2 special charcaters(e.g. @, %, *, <)" << endl << "- At least 2 numbers" << endl << "- A capital Letter" << endl;
        password = GetInput<string>("Please enter new password");
        for(char c : password)
        {
            if (c < 58 && c > 47)
            {
                digits++;
            }
            if (isalnum(c) == false)
            {
                special++;
            }
            if (isupper(c) == true)
            {
                capital++;
            }
        }
        if (special >= 2 && digits >= 2 && password.length() >= 12 && capital >= 1)
        {
            break;
        }
        else
        {
            cout << "Password is invalid, Please try again" << endl;
        }
    }
    money = 0.00;
    staff = false;
    userMovies = {};
    userSeats[1] = {};
    userSeats[2] = {};
    userSeats[3]= {};
    vector<string> credentials = { fName, lName, eMail, password, to_string(money), to_string(staff), MakeString(userMovies), MakeString(userSeats[1]), MakeString(userSeats[2]), MakeString(userSeats[3]) };
    SaveChanges(credentials, 0, username);
    verified = true;
    return verified;
}
bool SignIn()
{
    bool verified;
    bool x = true;
    while (x == true)
    {
        string tempUsername = GetInput<string>("Please enter username:");
        for(string file : accountList)
        {
            if (tempUsername == file)
            {
                x = false;
                username = tempUsername;
                OpenFile(0, username);
                break;
            }
        }
        if (x == true)
        {
            cout << "The account could not be found, please try again." << endl;
        }
    }
    while (true)
    {
        string passwordTemp = GetInput<string>("Please enter your password");
        if (passwordTemp == password)
        {
            break;
        }
        else
        {
            cout << "Your password wrong, please try again" << endl;
        }
    }
    verified = true;
    return verified;
}
bool SignOut()
{
    bool signOut = false;
    vector<string> options = { "Yes", "No" };
    int choice = GetChoice("Are you sure you want to exit?", options);
    if (choice == 1)
    {
        signOut = true;
        vector<string> credentials = { fName, lName, eMail, password, to_string(money), to_string(staff), MakeString(userMovies), MakeString(userSeats[1]), MakeString(userSeats[2]), MakeString(userSeats[3]) };
        SaveChanges(credentials, 0, username);
    }
    return signOut;
}
int AccountInfo()
{
    vector<string> options;
    cout << "Welcome, " << fName << endl;
    if (staff == false)
    {
        options = { "View Balance", "Deposit Money", "Withdraw Money", "Movies", "Sign-Out" };
    }
    else
    {
        options = { "Movies", "Sign-Out" };
    }
    int choice = GetChoice("What would you like to do?", options);
    return choice;
}
void AccountBalance()
{
    bool x = true;
    while (x == true)
    {
        cout << "Your account balance is: £" << money << endl;
        x = Finish();
    }
}
void DepositMoney()
{
    bool x = true;
    while (x == true)
    {
        float moneyAdd = GetInput<float>("How much money would you like to add?");
        money += moneyAdd;
        x = Finish();
    }
}
void WithdrawMoney()
{
    bool x = true;
    while (x == true)
    {
        while (true)
        {
            float moneyRemove = GetInput<float>("How much money would you like to remove?");
            if (moneyRemove > money)
            {
                cout << "You cannot withdraw that amount" << endl;
            }
            else
            {
                money -= moneyRemove;
                break;
            }
        }
        x = Finish();
    }
}
int MovieOptions()
{
    vector<string> options = { "View Bookings", "Make New Booking", "Remove Booking", "Back to Home" };
    int choice = GetChoice("What would you like to do?", options);
    return choice;
}
void GetRoomID(int movieIndex)
{
    movieTitle = movieList[movieIndex - 1];
    OpenFile(2, movieTitle);
    OpenFile(1, to_string(roomID));
}
void ViewBookings()
{
    int roomChoice = GetChoice("What booking would you like to view?", movieList);
    GetRoomID(roomChoice);
    RoomAscii();
}
void AddBooking()
{
    bool x = true;
    while (x == true)
    {
        bool bookSeat = true;
        bool movieExist = false;
        int choice = GetChoice("What booking would you like to add?", movieList);
        GetRoomID(choice);
        for(string film : userMovies)
        {
            if (movieTitle == film)
            {
                movieExist = true;
                cout << "You have already booked this movie, " << movieTitle << ", before..." << endl;
                vector<string> bookAgainConfirm = { "Yes", "No" };
                int bookAgain = GetChoice("Would you like to book more seats?", bookAgainConfirm);
                if (bookAgain == 2)
                {
                    bookSeat = false;
                }
                break;
            }
            else
            {
                movieExist = false;
            }
        }
        if (movieExist == false)
        {
            userMovies.push_back(movieTitle);
        }
        if (bookSeat == true)
        {
            int seatAmount = 0;
            OpenFile(1, to_string(roomID));
            while (true)
            {
                RoomAscii();
                seatAmount = GetInput<int>("Input the number of seats you would like to add");
                if (seatAmount > 100 - seatTotal)
                {
                    cout << "That is too many seats" << endl;
                }
                else
                {
                    double totalCost = seatAmount * price;
                    if (totalCost > money)
                    {
                        cout << "You do not have enough money for this" << endl;
                    }
                    else
                    {
                        money -= totalCost;
                        break;
                    }
                }
            }
            vector<string> seatsqueue = MakeSeatQueue(seatAmount);
            AddSeats(seatsqueue);
        }
        x = Finish();
    }
}
void RemoveBooking()
{
    bool x = true;
    while (x == true)
    {
        bool removeSeat = true;
        int choice = GetChoice("What booking would you like to remove?", movieList);
        GetRoomID(choice);
        for(string film : userMovies)
        {
            if (movieTitle == film)
            {
                vector<string> removeConfirm = { "Yes", "No" };
                int bookAgain = GetChoice("Are you sure you wish to remove bookings for " + movieTitle, removeConfirm);
                if (bookAgain == 2)
                {
                    removeSeat = false;
                }
                break;
            }
        }
        if (removeSeat == true)
        {
            vector<string> removeScaleIndex = { "Whole Movie", "Specific Seats" };
            int removeScale = GetChoice("What would you like to remove?", removeScaleIndex);
            if (removeScale == 1)
            {
                RemoveSeats(userSeats[roomID]);
                for (int i = 0; i < userMovies.size(); i++)
                {
                    if (userMovies[i] == movieTitle)
                    {
                        userMovies.erase(userMovies.begin() + i);
                    }
                }
            }
            else
            {
                int seatAmount = 0;
                OpenFile(1, to_string(roomID));
                while (true)
                {
                    RoomAscii();
                    seatAmount = GetInput<int>("Input the number of seats you would like to remove");
                    if (seatAmount > userSeats[roomID].size())
                    {
                        cout << "That is too many seats" << endl;
                    }
                    else
                    {
                        seatTotal -= seatAmount;
                        double totalCost = seatAmount * price;
                        money += totalCost;
                        break;
                    }
                }
                vector<string> seatsQueue = MakeSeatQueue(seatAmount);
                RemoveSeats(seatsQueue);
            }
        }
        x = Finish();
    }
}
int StaffMovieOptions()
{
    vector<string> staffOptions = { "Add Movie", "Remove Movie", "Back to Home" };
    int staffChoice = GetChoice("What would you like to do?", staffOptions);
    return staffChoice;
}
void AddMovie()
{
    bool x = true;
    while (x == true)
    {
        bool exist = true;
        bool roomTaken = true;
        string tempMovieTitle = "";
        while (exist == true)
        {
            tempMovieTitle = GetInput<string>("What is the name of the movie?");
            for(string movie : movieList)
            {
                if (movie != tempMovieTitle)
                {
                    exist = false;
                }
                else
                {
                    exist = true;
                    break;
                }
            }
            if (movieList.size() == 0)
            {
                exist = false;
            }
            if (exist == true)
            {
                cout << "This movie " << tempMovieTitle << " already exists" << endl;
            }
        }
        double tempPrice = GetInput<float>("How much will a seat cost?");
        int roomTemp = 0;
        while (roomTaken == true)
        {
            while (true)
            {
                roomTemp = GetInput<int>("What room would you like?");
                if (roomTemp > roomList.size() || roomTemp <= 0)
                {
                    cout << "this room does not exist" << endl;
                }
                else
                {
                    break;
                }
            }
            for(string movie : movieList)
            {
                OpenFile(2, movie);
                if (roomTemp == roomID)
                {
                    cout << "The room " << roomTemp << " is already taken" << endl;
                    roomTaken = true;
                    break;
                }
                else
                {
                    roomTaken = false;
                }
            }
            if (movieList.size() == 0)
            {
                roomTaken = false;
            }
        }
        roomID = roomTemp;
        movieTitle = tempMovieTitle;
        price = tempPrice;
        vector<string> tempBookedSeats;
        int tempSeatTotal = 0;
        vector<string> roomInfo = { movieTitle, MakeString(tempBookedSeats), to_string(tempSeatTotal) };
        vector<string> movieInfo = { movieTitle, to_string(price), to_string(roomID) };
        SaveChanges(movieInfo, 2, movieTitle);
        SaveChanges(roomInfo, 1, to_string(roomID));
        x = Finish();
    }
}
void RemoveMovie()
{
    bool x = true;
    while (x == true)
    {
        int choice = GetChoice("What movie would you like to remove?", movieList);
        roomID = choice;
        for (int y = 0; y < movieList.size(); y++)
        {
            if (choice - 1 == y)
            {
                fs::remove(folderPaths[2] + movieList[y] + ".txt");
                OpenFile(1, to_string(roomID));
                movieTitle = "";
                for(string account : accountList)
                {
                    OpenFile(0, account);
                    bookedSeats = {};
                    userSeats[roomID] = {};
                    vector<string> accountCreds = { fName, lName, eMail, password, to_string(money), to_string(staff), MakeString(userMovies), MakeString(userSeats[1]), MakeString(userSeats[2]), MakeString(userSeats[3]) };
                    SaveChanges(accountCreds, 0, account);
                }
                seatTotal = 0;
                cout << "This movie " << movieTitle << " has been removed" << endl;
                break;
            }
        }
        vector<string> roomInfo = { movieTitle, MakeString(bookedSeats), to_string(seatTotal) };
        SaveChanges(roomInfo, 1, to_string(roomID));
        x = Finish();
    }
}
vector<string> MakeLine(vector<string> list, int start, int end)
{
    vector<string> line;
    for (int x = start; x < end; x++)
    {
        line.push_back(list[x]);
    }
    return line;
}
void RoomAscii()
{
    vector<string> seats;
    vector<string> asciiSeats;
    for (char x = 'A'; x < 'K'; x++)
    {
        for (int y = 1; y <= 10; y++)
        {
            string coord = to_string(x) + to_string(y);
            seats.push_back(coord);
        }
    }
    for (string seat : seats)
    {
        asciiSeats.push_back("[" + seat + "]");
    }
    vector<vector<string>> lines;
    for (int x = 0; x < seats.size(); x++)
    {
        bool isBookedSeat = false;
        bool isUserSeat = false;
        for (string bookedSeat : bookedSeats)
        {
            if (bookedSeat == seats[x])
            {
                isBookedSeat == true;
            }
        }
        for (string userSeat : userSeats[roomID])
        {
            if (userSeat == seats[x])
            {
                isUserSeat == true;
            }
        }
        if (isBookedSeat == true && isUserSeat == false)
        {
            asciiSeats[x] = "[=" + seats[x] + "=]";
        }
        else if (isBookedSeat == true && isUserSeat == true)
        {
            asciiSeats[x] = "[#" + seats[x] + "#]";
        }
        for (int value = 0; value < 100; value += 10)
        {
            lines.push_back(MakeLine(asciiSeats, value, value + 10));
        }
    }
    cout << "Your bookings - '#'" << endl;
    cout << "Other bookings - '='" << endl;
    cout << "------------------------ " << movieTitle << " -------------------------" << endl;
    for(vector<string> line : lines)
    {
        string newLine = MakeString(line);
        cout << newLine << endl;
    }
}
vector<string> MakeSeatQueue(int seatAmount)
{
    vector<string> seatQueue;
    for (int i = 0; i < seatAmount; i++)
    {
        while (true)
        {
            string seatCoord = GetInput<string>("Input the coordinate of the seat");
            int x = 0;
            for (char c : seatCoord)
            {
                if (islower(c) == true)
                {
                    toupper(c);
                }
                else if (isspace(c) == true || c == ',')
                {
                    seatCoord.erase(seatCoord.begin() + x);
                }
                x++;
            }
            bool isBookedSeat = false;
            bool isUserSeat = false;
            bool isSeatQueue = false;
            for (string bookedSeat : bookedSeats)
            {
                if (bookedSeat == seatCoord)
                {
                    isBookedSeat == true;
                }
            }
            for (string userSeat : userSeats[roomID])
            {
                if (userSeat == seatCoord)
                {
                    isUserSeat == true;
                }
            }
            for (string seat : seatQueue)
            {
                if (seat == seatCoord)
                {
                    isSeatQueue == true;
                }
            }
            if (isBookedSeat == true)
            {
                cout << "This seat has already been booked" << endl;
            }
            else if (isUserSeat == true || isSeatQueue == true)
            {
                cout << "You have already booked this seat" << endl;
            }
            else
            {
                seatQueue.push_back(seatCoord);
                break;
            }
        }
    }
    return seatQueue;
}
void AddSeats(vector<string> seatQueue)
{
    for (string seat : seatQueue)
    {
        userSeats[roomID].push_back(seat);
        bookedSeats.push_back(seat);
    }
    vector<string> roomCreds = { movieTitle, MakeString(bookedSeats), to_string(roomID) };
    SaveChanges(roomCreds, 1, to_string(roomID));
}
void RemoveSeats(vector<string> seatQueue)
{
    int x = 0;
    for (string userSeat : userSeats[roomID])
    {
        for (string seat : seatQueue)
        {
            if (seat == userSeat)
            {
                userSeats[roomID].erase(userSeats[roomID].begin() + x);
            }
        }
        x++;
    }
    x = 0;
    for (string bookedSeat : bookedSeats)
    {
        for (string seat : seatQueue)
        {
            if (seat == bookedSeat)
            {
                bookedSeats.erase(bookedSeats.begin() + x);
            }
        }
        x++;
    }
    vector<string> roomCreds = { movieTitle, MakeString(bookedSeats), to_string(roomID) };
    SaveChanges(roomCreds, 1, to_string(roomID));
}
bool Finish()
{
    bool x = true;
    vector<string> options = { "Yes", "No" };
    int choice = GetChoice("Are you finished in this section?", options);
    if (choice == 1)
    {
        x = false;
    }
    return x;
}