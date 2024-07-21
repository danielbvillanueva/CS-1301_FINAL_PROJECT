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

    void removeReservation(string user) {
        if (!head) return;

        if (head->user == user) {
            Reservation* toDelete = head;
            head = head->next;
            delete toDelete;
            return;
        }

        Reservation* temp = head;
        while (temp->next && temp->next->user != user) {
            temp = temp->next;
        }

        if (temp->next) {
            Reservation* toDelete = temp->next;
            temp->next = temp->next->next;
            delete toDelete;
        }
    }

    Reservation* findReservationByUser(string user) {
        Reservation* temp = head;
        while (temp) {
            if (temp->user == user) {
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

    void updateReservation(string user, int newSpaceNumber, int newPlateNumber) {
        Reservation* reservation = findReservationByUser(user);
        if (reservation) {
            reservation->updateReservation(newSpaceNumber, newPlateNumber);
            cout << "Reservation updated successfully!" << endl;
        } else {
            cout << "Reservation not found for user " << user << "." << endl;
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
    do{
        cout << "Enter new username (at least 8 characters): ";
        cin.ignore(); // Ignore any leftover newline character in the input buffer
        getline(cin, username); // Use getline to allow spaces

        if (username.length() <= 8) {
            cout << "Username must be at least 8 characters long. Try again." << endl;
        }
    } while (username.length() <= 8);
    
    cout << "Enter PIN password (4 digits): ";
    getline(cin, pin);
    
    if (pin.length() != 4 || !isdigit(pin[0]) || !isdigit(pin[1]) || !isdigit(pin[2]) || !isdigit(pin[3])){
        cout << "PIN must be 4 digits." << endl;
    }
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
    do{
        cout << "Enter new username (at least 8 characters): ";
        cin.ignore(); // Ignore any leftover newline character in the input buffer
        getline(cin, username); // Use getline to allow spaces

        if (username.length() <= 8) {
            cout << "Username must be at least 8 characters long. Try again." << endl;
        }
    } while (username.length() <= 8);
    
    cout << "Enter PIN password (4 digits): ";
    getline(cin, pin);
    
    if (pin.length() != 4 || !isdigit(pin[0]) || !isdigit(pin[1]) || !isdigit(pin[2]) || !isdigit(pin[3])){
        cout << "PIN must be 4 digits." << endl;
    }
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
            cout << "This space is not available. Please choose another space." << endl;
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
            cout << "This parking slot is not available at the moment. Please choose another space." << endl;
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
    int newSpaceNumber, newPlateNumber;

    cout << "Enter your username: ";
    cin.ignore(); 
    getline(cin, username); 
    Reservation* reservation = reservations.findReservationByUser(username);
    if (!reservation) {
        cout << "You do not have a reservation." << endl;
        return;
    }

    cout << "Enter new space number (1-15): ";
    newSpaceNumber = getValidNumber();
    while (newSpaceNumber < 1 || newSpaceNumber > 15 || parkingSpaces[newSpaceNumber - 1] != "Open") {
        if (parkingSpaces[newSpaceNumber - 1] != "Open") {
            cout << "This parking slot is not available at the moment. Please choose another space." << endl;
        } else {
            cout << "Invalid space number. Enter another space number (1-15): ";
        }
        newSpaceNumber = getValidNumber();
    }

    cout << "Enter new Car Plate Number: ";
    newPlateNumber = getValidNumber();

    reservations.updateReservation(username, newSpaceNumber, newPlateNumber);
    parkingSpaces[reservation->spaceNumber - 1] = "Open";
    parkingSpaces[newSpaceNumber - 1] = "Reserved";
    cout << "Reservation updated successfully!" << endl;
}

void deleteReservation() {
    string username;
    cout << "Enter your username: ";
    cin.ignore();
    getline(cin, username); 
    Reservation* reservation = reservations.findReservationByUser(username);
    if (!reservation) {
        cout << "You do not have a reservation." << endl;
        return;
    }

    reservations.removeReservation(username);
    parkingSpaces[reservation->spaceNumber - 1] = "Open";
    availableSpaces++;
    cout << "Reservation deleted successfully!" << endl;
}
