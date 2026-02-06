#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <vector>
#include <cstdlib>
#include <fcntl.h>       // Добавляем для O_CREAT, O_EXCL
#include <sys/stat.h>    // Добавляем для прав доступа

const int NBUFF = 10;
const int NITEMS = 20;

struct SharedData {
    std::vector<int> buff;
    int put_index;
    int get_index;
    sem_t *mutex;
    sem_t *nempty;
    sem_t *nstored;
    
    SharedData() : buff(NBUFF, 0), put_index(0), get_index(0) {}
};

SharedData shared;

void* produce(void* arg) {
    for (int i = 0; i < NITEMS; i++) {
        sem_wait(shared.nempty);
        sem_wait(shared.mutex);
        
        shared.buff[shared.put_index] = i;
        std::cout << "Producer: put item " << i << " at position " << shared.put_index << std::endl;
        shared.put_index = (shared.put_index + 1) % NBUFF;
        
        sem_post(shared.mutex);
        sem_post(shared.nstored);
        
        usleep(100000);
    }
    return nullptr;
}

void* consume(void* arg) {
    for (int i = 0; i < NITEMS; i++) {
        sem_wait(shared.nstored);
        sem_wait(shared.mutex);
        
        int item = shared.buff[shared.get_index];
        std::cout << "Consumer: got item " << item << " from position " << shared.get_index << std::endl;
        shared.get_index = (shared.get_index + 1) % NBUFF;
        
        sem_post(shared.mutex);
        sem_post(shared.nempty);
        
        usleep(150000);
    }
    return nullptr;
}

int main() {
    pthread_t producer_thread, consumer_thread;
    
    // Создаем именованные семафоры
    shared.mutex = sem_open("/mutex_sem", O_CREAT | O_EXCL, 0666, 1);
    shared.nempty = sem_open("/nempty_sem", O_CREAT | O_EXCL, 0666, NBUFF);
    shared.nstored = sem_open("/nstored_sem", O_CREAT | O_EXCL, 0666, 0);
    
    // Проверяем создание семафоров
    if (shared.mutex == SEM_FAILED || shared.nempty == SEM_FAILED || shared.nstored == SEM_FAILED) {
        std::cerr << "Error creating semaphores" << std::endl;
        // Если семафоры уже существуют, удаляем и пробуем снова
        sem_unlink("/mutex_sem");
        sem_unlink("/nempty_sem");
        sem_unlink("/nstored_sem");
        
        shared.mutex = sem_open("/mutex_sem", O_CREAT, 0666, 1);
        shared.nempty = sem_open("/nempty_sem", O_CREAT, 0666, NBUFF);
        shared.nstored = sem_open("/nstored_sem", O_CREAT, 0666, 0);
        
        if (shared.mutex == SEM_FAILED || shared.nempty == SEM_FAILED || shared.nstored == SEM_FAILED) {
            std::cerr << "Failed to create semaphores after cleanup" << std::endl;
            return -1;
        }
    }
    
    std::cout << "Starting producer-consumer simulation..." << std::endl;
    std::cout << "Buffer size: " << NBUFF << std::endl;
    std::cout << "Items to produce: " << NITEMS << std::endl;
    
    // Создаем потоки
    if (pthread_create(&producer_thread, nullptr, produce, nullptr) != 0) {
        std::cerr << "Error creating producer thread" << std::endl;
        return -1;
    }
    
    if (pthread_create(&consumer_thread, nullptr, consume, nullptr) != 0) {
        std::cerr << "Error creating consumer thread" << std::endl;
        return -1;
    }
    
    // Ждем завершения потоков
    pthread_join(producer_thread, nullptr);
    pthread_join(consumer_thread, nullptr);
    
    std::cout << "Simulation completed successfully!" << std::endl;
    
    // Закрываем и удаляем семафоры
    sem_close(shared.mutex);
    sem_close(shared.nempty);
    sem_close(shared.nstored);
    
    sem_unlink("/mutex_sem");
    sem_unlink("/nempty_sem");
    sem_unlink("/nstored_sem");
    
    return 0;
}
