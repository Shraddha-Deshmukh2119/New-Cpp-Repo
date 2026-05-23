#include <iostream>
#include <iomanip>
#include <conio.h>
#include <string>
#include "Headers/Client.h"
#include "Headers/Person.h"
#include "Headers/Customer.h"
#include "Headers/Employee.h"
#include "Headers/Admin.h"
using namespace std;

int main()
{
    Client client;

    if (client.start())
    {
        bool go = true;
        int ans;

        while(go)
        {        
            system("cls");
            welcome();
            cout<< " Proceed as:\n"
                << "\t1) Admin\n"
                << "\t2) Employee\n"
                << "\t3) Customer\n"
                << "\t4) Exit\n";
            
            cout << "\n Your choice > ";
            cin >> ans;
            fflush(stdin);

            switch (ans)
            {
            case 1:
                {
                    client.Send(1);
                    Admin admin(client);
                    admin.login();
                }
                break;
            
            case 2:
                {
                    client.Send(2);
                    Employee employee(client);
                    employee.login();
                }
                break;
            
            case 3:
                {
                    client.Send(3);
                    Customer customer(client);
                    customer.start();
                }
                break;
            
            case 4:
                client.Send(4);
                go = false;
                break;
        
            default:
                cout << " Invalid choice!";
                getch();
                break;
            }

            fflush(stdin);
        }
    }

    cin.get();
    return 0;
}
