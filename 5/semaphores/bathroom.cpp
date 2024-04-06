#include <iostream>
#include <semaphore>
#include <thread> 

using namespace std;

enum RoomState {
    empty,
    male,
    female
};

RoomState state = RoomState::empty;
binary_semaphore semaphore(1); 

void enter_bathroom(const std::string& gender) {
    cout << "Пользователь типа " << gender << " хочет войти в ванную комнату\n";
    
    semaphore.acquire();

    while((gender == "женщина" && state == RoomState::male) || (gender == "мужчина" && state == RoomState::female)) { 
        semaphore.release();
        semaphore.acquire();
    }
    
    if (gender == "мужчина") {
        state = RoomState::male;
    } else {
        state = RoomState::female;
    }
    cout << "Пользователь типа " << gender << " успешно вошел в ванную комнату\n";
    semaphore.release();
}

// acquire:
// Atomically decrements the internal counter by 1 if it is greater than ​0​;
// otherwise blocks until it is greater than ​0​ and can successfully decrement the internal counter.

// release:
// Atomically increments the internal counter by the value of update. 
// Any thread(s) waiting for the counter to be greater than ​0​, 
// such as due to being blocked in acquire, will subsequently be unblocked.

// detach:
// Separates the thread of execution from the thread object,
//  allowing execution to continue independently.
// Any allocated resources will be freed once the thread exits.
// 
// After calling detach *this no longer owns any thread.

void exit_all() {
    semaphore.acquire();

    state = RoomState::empty;
    cout << "Ванная комната освобождена" << endl;    
    semaphore.release();
}
int main() {
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
            std::thread female_thread(enter_bathroom, "женщина");
            female_thread.detach();
        }
        break;
        case 2: {
            std::thread male_thread(enter_bathroom, "мужчина");
            male_thread.detach();
        }
        break;
        case 3: {
            exit_all();
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