#include <iostream>
using namespace std;

class User
{
protected:
    string username;
    string password;
    int accessLevel; // 1 = User, 2 = Admin
    static int totalUsers;

public:
    User(string u = "", string p = "", int level = 1)
        : username(u), password(p), accessLevel(level)
    {
        totalUsers++;
    }
    friend bool comparePassword(const User &u1, const User &u2);
    bool login(string u, string p) { return (u == username && p == password); }
    bool login(string u) { return (u == username); }
    string getUsername() const { return username; }
    int getAccessLevel() const { return accessLevel; }
    void operator++()
    {
        if (accessLevel == 1)
            accessLevel = 2;
    }

    User operator+(const User &other)
    {
        return User(username + "&" + other.username, "merged", 1);
    }
    void showDetails() const
    {
        cout << "Username: " << username << ", Role: " << (accessLevel == 2 ? "Admin" : "User") << endl;
    }
    static int getTotalUsers() { return totalUsers; }
    void setPassword(string newPass) { password = newPass; }
};
int User::totalUsers = 0;
bool comparePassword(const User &u1, const User &u2)
{
    return u1.password == u2.password;
}

class SecurityLogger
{
protected:
    string lastLog;

public:
    void addLog(string msg) { lastLog = msg; }
    void showLog() { cout << "Log: " << lastLog << endl; }
};

class Admin : public User, public SecurityLogger
{
public:
    Admin(string u = "", string p = "") : User(u, p, 2) {}
    void resetPassword(User &u, string newPass)
    {
        u.setPassword(newPass);
        addLog("Admin reset password for: " + u.getUsername());
    }

    void deleteUser(User users[], int &count, string uname)
    {
        for (int i = 0; i < count; i++)
        {
            if (users[i].getUsername() == uname)
            {
                for (int j = i; j < count - 1; j++)
                {
                    users[j] = users[j + 1];
                }
                count--;
                addLog("Admin deleted user: " + uname);
                cout << "User '" << uname << "' deleted.\n";
                return;
            }
        }
        cout << "User not found.\n";
    }

    void viewAllUsers(User users[], int count)
    {
        cout << "\n--- All Users ---\n";
        for (int i = 0; i < count; i++)
        {
            users[i].showDetails();
        }
    }
};

int main()
{
    const int MAX = 10;
    User users[MAX];
    int userCount = 0;
    Admin admin("root", "admin123");

    bool adminLoggedIn = false;
    int choice;

    do
    {
        cout << "\n--- Cyber Security Projects Menu ---\n";
        cout << "1. Register User\n";
        cout << "2. Login (Overloaded)\n";
        cout << "3. Compare Passwords (Friend Function)\n";
        cout << "4. Promote User (++ Operator)\n";
        cout << "5. Merge Users (+ Operator)\n";
        cout << "6. Show Total Users (Static Function)\n";
        cout << "7. Admin Login\n";
        if (adminLoggedIn)
        {
            cout << "8. Admin: Reset Password\n";
            cout << "9. Admin: Delete User\n";
            cout << "10. Admin: View All Users\n";
            cout << "11. Show Logs\n";
            cout << "12. Admin Logout\n";
        }
        cout << "0. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1)
        {
            if (userCount < MAX)
            {
                string u, p;
                int level;
                cout << "Enter username: ";
                cin >> u;
                cout << "Enter password: ";
                cin >> p;
                cout << "Role? (1=User,2=Admin): ";
                cin >> level;
                users[userCount] = User(u, p, level);
                userCount++;
            }
            else
                cout << "User database full!\n";
        }
        else if (choice == 2)
        {
            string u, p;
            cout << "Enter username: ";
            cin >> u;
            cout << "Enter password (or '-' for guest): ";
            cin >> p;
            bool success = false;
            for (int i = 0; i < userCount; i++)
            {
                if ((p == "-" && users[i].login(u)) || users[i].login(u, p))
                {
                    cout << "Login success!\n";
                    users[i].showDetails();
                    success = true;
                    break;
                }
            }
            if (!success)
                cout << "Login failed.\n";
        }
        else if (choice == 3)
        {
            if (userCount >= 2)
            {
                cout << (comparePassword(users[0], users[1]) ? "Passwords match.\n" : "Passwords differ.\n");
            }
            else
                cout << "Need at least 2 users.\n";
        }
        else if (choice == 4)
        {
            if (userCount > 0)
            {
                ++users[0];
                cout << "Promoted first user:\n";
                users[0].showDetails();
            }
        }
        else if (choice == 5)
        {
            if (userCount >= 2)
            {
                User merged = users[0] + users[1];
                cout << "Merged User: ";
                merged.showDetails();
            }
        }
        else if (choice == 6)
        {
            cout << "Total users registered: " << User::getTotalUsers() << endl;
        }
        else if (choice == 7)
        {
            string uname, pass;
            cout << "Enter admin username: ";
            cin >> uname;
            cout << "Enter admin password: ";
            cin >> pass;
            if (admin.login(uname, pass))
            {
                adminLoggedIn = true;
                admin.addLog("Admin logged in.");
                cout << "Admin login successful.\n";
            }
            else
            {
                cout << "Invalid admin credentials!\n";
            }
        }
        else if (adminLoggedIn && choice == 8)
        {
            string uname, newPass;
            cout << "Enter target username: ";
            cin >> uname;
            cout << "Enter new password: ";
            cin >> newPass;
            for (int i = 0; i < userCount; i++)
            {
                if (users[i].getUsername() == uname)
                {
                    admin.resetPassword(users[i], newPass);
                    cout << "Password reset.\n";
                }
            }
        }
        else if (adminLoggedIn && choice == 9)
        {
            string uname;
            cout << "Enter username to delete: ";
            cin >> uname;
            admin.deleteUser(users, userCount, uname);
        }
        else if (adminLoggedIn && choice == 10)
        {
            admin.viewAllUsers(users, userCount);
        }
        else if (adminLoggedIn && choice == 11)
        {
            admin.showLog();
        }
        else if (adminLoggedIn && choice == 12)
        {
            adminLoggedIn = false;
            admin.addLog("Admin logged out.");
            cout << "Admin logged out successfully.\n";
        }
        else if (choice != 0)
        {
            cout << "Invalid choice or admin not logged in for that option.\n";
        }
    } while (choice != 0);

    cout << "Exiting system...\n";
    return 0;
}
