#include <iostream>   // For cin, cout, endl
#include <string>     // For string, getline, find
#include <vector>     // For vector
#include <limits>     // For numeric_limits
#include <cstddef>    // For size_t
using namespace std;

struct User {
    string username;
    string password;
    vector<string> permissions;
    User* next;

    User(const string& u, const string& p, const vector<string>& perms = {"view"}) {
        username = u;
        password = p;
        permissions = perms;
        next = nullptr;
    }
};

// Insert a new user
bool insertUser(User*& head, const string& username, const string& password, const vector<string>& permissions) {
    User* current = head;
    while (current) {
        if (current->username == username) return false;
        current = current->next;
    }

    User* newUser = new User(username, password, permissions);
    if (!head) {
        head = newUser;
    } else {
        User* tail = head;
        while (tail->next) tail = tail->next;
        tail->next = newUser;
    }
    return true;
}

// Print all users
void printUsers(User* head) {
    if (!head) {
        cout << "List is empty." << endl;
        return;
    }
    while (head) {
        cout << head->username << " (permissions: ";
        for (size_t i = 0; i < head->permissions.size(); i++) {
            cout << head->permissions[i];
            if (i + 1 < head->permissions.size()) cout << ", ";
        }
        cout << ") -> ";
        head = head->next;
    }
    cout << "NULL" << endl;
}

// Authenticate a user
bool authenticate(User* head, const string& username, const string& password) {
    while (head) {
        if (head->username == username && head->password == password) return true;
        head = head->next;
    }
    return false;
}

// Find a user by username
User* findUser(User* head, const string& username) {
    while (head) {
        if (head->username == username) return head;
        head = head->next;
    }
    return nullptr;
}

// Authorize a user for an action
bool authorize(User* head, const string& username, const string& action) {
    User* user = findUser(head, username);
    if (!user) return false;

    for (const string& perm : user->permissions) {
        if (perm == action) return true;
    }
    return false;
}

// Remove a user by username
bool removeByUsername(User*& head, const string& username) {
    if (!head) return false;
    if (head->username == username) {
        User* temp = head;
        head = head->next;
        delete temp;
        return true;
    }

    User* current = head;
    while (current->next && current->next->username != username) {
        current = current->next;
    }

    if (current->next) {
        User* temp = current->next;
        current->next = temp->next;
        delete temp;
        return true;
    }

    return false;
}

// Clear the entire list
void clearList(User*& head) {
    while (head) {
        User* temp = head;
        head = head->next;
        delete temp;
    }
}

// Count users
int listSize(User* head) {
    int count = 0;
    while (head) {
        count++;
        head = head->next;
    }
    return count;
}

// Helper to trim spaces
string trim(const string& s) {
    size_t start = s.find_first_not_of(" \t");
    size_t end   = s.find_last_not_of(" \t");
    return (start == string::npos) ? "" : s.substr(start, end - start + 1);
}

// Update user permissions
bool updatePermissions(User* head, const string& username, const vector<string>& newPermissions) {
    User* user = findUser(head, username);
    if (!user) return false;
    user->permissions = newPermissions;
    return true;
}

int main() {
    User* head = nullptr;
    int choice;

    do {
        cout << "\n--- Credential Store Menu ---\n";
        cout << "1. Insert User\n";
        cout << "2. Print Users\n";
        cout << "3. Authenticate User\n";
        cout << "4. Authorize Action\n";
        cout << "5. Remove User\n";
        cout << "6. Clear List\n";
        cout << "7. Show Size\n";
        cout << "8. Update Permissions\n";
        cout << "0. Exit\n";
        cout << "Choose an option: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        string username, password, action;
        vector<string> permissions;
        string permInput;

        switch (choice) {
        case 1: {
            cout << "Enter username: ";
            getline(cin, username);
            cout << "Enter password: ";
            getline(cin, password);
            cout << "Enter permissions (comma-separated, e.g., view,edit): ";
            getline(cin, permInput);
            permissions.clear();

            size_t pos = 0;
            while ((pos = permInput.find(',')) != string::npos) {
                permissions.push_back(trim(permInput.substr(0, pos)));
                permInput.erase(0, pos + 1);
            }
            if (!permInput.empty()) permissions.push_back(trim(permInput));
            if (permissions.empty()) permissions.push_back("view");

            if (insertUser(head, username, password, permissions))
                cout << "User inserted.\n";
            else
                cout << "Username already exists.\n";
            break;
        }

        case 2: {
            printUsers(head);
            break;
        }

        case 3: {
            cout << "Enter username: ";
            getline(cin, username);
            cout << "Enter password: ";
            getline(cin, password);
            cout << (authenticate(head, username, password) ? "Authentication successful.\n" : "Authentication failed.\n");
            break;
        }

        case 4: {
            cout << "Enter username: ";
            getline(cin, username);
            cout << "Enter action to authorize (e.g., view, edit, create): ";
            getline(cin, action);
            cout << (authorize(head, username, action) ? "Action authorized.\n" : "Action denied.\n");
            break;
        }

        case 5: {
            cout << "Enter username to remove: ";
            getline(cin, username);
            cout << (removeByUsername(head, username) ? "User removed.\n" : "User not found.\n");
            break;
        }

        case 6: {
            clearList(head);
            cout << "List cleared.\n";
            break;
        }

        case 7: {
            cout << "Size of list: " << listSize(head) << endl;
            break;
        }

        case 8: {
            cout << "Enter username to update: ";
            getline(cin, username);
            cout << "Enter new permissions (comma-separated, e.g., view,edit): ";
            getline(cin, permInput);
            permissions.clear();

            size_t pos = 0;
            while ((pos = permInput.find(',')) != string::npos) {
                permissions.push_back(trim(permInput.substr(0, pos)));
                permInput.erase(0, pos + 1);
            }
            if (!permInput.empty()) permissions.push_back(trim(permInput));
            if (permissions.empty()) permissions.push_back("view");

            if (updatePermissions(head, username, permissions))
                cout << "Permissions updated.\n";
            else
                cout << "User not found.\n";
            break;
        }

        case 0: {
            cout << "Exiting program.\n";
            break;
        }

        default: {
            cout << "Invalid option. Try again.\n";
        }
        }

    } while (choice != 0);

    clearList(head); // Final cleanup
    return 0;
}
