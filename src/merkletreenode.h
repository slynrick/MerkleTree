#include <cstdlib>
#include <string>

#include "hash.h"

using namespace std;

class MerkleTreeNode
{
    MerkleTreeNode * left;
    MerkleTreeNode * right;
    MerkleTreeNode * parent;
    string hash;
    string data;

    public:
        MerkleTreeNode();
        ~MerkleTreeNode();

        bool isLeaf(){ return ( !getLeft() && !getRight() ); }
        bool isRoot(){ return !getParent(); }

        int getNumChidren(){ if( isLeaf() ) return 0; else if( getLeft() && getRight() ) return 2; else return 1; }

        unsigned getHeight();

        bool setData( string data );
        string getData(){ return data; }

        bool setHash( string hash ){ if( hash.empty() ) return false; hash = hash; return true; }
        string calculateHash();
        string getHash(){ return hash; }

        bool setChilden( MerkleTreeNode * left, MerkleTreeNode * right );

        bool setLeft( MerkleTreeNode * right );
        MerkleTreeNode * getLeft(){ return left; }

        bool setRight( MerkleTreeNode * right );
        MerkleTreeNode * getRight(){ return right; }

        void setParent( MerkleTreeNode * parent );
        MerkleTreeNode * getParent(){ return parent; }

        bool isValid();
        bool sync( MerkleTreeNode * newNode );
};
