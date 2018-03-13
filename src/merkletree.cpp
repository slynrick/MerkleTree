#include "merkletree.h"

#include <sstream>
#include <queue>

MerkleTree::MerkleTree()
{
    this->root = new MerkleTreeNode();
}

MerkleTree::~MerkleTree()
{
    delete root;
    this->leaves.clear();
    this->leaves.shrink_to_fit();
}

bool MerkleTree::insert( string data )
{
    if( data.length() <= 0 )
        return false;

    MerkleTreeNode * newnode = new MerkleTreeNode();
    newnode->setData( data );

    MerkleTreeNode * node = this->root;

    while( node )
    {
        if( node->isLeaf() )
        {
            MerkleTreeNode * leaf = new MerkleTreeNode();
            leaf->setData( node->getData() );
            node->setLeft( leaf );
            node->setRight( newnode );
        }

        if( !node->getLeft() )
        {
            node->setLeft( newnode );
            break;
        }
        else if( !node->getRight() )
        {
            node->setRight( newnode );
            break;
        }

        int balance = node->getLeft()->getHeight() - node->getRight()->getHeight();
        if( balance >= 0 )
            node = node->getLeft();
        else
            node = node->getRight();

    }

    node = node->getParent();

    while( node )
    {
        node->setHash( node->calculateHash() );
        node = node->getParent();
    }


    return true;
}

bool MerkleTree::remove( string hash )
{

    int idx = search( hash );
    if( idx >= 0 && this->leaves[idx] )
    {
        MerkleTreeNode * parent = this->leaves[idx]->getParent();
        while( parent->getNumChidren() < 2 )
        {
            parent = parent->getParent();
        }
        delete parent;
        this->leaves.erase( this->leaves.begin() + idx );
        this->leaves.shrink_to_fit();
        return true;
    }
    return false;
}

int MerkleTree::search( string hash )
{
    for( int i = 0; i < this->leaves.size(); ++i )
    {
        if( this->leaves[i] && this->leaves[i]->isLeaf() )
        {
            if( this->leaves[i]->getHash() == hash )
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

    for( int i = 0; i < data.size(); ++i )
        if( !insert( data[i] ) )
            return false;

    return true;
}

bool MerkleTree::isValid()
{
    if( !getRoot() )
        return false;

    return this->getRoot()->isValid();
}

bool MerkleTree::syncronize( MerkleTree * tree )
{
    if( !getRoot() || !tree->getRoot() )
        return false;

    if( getRoot()->getHash() == tree->getRoot()->getHash() )
        return true;

    return getRoot()->sync( tree->getRoot() );
}

vector<string> MerkleTree::auditProof( string hash )
{
    vector<string> data;

    int idx = search( hash );
    MerkleTreeNode * node = this->leaves[idx];

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

