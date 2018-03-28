#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <chrono>
#include <sstream>
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

double creatingMerkleTree( MerkleTree * tree, int len )
{    
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    tree->build( creatingData( len ) );
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
    return time_span.count();
}

double validatingMerkleTree( MerkleTree * tree )
{
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    tree->isValid();
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
    return time_span.count();
}

double syncMerkleTree( MerkleTree * tree, MerkleTree * old )
{
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    tree->syncronize( old );
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
    return time_span.count();
}


double insertDataMerkleTree( MerkleTree * tree , string data )
{
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    tree->insert( data );
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
    return time_span.count();
}

pair<double,bool> deleteDataMerkleTree( MerkleTree * tree, string hash )
{
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    bool find = tree->remove( hash );
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
    return make_pair( time_span.count(), find );
}

pair<double,bool> searchDataMerkleTree( MerkleTree * tree, string hash )
{
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    bool find = tree->search( hash ) != NULL;
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
    return make_pair( time_span.count(), find );
}

int main( int argv, char** argc )
{
    cout << "Initializing Test Cases" << endl << endl;

    ofstream validateTreeFile;
    validateTreeFile.open( "validateTreeFile.dat", ios::out );
    ofstream buildTreeFile;
    buildTreeFile.open( "buildTreeFile.dat", ios::out );
    ofstream syncTreeFile;
    syncTreeFile.open( "syncTreeFile.dat", ios::out );
    ofstream insertDataTreeFile;
    insertDataTreeFile.open( "insertDataTreeFile.dat", ios::out );
    ofstream searchDataTreeFile;
    searchDataTreeFile.open( "searchDataTreeFile.dat", ios::out );
    ofstream deleteDataTreeFile;
    deleteDataTreeFile.open( "deleteDataTreeFile.dat", ios::out );

    MerkleTree * baseTree = new MerkleTree();
    creatingMerkleTree( baseTree, 1000 );

    for( int i = 0; i < 4000; i += 100 )
    {
        cout << "Testing Merkle Tree with " << i << " elements" << endl;
        MerkleTree * tree = new MerkleTree();
        buildTreeFile << i << "\t" << fixed << setprecision( PRECISION ) << creatingMerkleTree( tree, i ) << endl;


        validateTreeFile << i << "\t" << fixed << setprecision( PRECISION ) <<  validatingMerkleTree( tree ) << endl;
        syncTreeFile << i << "\t" << fixed << setprecision( PRECISION ) << syncMerkleTree( baseTree, tree ) << endl;
        insertDataTreeFile << i << "\t" << fixed << setprecision( PRECISION ) << insertDataMerkleTree( tree, "data" ) << endl;

        pair<double,bool> search = searchDataMerkleTree( tree, Hash::hash( FIND_THIS ) );
        searchDataTreeFile << i << "\t" << fixed << setprecision( PRECISION ) << search.first << "\t" << search.second << endl;

        pair<double,bool> del = deleteDataMerkleTree( tree, Hash::hash( FIND_THIS ) );
        deleteDataTreeFile << i << "\t" << fixed << setprecision( PRECISION ) << del.first << "\t" << del.second << endl;

        delete tree;
    }

    validateTreeFile.close();
    buildTreeFile.close();
    syncTreeFile.close();
    insertDataTreeFile.close();
    searchDataTreeFile.close();
    deleteDataTreeFile.close();

    cout << "End" << endl << endl;
    return 0;
}
