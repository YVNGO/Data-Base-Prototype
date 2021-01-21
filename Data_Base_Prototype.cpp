#include <iostream>
#include <string>
#include <map>
#include <set>
#include <sstream>
#include <iomanip>

using namespace std;

class Date {
public:
    Date(int new_year, int new_month, int new_day) {
        if (1 > new_month || 12 < new_month) {
            throw invalid_argument("Month value is invalid: " + to_string(new_month));
        }
        if (1 > new_day || 31 < new_day) {
            throw invalid_argument("Day value is invalid: " + to_string(new_day));
        }
        year = new_year;
        month = new_month;
        day = new_day;
    }
    int GetYear() const {
        return year;
    }
    int GetMonth() const {
        return month;
    }
    int GetDay() const {
        return day;
    }
private:
    int year=1;
    int month=1;
    int day=1;
};

bool operator<(const Date& lhs, const Date& rhs) {
    if (lhs.GetYear() != rhs.GetYear()) {
        return lhs.GetYear() < rhs.GetYear();
    }
    else if (lhs.GetMonth() != rhs.GetMonth()) {
        return lhs.GetMonth() < rhs.GetMonth();
    }
    else if (lhs.GetDay() != rhs.GetDay()) {
        return lhs.GetDay() < rhs.GetDay();
    }
    return false;
}

ostream& operator<<(ostream& stream, const Date& date) {
    stream << setfill('0');
    stream << setw(4) << date.GetYear() << "-" << setw(2) << date.GetMonth() << "-" << setw(2) << date.GetDay();
    return stream;
}

void PrintEventsOnDate(const set<string>& events)  {
    for (const string& e : events) {
        cout << e << endl;
    }
}

class Database {
public:
    void AddEvent(const Date& date, const string& event) {
        database[date].insert(event);
    }
    bool DeleteEvent(const Date& date, const string& event) {
        if (database.count(date) > 0 && database.at(date).count(event) > 0) {
            database[date].erase(event);
            return true;
        }
        return false;
    }
    int  DeleteDate(const Date& date) {
        int deleted_count = 0;
        if (database.count(date) > 0) {   
            deleted_count = database.at(date).size();
            database.erase(date);
        }
        return deleted_count;
    }

    void Find(const Date& date) const {
        if (database.count(date) > 0) {
            PrintEventsOnDate(database.at(date));
        }
    }

    void Print() const {       
        for (const auto& item : database) {
            for (const auto& event : item.second) {
                cout << item.first << " " << event << endl;
            }
        }
    }
private:
    map<Date, set<string>> database;
};

void CheckDateValidity(istream& input, int& year, int& month, int& day, string& date_str) {
    if (!(input >> year)) {
        throw invalid_argument("Wrong date format: " + date_str);
    }
    if ((char)input.peek() != '-') {
        throw invalid_argument("Wrong date format: " + date_str);
    }
    input.ignore(1);
    if (!(input >> month)) {
        throw invalid_argument("Wrong date format: " + date_str);
    }
    if ((char)input.peek() != '-') {
        throw invalid_argument("Wrong date format: " + date_str);
    }
    input.ignore(1);

    if (!(input >> day)) {
        throw invalid_argument("Wrong date format: " + date_str);
    }
}

Date ParseDate(const string& date) {
    istringstream date_stream(date);
    bool ok = true;

    int year;
    ok = ok && (date_stream >> year);
    ok = ok && (date_stream.peek() == '-');
    date_stream.ignore(1);

    int month;
    ok = ok && (date_stream >> month);
    ok = ok && (date_stream.peek() == '-');
    date_stream.ignore(1);

    int day;
    ok = ok && (date_stream >> day);
    ok = ok && date_stream.eof();

    if (!ok) {
        throw logic_error("Wrong date format: " + date);
    }
    return Date(year, month, day);
}

int main() {
    Database db;

    string command;
    while (getline(cin, command)) {
        // read commands from stdin 
        stringstream input(command);
        string operation;
        input >> operation;
        try {
            if (operation == "Add") {
                string date_str, event;
                input >> date_str >> event;
                const Date date = ParseDate(date_str);               
                db.AddEvent(date, event);                
            }
            else if (operation == "Find") {
                string date_str;
                input >> date_str;
                const Date date = ParseDate(date_str);
                db.Find(date);
            }
            else if (operation == "Del") {
                string date_str;
                input >> date_str;
                string event;
                if (!input.eof()) {
                    input >> event;
                }
                const Date date = ParseDate(date_str);
                if (event.empty()) {
                    const int count = db.DeleteDate(date);
                    cout << "Deleted " << count << " events" << endl;
                }
                else {
                    if (db.DeleteEvent(date, event)) {
                        cout << "Deleted successfully" << endl;
                    }
                    else {
                        cout << "Event not found" << endl;
                    }
                }
            }
            else if (operation == "Print") {
                db.Print();
            }
            else {
                if (!operation.empty()) {
                    throw domain_error("Unknown command: " + operation);
                }
            }
        }catch (const exception& ex) {
            cout << ex.what() << endl;
            return 1;
        }
    }

    return 0;
}
