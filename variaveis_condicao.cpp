#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <chrono>

using namespace std;

mutex mtx;
condition_variable cv;

int leitores = 0;
bool escrevendo = false;

void leitor(int id) {

    unique_lock<mutex> lock(mtx);

    // Espera caso exista escritor
    cv.wait(lock, [] {
        return !escrevendo;
    });

    leitores++;

    cout << "Leitor " << id
         << " entrou. Leitores ativos: "
         << leitores << endl;

    lock.unlock();

    // Simula leitura
    this_thread::sleep_for(chrono::seconds(2));

    lock.lock();

    leitores--;

    cout << "Leitor " << id
         << " saiu. Leitores restantes: "
         << leitores << endl;

    if (leitores == 0) {
        cv.notify_all();
    }
}

void escritor(int id) {

    unique_lock<mutex> lock(mtx);

    // Espera até não existir leitores
    cv.wait(lock, [] {
        return leitores == 0 && !escrevendo;
    });

    escrevendo = true;

    cout << ">>> Escritor "
         << id
         << " escrevendo..." << endl;

    lock.unlock();

    // Simula escrita
    this_thread::sleep_for(chrono::seconds(3));

    lock.lock();

    escrevendo = false;

    cout << ">>> Escritor "
         << id
         << " terminou." << endl;

    cv.notify_all();
}

int main() {

    vector<thread> threads;

    // Leitores
    threads.push_back(thread(leitor, 1));
    threads.push_back(thread(leitor, 2));
    threads.push_back(thread(leitor, 3));

    // Escritor
    threads.push_back(thread(escritor, 1));

    // Mais leitores
    threads.push_back(thread(leitor, 4));
    threads.push_back(thread(leitor, 5));

    for (auto &t : threads) {
        t.join();
    }

    return 0;
}
