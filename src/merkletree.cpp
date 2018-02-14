#include "merkletree.h"

#include <sstream>
#include <vector>

#define MAX_LEAF_SIZE 256 //256 bytes

MerkleTree::MerkleTree()
{
    this->height = 0;
}

MerkleTree::~MerkleTree()
{
    this->nodes.clear();
    this->nodes.shrink_to_fit();
}

bool MerkleTree::insert( const char * data , int len )
{
    if(len > MAX_LEAF_SIZE )
        return false;

    MerkleTreeNode * newnode = new MerkleTreeNode();
    newnode->setData( data );

    for ( vector<MerkleTreeNode*>::iterator it = this->nodes.end() ; it != this->nodes.begin(); ++it )
    {
        if( (*it)->getHeight() < this->getHeight() - 1 )
        {
            ++this->height;
            unsigned numElements = (unsigned)ceil( pow( 2, this->height + 1 ) - 1 );
            this->nodes.resize( numElements );
            unsigned maxNumLeaves = (unsigned)pow( 2, height );


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
            break;

        }


        if( !(*it)->getParent()->getLeft() )
        {
            (*it)->getParent()->setLeft( newnode );
            break;
        }

        if( !(*it)->getParent()->getRight() )
        {
            (*it)->getParent()->setRight( newnode );
            break;
        }
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

    MerkleTreeNode * node = search( hash );
    if( node )
    {
        if( node->getHash() == hash )
        {
            if( node->getParent()->getLeft()->getHash() == hash )
                node->getParent()->setLeft( NULL );
            else
                node->getParent()->setRight( NULL );

            MerkleTreeNode * currnode = node->getParent();
            while( currnode != NULL )
            {
                if( currnode->setHash( currnode->calculateHash() ) )
                    return false;
                currnode = currnode->getParent();
            }

            delete node;
            return true;
        }
    }

    return false;
}

MerkleTreeNode * MerkleTree::search( string hash )
{
    for ( vector<MerkleTreeNode*>::iterator it = this->nodes.end() ; it != this->nodes.begin(); ++it )
    {
        if( (*it)->getHeight() < this->getHeight() - 1 )
            return NULL;

        if( (*it)->getHash() == hash )
        {
            return (*it);
        }
    }

    return NULL;
}

bool MerkleTree::build( const char * data , int len )
{
    if( len == 0 )
        return false;

    bool isThereRemainder = len % MAX_LEAF_SIZE != 0;
    unsigned numChildren = len/MAX_LEAF_SIZE;
    if( isThereRemainder )
        numChildren += 1;
    this->height = (unsigned)ceil( log2( numChildren ) );
    unsigned numElements = (unsigned)ceil( pow( 2, this->height + 1 ) - 1 );

    this->nodes.resize( numElements );

    unsigned maxNumLeaves = (unsigned)pow( 2, height );

    stringstream ss;
    for( int i = 0, count = 0; i < len; ++i, ++count )
    {
        if( count == MAX_LEAF_SIZE )
        {
            MerkleTreeNode * leaf = new MerkleTreeNode();
            leaf->setData( ss.str() );
            this->nodes[ numElements - maxNumLeaves ] = leaf;
            --maxNumLeaves;
            count = 0;
            ss.str("");
        }
        ss << &data[i];
    }

    if( isThereRemainder )
    {
        MerkleTreeNode * leaf = new MerkleTreeNode();
        leaf->setData( ss.str() );
        this->nodes[ numElements - maxNumLeaves ] = leaf;
    }

    if( numChildren == 1 )
        return true;

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

    for ( unsigned i = this->nodes.size() - 1; i >= 0; --i )
    {
        if( this->nodes[i]->getHeight() < this->getHeight() - 1 )
            break;

        if( this->nodes[i]->isLeaf() )
            if( this->nodes[i]->getHash() == hash )
            {
                MerkleTreeNode * node = this->nodes[i];
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

                break;
            }
    }

    return data;
}

