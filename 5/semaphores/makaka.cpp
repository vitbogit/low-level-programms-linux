#include <iostream>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>

#define THINKING_TIME 1
#define CROSSING_TIME 2

sem_t west, east, mutex;

void* baboon(void* arg) {
    char direction = *((char*) arg);
    while (true) {
        // Получение направления от пользователя
        std::cout << "Baboon goes to";
        

        // Запрос на пересечение каньона
        sem_wait(&mutex);
        if (direction == 'l') {
            sem_wait(&west);
            std::cout << "Baboon starts crossing the canyon to the left." << std::endl;
        } else if (direction == 'r') {
            sem_wait(&east);
            std::cout << "Baboon starts crossing the canyon to the right." << std::endl;
        }
        sem_post(&mutex);

        // Бабуин пересекает каньон
        sleep(CROSSING_TIME);

        // Бабуин заканчивает пересечение каньона
        sem_wait(&mutex);
        if (direction == 'l') {
            std::cout << "Baboon finished crossing the canyon to the left." << std::endl;
            sem_post(&west);
        } else if (direction == 'r') {
            std::cout << "Baboon finished crossing the canyon to the right." << std::endl;
            sem_post(&east);
        }
        sem_post(&mutex);
    }
}

int main() {
    pthread_t baboon_thread;

    // Инициализация семафоров
    sem_init(&west, 0, 1);
    sem_init(&east, 0, 1);
    sem_init(&mutex, 0, 1);

    // Бесконечный цикл для создания бабуинов
    while (true) {
        std::cout << "New baboon " << "will go (l/r): " << std::endl;
        char direction;
        std::cin >> direction;
        pthread_create(&baboon_thread, NULL, baboon, &direction);
    }

    // Удаление семафоров (не достижимо из-за бесконечного цикла)
    sem_destroy(&west);
    sem_destroy(&east);
    sem_destroy(&mutex);

    return 0;
}
