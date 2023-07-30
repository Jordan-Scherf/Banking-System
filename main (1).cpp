/*
Name: Jordan Scherf (Scherf@unlv.nevada.edu)
Program Description: This Program is a bank program that combines all bank records from multiple different banks and
 allows transactions and displays bank records when requested. All is done within a selfmade hashmap in a header class
 called hashMap.
*/

#include "hashMap.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <unordered_map>
#include <string>
struct bankType
{
	struct transactionType
	{
		transactionType(bool t, double a)
		{
			type = t;
			amount = a;
		}

		bool type;
		double amount;
	};

	bankType(std::string first, std::string last, std::string acctNo,
		double balance,unsigned int pin)
	{
		name = last + ", " + first;
		this->balance = balance;
		this->pin = pin;
		this->acctNo = acctNo;
		locked = false;
	}

	bankType()
	{
		name = "";
		balance = 0;
		pin = 0;
		locked = false;
	}

	std::string name;
	std::string acctNo;
	double balance;
	unsigned int pin;
	bool locked;

	std::vector<transactionType> transactions;
};


int main() {
    std::string line;
    std::cout << std::fixed << std::setprecision(2);
    std::cout << '\n';

    hashMap<std::string, hashMap<std::string, bankType> > bank;
    hashMap<std::string, bool> locationsHash;
    std::ifstream infile("data.csv");
    if (!infile.is_open()) {
        std::cout << "Error opening File\n";
        return 1;
    }
    std::getline(infile, line);
    while (std::getline(infile, line)) {
        std::string firstname = line.substr(0, line.find(","));
        line.erase(0, line.find(",") + 1);
        std::string lastName = line.substr(0, line.find(","));
        line.erase(0, line.find(",") + 1);
        std::string location = line.substr(0, line.find(","));
        line.erase(0, line.find(",") + 1);
        for (auto& c : location) {
            c = toupper(c);
        }
        std::string accNo = line.substr(0, line.find(","));
        line.erase(0, line.find(",") + 1);
        std::string balance = line.substr(0, line.find(","));
        line.erase(0, line.find(",") + 1);
        std::string pin = line.substr(0, line.find(","));

        // Getting the last 4 of the account number
        std::string acct4Digits = "";
        for (int i = accNo.size() - 4; i < accNo.size(); i++) {
            acct4Digits += accNo.at(i);
        }
        double balance2 = stod(balance);
        unsigned int pin2 = stoi(pin);
        locationsHash[location] = true;

        bank[location][acct4Digits] = bankType(firstname, lastName, accNo, balance2, pin2);
      //  std::cout <<acct4Digits << " " << location << " " << bank[location][acct4Digits].name << " " << bank[location][acct4Digits].acctNo << '\n';
    }

   // std::cout << bank["MARYLAND"]["9718"].name << " " << bank["MARYLAND"]["9718"].pin << '\n';
    std::string location = "RAINBOW";
    std::string sm = "6759";
    bool Continue = true;
    bool validLoc = true;
    bool invalidPin = false;

    //std::cout << bank[location][sm].name << " " << bank[location][sm].pin;


    while (Continue) {
        // Continuously prompt the user for bank branch location until valid location is provided
        Continue = true;
        validLoc = true;
        invalidPin = false;
        while (validLoc) { // While the location is valid
            std::cout << "\n Please enter bank branch location: ";
            std::getline(std::cin, location);  // gets the entirety of the line
            validLoc = true;

            std::cout << '\n';
            for (auto& c : location) {  // Capatilizes the location
                c = toupper(c);
            }
            if (locationsHash[location] == false) {  // if the location doesnt exist, dont continue
                validLoc = false;
            }

            if (validLoc) {  // if the location is good continue
                validLoc = false;
                std::string last4;
                unsigned int enterPin;
                int count = 0;
                std::cout << "Enter last 4 digits of account: ";
                std::cin >> last4;
                std::cout << '\n';
                if (bank[location][last4].locked == false) {
                    auto pins =bank[location][last4].pin;
                    auto names = bank[location][last4].name;
                    if (pins != 0 ) {  // This checks to see if the account exist, if it has a pin then it should exist
                        unsigned int pin = bank[location][last4].pin;
                        while (!invalidPin && (count < 3)) {  // if the pin is incorrect or if its attempted to many
                                                                // times then its locked
                            invalidPin = true;
                            std::cout << "Enter a pin: ";
                            std::cin >> enterPin;
                            std::cout << '\n';
                            if (enterPin != pin) {
                                std::cout << "Invalid pin\n\n";
                                invalidPin = false;
                                count++;
                            }

                        }
                        char choice;
                        double n = 0;
                        bool done = false;
                        if (count < 3) {


                            while (!done) {  // These are choices to do
                                std::cout
                                        << "(D)eposit\n (W)ithdrawal\n (V)iew account\n (E)xit account\n Enter choice: ";
                                std::cin >> choice;
                                std::cout << '\n';

                                if (choice == 'V' || (choice == 'v')) {
                                    std::cout << "Name: " << bank[location][last4].name << '\n';
                                    std::cout << "Balance: " << '$' << bank[location][last4].balance << '\n' << '\n';

                                    if (bank[location][last4].transactions.empty()) {
                                        std::cout << "No transactions \n \n";
                                    } else {
                                        std::cout << "List of Transactions\n";
                                        for (const auto &it: bank[location][last4].transactions) {
                                            //  Decides on the type of transaction based on boolian type
                                            std::cout << (it.type ? "Deposit" : "Withdrawal") << " Amount $"
                                                      << it.amount
                                                      << '\n';
                                        }
                                    }
                                } else if (choice == 'D' || (choice == 'd') ) {
                                    std::cout << "Enter Deposit Amount: ";
                                    std::cin >> n;
                                    std::cout << '\n' << '\n';

                                    bank[location][last4].balance = bank[location][last4].balance + n;
                                    bank[location][last4].transactions.push_back(bankType::transactionType(true, n));
                                } else if (choice == 'W' || (choice == 'w') ){
                                    std::cout << "Enter Withdrawal Amount: ";
                                    std::cin >> n;
                                    std::cout << '\n' << '\n';
                                    if (bank[location][last4].balance - n >= 0) {
                                        bank[location][last4].balance = bank[location][last4].balance - n;
                                        bank[location][last4].transactions.push_back(
                                                bankType::transactionType(false, n));
                                    } else {
                                        std::cout << "\n Not enough funds to make transaction\n";  // if they dont have enough cash, leave
                                    }
                                } else {
                                    done = true;
                                }
                            }
                        } else {
                            std::cout << "3 failed login attempts, account is now locked\n\n";  // Message if locked
                            bank[location][last4].locked = true;
                        }
                    } else {
                        std::cout << "Account not Found \n \n";  // If the account doesnt exist
                    }

                } else {
                    std::cout << "Account has a lock\n";  // Message if locked
                }

            }else{
                std::cout << "Bank branch not found\n\n";

            }


        }
        char c;
        std::cout << "\nContinue? (Y/N): \n";
        std::cin >> c;
        if (c == 'N' || c == 'n') {
            Continue = false;
        }else{

        }
        std::cin.ignore();  // Clears the input buffer for getline
    }

    return 0;
}