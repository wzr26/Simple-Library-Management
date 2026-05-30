#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
using namespace std;

// BASE CLASS
class Book {
protected:
    int id;
    string title;
    string author;
    int quantity;
    int maxQuantity;
public:
    Book(int i, string t, string a, int q) {
        id = i;
        title = t;
        author = a;
        quantity = q;
        maxQuantity = q;
    }

    virtual void displayBook() {
        cout << "ID: " << id
             << ", Title: " << title
             << ", Author: " << author
             << ", Quantity: " << quantity;
    }

    int getId() {
        return id;
    }

    string getTitle() {
        return title;
    }

    string getAuthor() {
        return author;
    }

    int getQuantity() {
        return quantity;
    }

    void setTitle(string t) {
        title = t;
    }

    void setAuthor(string a) {
        author = a;
    }

    void setQuantity(int q) {
        quantity = q;
    }

    virtual void issueBook() {
        if (quantity > 0) {
            quantity--;
            cout << "Book issued successfully!\n";
        } else {
            cout << "Book is out of stock!\n";
        }
    }

    virtual void returnBook() {

    if (quantity < maxQuantity) {
        quantity++;
        cout << "Book returned successfully!\n";
    }
    else {
        cout << "Cannot return more books than available!\n";
    }
}

    virtual string toFileString() {
        return to_string(id) + "|" + title + "|" + author + "|" + to_string(quantity);
    }

    virtual ~Book() {}
};

// DERIVED CLASS
class BorrowableBook : public Book {
private:
    string borrowDate;

public:
    BorrowableBook(int i, string t, string a, int q, string bDate = "")
        : Book(i, t, a, q) {
        borrowDate = bDate;
    }

    void displayBook() override {
        Book::displayBook();

        if (borrowDate != "") {
            cout << ", Borrow Date: " << borrowDate;
        }

        cout << "\n";
    }

    void issueBook() override {
        if (quantity > 0) {
            quantity--;

            cout << "Enter borrow date: ";
            cin.ignore();
            getline(cin, borrowDate);

            cout << "Book borrowed successfully!\n";
        } else {
            cout << "Book is out of stock!\n";
        }
    }

    void returnBook() override {

    if (quantity < maxQuantity) {

        quantity++;
        borrowDate = "";

        cout << "Book returned successfully!\n";
    }
    else {
        cout << "Cannot return more books than available!\n";
    }
}

    string toFileString() override {
        return to_string(id) + "|" + title + "|" + author + "|" +
               to_string(quantity) + "|" + borrowDate;
    }
};

// FUNCTIONS
void saveToFile(vector<Book*> &library) {
    ofstream file("library.txt");

    for (int i = 0; i < library.size(); i++) {
        file << library[i]->toFileString() << endl;
    }

    file.close();
}

void loadFromFile(vector<Book*> &library) {
    ifstream file("library.txt");

    if (!file) {
        return;
    }

    string line;

    while (getline(file, line)) {
        stringstream ss(line);

        string id, title, author, quantity, borrowDate;

        getline(ss, id, '|');
        getline(ss, title, '|');
        getline(ss, author, '|');
        getline(ss, quantity, '|');
        getline(ss, borrowDate, '|');

        BorrowableBook* b = new BorrowableBook(
            stoi(id),
            title,
            author,
            stoi(quantity),
            borrowDate
        );

        library.push_back(b);
    }

    file.close();
}

void viewBooks(vector<Book*> &library) {
    if (library.size() == 0) {
        cout << "No books available!\n";
        return;
    }

    cout << "\n===== BOOK LIST =====\n";

    for (int i = 0; i < library.size(); i++) {
        library[i]->displayBook();
    }
}

bool isDuplicateID(vector<Book*> &library, int id) {

    for (int i = 0; i < library.size(); i++) {

        if (library[i]->getId() == id) {
            return true;
        }
    }

    return false;
}

void addBook(vector<Book*> &library) {
    int id, quantity;
    string title, author;

    cout << "Enter book ID: ";
    cin >> id;
    if (isDuplicateID(library, id)) {

        cout << "Book ID already exists!\n";
        return;
    }
    cin.ignore();

    cout << "Enter title: ";
    getline(cin, title);

    cout << "Enter author: ";
    getline(cin, author);

    cout << "Enter quantity: ";
    cin >> quantity;

    BorrowableBook* newBook =
        new BorrowableBook(id, title, author, quantity);

    library.push_back(newBook);

    cout << "Book added successfully!\n";
}

void searchBook(vector<Book*> &library) {
    cin.ignore();

    string keyword;

    cout << "Enter title or author: ";
    getline(cin, keyword);

    bool found = false;

    for (int i = 0; i < library.size(); i++) {
        if (library[i]->getTitle().find(keyword) != string::npos ||
            library[i]->getAuthor().find(keyword) != string::npos) {

            library[i]->displayBook();
            found = true;
        }
    }

    if (!found) {
        cout << "Book not found!\n";
    }
}

void issueBook(vector<Book*> &library) {
    int id;

    cout << "Enter book ID to issue: ";
    cin >> id;

    for (int i = 0; i < library.size(); i++) {
        if (library[i]->getId() == id) {
            library[i]->issueBook();
            return;
        }
    }

    cout << "Book not found!\n";
}

void returnBook(vector<Book*> &library) {
    int id;

    cout << "Enter book ID to return: ";
    cin >> id;

    for (int i = 0; i < library.size(); i++) {
        if (library[i]->getId() == id) {
            library[i]->returnBook();
            return;
        }
    }

    cout << "Book not found!\n";
}

// NEW FEATURE
// Delete book
void deleteBook(vector<Book*> &library) {
    int id;

    cout << "Enter book ID to delete: ";
    cin >> id;

    for (int i = 0; i < library.size(); i++) {
        if (library[i]->getId() == id) {

            delete library[i];

            library.erase(library.begin() + i);

            cout << "Book deleted successfully!\n";
            return;
        }
    }

    cout << "Book not found!\n";
}

// Update book information
void updateBook(vector<Book*> &library) {
    int id;

    cout << "Enter book ID to update: ";
    cin >> id;

    for (int i = 0; i < library.size(); i++) {

        if (library[i]->getId() == id) {

            cin.ignore();

            string title, author;
            int quantity;

            cout << "Enter new title: ";
            getline(cin, title);

            cout << "Enter new author: ";
            getline(cin, author);

            cout << "Enter new quantity: ";
            cin >> quantity;

            library[i]->setTitle(title);
            library[i]->setAuthor(author);
            library[i]->setQuantity(quantity);

            cout << "Book updated successfully!\n";

            return;
        }
    }

    cout << "Book not found!\n";
}

// Sort books by title
void sortBooks(vector<Book*> &library) {

    sort(library.begin(), library.end(),
         [](Book* a, Book* b) {

             return a->getTitle() < b->getTitle();

         });

    cout << "Books sorted by title!\n";
}

// MAIN
int main() {
    vector<Book*> library;

    loadFromFile(library);

    int choice;

    while (1) {
        cout << "\n===== LIBRARY MANAGEMENT SYSTEM =====\n";
        cout << "1. Add Book\n";
        cout << "2. View All Books\n";
        cout << "3. Search Book\n";
        cout << "4. Issue Book\n";
        cout << "5. Return Book\n";
        cout << "6. Delete Book\n";
        cout << "7. Update Book\n";
        cout << "8. Sort Books By Title\n";
        cout << "9. Save Data\n";
        cout << "99. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {

        case 1:
            addBook(library);
            break;

        case 2:
            viewBooks(library);
            break;

        case 3:
            searchBook(library);
            break;

        case 4:
            issueBook(library);
            break;

        case 5:
            returnBook(library);
            break;

        case 6:
            deleteBook(library);
            break;

        case 7:
            updateBook(library);
            break;

        case 8:
            sortBooks(library);
            break;

        case 9:
            saveToFile(library);
            cout << "Data saved successfully!\n";
            break;

        case 99:

            saveToFile(library);

            for (int i = 0; i < library.size(); i++) {
                delete library[i];
            }

            cout << "Program exited.\n";

            return 0;

        default:
            cout << "Invalid choice!\n";
        }
    }

    return 0;
}
