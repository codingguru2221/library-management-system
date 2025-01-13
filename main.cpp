#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cstdlib> // For rand() and system()
#include <ctime>   // For time()
#include <iomanip> // For std::quoted
#include <limits>  // Include this header for std::numeric_limits

using namespace std; // Add this line to use the standard namespace

#ifdef _WIN32
#include <windows.h> // For Windows-specific functions
#endif

void clearScreen() {
#ifdef _WIN32
    system("cls"); // Clear screen for Windows
#else
    system("clear"); // Clear screen for Unix/Linux
#endif
}

class Book {
public:
    string title;
    string author;
    string genre;
    bool isAvailable;

    Book(string t, string a, string g) : title(t), author(a), genre(g), isAvailable(true) {}
};

class Member {
public:
    string name;
    vector<Book*> borrowedBooks;

    Member(string n) : name(n) {}
    
    void borrowBook(Book* book) {
        if (book->isAvailable) {
            borrowedBooks.push_back(book);
            book->isAvailable = false;
            cout << name << " borrowed " << book->title << endl;
        } else {
            cout << book->title << " is not available." << endl;
        }
    }

    void returnBook(Book* book) {
        for (auto it = borrowedBooks.begin(); it != borrowedBooks.end(); ++it) {
            if (*it == book) {
                borrowedBooks.erase(it);
                book->isAvailable = true;
                cout << name << " returned " << book->title << endl;
                return;
            }
        }
        cout << name << " did not borrow " << book->title << endl;
    }
};

class User {
public:
    string username;
    string password;

    User(string u, string p) : username(u), password(p) {}
};

class Library {
public:
    vector<Book> books;
    vector<Member> members;
    vector<User> users; // Store registered users

    void addBook(const Book& book) {
        books.push_back(book);
    }

    void addMember(const Member& member) {
        members.push_back(member);
    }

    void registerUser(const string& username, const string& password) {
        users.emplace_back(username, password);
        ofstream outFile("users.txt", ios::app); // Open file in append mode
        if (outFile.is_open()) {
            outFile << username << " " << password << endl; // Save username and password
            outFile.close();
            cout << "User " << username << " registered successfully." << endl;
        } else {
            cout << "Error opening file for writing." << endl;
        }
    }

    bool loginUser(const string& username, const string& password) {
        ifstream inFile("users.txt"); // Open file for reading
        if (inFile.is_open()) {
            string u, p;
            while (inFile >> u >> p) {
                if (u == username && p == password) {
                    cout << "User " << username << " logged in successfully." << endl;
                    inFile.close();
                    return true;
                }
            }
            inFile.close();
        }
        cout << "Invalid username or password." << endl;
        return false;
    }

    void displayBooksByGenre(const string& genre) {
        cout << "Books in genre: " << genre << endl;
        for (const auto& book : books) {
            if (book.genre == genre) {
                cout << "- " << book.title << " by " << book.author << endl;
            }
        }
    }

    void searchBook(const string& title) {
        for (const auto& book : books) {
            if (book.title == title) {
                cout << "Book found: " << book.title << " by " << book.author << ", Genre: " << book.genre << ", Available: " << (book.isAvailable ? "Yes" : "No") << endl;
                return;
            }
        }
        cout << "Book not found." << endl;
    }

    void loadBooksFromFile(const string& filename) {
        ifstream inFile(filename);
        if (inFile.is_open()) {
            string title, author, genre;
            while (getline(inFile, title) && getline(inFile, author) && getline(inFile, genre)) {
                books.emplace_back(title, author, genre);
            }
            inFile.close();
            cout << "Loaded " << books.size() << " books from " << filename << endl;
        } else {
            cout << "Error opening file for reading." << endl;
        }
    }

    void generateSampleBooks(const string& filename, int count) {
        ofstream outFile(filename);
        if (outFile.is_open()) {
            for (int i = 1; i <= count; ++i) {
                outFile << quoted("Book " + to_string(i)) << " "
                        << quoted("Author " + to_string(rand() % 100)) << " "
                        << quoted("Genre " + to_string(rand() % 5)) << endl;
            }
            outFile.close();
            cout << "Generated " << count << " sample books in " << filename << endl;
        } else {
            cout << "Error opening file for writing." << endl;
        }
    }
};

int main() {
    srand(static_cast<unsigned int>(time(0))); // Seed for random number generation
    Library library;

    // Generate sample books and save to a file
    library.generateSampleBooks("books.txt", 10000);

    // Load books from the file
    library.loadBooksFromFile("books.txt");

    int choice;
    string username, password;
    bool loggedIn = false;

    while (true) {
        if (!loggedIn) {
            cout << "Welcome to the Library Management System" << endl;
            cout << "1. Login" << endl;
            cout << "2. Signup" << endl;
            cout << "3. Exit" << endl;
            cout << "Enter your choice: ";
            cin >> choice;

            if (cin.fail()) {
                cin.clear(); // Clear the error flag
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore invalid input
                cout << "Invalid input. Please enter a number." << endl; // New message for invalid input
                continue; // Restart the loop
            }

            switch (choice) {
                case 1: // Login
                    cout << "Enter username: ";
                    cin >> username;
                    if (username == "cls") {
                        clearScreen();
                        continue; // Restart the loop
                    }
                    cout << "Enter password: ";
                    cin >> password;
                    if (password == "cls") {
                        clearScreen();
                        continue; // Restart the loop
                    }
                    loggedIn = library.loginUser(username, password);
                    break;

                case 2: // Signup
                    cout << "Enter username: ";
                    cin >> username;
                    if (username == "cls") {
                        clearScreen();
                        continue; // Restart the loop
                    }
                    cout << "Enter password: ";
                    cin >> password;
                    if (password == "cls") {
                        clearScreen();
                        continue; // Restart the loop
                    }
                    library.registerUser(username, password);
                    break;

                case 3: // Exit
                    cout << "Exiting the system." << endl;
                    return 0;

                default:
                    cout << "Invalid choice. Please try again." << endl;
            }
        } else {
            cout << "1. Search for a Book" << endl;
            cout << "2. Logout" << endl;
            cout << "Enter your choice: ";
            cin >> choice;

            if (cin.fail()) {
                cin.clear(); // Clear the error flag
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore invalid input
                cout << "Invalid input. Please enter a number." << endl; // New message for invalid input
                continue; // Restart the loop
            }

            switch (choice) {
                case 1: // Search for a Book
                    cout << "Enter the book title you want to search for: ";
                    cin.ignore(); // Clear the input buffer
                    getline(cin, username); // Use username variable to store book title
                    if (username == "cls") {
                        clearScreen();
                        continue; // Restart the loop
                    }
                    library.searchBook(username);
                    break;

                case 2: // Logout
                    loggedIn = false;
                    cout << "You have logged out." << endl;
                    break;

                default:
                    cout << "Invalid choice. Please try again." << endl;
            }
        }
    }

    return 0;
}

