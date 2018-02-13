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
    string data;// hash quando não for uma folha. o que define uma folha é não ter filhos

    public:
        MarkleTreeNode();
        ~MarkleTreeNode();

        bool isLeaf(){ return ( !getLeft() && !getRight() ); }
        bool isRoot(){ return !getParent(); }

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
