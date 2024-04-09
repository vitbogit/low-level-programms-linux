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

sem_t semaphore;

void baboon_enters_canyon(const std::string& directionStr) {
    cout << "Бабуин хочет пойти на " << directionStr << "\n";

    // Конвертируем строку в енам
    CanyonState BaboonWantsToGo;
    if (directionStr == "запад"){
        BaboonWantsToGo = CanyonState::west;
    } else{
        BaboonWantsToGo = CanyonState::east;
    }

    // Хороший случай - получилось запустить бабуина
    if(state == CanyonState::empty){ // в пустой каньон
        sem_post(&semaphore);
        state = BaboonWantsToGo; // устанавливаем направление каньона сонаправлено вошедшему бабуину
        cout << "Начали заполнять каньон. +1 на " << directionStr << endl;
        return; // leave
    } else if(state == BaboonWantsToGo){ // в каньон с бабуинами, которые уже шли в одном с ним направлении
        sem_post(&semaphore);
        cout << "Добавляем в каньон. +1 на " << directionStr << endl;
        return; // leave
    }

    // Ждем пока каньон не освободится или там не наступит сонаправленное направление
    while(state != BaboonWantsToGo || state != CanyonState::empty){
        sem_post(&semaphore);
        sem_wait(&semaphore);
    }
    
    sem_post(&semaphore);
    if(state == CanyonState::empty) state = BaboonWantsToGo;
    cout << "Дождались. Добавляем в каньон. +1 на " << directionStr << endl;
}

void baboon_leaves_canyon() {
    sem_wait(&semaphore);
    cout << "-1 бабуинчик" << endl;
}

int main() {
    sem_init(&semaphore, 0, 1);
    int option = 0;

    while (option != 4) {
        cout << "------------------------------------------" << endl;
        cout << "Список опций:" << endl;
        cout << "1) Бабуина запустить на восток" << endl;
        cout << "2) Бабуина запустить на запад" << endl;  
        cout << "3) Бабуину покинуть каньон" << endl;
        cout << "4) Выход из программы" << endl;
        cin >> option;
        switch (option)
        {
            case 1: {
                std::thread east_thread(baboon_enters_canyon, "восток");
                east_thread.detach();
            }
                break;
            case 2: {
                std::thread west_thread(baboon_enters_canyon, "запад");
                west_thread.detach();
            }
                break;
            case 3: {
                baboon_leaves_canyon();
            }
                break;
            case 4:
                break;
            default:
                cout << "НЕИЗВЕСТНАЯ ОПЦИЯ!" << endl;
                break;
        }
        sleep(1);
    }
    return 0;
}