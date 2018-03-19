#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <chrono>
#include <sstream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <fstream>

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

    ofstream buildTreeFile;
    buildTreeFile.open( "buildTreeFile.dat", ios::out );

    for( int i = 1; i < 100; ++i )
    {
        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        MerkleTree * tree = new MerkleTree();
        tree->build( creatingData( i ) );
        high_resolution_clock::time_point t2 = high_resolution_clock::now();

        trees.push_back( tree );

        duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
        cout << "Leaves: " << i << " duration: " << fixed << setprecision( PRECISION ) << time_span.count() << "s" << endl << endl;

        buildTreeFile << i << "\t" << time_span.count() << endl;
    }
    buildTreeFile.close();
    return trees;
}

void validatingMerkleTree( const vector<MerkleTree*> &trees )
{
    cout << "Time to validate all trees" << endl;

    ofstream validateTreeFile;
    validateTreeFile.open( "validateTreeFile.dat", ios::out );

    for( int i = 0; i < trees.size(); ++i )
    {
        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        trees[i]->isValid();
        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
        cout << "Tree : " << i << " duration: " << fixed << setprecision( PRECISION ) << time_span.count() << "s" << endl << endl;

        validateTreeFile << i << "\t" << time_span.count() << endl;
    }
    validateTreeFile.close();
}

void syncMerkleTree( const vector<MerkleTree*> &trees )
{
    cout << "Time to sync all trees" << endl;
    // sync all trees with the biggest one

    ofstream syncTreeFile;
    syncTreeFile.open( "syncTreeFile.dat", ios::out );

    for( int i = 0; i < trees.size(); ++i )
    {
        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        trees[i]->syncronize( trees.back() );
        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
        cout << "Tree : " << i << " duration: " << fixed << setprecision( PRECISION ) << time_span.count() << "s" << endl << endl;

        syncTreeFile << i << "\t" << time_span.count() << endl;
    }
    syncTreeFile.close();
}


void insertDataMerkleTree( const vector<MerkleTree*> &trees , string data )
{
    cout << "Time to insert data" << endl;

    ofstream insertDataTreeFile;
    insertDataTreeFile.open( "insertDataTreeFile.dat", ios::out );

    for( int i = 0; i < trees.size(); ++i )
    {
        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        trees[i]->insert( data );
        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
        cout << "Tree : " << i << " duration: " << fixed << setprecision( PRECISION ) << time_span.count() << "s" << endl << endl;

        insertDataTreeFile << i << "\t" << time_span.count() << endl;
    }
    insertDataTreeFile.close();
}

void deleteDataMerkleTree( const vector<MerkleTree*> &trees, string hash )
{
    cout << "Time to delete data" << endl;

    ofstream deleteDataTreeFile;
    deleteDataTreeFile.open( "deleteDataTreeFile.dat", ios::out );

    for( int i = 0; i < trees.size(); ++i )
    {
        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        bool find = trees[i]->remove( hash );
        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
        cout << "Tree : " << i << " duration: " << fixed << setprecision( PRECISION ) << time_span.count() << "s" << " found? " << find << endl << endl;

        deleteDataTreeFile << i << "\t" << time_span.count() << endl;
    }
    deleteDataTreeFile.close();
}

void searchDataMerkleTree( const vector<MerkleTree*> &trees, string hash )
{
    cout << "Time to search data" << endl;

    ofstream searchDataTreeFile;
    searchDataTreeFile.open( "searchDataTreeFile.dat", ios::out );

    for( int i = 0; i < trees.size(); ++i )
    {
        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        bool find = trees[i]->search( hash ) != NULL;
        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
        cout << "Tree : " << i << " duration: " << fixed << setprecision( PRECISION ) << time_span.count() << "s" << " found? " << find << endl << endl;

        searchDataTreeFile << i << "\t" << time_span.count() << endl;
    }
    searchDataTreeFile.close();
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
