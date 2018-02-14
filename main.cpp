#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <chrono>
#include <sstream>
#include <vector>
#include <random>
#include <iomanip>

#include "src/merkletree.h"

using namespace std;
using namespace std::chrono;

#define FIND_THIS "find this"
#define PRECISION 10

vector<string> creatingData( int len )
{
    if( len == 0 )
        return vector<string>();

    vector<string> data;
    stringstream ss;
    for( int i = 0; i < len - 1; ++i )
    {
        ss << (int)pow( 10, len ) + i;
        string teste = ss.str();
        data.push_back( teste );
    }
    data.push_back( FIND_THIS );

    return data;
}

vector<MerkleTree*> creatingMerkleTree()
{
    cout << "Time to build a merkletree" << endl;
    srand (time(NULL));
    vector<MerkleTree*> trees;
    for( int i = 1; i < 50; ++i )
    {
        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        MerkleTree * tree = new MerkleTree();
        tree->build( creatingData( i ) );
        high_resolution_clock::time_point t2 = high_resolution_clock::now();

        trees.push_back( tree );

        duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
        cout << "Leaves: " << i << " duration: " << fixed << setprecision( PRECISION ) << time_span.count() << "s" << endl << endl;
    }
    return trees;
}

void validatingMerkleTree( const vector<MerkleTree*> &trees )
{
    cout << "Time to validate all trees" << endl;
    for( int i = 0; i < trees.size(); ++i )
    {
        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        trees[i]->isValid();
        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
        cout << "Tree : " << i << " duration: " << fixed << setprecision( PRECISION ) << time_span.count() << "s" << endl << endl;
    }
}

void syncMerkleTree( const vector<MerkleTree*> &trees )
{
    cout << "Time to sync all trees" << endl;
    // sync all trees with the biggest one
    for( int i = 0; i < trees.size(); ++i )
    {
        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        trees[i]->syncronize( trees.back() );
        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
        cout << "Tree : " << i << " duration: " << fixed << setprecision( PRECISION ) << time_span.count() << "s" << endl << endl;
    }
}


void insertDataMerkleTree( const vector<MerkleTree*> &trees , string data )
{
    cout << "Time to insert data" << endl;
    for( int i = 0; i < trees.size(); ++i )
    {
        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        trees[i]->insert( data );
        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
        cout << "Tree : " << i << " duration: " << fixed << setprecision( PRECISION ) << time_span.count() << "s" << endl << endl;
    }
}

void deleteDataMerkleTree( const vector<MerkleTree*> &trees, string hash )
{
    cout << "Time to delete data" << endl;
    for( int i = 0; i < trees.size(); ++i )
    {
        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        bool find = trees[i]->remove( hash );
        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
        cout << "Tree : " << i << " duration: " << fixed << setprecision( PRECISION ) << time_span.count() << "s" << " found? " << find << endl << endl;
    }
}

void searchDataMerkleTree( const vector<MerkleTree*> &trees, string hash )
{
    cout << "Time to search data" << endl;
    for( int i = 0; i < trees.size(); ++i )
    {
        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        bool find = trees[i]->search( hash ) != -1;
        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
        cout << "Tree : " << i << " duration: " << fixed << setprecision( PRECISION ) << time_span.count() << "s" << " found? " << find << endl << endl;
    }
}

int main( int argv, char** argc )
{
    cout << "Initializing Test Cases" << endl << endl;
    vector<MerkleTree*> trees = creatingMerkleTree();

    validatingMerkleTree( trees );
    syncMerkleTree( trees );

    insertDataMerkleTree( trees, "data" );
    searchDataMerkleTree( trees, Hash::hash( FIND_THIS ) );
    deleteDataMerkleTree( trees, Hash::hash( FIND_THIS ) );

    cout << "End" << endl << endl;
    return 0;
}
