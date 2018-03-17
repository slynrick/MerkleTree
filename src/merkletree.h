#include <cstdlib>
#include <vector>

#include "customavl.h"

class MerkleTree
{
    MerkleTreeNode * root;
    CustomAVL * leaves;
    public:
        MerkleTree();
        ~MerkleTree();

        MerkleTreeNode * getRoot(){ return root; }
        CustomAVL * getLeaves(){ return leaves; }
        unsigned getHeight(){ return this->root->getHeight(); }

        bool insert(string data );
        bool remove( string hash );
        MerkleTreeNode * search( string hash );
        bool build( vector<string> data );
        bool isValid();
        bool syncronize( MerkleTree * newTree );
        vector<string> auditProof( string hash );
};
