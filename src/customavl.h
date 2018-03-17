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
        CustomAVLNode * rr_rotation( CustomAVLNode * parent );
        CustomAVLNode * ll_rotation( CustomAVLNode * parent );
        CustomAVLNode * lr_rotation( CustomAVLNode * parent );
        CustomAVLNode * rl_rotation( CustomAVLNode * parent );

        CustomAVLNode * balance( CustomAVLNode * node );
};
