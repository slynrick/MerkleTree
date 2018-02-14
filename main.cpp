#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <chrono>
#include <sstream>
#include <vector>
#include <random>

#include "src/merkletree.h"

using namespace std;
using namespace std::chrono;

vector<string> creatingData( int len )// cria a quantidade de dados pedida com o mesmo tamanho pedido em cada dado
{
    vector<string> data;
    stringstream ss;
    for( int i = 0; i < len; ++i )
    {
        ss << (int)pow( 10, len ) + i;
        string teste = ss.str();
        data.push_back( teste );
    }

    return data;
}

MerkleTree* creatingMerkleTree( vector<string> data )
{
    cout << "Contabilizando tempo para criação de uma MerkleTree" << endl;
    srand (time(NULL));
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    MerkleTree * tree = new MerkleTree();
    tree->build( data );
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
    cout << "Número de folhas: " << data.size() << " Tempo estimado: " << time_span.count() << "s" << endl << endl;
    return tree;
}

void validatingMerkleTree( vector<MerkleTree*> trees )
{
    cout << "Contabilizando tempo para validação de arvores MerkleTree criadas acima" << endl;
    for( int i = 0; i < trees.size(); ++i )
    {
        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        trees[i]->isValid();
        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
        cout << "Arvore : " << i << " Tempo estimado: " << time_span.count() << "s" << endl << endl;
    }
}

void syncMerkleTree( vector<MerkleTree*> trees )
{
    cout << "Contabilizando tempo para sincronização entre MerkleTrees criadas acima com a última criada" << endl;
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


void insertDataMerkleTree( vector<MerkleTree*> trees , string data )
{
    cout << "Contabilizando tempo para inserção de dados na MerkleTrees" << endl;
    for( int i = 0; i < trees.size(); ++i )
    {
        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        trees[i]->insert( data.c_str(), data.length() );
        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
        cout << "Arvore : " << i << " Tempo estimado: " << time_span.count() << "s" << endl << endl;
    }
}

void deleteDataMerkleTree( vector<MerkleTree*> trees, string hash )
{
    cout << "Contabilizando tempo para deletar dados na MerkleTrees" << endl;
    for( int i = 0; i < trees.size(); ++i )
    {
        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        trees[i]->remove( hash );
        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
        cout << "Arvore : " << i << " Tempo estimado: " << time_span.count() << "s" << endl << endl;
    }
}

void searchDataMerkleTree( vector<MerkleTree*> trees, string hash )
{
    cout << "Contabilizando tempo para procurar dados na MerkleTrees" << endl;
    for( int i = 0; i < trees.size(); ++i )
    {
        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        trees[i]->search( hash );
        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
        cout << "Arvore : " << i << " Tempo estimado: " << time_span.count() << "s" << endl << endl;
    }
}

int main( int argv, char** argc )
{
    cout << "Iniciando testes" << endl << endl;
    vector<MerkleTree*> trees;

    for( int i = 1; i < 50; ++i )
        trees.push_back( creatingMerkleTree( creatingData( i ) ) );

    validatingMerkleTree( trees );
    syncMerkleTree( trees );
    cout << "Fim dos testes" << endl << endl;
    return 0;
}
