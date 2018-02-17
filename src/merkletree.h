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
        vector<MerkleTreeNode*> * getNodes(){ return &nodes; }
        unsigned getHeight(){ return this->height; }

        bool insert(string data ); //TODO: verificar melhor forma de inclusão
        bool remove( string hash );
        int search( string hash );
        bool build( vector<string> data );
        bool isValid();
        bool syncronize( MerkleTree * tree );
        vector<string> auditProof( string hash );

    private:
        int getParentIdx( int idx ){ return (int)floor( ( idx - 1 ) / 2. ); }
        int getChildIdx( int idx, bool left ){ if( left ) return (2*idx)+1; else return (2*idx)+2; }
        int getMaxNumElements( int height ) { return (int)ceil( pow( 2, height + 1 ) - 1 ); }
        int getMaxNumLeaves( int height ){ return (int)pow( 2, height ); }
        int getFirstElementInHeight( int height ){ return getMaxNumElements( height ) - (int)pow( 2, height ); }
};
