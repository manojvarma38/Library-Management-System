#include <bits/stdc++.h>
using namespace std;

class Book
{

private:
    string isbn;
    int copiesAvailable, totalCopies;

public:
    string title, author;
    Book() // Default constructor
    {
        this->title = "UnknownTitle";
        this->author = "UnknownAuthor";
        this->isbn = "ISBN";
        this->copiesAvailable = 0;
        this->totalCopies = 5;
    }
    Book(string title, string author, string isbn, int copiesAvailable, int totalCopies) // Parameterized constructor
    {
        this->title = title;
        this->author = author;
        this->isbn = isbn;
        this->copiesAvailable = copiesAvailable;
        this->totalCopies = totalCopies;
    }
    Book(const Book &book, string newIsbn) // Copy constructor with newIsbn
    {
        this->title = book.title;
        this->author = book.author;
        this->isbn = newIsbn;
        this->copiesAvailable = book.copiesAvailable;
        this->totalCopies = book.totalCopies;
    }
    ~Book() {}
    string getIsbn() { return this->isbn; }
    int getcopiesAvailable() { return this->copiesAvailable; }
    int gettotalCopies() { return this->totalCopies; }
    void updateCopies(int count) // Updates copiesAvailable and totalCopies with value count
    {
        int curcopiesAvailable = this->copiesAvailable;
        int curtotalCopies = this->totalCopies;
        if ((curcopiesAvailable + count < 0) || (curtotalCopies + count < 0)) // Check the non-negativity condition
        {
            cout << "Invalid request! Count becomes negative" << '\n';
            return;
        }
        else
        {
            this->copiesAvailable = curcopiesAvailable + count;
            this->totalCopies = curtotalCopies + count;
        }
    }
    void singleBookUpdate(int count) // Utility function which can be used to only update copiesAvailable
    {
        this->copiesAvailable += count;
    }
    bool borrowBook()
    {
        int curcopiesAvailable = this->copiesAvailable;
        if (curcopiesAvailable > 0) // Check the availability condition
        {
            this->copiesAvailable--;
            return true;
        }
        else
        {
            cout << "Invalid request! Copy of book not available" << '\n';
            return false;
        }
    }
    bool returnBook()
    {
        int curcopiesAvailable = this->copiesAvailable;
        int curtotalCopies = this->totalCopies;
        if (curcopiesAvailable + 1 <= curtotalCopies) // Check the totalCopies condition
        {
            this->copiesAvailable++;
            return true;
        }
        else
        {
            cout << "Invalid request! Copy of book exceeds total copies" << '\n';
            return false;
        }
    }
    void printDetails()
    {
        cout << this->title << ' ' << this->author << '\n';
    }
};

class Member
{
private:
    string memberID;
    map<string, int> borrowedBooks; // Indicates the number of currently borrowed books of each type by the member
    int borrowLimit;
    int curBorrowed; // Indicates the total number of currently borrowed books by the member

public:
    string name;
    Member(string memberID, string name, int borrowLimit = 3)
    {
        this->name = name;
        this->memberID = memberID;
        this->borrowLimit = borrowLimit;
        this->curBorrowed = 0; // Initialized to 0
    }
    Member() {}
    ~Member() {}
    bool borrowBook(string isbn)
    {
        if (this->curBorrowed + 1 <= this->borrowLimit) // Check the borrowLimit condition
        {
            this->curBorrowed++;
            borrowedBooks[isbn]++;
            return true;
        }
        else
        {
            cout << "Invalid request! Borrow limit exceeded" << '\n';
            return false;
        }
    }
    bool returnBook(string isbn)
    {
        if (borrowedBooks.find(isbn) != borrowedBooks.end()) // Check if the book is currently borrowed by the member
        {
            this->curBorrowed--;
            borrowedBooks[isbn]--;
            if (borrowedBooks[isbn] == 0) // If the number of copies of that book present with member becomes 0,erase the book's isbn from borrowedBooks Map
            {
                borrowedBooks.erase(isbn);
            }
            return true;
        }
        else
        {
            cout << "Invalid request! Book not borrowed" << '\n';
            return false;
        }
    }
    void printDetails()
    {
        for (auto it : borrowedBooks)
        {
            cout << this->memberID << ' ' << this->name << ' ' << it.first << ' ' << it.second << '\n';
        }
    }
    string getMemberId()
    {
        return this->memberID;
    }
};

class Library
{
private:
    vector<Member> members;
    vector<Book> books;

public:
    bool addBook(Book &book)
    {
        string newIsbn = book.getIsbn();
        if (isIsbnpPresent(newIsbn)) // If isbn is already present in library,return false
        {
            cout << "Invalid request! Book with same isbn already exists" << '\n';
            return false;
        }
        else
        {
            books.push_back(book); // Add the new Book
            return true;
        }
    }
    bool registerMember(Member &member)
    {
        string newMemberId = member.getMemberId();
        if (isMemberIdPresent(newMemberId)) // If memberId is already present in library,return false
        {
            cout << "Invalid request! Member with same id already exists" << '\n';
            return false;
        }
        else
        {
            members.push_back(member); // Add the new member
            return true;
        }
    }
    bool borrowBook(string memberID, string isbn)
    { // Extra check
        // If isbn is not present in library,member can't borrow that Book
        if (!isIsbnpPresent(isbn))
        {
            cout << "Invalid request! Book not borrowed" << '\n';
            return false;
        }
        Book &book = getBook(isbn);
        if (book.borrowBook()) // If book can be borrowed ,check conditions for member
        {
            // Extra check
            // If memberId is not present in library,member doesn't exist
            if (!isMemberIdPresent(memberID))
            {
                cout << "Invalid request! Borrow limit exceeded" << '\n';
                book.singleBookUpdate(1);
                return false;
            }
            Member &member = getMember(memberID);
            if (member.borrowBook(isbn)) // If member satisfies conditions,return true
            {
                return true;
            }
            else
            {
                book.singleBookUpdate(1); // We need to compensate to the Book count beacuse we have reduced one copy in "book.borrowBook()" function
            }
        }
        return false;
    }
    bool returnBook(string memberID, string isbn)
    {
        // Extra check
        // If isbn is not present in library,member can't return that Book
        if (!isIsbnpPresent(isbn))
        {
            cout << "Invalid request! Book not borrowed" << '\n';
            return false;
        }
        Book &book = getBook(isbn);
        if (book.returnBook()) // If book can be returned ,check conditions for member
        {
            // Extra check
            // If memberId is not present in library,member doesn't exist
            if (!isMemberIdPresent(memberID))
            {
                cout << "Invalid request! Borrow limit exceeded" << '\n';
                book.singleBookUpdate(-1);
                return false;
            }
            Member &member = getMember(memberID);
            if (member.returnBook(isbn)) // If member satisfies conditions,return true
            {
                return true;
            }
            else
            {
                book.singleBookUpdate(-1); // We need to compensate to the Book count beacuse we have increased one copy in "book.returnBook()" function
            }
        }
        return false;
    }
    void printLibraryDetails()
    {
        for (auto it : books)
        {
            Book &book = it;
            cout << book.title << ' ' << book.author << ' ' << book.getcopiesAvailable() << '\n';
        }
        for (auto it : members)
        {
            Member &member = it;
            cout << member.getMemberId() << ' ' << member.name << '\n';
        }
    }
    bool isIsbnpPresent(string isbn) // Checks if Isbn is already present in the library
    {
        for (Book &book : books)
        {
            if (book.getIsbn() == isbn)
            {
                return true;
            }
        }
        return false;
    }
    bool isMemberIdPresent(string memberId) // Checks if memberId is already present in the library
    {
        for (Member &member : members)
        {
            if (member.getMemberId() == memberId)
            {
                return true;
            }
        }
        return false;
    }
    Book &getBook(string isbn) // Returns reference to the Book
    {
        for (Book &book : books)
        {
            if (book.getIsbn() == isbn)
            {
                return book;
            }
        }
        static Book dummyBook; // Code never reaches this position,return dummyBook as we need to return reference to object of type Book
        return dummyBook;
    }
    Member &getMember(string memberId) // Returns reference to the Member
    {
        for (Member &member : members)
        {
            if (member.getMemberId() == memberId)
            {
                return member;
            }
        }
        static Member dummyMember; // Code never reaches this position,return dummyMember as we need to return reference to object of type Member
        return dummyMember;
    }
};
int main()
{
    Library library;
    string Instruction;
    cin >> Instruction;
    while (Instruction != "Done") // Break when Instruction is "Done"
    {
        if (Instruction == "Book")
        {
            string bookDetails;
            cin >> bookDetails;
            if (bookDetails == "None")
            {
                Book newBook; // Default constructor is called
                library.addBook(newBook);
            }
            else if (bookDetails == "ExistingBook")
            {
                string oldBookIsbn, newBookIsbn;
                cin >> oldBookIsbn >> newBookIsbn;
                Book &oldBook = library.getBook(oldBookIsbn); // Old Book
                Book newBook(oldBook, newBookIsbn);           // Copy constructor is called
                library.addBook(newBook);
            }
            else
            {
                string title = bookDetails;
                string author, newBookIsbn;
                int copiesAvailable, totalCopies;
                cin >> author >> newBookIsbn >> copiesAvailable >> totalCopies;
                Book newBook(title, author, newBookIsbn, copiesAvailable, totalCopies); // Parameterized constructor is called
                library.addBook(newBook);
            }
        }
        else if (Instruction == "UpdateCopiesCount")
        {
            string Isbn;
            int NewCount;
            cin >> Isbn >> NewCount;
            Book &book = library.getBook(Isbn);
            book.updateCopies(NewCount); // updateCopies function
        }
        else if (Instruction == "Member")
        {
            string memberDetails;
            cin >> memberDetails;
            if (memberDetails == "NoBorrowLimit")
            {
                string MemberId, Name;
                cin >> MemberId >> Name;
                Member newMember(MemberId, Name); // BorrowLimit is set to Default argument
                library.registerMember(newMember);
            }
            else
            {
                string MemberId, Name;
                int BorrowLimit;
                MemberId = memberDetails;
                cin >> Name >> BorrowLimit;
                Member newMember(MemberId, Name, BorrowLimit); // BorrowLimit is set to the value given to constructor
                library.registerMember(newMember);
            }
        }
        else if (Instruction == "Borrow")
        {
            string MemberId, isbn;
            cin >> MemberId >> isbn;
            library.borrowBook(MemberId, isbn); // borrowBook function
        }
        else if (Instruction == "Return")
        {
            string MemberId, isbn;
            cin >> MemberId >> isbn;
            library.returnBook(MemberId, isbn); // returnBook function
        }
        else if (Instruction == "PrintBook")
        {
            string isbn;
            cin >> isbn;
            Book &book = library.getBook(isbn); // Get the Book using library function
            book.printDetails();
        }
        else if (Instruction == "PrintMember")
        {
            string memberId;
            cin >> memberId;
            Member &member = library.getMember(memberId); // Get the Member using library function
            member.printDetails();
        }
        else if (Instruction == "PrintLibrary")
        {
            library.printLibraryDetails();
        }
        else if (Instruction == "Done")
            break;
        cin >> Instruction;
    }
    return 0;
}