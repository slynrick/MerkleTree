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

        unsigned getHeight(){ return getParent() ? getParent()->getHeight() + 1 : 0; }

        bool setData( string data );
        string getData(){ return this->data; }

        bool setHash();
        void setHash( string hash ){ this->hash = hash; }
        string getHash(){ return this->hash; }

        bool setChilden( MerkleTreeNode * left, MerkleTreeNode * right );

        bool setLeft( MerkleTreeNode * right );
        MerkleTreeNode * getLeft(){ return this->left; }

        bool setRight( MerkleTreeNode * right );
        MerkleTreeNode * getRight(){ return this->right; }

        void setParent( MerkleTreeNode * parent );
        MerkleTreeNode * getParent(){ return this->parent; }

        bool isValid();

    private:
        string calculateHash();
};
