#include <cstdlib>
#include <vector>
#include <deque>

#include "markletreenode.h"

class MarkleTree
{
    MarkleTreeNode * root;
    deque<MarkleTreeNode*> leaves;
    unsigned height;
    public:
        MarkleTree();
        ~MarkleTree();

        MarkleTreeNode * getRoot(){ return this->root; }
        unsigned getHeight(){ return this->height; }

        bool insert( const char * data , int len ); //TODO: verificar melhor forma de inclusão
        bool remove( string hash );
        MarkleTreeNode * search( string hash );
        bool build( const char * data, int len );
        bool isValid();
        bool syncronize( MarkleTree * tree );
        bool appendTree( MarkleTree * tree );

        vector<string> auditProof( string hash );

    private:
        deque<MarkleTreeNode*> populate( deque<MarkleTreeNode*> data );
        bool childrenIsValid( deque<MarkleTreeNode*> data );
        bool syncronizeChildren( deque<MarkleTreeNode*> owndata , deque<MarkleTreeNode *> treedata );
        bool addremoveNodes( MarkleTreeNode * ownNode , MarkleTreeNode *treeNode , bool isleft );
        void updateLeaves( bool remove, MarkleTreeNode * node = NULL );
        deque<MarkleTreeNode*> getLeavesFromNodes( deque<MarkleTreeNode*> nodes );
};
