#include <cstdlib>
#include <string>

using namespace std;

class MerkleTreeNode;
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

        int balanceFactor();

        bool set_mt_node( MerkleTreeNode * node );
        MerkleTreeNode * get_mt_node(){ return mt_node; }

        bool setLeft( CustomAVLNode * right );
        CustomAVLNode * getLeft(){ return left; }

        bool setRight( CustomAVLNode * right );
        CustomAVLNode * getRight(){ return right; }

        void setParent( CustomAVLNode * parent );
        CustomAVLNode * getParent(){ return parent; }

        bool setChilden( CustomAVLNode * left, CustomAVLNode * right );

};
