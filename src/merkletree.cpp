#include "merkletree.h"

#include <sstream>
#include <vector>

MerkleTree::MerkleTree()
{
    this->height = 0;
}

MerkleTree::~MerkleTree()
{
    this->nodes.clear();
    this->nodes.shrink_to_fit();
}

bool MerkleTree::insert( string data )
{
    if( data.length() <= 0 )
        return false;

    MerkleTreeNode * newnode = new MerkleTreeNode();
    newnode->setData( data );

    for( int i = getFirstElementInHeight( this->height ); i < this->nodes.size(); ++i )
    {
        if( !this->nodes[i] )
        {
            this->nodes[i] = newnode;
            int parentIdx = getParentIdx( i );
            MerkleTreeNode * parent = this->nodes[parentIdx];
            if( !parent )
            {
                parent = new MerkleTreeNode();
                parent->setLeft( this->nodes[i] );

                int idxc = i;
                int idxp = getParentIdx( parentIdx );
                while( idxp > 0 )
                {
                    if( !this->nodes[idxp] )
                    {
                        this->nodes[idxp] = new MerkleTreeNode();
                        this->nodes[idxp]->setLeft( this->nodes[idxc] );
                    }
                    idxc = idxp;
                    idxp = getParentIdx( idxp );
                }
            }
            else
            {
                if( parent->getLeft() )
                    parent->setRight( this->nodes[i] );
                else
                    parent->setLeft( this->nodes[i] );
            }

        }
    }

    if( !newnode->getParent() )
    {
        ++this->height;
        unsigned numElements = getMaxNumElements( this->height );
        this->nodes.resize( numElements );
        unsigned maxNumLeaves = getMaxNumLeaves( this->height );


        int parentIdx = getParentIdx( numElements - maxNumLeaves );
        MerkleTreeNode * parent = this->nodes[parentIdx];
        MerkleTreeNode * left = new MerkleTreeNode();
        left->setData( parent->getData() );

        this->nodes[ numElements - maxNumLeaves ] = newnode;
        --maxNumLeaves;
        this->nodes[ numElements - maxNumLeaves ] = left;

        parent->setRight( newnode );
        parent->setLeft( left );
        newnode->setParent( parent );
        left->setParent( parent );
    }

    MerkleTreeNode * currnode = newnode->getParent();
    while( currnode != NULL )
    {
        if( currnode->setHash( currnode->calculateHash() ) )
            return false;
        currnode = currnode->getParent();
    }

    return true;
}

bool MerkleTree::remove( string hash )
{

    int idx = search( hash );
    if( idx >= 0 && this->nodes[idx] )
    {
        int parentIdx = getParentIdx( idx );
        while( parentIdx >= 0 && this->nodes[parentIdx]->getNumChidren() <= 1 )
        {
            delete this->nodes[idx];

            idx = parentIdx;
            parentIdx = getParentIdx( idx );
        }
        return true;
    }
    return false;
}

int MerkleTree::search( string hash )
{
    for( int i = getFirstElementInHeight( this->height - 1 ); i < this->nodes.size(); ++i )
    {
        if( this->nodes[i] && this->nodes[i]->isLeaf() )
        {
            if( this->nodes[i]->getHash() == hash )
            {
                return i;
            }
        }
    }
    return -1;
}

bool MerkleTree::build( vector<string> data )
{
    if( data.size() == 0 )
        return false;

    unsigned numChildren = data.size();
    this->height = (unsigned)ceil( log2( numChildren ) );
    unsigned numElements = (unsigned)ceil( pow( 2, this->height + 1 ) - 1 );

    this->nodes.resize( numElements );

    unsigned maxNumLeaves = (unsigned)pow( 2, height );

    for( int i = 0; i < data.size(); ++i )
    {
        MerkleTreeNode * leaf = new MerkleTreeNode();
        leaf->setData( data[i] );
        this->nodes[ numElements - maxNumLeaves ] = leaf;
        --maxNumLeaves;
    }

    for ( int i = this->nodes.size() - 1; i >= 0; --i )
    {
        int parentIdx = getParentIdx( i );
        if( parentIdx < 0 )
            break;

        if( this->nodes[i] )
        {
            if( !this->nodes[parentIdx] )
            {
                this->nodes[parentIdx] = new MerkleTreeNode();
                this->nodes[parentIdx]->setRight( this->nodes[i] );
            }
            else
            {
                this->nodes[parentIdx]->setLeft( this->nodes[i] );
            }

        }
    }

    return true;
}

bool MerkleTree::isValid()
{
    if( !getRoot() )
        return false;

    bool valid = true; // isso garante contra timming attack
    for ( int i = this->nodes.size() - 1; i >= 0; --i )
    {
        if( this->nodes[i] )
        {
            if( this->nodes[i]->calculateHash() == this->nodes[i]->getHash() )
                valid = false;
        }
    }

    return valid;
}

bool MerkleTree::syncronize( MerkleTree * tree )
{
    if( !getRoot() || !tree->getRoot() )
        return false;

    if( getRoot()->getHash() == tree->getRoot()->getHash() )
        return true;

    this->height = tree->getHeight();

    int numElements = (int)ceil( pow( 2, this->height + 1 ) - 1 );

    this->nodes.resize( numElements );

    for ( int i = this->nodes.size() - 1; i >= 0; --i )
    {
        if( !this->nodes[i] && !tree->getNodes()[i] )
            continue;
        else if( this->nodes[i] && !tree->getNodes()[i] )
            delete this->nodes[i];
        else if( !this->nodes[i] && tree->getNodes()[i] )
        {
            int r =  ( i - 1 ) % 2;
            int parentIdx = getParentIdx( i );
            if( !this->nodes[parentIdx] )
            {
                MerkleTreeNode * parent = new MerkleTreeNode();
                if( r > 0 )
                    parent->setRight( this->nodes[i] );
                else
                    parent->setLeft( this->nodes[i] );

                this->nodes[parentIdx] = parent;
            }
            else
            {
                if( r > 0 )
                    this->nodes[parentIdx]->setRight( this->nodes[i] );
                else
                    this->nodes[parentIdx]->setLeft( this->nodes[i] );
            }
        }
        else
        {
            if ( this->nodes[i]->getHash() != tree->getNodes()[i]->getHash() )
            {
                this->nodes[i]->setHash( tree->getNodes()[i]->getHash() );
                if( !tree->getNodes()[i]->getData().empty() )
                    this->nodes[i]->setData( tree->getNodes()[i]->getData() );
            }
        }


    }

    return true;
}

vector<string> MerkleTree::auditProof( string hash )
{
    vector<string> data;

    int idx = search( hash );
    MerkleTreeNode * node = this->nodes[idx];

    if( node )
        if( node->isLeaf() )
            if( node->getHash() == hash )
            {
                while( !node->isRoot() )
                {
                    if( node->getParent()->getLeft() )
                    {
                        if( node->getParent()->getLeft()->getHash() != node->getHash() )
                        {
                            data.push_back( node->getParent()->getLeft()->getHash() );
                        }
                        else
                        {
                            if( node->getParent()->getRight() )
                                data.push_back( node->getParent()->getRight()->getHash() );
                            else
                                data.push_back( node->getParent()->getLeft()->getHash() );
                        }
                    }

                    if( node->getParent()->getRight() )
                    {
                        if( node->getParent()->getRight()->getHash() != node->getHash() )
                        {
                            data.push_back( node->getParent()->getRight()->getHash() );
                        }
                        else
                        {
                            if( node->getParent()->getLeft() )
                                data.push_back( node->getParent()->getLeft()->getHash() );
                            else
                                data.push_back( node->getParent()->getRight()->getHash() );
                        }
                    }
                    node = node->getParent();
                }
            }

    return data;
}

