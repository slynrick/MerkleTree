#include <cstdlib>
#include <vector>
#include <cmath>

#include "merkletreenode.h"

class MerkleTree
{
    vector<MerkleTreeNode*> nodes;
    unsigned height;
    public:
        MerkleTree();
        ~MerkleTree();

        MerkleTreeNode * getRoot(){ if( this->nodes.empty() ) return NULL; else return this->nodes.front(); }
        vector<MerkleTreeNode*> getNodes(){ return nodes; }
        unsigned getHeight(){ return this->height; }

        bool insert(const char *data , int len ); //TODO: verificar melhor forma de inclusão
        bool remove( string hash );
        MerkleTreeNode * search( string hash );
        bool build( vector<string> data );
        bool isValid();
        bool syncronize( MerkleTree * tree );
        vector<string> auditProof( string hash );

    private:
        int getParentIdx( int idx ){ return (int)floor( ( idx - 1 ) / 2. ); }
};
