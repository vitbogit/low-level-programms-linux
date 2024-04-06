#include <iostream>
#include <semaphore>
#include <thread> 

using namespace std;

enum CanyonState {
    empty,
    west,
    east
};

CanyonState state = CanyonState::empty;
std::counting_semaphore semaphore(1);
// sem_t semaphore; 
// sem_init(&semaphore, 0, 1); 

void enter_canyon(const std::string& direction) {
    cout << "Бабуин хочет пойти на " << direction << "\n";
    
    semaphore.acquire();

    while((direction == "запад" && state == CanyonState::east) || (direction == "восток" && state == CanyonState::west)) { 
        semaphore.release();
        semaphore.acquire();
    }
    
    if (direction == "восток") {
        state = CanyonState::east;
    } else {
        state = CanyonState::west;
    }
    cout << "Бабуин пошел на " << direction << "\n";
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

void exit_one() {
    semaphore.acquire();
}

int main() {
    int option = 0;

    while (option != 4) {
        sleep(1);
        cout << "------------------------------------------" << endl;
       cout << "Список опций:" << endl;
       cout << "1) Бабуина запустить на восток" << endl;
       cout << "2) Бабуина запустить на запад" << endl;  
       cout << "3) Бабуину одному пройти каньон до конца и выйти" << endl;
       cout << "4) Выход из программы" << endl;
       cin >> option;
       switch (option)
       {
        case 1: {
            std::thread east_thread(enter_canyon, "восток");
            east_thread.detach();
        }
        break;
        case 2: {
            std::thread west_thread(enter_canyon, "запад");
            west_thread.detach();
        }
        break;
        case 3: {
            exit_one();
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