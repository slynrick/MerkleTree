#include <cstdlib>
#include <string>

#include "merkletreenode.h"

using namespace std;

class CustomAVLNode
{
    CustomAVLNode * left;
    CustomAVLNode * right;
    CustomAVLNode * parent;
    MerkleTreeNode * mt_node;

    public:
        CustomAVLNode();
        ~CustomAVLNode();

        bool isLeaf(){ return ( !getLeft() && !getRight() ); }
        bool isRoot(){ return !getParent(); }

        int getNumChidren(){ if( isLeaf() ) return 0; else if( getLeft() && getRight() ) return 2; else return 1; }

        unsigned getHeight();

        bool set_mt_node( MerkleTreeNode * node );
        MerkleTreeNode * get_mt_node(){ return this->mt_node; }

        bool setLeft( CustomAVLNode * right );
        CustomAVLNode * getLeft(){ return this->left; }

        bool setRight( CustomAVLNode * right );
        CustomAVLNode * getRight(){ return this->right; }

        void setParent( CustomAVLNode * parent );
        CustomAVLNode * getParent(){ return this->parent; }

        bool setChilden( CustomAVLNode * left, CustomAVLNode * right );

};
