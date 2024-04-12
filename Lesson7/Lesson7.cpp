#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <ctime> // Для работы с временем

using namespace std;

class Book {
public:
    string title;
    string author;
    string genre;
    int year;
    bool borrowed;
    time_t timeBorrowed;
    time_t timeReturned;

    Book(string title, string author, string genre, int year)
        : title(title), author(author), genre(genre), year(year), borrowed(false), timeBorrowed(0), timeReturned(0) {}

    void borrowBook() {
        borrowed = true;
        time(&timeBorrowed); // Установка времени взятия книги
    }

    void returnBook() {
        borrowed = false;
        time(&timeReturned); // Установка времени возврата книги
    }

    void printInfo() const {
        char buffer[26];
        struct tm tmBorrowed, tmReturned;
        cout << "Название: " << title << ", Автор: " << author
            << ", Жанр: " << genre << ", Год: " << year;
        if (borrowed) {
            localtime_s(&tmBorrowed, &timeBorrowed);
            asctime_s(buffer, sizeof(buffer), &tmBorrowed);
            cout << ", Взята: " << buffer;
        }
        else {
            localtime_s(&tmReturned, &timeReturned);
            asctime_s(buffer, sizeof(buffer), &tmReturned);
            cout << ", Возвращена: " << buffer;
        }
        cout << endl;
    }
};

class Library {
private:
    vector<Book> books;

public:
    void addBook(const Book& book) {
        books.push_back(book);
    }

    void addNewBook() {
        string title, author, genre;
        int year;

        cout << "Введите название книги: ";
        cin.ignore(); // Пропускаем символ новой строки из предыдущего ввода
        getline(cin, title);

        cout << "Введите автора книги: ";
        getline(cin, author);

        cout << "Введите жанр книги: ";
        getline(cin, genre);

        cout << "Введите год выпуска книги: ";
        cin >> year;

        Book newBook(title, author, genre, year);
        addBook(newBook);
        cout << "Книга \"" << title << "\" успешно добавлена в библиотеку." << endl;
    }
    


    void borrowBook(const string& title) {
        for (auto& book : books) {
            if (book.title == title && !book.borrowed) {
                book.borrowBook();
                cout << "Вы взяли книгу \"" << book.title << "\" взаймы." << endl;
                return;
            }
            else if (book.title == title && book.borrowed) {
                cout << "Книга \"" << title << "\" уже взята взаймы." << endl;
                return;
            }
        }
        cout << "Книга \"" << title << "\" не найдена." << endl;
    }

    void returnBook(const string& title) {
        for (auto& book : books) {
            if (book.title == title && book.borrowed) {
                book.returnBook();
                cout << "Вы вернули книгу \"" << book.title << "\"." << endl;
                return;
            }
            else if (book.title == title && !book.borrowed) {
                cout << "Книга \"" << title << "\" уже находится в библиотеке." << endl;
                return;
            }
        }
        cout << "Книга \"" << title << "\" не найдена." << endl;
    }

    void printBooks() const {
        for (const auto& book : books) {
            book.printInfo();
        }
    }

    const vector<Book>& getBooks() const {
        return books;
    }
};

void saveLibrary(const Library& library, const string& filename) {
    ofstream fileOut(filename);
    if (!fileOut.is_open()) {
        cerr << "Ошибка открытия файла для записи." << endl;
        return;
    }
    for (const auto& book : library.getBooks()) {
        // Переводим bool в строку
        ostringstream borrowedStr;
        borrowedStr << boolalpha << book.borrowed;
        ostringstream timeBorrowedStr;
        timeBorrowedStr << book.timeBorrowed;
        ostringstream timeReturnedStr;
        timeReturnedStr << book.timeReturned;
        // Записываем в файл
        fileOut << book.title << ";" << book.author << ";" << book.genre << ";" << book.year << ";" << borrowedStr.str() << ";"
            << timeBorrowedStr.str() << ";" << timeReturnedStr.str() << endl;
    }
    fileOut.close();
}

void loadLibrary(Library& library, const string& filename) {
    ifstream fileIn(filename);
    string line;
    while (getline(fileIn, line)) {
        string title, author, genre;
        int year;
        bool borrowed;
        time_t timeBorrowed, timeReturned;
        istringstream iss(line);
        getline(iss, title, ';');
        getline(iss, author, ';');
        getline(iss, genre, ';');
        string yearStr;
        getline(iss, yearStr, ';');
        year = stoi(yearStr);
        string borrowedStr;
        getline(iss, borrowedStr, ';');
        // Переводим строку в bool
        istringstream(borrowedStr) >> boolalpha >> borrowed;
        string timeBorrowedStr, timeReturnedStr;
        getline(iss, timeBorrowedStr, ';');
        timeBorrowed = stoi(timeBorrowedStr);
        getline(iss, timeReturnedStr, ';');
        timeReturned = stoi(timeReturnedStr);
        Book book(title, author, genre, year);
        if (borrowed) {
            book.borrowBook();
            book.timeBorrowed = timeBorrowed;
        }
        else {
            book.returnBook();
            book.timeReturned = timeReturned;
        }
        library.addBook(book);
    }
    fileIn.close();
}

int main() {

    Library myLibrary;

    loadLibrary(myLibrary, "library.txt");

    int choice;
    string title;

    while (true) {
        cout << "Что вы хотите сделать?" << endl;
        cout << "1. Взять книгу" << endl;
        cout << "2. Вернуть книгу" << endl;
        cout << "3. Вывести список книг" << endl;
        cout << "4. Добавить новую книгу" << endl;
        cout << "5. Выйти" << endl;
        cout << "Выберите действие (1-5): ";
        cin >> choice;

        switch (choice) {
        case 1:
            cout << "Введите название книги: ";
            cin.ignore(); // Чтобы пропустить символ новой строки из предыдущего ввода
            getline(cin, title);
            myLibrary.borrowBook(title);
            break;
        case 2:
            cout << "Введите название книги: ";
            cin.ignore(); // Чтобы пропустить символ новой строки из предыдущего ввода
            getline(cin, title);
            myLibrary.returnBook(title);
            break;
        case 3:
            myLibrary.printBooks();
            break;
        case 4:
            myLibrary.addNewBook();
            break;
        case 5:
            saveLibrary(myLibrary, "library.txt");
            return 0;
        default:
            cout << "Неверный ввод. Попробуйте снова." << endl;
            break;
        }
    }

}
