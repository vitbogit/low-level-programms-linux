#include <iostream>
#include <mutex>
#include <condition_variable>
#include <thread>

using namespace std;

enum RoomState {
    empty,
    male,
    female
};

class BathroomMonitor {
public:
    std::mutex mutex;
    std::condition_variable cv;
    RoomState state = RoomState::empty;

    void enter_bathroom(const std::string& gender) {
        
        cout << "Пользователь типа " << gender << " хочет войти в ванную комнату\n";
        
        std::unique_lock<std::mutex> lock(mutex);

        while((gender == "женщина" && state == RoomState::male) || (gender == "мужчина" && state == RoomState::female)) { 
            cv.wait(lock);
        }
        
        if (gender == "мужчина") {
            state = RoomState::male;
        } else {
            state = RoomState::female;
        }
        cout << "Пользователь типа " << gender << " успешно вошел в ванную комнату\n";
        cv.notify_all();
    }

    void exit_all() {
        unique_lock<std::mutex> lock(mutex);

        state = RoomState::empty;
        cout << "Ванная комната освобождена" << endl;    
        cv.notify_all();    
    }
};

int main() {

    BathroomMonitor monitor;
    int option = 0;

    while (option != 4) {
       cout << "Список опций:" << endl;
       cout << "1) Зайти женщине" << endl;
       cout << "2) Зайти мужчине" << endl;  
       cout << "3) Выгнать всех" << endl;
       cout << "4) Выход из программы" << endl;
       cin >> option;
       switch (option)
       {
        case 1: {
            std::thread female_thread(&BathroomMonitor::enter_bathroom, &monitor, "женщина");
            female_thread.detach();
        }
        break;
        case 2: {
            std::thread male_thread(&BathroomMonitor::enter_bathroom, &monitor, "мужчина");
            male_thread.detach();
        }
        break;
        case 3: {
            monitor.exit_all();
        }
        break;
        case 4:
        break;
        default:
            cout << "НЕИЗВЕСТНАЯ ОПЦИЯ!" << endl;
        break;
       }
    }
    return 0;
}