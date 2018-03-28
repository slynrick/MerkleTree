#include <cstdlib>
#include <vector>

#include "merkletreenode.h"
#include "customavl.h"

class MerkleTree
{
    MerkleTreeNode * root;
    CustomAVL * leaves;
    public:
        MerkleTree();
        ~MerkleTree();

        MerkleTreeNode * getRoot(){ return this->root; }
        CustomAVL * getLeaves(){ return this->leaves; }
        unsigned getHeight(){ return this->root->getHeight(); }

        bool insert(string data );
        bool remove( string hash );
        MerkleTreeNode * search( string hash );
        bool build( vector<string> data );
        bool isValid();
        bool syncronize( MerkleTree * newTree );
        vector<string> auditProof( string hash );
};
