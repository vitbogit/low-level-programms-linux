#include <iostream>
#include <thread> 
#include <semaphore.h>
#include <unistd.h> // sleep

using namespace std;

// Постановка задачи:
// - Можно послать бабуина на запад, если каньон пуст или в нем идет движение на запад
// - Можно послать бабуина на восток, если каньон пуст или в нем идет движение на восток
// - Можно "вытащить" одного бабуина из каньона, если в каньоне есть хотя бы 1 бабуин (направление неважно)

// Для решения понадобится запоминать:
// - состояние каньона 
// - количество бабуинов - семафор

// Состояние каньона
enum CanyonState {
    empty, // пуст
    west, // движение бабуинов на запад (>=1 макаки)
    east // движение бабуинов на восток (>=1 макаки)
};
CanyonState state = CanyonState::empty; // изначально пуст

// Define semaphores
sem_t sem_east, sem_west;

void baboon_cross_east() {
    // Wait for semaphore, i.e., check if any baboon is moving west
    sem_wait(&sem_west);

    // Simulate crossing
    std::cout << "Baboon is crossing east." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Signal that the baboon has finished crossing
    sem_post(&sem_east);
}

void baboon_cross_west() {
    // Wait for semaphore, i.e., check if any baboon is moving east
    sem_wait(&sem_east);

    // Simulate crossing
    std::cout << "Baboon is crossing west." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Signal that the baboon has finished crossing
    sem_post(&sem_west);
}

int main() {
    
    sem_init(&sem_east, 0, 1);
    sem_init(&sem_west, 0, 1);

    int option = 0;

    while (option != 4) {
        cout << "------------------------------------------" << endl;
        cout << "Список опций:" << endl;
        cout << "1) Бабуина запустить на восток" << endl;
        cout << "2) Бабуина запустить на запад" << endl;  
        cout << "3) Выход из программы" << endl;
        cin >> option;
        switch (option)
        {
            case 1: {
                std::thread east_thread(baboon_cross_east);
                east_thread.detach();
            }
                break;
            case 2: {
                std::thread west_thread(baboon_cross_west);
                west_thread.detach();
            }
                break;
            case 3:
                exit(0);
            default:
                cout << "НЕИЗВЕСТНАЯ ОПЦИЯ!" << endl;
                break;
        }
        sleep(1);
    }
    return 0;
}