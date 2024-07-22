#include <iostream>
#include <unordered_map>
#include <string>
#include <cctype>

using namespace std;

const int MAX_PARKING_SPACES = 15;

struct Reservation {
    string user;
    int spaceNumber;
    int plateNumber;
    Reservation* next;

    Reservation(string u, int s, int p) : user(u), spaceNumber(s), plateNumber(p), next(nullptr) {}

    void updateReservation(int newSpaceNumber, int newPlateNumber) {
        spaceNumber = newSpaceNumber;
        plateNumber = newPlateNumber;
    }
};

class ReservationList {
public:
    Reservation* head;

    ReservationList() : head(nullptr) {}

    void addReservation(string user, int spaceNumber, int plateNumber) {
        Reservation* newReservation = new Reservation(user, spaceNumber, plateNumber);
        if (!head) {
            head = newReservation;
        } else {
            Reservation* temp = head;
            while (temp->next) {
                temp = temp->next;
            }
            temp->next = newReservation;
        }
    }

    void removeReservation(string user, int plateNumber) {
        if (!head) return;

        if (head->user == user && head->plateNumber == plateNumber) {
            Reservation* toDelete = head;
            head = head->next;
            delete toDelete;
            return;
        }

        Reservation* temp = head;
        while (temp->next && (temp->next->user != user || temp->next->plateNumber != plateNumber)) {
            temp = temp->next;
        }

        if (temp->next) {
            Reservation* toDelete = temp->next;
            temp->next = temp->next->next;
            delete toDelete;
        }
    }

    Reservation* findReservationByUser(string user, int plateNumber) {
        Reservation* temp = head;
        while (temp) {
            if (temp->user == user && temp->plateNumber == plateNumber) {
                return temp;
            }
            temp = temp->next;
        }
        return nullptr;
    }

    void printReservations() {
        Reservation* temp = head;
        while (temp) {
            cout << "User: " << temp->user << ", Space Number: " << temp->spaceNumber << ", Plate Number: " << temp->plateNumber << endl;
            temp = temp->next;
        }
    }

    void updateReservation(string user, int newSpaceNumber, int plateNumber) {
        Reservation* reservation = findReservationByUser(user, plateNumber);
        if (reservation) {
            reservation->updateReservation(newSpaceNumber, plateNumber);
            cout << "Reservation updated successfully!" << endl;
        } else {
            cout << "Reservation not found for user " << user << " with plate number " << plateNumber << "." << endl;
        }
    }

    void sortReservations() {
        if (!head || !head->next) return;

        bool swapped;
        do {
            swapped = false;
            Reservation* current = head;
            Reservation* prev = nullptr;
            Reservation* next = nullptr;

            while (current && current->next) {
                next = current->next;
                if (current->spaceNumber > next->spaceNumber) {
                    swapped = true;
                    if (prev) {
                        prev->next = next;
                    } else {
                        head = next;
                    }
                    current->next = next->next;
                    next->next = current;

                    prev = next;
                } else {
                    prev = current;
                    current = next;
                }
            }
        } while (swapped);
    }
};


string parkingSpaces[MAX_PARKING_SPACES];
int availableSpaces = MAX_PARKING_SPACES;

unordered_map<string, string> users;
unordered_map<string, int> parkedCars;

ReservationList reservations;

bool isValidNumber(const string& str) {
    for (char c : str) {
        if (!isdigit(c)) {
            return false;
        }
    }
    return true;
}

int getValidNumber() {
    string input;
    cin >> input;
    while (!isValidNumber(input)) {
        cout << "Invalid input. Please enter a number: ";
        cin >> input;
    }
    return stoi(input);
}

void registerUser() {
    string username, password, pin;
    cin.ignore(); 
    do {
        cout << "Enter new username (must be 8 characters long): ";
        getline(cin, username); 

        if (username.length() < 8) {
            cout << "Username must be at least 8 characters long. Try again." << endl;
        }
    } while (username.length() < 8);

    do {
        cout << "Enter PIN password (4 digits): ";
        getline(cin, pin);

        bool validPin = (pin.length() == 4);
        for (char c : pin) {
            if (!isdigit(c)) {
                validPin = false;
                break;
            }
        }

        if (!validPin) {
            cout << "PIN must be 4 digits. Try again." << endl;
        }
    } while (pin.length() != 4 || !isdigit(pin[0]) || !isdigit(pin[1]) || !isdigit(pin[2]) || !isdigit(pin[3]));

    password = username + pin;

    if (users.find(username) == users.end()) {
        users[username] = password;
        cout << "User registered successfully!" << endl;
    } else {
        cout << "Username already exists!" << endl;
    }
}

bool loginUser(string &loggedInUser) {
    string username, password, pin;
    cin.ignore(); 
    do {
        cout << "Enter your username: ";
        getline(cin, username); 

        if (username.length() < 8){
            cout << "Username must be at least 8 characters long. Try again." << endl;
        }
    } while (username.length() < 8);

    do {
        cout << "Enter PIN password (4 digits): ";
        getline(cin, pin);

        bool validPin = (pin.length() == 4);
        for (char c : pin) {
            if (!isdigit(c)) {
                validPin = false;
                break;
            }
        }

        if (!validPin) {
            cout << "PIN must be 4 digits. Try again." << endl;
        }
    } while (pin.length() != 4 || !isdigit(pin[0]) || !isdigit(pin[1]) || !isdigit(pin[2]) || !isdigit(pin[3]));

    password = username + pin;

    if (users.find(username) != users.end() && users[username] == password) {
        cout << "Login successful!" << endl;
        loggedInUser = username;
        return true;
    } else {
        cout << "Login failed!" << endl;
        return false;
    }
}

void parkCar(string loggedInUser) {
    if (availableSpaces <= 0) {
        cout << "All parking spaces are full." << endl;
        return;
    }

    cout << "Available parking spaces:" << endl;
    for (int i = 0; i < MAX_PARKING_SPACES; ++i) {
        if (parkingSpaces[i] == "Open") {
            cout << "Space " << (i + 1) << endl;
        }
    }

    int spaceNumber;
    cout << "Enter space number to park (1-15): ";
    spaceNumber = getValidNumber();

    while (spaceNumber < 1 || spaceNumber > 15 || parkingSpaces[spaceNumber - 1] != "Open") {
        if (parkingSpaces[spaceNumber - 1] != "Open") {
            cout << "This space is not available. Please choose another space: ";
        } else {
            cout << "Invalid space number. Enter another space number (1-15): ";
        }
        spaceNumber = getValidNumber();
    }

    parkingSpaces[spaceNumber - 1] = "Occupied";
    parkedCars[loggedInUser] = spaceNumber;
    availableSpaces--;
    cout << "Car parked successfully in space " << spaceNumber << ". " << availableSpaces << " parking spaces remaining." << endl;
}

void unparkCar(string loggedInUser) {
    if (parkedCars.find(loggedInUser) == parkedCars.end()) {
        cout << "You do not have a car parked." << endl;
        return;
    }

    int spaceNumber = parkedCars[loggedInUser];
    cout << "Your car is parked at space " << spaceNumber << "!" << endl;
    cout << "Would you like to unpark your car? [Y/N]: ";
    char choice;
    cin >> choice;
    if (choice == 'Y' || choice == 'y') {
        parkingSpaces[spaceNumber - 1] = "Open";
        parkedCars.erase(loggedInUser);
        availableSpaces++;
        cout << "Car unparked from space " << spaceNumber << ". " << availableSpaces << " parking spaces remaining." << endl;
    } else {
        cout << "Unparking cancelled." << endl;
    }
}

void checkAvailableSpaces() {
    for (int i = 0; i < MAX_PARKING_SPACES; ++i) {
        cout << "Space " << (i + 1) << ": " << parkingSpaces[i] << endl;
    }
}

void createReservation() {
    if (availableSpaces <= 0) {
        cout << "All parking spaces are full. No more reservations can be made." << endl;
        return;
    }

    string user;
    int spaceNumber, plateNumber;

    cout << "Enter username: ";
    cin.ignore(); 
    getline(cin, user); 
    if (users.find(user) == users.end()) {
        cout << "User not registered. Please register first." << endl;
        return;
    }

    cout << "Enter space number (1-15): ";
    spaceNumber = getValidNumber();

    while (spaceNumber < 1 || spaceNumber > 15 || parkingSpaces[spaceNumber - 1] != "Open") {
        if (parkingSpaces[spaceNumber - 1] != "Open") {
            cout << "This parking slot is not available at the moment. Please choose another space: " ;
        } else {
            cout << "Invalid space number. Enter another space number (1-15): ";
        }
        spaceNumber = getValidNumber();
    }

    cout << "Enter Car Plate Number: ";
    plateNumber = getValidNumber();

    reservations.addReservation(user, spaceNumber, plateNumber);
    parkingSpaces[spaceNumber - 1] = "Reserved";
    availableSpaces--;
    cout << "Reservation created successfully! " << availableSpaces << " parking spaces remaining." << endl;
}

void readReservations() {
    reservations.printReservations();
}

void updateReservation() {
    string username;
    int newSpaceNumber, enteredPlateNumber;

    cout << "Enter your username: ";
    cin.ignore();
    getline(cin, username);

    cout << "Enter your original plate number: ";
    enteredPlateNumber = getValidNumber();

    Reservation* reservation = reservations.findReservationByUser(username, enteredPlateNumber);
    if (!reservation) {
        cout << "No reservation found for the given username and plate number." << endl;
        return;
    }

    cout << "Enter new space number (1-15): ";
    newSpaceNumber = getValidNumber();
    while (newSpaceNumber < 1 || newSpaceNumber > 15 || parkingSpaces[newSpaceNumber - 1] != "Open") {
        if (parkingSpaces[newSpaceNumber - 1] != "Open") {
            cout << "This parking slot is not available at the moment. Please choose another space: ";
        } else {
            cout << "Invalid space number. Enter another space number (1-15): ";
        }
        newSpaceNumber = getValidNumber();
    }

    parkingSpaces[reservation->spaceNumber - 1] = "Open";
    reservations.updateReservation(username, newSpaceNumber, reservation->plateNumber);
    parkingSpaces[newSpaceNumber - 1] = "Reserved";
}

void deleteReservation() {
    string username;
    int enteredPlateNumber;

    cout << "Enter your username: ";
    cin.ignore();
    getline(cin, username);

    cout << "Enter your plate number: ";
    enteredPlateNumber = getValidNumber();

    Reservation* reservation = reservations.findReservationByUser(username, enteredPlateNumber);
    if (!reservation) {
        cout << "No reservation found for the given username and plate number." << endl;
        return;
    }

    int spaceNumber = reservation->spaceNumber;
    reservations.removeReservation(username, enteredPlateNumber);
    parkingSpaces[spaceNumber - 1] = "Open";
    availableSpaces++;
    cout << "Reservation deleted successfully!" << endl;
}


void showMainMenu() {
    cout << "_____________________________________________" << endl;
    cout << "\nWelcome to Park Smart. Quick Park!" << endl;
    cout << "\n1. Parking Menu" << endl;
    cout << "2. Reservation Menu" << endl;
    cout << "3. Back to Login" << endl;
    cout << "\n________________________________________________________" << endl;
}

void showParkingMenu() {
    cout << "_____________________________________________";
    cout << "\nParking Menu:";
    cout << "\n1. Park Car";
    cout << "\n2. Unpark Car";
    cout << "\n3. Check Available Spaces";
    cout << "\n4. Back to Main Menu";
    cout << "\n_______________________________________________________";
}

void showReservationMenu() {
    cout << "_______________________________________________";
    cout << "\nReservation Menu:";
    cout << "\n1. Create Reservation";
    cout << "\n2. Read Reservations";
    cout << "\n3. Update Reservation";
    cout << "\n4. Delete Reservation";
    cout << "\n5. Sort Reservations";
    cout << "\n6. Back to Main Menu";
    cout << "\n_________________________________________________________";
}

int main() {
    for (int i = 0; i < MAX_PARKING_SPACES; ++i) {
        parkingSpaces[i] = "Open";
    }

    int choice;
    bool loggedIn = false;
    string loggedInUser;

    do {
        if (!loggedIn) {
            cout << "_________________________" << endl;
            cout << "\nUser Registration!" << endl;
            cout << "1. Register" << endl;
            cout << "2. Login" << endl;
            cout << "3. Exit" << endl;
            cout << "\n__________________________________" << endl;
            cout << "Enter choice: ";
            choice = getValidNumber();

            switch (choice) {
                case 1:
                    registerUser();
                    break;
                case 2:
                    loggedIn = loginUser(loggedInUser);
                    break;
                case 3:
                    cout << "Exiting..." << endl;
                    return 0;
                default:
                    cout << "Invalid choice!" << endl;
            }

            if (loggedIn) {
                cout << "Proceeding to main menu..." << endl;
            }
        } else {
            showMainMenu();
            cout << "\nEnter choice: ";
            choice = getValidNumber();

            switch (choice) {
                case 1:
                    int parkingChoice;
                    do {
                        showParkingMenu();
                        cout << "\nEnter choice: ";
                        parkingChoice = getValidNumber();

                        switch (parkingChoice) {
                            case 1:
                                parkCar(loggedInUser);
                                break;
                            case 2:
                                unparkCar(loggedInUser);
                                break;
                            case 3:
                                checkAvailableSpaces();
                                break;
                            case 4:
                                cout << "Returning to main menu..." << endl;
                                break;
                            default:
                                cout << "Invalid choice!" << endl;
                        }
                    } while (parkingChoice != 4);
                    break;
                case 2:
                    int reservationChoice;
                    do {
                        showReservationMenu();
                        cout << "\nEnter choice: ";
                        reservationChoice = getValidNumber();

                        switch (reservationChoice) {
                            case 1:
                                createReservation();
                                break;
                            case 2:
                                readReservations();
                                break;
                            case 3:
                                updateReservation();
                                break;
                            case 4:
                                deleteReservation();
                                break;
                            case 5:
                                reservations.sortReservations();
                                cout << "Reservations sorted by space number!" << endl;
                                break;
                            case 6:
                                cout << "Returning to main menu..." << endl;
                                break;
                            default:
                                cout << "Invalid choice!" << endl;
                        }
                    } while (reservationChoice != 6);
                    break;
                case 3:
                    loggedIn = false;
                    cout << "Logged out successfully!" << endl;
                    break;
                default:
                    cout << "Invalid choice!" << endl;
            }
        }
    } while (true);

    return 0;
}
