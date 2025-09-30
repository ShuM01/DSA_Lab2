#include #include #include #include using namespace std;

struct User {
string username;
string password;
vector permissions;
User* next;

User(const string& u, const string& p, const vector& perms = {"view"}) {
username = u;
password = p;
permissions = perms;
next = nullptr;
}
};

// Insert a new user
bool insertUser(User*& head, const string& username, const string& password, const vector& permissions = {"view"}) {
User* current = head;
while (current) {
if (current->username == username) return false;
current = current->next;
}

User* newUser = new User(username, password, permissions);
if (!head) head = newUser;
else {
current = head;
while (current->next) current = current->next;
current->next = newUser;
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
cout << head->username << " -> ";
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
int size(User* head) {
int count = 0;
while (head) {
count++;
head = head->next;
}
return count;
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
cout << "0. Exit\n";
cout << "Choose an option: ";
cin >> choice;
cin.ignore(numeric_limits::max(), '\n');

string username, password, action;
vector permissions;
string permInput;

switch (choice) {
case 1:
cout << "Enter username: ";
getline(cin, username);
cout << "Enter password: ";
getline(cin, password);
cout << "Enter permissions (comma-separated, e.g., view,edit): ";
getline(cin, permInput);
permissions.clear();
size_t pos = 0;
while ((pos = permInput.find(',')) != string::npos) {
permissions.push_back(permInput.substr(0, pos));
permInput.erase(0, pos + 1);
}
if (!permInput.empty()) permissions.push_back(permInput);

if (insertUser(head, username, password, permissions))
cout << "User inserted.\n";
else
cout << "Username already exists.\n";
break;

case 2:
printUsers(head);
break;

case 3:
cout << "Enter username: ";
getline(cin, username);
cout << "Enter password: ";
getline(cin, password);
cout << (authenticate(head, username, password) ? "Authentication successful.\n" : "Authentication failed.\n");
break;

case 4:
cout << "Enter username: ";
getline(cin, username);
cout << "Enter action to authorize (e.g., view, edit, create): ";
getline(cin, action);
cout << (authorize(head, username, action) ? "Action authorized.\n" : "Action denied.\n");
break;

case 5:
cout << "Enter username to remove: ";
getline(cin, username);
cout << (removeByUsername(head, username) ? "User removed.\n" : "User not found.\n");
break;

case 6:
clearList(head);
cout << "List cleared.\n";
break;

case 7:
cout << "Size of list: " << size(head) << endl;
break;

case 0:
cout << "Exiting program.\n";
break;

default:
cout << "Invalid option. Try again.\n";
}

} while (choice != 0);

clearList(head);
return 0;
}