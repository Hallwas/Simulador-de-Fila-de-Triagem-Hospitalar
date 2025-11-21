#include <iostream>
#include <queue>
#include <vector>
#include <chrono>
#include <algorithm>
#include <iomanip>
#include <string>

using namespace std;
using namespace chrono;

struct Paciente {
    string id;
    char prior;
    minutes hora_chegada;
};

//Calcular o temp de espera
void calcular_espera(Paciente &p, int hora_atual_h, int min_atual_m, vector<minutes> &historico_espera) {
    hours h(hora_atual_h);
    minutes m(min_atual_m);
    minutes momento_atendimento = duration_cast<minutes>(h + m);
    minutes espera = momento_atendimento - p.hora_chegada;
    historico_espera.push_back(espera);
}

// Função para registrar chegada
void registrar(queue<Paciente> &vermelho, queue<Paciente> &amarelo, queue<Paciente> &verde, queue<Paciente> &branco, int &pico, int &lotacao_atual) {
    Paciente p;
    int h, m;
   
    cin >> p.id >> p.prior >> h >> m;

    hours hora(h);
    minutes minuto(m);
    p.hora_chegada = duration_cast<minutes>(hora + minuto);

    if (p.prior == 'V') vermelho.push(p);
    else if (p.prior == 'A') amarelo.push(p);
    else if (p.prior == 'D') verde.push(p);
    else if (p.prior == 'B') branco.push(p);

    lotacao_atual++;
    if (lotacao_atual > pico) {
        pico = lotacao_atual;
    }
}

// Função para realizar atendimento
void atender(queue<Paciente> &vermelho, queue<Paciente> &amarelo, queue<Paciente> &verde, queue<Paciente> &branco, vector<minutes> &historico_espera, int &total_atendidos, int &cont_v, int &cont_a, int &cont_d, int &cont_b, int &lotacao_atual) {
   
    int h, m;
    cin >> h >> m;

    if (vermelho.empty() && amarelo.empty() && verde.empty() && branco.empty()) {
        cout << setfill('0') << setw(2) << h << " "
             << setfill('0') << setw(2) << m
             << " Sem pacientes aguardando atendimento" << endl;

    } else {
        Paciente p;
       
        if (!vermelho.empty()) {
            p = vermelho.front();
            vermelho.pop();
            cont_v++;
        } else if (!amarelo.empty()) {
            p = amarelo.front();
            amarelo.pop();
            cont_a++;
        } else if (!verde.empty()) {
            p = verde.front();
            verde.pop();
            cont_d++;
        } else {
            p = branco.front();
            branco.pop();
            cont_b++;
        }


        calcular_espera(p, h, m, historico_espera);
        total_atendidos++;
        lotacao_atual--;
    }
}

// Função para exibir status
void infos(queue<Paciente> &vermelho, queue<Paciente> &amarelo, queue<Paciente> &verde, queue<Paciente> &branco, int total_atendidos) {
    cout << "V:" << vermelho.size() << " A:" << amarelo.size() << " D:" << verde.size() << " B:" << branco.size() << " | Atendidos:" << total_atendidos << endl;
}

// Função para relatório final
void finalizar(int total_atendidos, int cont_v, int cont_a, int cont_d, int cont_b, int pico, vector<minutes> &historico_espera) {
    int maior_espera = 0;
   
    if (!historico_espera.empty()){
        auto it = max_element(historico_espera.begin(), historico_espera.end());
        maior_espera = it->count();
    }

    cout << "--- RELATORIO FINAL ---" << endl;
    cout << "Total atendidos: " << total_atendidos << endl;
    cout << "Por prioridade: V=" << cont_v << " A=" << cont_a << " D=" << cont_d << " B=" << cont_b << endl;
    cout << "Pico de lotacao: " << pico << endl;
    cout << "Espera maxima: " << maior_espera << " min" << endl;
}

int main() {
    queue<Paciente> vermelho;
    queue<Paciente> amarelo;
    queue<Paciente> verde;
    queue<Paciente> branco;

    vector<minutes> historico_espera;

    int total_atendidos = 0;
    int cont_v = 0, cont_a = 0, cont_d = 0, cont_b = 0;
    int pico = 0, lotacao_atual = 0;

    char comando;

    cout << "--- COMANDOS ---" << endl;
    cout << "C <senha> <prior> <hh> <mm>    - Chegada de um paciente." << endl;
    cout << "A <hh> <mm>                    - Atendimento de um paciente." << endl;
    cout << "D                              - Exibe o estado atual das filas." << endl;
    cout << "Q                              - Encerra." << endl;
    cout << "Prioridades: V = Vermelho | A = Amarelo | D = Verde | B = Branco" << endl;

    while (cin >> comando && comando != 'Q') {
        if (comando == 'C') {
            registrar(vermelho, amarelo, verde, branco, pico, lotacao_atual);
        }
        else if (comando == 'A') {
            atender(vermelho, amarelo, verde, branco, historico_espera, total_atendidos, cont_v, cont_a, cont_d, cont_b, lotacao_atual);
        }
        else if (comando == 'D') {
            infos(vermelho, amarelo, verde, branco, total_atendidos);
        }
    }
   
    finalizar(total_atendidos, cont_v, cont_a, cont_d, cont_b, pico, historico_espera);

    return 0;
}