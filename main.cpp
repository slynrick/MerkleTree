#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <chrono>
#include <sstream>
#include <vector>
#include <random>

#include "src/markletree.h"

using namespace std;
using namespace std::chrono;

vector<MarkleTree*> timeCreatingMarkleTree()
{
    cout << "Contabilizando tempo para criação de uma MarkleTree" << endl;
    srand (time(NULL));
    vector<MarkleTree*> trees;
    stringstream ss;
    for( int i = 0; i < 5000; ++i )
    {
        if( i % 100 == 0 )
        {
            high_resolution_clock::time_point t1 = high_resolution_clock::now();
            MarkleTree * tree = new MarkleTree();
            tree->build( ss.str().c_str(), i );
            high_resolution_clock::time_point t2 = high_resolution_clock::now();
            duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
            trees.push_back( tree );
            cout << "Número de bytes do dato: " << i << " Tempo estimado: " << time_span.count() << "s" << endl << endl;
        }
        ss << rand() % 10;
    }
    return trees;
}

void timeValidatingMarkleTree( vector<MarkleTree*> trees )
{
    cout << "Contabilizando tempo para validação de arvores MarkleTree criadas acima" << endl;
    for( int i = 0; i < trees.size(); ++i )
    {
        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        trees[i]->isValid();
        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
        cout << "Arvore : " << i << " Tempo estimado: " << time_span.count() << "s" << endl << endl;
    }
}

void timeComparingMarkleTree( vector<MarkleTree*> trees )
{
    cout << "Contabilizando tempo para sincronização entre MarkleTrees criadas acima com a última criada" << endl;
    // utilizarei a maior arvore para comparar com todas
    for( int i = 0; i < trees.size(); ++i )
    {
        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        trees[i]->syncronize( trees.back() );
        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
        cout << "Arvore : " << i << " Tempo estimado: " << time_span.count() << "s" << endl << endl;
    }
}

int main( int argv, char** argc )
{
    cout << "Iniciando testes" << endl << endl;
    vector<MarkleTree*> trees = timeCreatingMarkleTree();
    timeValidatingMarkleTree( trees );
    timeComparingMarkleTree( trees );
    cout << "Fim dos testes" << endl << endl;
    return 0;
}
