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
