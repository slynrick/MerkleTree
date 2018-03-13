#include <cstdlib>
#include <vector>
#include <cmath>

#include "merkletreenode.h"

class MerkleTree
{
    MerkleTreeNode * root;
    vector<MerkleTreeNode*> leaves;
    public:
        MerkleTree();
        ~MerkleTree();

        MerkleTreeNode * getRoot(){ return root; }
        vector<MerkleTreeNode*> * getLeaves(){ return &leaves; }
        unsigned getHeight(){ return this->root->getHeight(); }

        bool insert(string data ); //TODO: verificar melhor forma de inclusão
        bool remove( string hash );
        int search( string hash );
        bool build( vector<string> data );
        bool isValid();
        bool syncronize( MerkleTree * tree );
        vector<string> auditProof( string hash );
};
