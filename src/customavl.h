#include <cstdlib>
#include <string>

#include "customavlnode.h"

class CustomAVL
{
    CustomAVLNode * root;
    public:
        CustomAVL();
        ~CustomAVL();

        CustomAVLNode * getRoot(){ return root; }

        bool insert( MerkleTreeNode * mt_node  );
        bool remove( MerkleTreeNode * mt_node );
        MerkleTreeNode * search( string hash );

private:
        CustomAVLNode * rotate_left( CustomAVLNode * node );
        CustomAVLNode * rotate_right(CustomAVLNode * node );

        CustomAVLNode * findNode( string value );
};
