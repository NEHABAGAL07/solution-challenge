#define _WIN32_WINNT 0x0601
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <strings.h>
#include <sstream>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <vector>
#include <algorithm>
#include <regex>
#include <ctime>

#pragma comment(lib, "ws2_32.lib")

#define PORT 8080
#define BUFFER_SIZE 1024

std::vector<SOCKET> clients;
CRITICAL_SECTION clientsLock;

struct User
{
    int ID;
    std::string First_Name;
    std::string Middle_Name;
    std::string Last_Name;
    std::string DOB;
    int Age;
    char Gender;
    std::string Phone_Number;
    std::string Password;
};

bool isValidName(const std::string &name)
{
    if (name.empty() || name.length() > 50)
        return false;
    return std::all_of(name.begin(), name.end(), [](char c)
                       { return isalpha(c) || c == ' '; });
}

bool isValidDOB(const std::string &dob)
{
    std::regex dobPattern(R"(\d{4}-\d{2}-\d{2})"); // YYYY-MM-DD format
    if (!std::regex_match(dob, dobPattern))
        return false;

    int year, month, day;
    sscanf(dob.c_str(), "%d-%d-%d", &year, &month, &day);

    time_t t = time(nullptr);
    tm *now = localtime(&t);
    int currentYear = now->tm_year + 1900;

    if (year < 1900 || year > currentYear)
        return false; // Year validation
    if (month < 1 || month > 12)
        return false; // Month validation
    if (day < 1 || day > 31)
        return false; // Day validation

    return true;
}

bool isValidAge(int age)
{
    return (age >= 1 && age <= 120);
}

bool isValidGender(char gender)
{
    return (gender == 'M' || gender == 'F' || gender == 'O');
}

bool isValidPhoneNumber(const std::string &phone)
{
    if (phone.length() < 10 || phone.length() > 15)
        return false;
    return std::all_of(phone.begin(), phone.end(), ::isdigit);
}

bool isValidPassword(const std::string &password)
{
    if (password.length() < 8)
        return false;

    std::regex uppercase(R"([A-Z])");
    std::regex lowercase(R"([a-z])");
    std::regex digit(R"(\d)");
    std::regex specialChar(R"([!@#$%^&*])");

    return std::regex_search(password, uppercase) &&
           std::regex_search(password, lowercase) &&
           std::regex_search(password, digit) &&
           std::regex_search(password, specialChar);
}

DWORD WINAPI handleClient(LPVOID clientSocket)
{
    SOCKET client = *(SOCKET *)clientSocket;
    char buffer[BUFFER_SIZE];
    while (true)
    {
        memset(buffer, 0, BUFFER_SIZE);
        int bytesReceived = recv(client, buffer, BUFFER_SIZE, 0);
        if (bytesReceived <= 0)
        {
            std::cout << "Client disconnected." << std::endl;
            closesocket(client);
            EnterCriticalSection(&clientsLock);
            clients.erase(std::remove(clients.begin(), clients.end(), client), clients.end());
            LeaveCriticalSection(&clientsLock);
            break;
        }
        std::cout << "Client: " << buffer << std::endl; // Temperory

        char response[BUFFER_SIZE];

        std::stringstream ss(buffer);
        std::string segment;
        User user;

        // Ignore the first token (process identifier)
        getline(ss, segment, ';');

        if (segment == "0")
        {
            // add user

            // Extract and assign data
            getline(ss, user.First_Name, ';');
            getline(ss, user.Middle_Name, ';');
            getline(ss, user.Last_Name, ';');
            getline(ss, user.DOB, ';');

            getline(ss, segment, ';');
            user.Age = stoi(segment);

            getline(ss, segment, ';');
            user.Gender = segment[0];

            getline(ss, user.Phone_Number, ';');
            getline(ss, user.Password, ';');

            // Validate the Data
            if (!isValidName(user.First_Name))
            {
                strcpy(response, "Invalid_First_Name");
            }
            else if (!isValidDOB(user.DOB))
            {
                strcpy(response, "Invalid_DOB");
            }
            else if (!isValidAge(user.Age))
            {
                strcpy(response, "Invalid_Age");
            }
            else if (!isValidGender(user.Gender))
            {
                strcpy(response, "Invalid_Gender");
            }
            else if (!isValidPhoneNumber(user.Phone_Number))
            {
                strcpy(response, "Invalid_Phone_Number");
            }
            else if (!isValidPassword(user.Password))
            {
                strcpy(response, "Weak_Password");
            }

            // Assign number or ID
            std::fstream file1("users.txt");
            std::string line, lastLine;

            while (getline(file1, line))
            {
                lastLine = line;
            }

            if (lastLine.empty())
                user.ID = 1;

            std::stringstream ss(lastLine);
            std::string idStr;
            getline(ss, idStr, ',');
            user.ID = stoi(idStr) + 1;
            file1.close();

            // Display extracted data (temp)
            std::cout << "User Data Extracted:\n";
            std::cout << "ID : " << user.ID << std::endl;
            std::cout << "First Name: " << user.First_Name << std::endl;
            std::cout << "Middle Name: " << user.Middle_Name << std::endl;
            std::cout << "Last Name: " << user.Last_Name << std::endl;
            std::cout << "DOB: " << user.DOB << std::endl;
            std::cout << "Age: " << user.Age << std::endl;
            std::cout << "Gender: " << user.Gender << std::endl;
            std::cout << "Phone: " << user.Phone_Number << std::endl;
            std::cout << "Password: " << user.Password << std::endl;

            // Store the user in a file
            std::ofstream file("users.txt", std::ios::app);
            if (file.is_open())
            {
                file << std::to_string(user.ID) << "," << user.First_Name << "," << user.Middle_Name << "," << user.Last_Name << "," << user.DOB << "," << user.Age << "," << user.Gender << "," << user.Phone_Number << "," << user.Password << ",\n";
                file.close();
                strcpy(response, "User_Added");
            }
            else
            {
                strcpy(response, "Failed_To_Open_File");
            }
            send(client, response, BUFFER_SIZE, 0);
        }

        // }
        //     case 1:
        //         // verify user
        //             // extract data
        //             // seach user
        //             // get data
        //             // create respone
        //             // send data
        //         break;

        //     case 2:
        //         // get user health data
        //             // extract Data
        //             // Search User ID
        //             // Check if Data Already Exist
        //             // Modify if true else create a new record
        //         break;

        //     case 3:
        //         // send user health data
        //             // Extract data
        //             // Search User ID
        //             // Get data
        //             // create Respone
        //             // send Data
        //             send(client, response, BUFFER_SIZE, 0);
        //         break;

        //     default:
        //         std::cout<<"Invalid Signal\n";
        //         break;
        //     }

        LeaveCriticalSection(&clientsLock);
    }
    return 0;
}

void startServer()
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        std::cerr << "WSAStartup failed" << std::endl;
        return;
    }

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET)
    {
        std::cerr << "Socket creation failed" << std::endl;
        WSACleanup();
        return;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    if (bind(serverSocket, (sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
    {
        std::cerr << "Bind failed" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return;
    }

    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR)
    {
        std::cerr << "Listen failed" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return;
    }

    InitializeCriticalSection(&clientsLock);
    std::cout << "Server is listening on port " << PORT << "..." << std::endl;

    while (true)
    {
        sockaddr_in clientAddr;
        int clientSize = sizeof(clientAddr);
        SOCKET clientSocket = accept(serverSocket, (sockaddr *)&clientAddr, &clientSize);
        if (clientSocket == INVALID_SOCKET)
        {
            std::cerr << "Failed to accept client" << std::endl;
            continue;
        }

        EnterCriticalSection(&clientsLock);
        clients.push_back(clientSocket);
        LeaveCriticalSection(&clientsLock);

        std::cout << "New client connected!" << std::endl;
        HANDLE clientThread = CreateThread(NULL, 0, handleClient, &clientSocket, 0, NULL);
        if (clientThread == NULL)
        {
            std::cerr << "Failed to create client thread." << std::endl;
            closesocket(clientSocket);
        }
    }

    closesocket(serverSocket);
    WSACleanup();
}

void addUser()
{
}

int main()
{
    startServer();
    return 0;
}
