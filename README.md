# variaveis_condicao: explicação

Bibliotecas

#include <iostream> 
Permite usar:
cout: para imprimir na tela.

===============//===============

#include <thread>
Permite criar threads.
Exemplo:
thread t1(funcao);

===============//===============

#include <mutex>
Biblioteca do mutex. Usada para sincronização e exclusão mútua.

===============//===============

#include <condition_variable>
Permite usar variável de condição. Ela faz threads:
esperarem
acordarem

===============//===============

#include <vector>
Permite usar: vector, que é uma lista dinâmica.

===============//===============

#include <chrono>
Biblioteca de tempo. Usada com:
sleep_for
Namespace
using namespace std;

Evita escrever:
std::cout
std::thread
std::mutex

===============//===============

Mutex principal
mutex mtx;

Cria um mutex chamado mtx. Ele protege:
leitores
escritor
região crítica

===============//===============

Variável de condição
condition_variable cv;

Usada para:
fazer threads esperarem
acordar threads depois
Quantidade de leitores
int leitores = 0;

Conta quantos leitores estão lendo no momento.

===============//===============

Controle do escritor
bool escrevendo = false;

Indica se existe escritor escrevendo.

true → existe escritor
false → não existe
Função leitor
void leitor(int id)

===============//===============

Função executada pelas threads leitoras. void leitor(int id)

Recebe:

identificador do leitor

Trava o mutex
unique_lock<mutex> lock(mtx);

A thread pega o mutex.
Agora só ela pode acessar a região crítica.

Espera escritor terminar, caso ele exista
cv.wait(lock, [] {
    return !escrevendo;
});

Enquanto:

escrevendo == true
o leitor fica esperando.

Lambda
[] {
    return !escrevendo;
}

É uma função anônima. Ela verifica:
!escrevendo

ou seja:
“não existe escritor escrevendo?”
Incrementa leitores
leitores++;

Mais um leitor entrou.
Mostra mensagem
cout << "Leitor " << id

Imprime:
qual leitor entrou
quantidade de leitores
Libera mutex
lock.unlock();

O leitor não precisa mais bloquear outros leitores. Agora:

vários leitores podem ler juntos.
Simula leitura
this_thread::sleep_for(chrono::seconds(2));

Thread “dorme” por 2 segundos. Simula tempo de leitura.

Trava mutex novamente
lock.lock();

Antes de alterar:

leitores
precisa proteger novamente.

Leitor saiu
leitores--;
Diminui quantidade de leitores ativos.

Mensagem
cout << "Leitor " << id
Mostra saída do leitor.

Último leitor
if (leitores == 0)
Verifica se:
não existe mais nenhum leitor
Acorda threads

cv.notify_all();
Avisa:
escritores
leitores esperando
que o recurso foi liberado.

===============//===============

Função escritor
void escritor(int id)
Função executada pelo escritor.

Trava mutex
unique_lock<mutex> lock(mtx);

Escritor pega acesso exclusivo.

Espera leitores saírem
cv.wait(lock, [] {
    return leitores == 0 && !escrevendo;
});

O escritor só entra quando:
- não existem leitores
- não existe outro escritor

Marca escritor ativo
escrevendo = true;
Agora existe um escritor escrevendo.

Mensagem
cout << ">>> Escritor "
Mostra que escritor começou.

Libera mutex
lock.unlock();

Enquanto escreve:
não precisa segurar mutex o tempo todo
Simula escrita

this_thread::sleep_for(chrono::seconds(3));

Escritor escreve por 3 segundos.

Pega mutex novamente
lock.lock();
Para alterar variável compartilhada novamente.

Finaliza escrita
escrevendo = false;
Agora:

ninguém está escrevendo
Mensagem final
cout << ">>> Escritor "
Mostra que terminou.

Acorda threads
cv.notify_all();

Libera:
leitores esperando
escritores esperando

===============//===============

Main
int main()
Função principal.

Vetor de threads
vector<thread> threads;
Armazena todas as threads.

Cria leitores
threads.push_back(thread(leitor, 1));

Cria thread executando:
leitor(1)

Mais leitores
threads.push_back(thread(leitor, 2));
threads.push_back(thread(leitor, 3));
Agora existem vários leitores simultâneos.

Cria escritor
threads.push_back(thread(escritor, 1));
Cria uma thread escritora.
Novos leitores
threads.push_back(thread(leitor, 4));
threads.push_back(thread(leitor, 5));
Mais leitores entram depois.

Espera threads terminarem
t.join();
O programa espera cada thread terminar.

Loop
for (auto &t : threads)
Percorre todas as threads.

Final do programa
return 0;

Programa terminou corretamente.
===============//===============

===============//=============== Fluxo visual do programa ===============//===============

Leitores entram juntos
Leitor 1 entrou
Leitor 2 entrou
Leitor 3 entrou

Escritor espera
Escritor aguardando...

Leitores saem
Leitor 1 saiu
Leitor 2 saiu
Leitor 3 saiu
Escritor entra sozinho

>>> Escritor 1 escrevendo...
Escritor termina

>>> Escritor 1 terminou.
Novos leitores entram

Leitor 4 entrou
Leitor 5 entrou
