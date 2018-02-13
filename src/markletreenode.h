#include <cstdlib>
#include <string>

#include "hash.h"

using namespace std;

class MarkleTreeNode
{
    MarkleTreeNode * left;
    MarkleTreeNode * right;
    MarkleTreeNode * parent;
    string hash;
    string data;

    public:
        MarkleTreeNode();
        ~MarkleTreeNode();

        bool isLeaf(){ return ( !getLeft() && !getRight() ); }
        bool isRoot(){ return !getParent(); }

        unsigned getHeight(){ return getParent() ? getParent()->getHeight() - 1 : 0; }

        bool setData( string data );
        string getData(){ return this->data; }

        bool setHash();
        void setHash( string hash ){ this->hash = hash; }
        string getHash(){ return this->hash; }

        bool setChilden( MarkleTreeNode * left, MarkleTreeNode * right );

        bool setLeft( MarkleTreeNode * right );
        MarkleTreeNode * getLeft(){ return this->left; }

        bool setRight( MarkleTreeNode * right );
        MarkleTreeNode * getRight(){ return this->right; }

        void setParent( MarkleTreeNode * parent );
        MarkleTreeNode * getParent(){ return this->parent; }

        bool isValid();

    private:
        string calculateHash();
};
