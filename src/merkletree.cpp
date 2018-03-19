#include "merkletree.h"

MerkleTree::MerkleTree()
{
    this->root = NULL;
    this->leaves = new CustomAVL();
}

MerkleTree::~MerkleTree()
{
    delete root;
    delete this->leaves;
}

bool MerkleTree::insert( string data )
{
    if( data.length() <= 0 )
        return false;

    MerkleTreeNode * newnode = new MerkleTreeNode();
    newnode->setData( data );

    if( !this->getRoot() )
    {
        root = newnode;
        return leaves->insert( newnode );
    }

    MerkleTreeNode * node = this->getRoot();

    while( node )
    {
        if( node->isLeaf() )
        {
            MerkleTreeNode * leaf = new MerkleTreeNode();
            leaf->setData( node->getData() );
            node->setLeft( leaf );
            node->setRight( newnode );
            break;
        }
        else if( !node->getLeft() )
        {
            node->setLeft( newnode );
            break;
        }
        else if( !node->getRight() )
        {
            node->setRight( newnode );
            break;
        }
        else
        {
            int balance = node->getLeft()->getHeight() - node->getRight()->getHeight();
            if( balance > 0 )
                node = node->getRight();
            else
                node = node->getLeft();
        }
    }

    node = node->getParent();

    while( node )
    {
        node->setHash( node->calculateHash() );
        node = node->getParent();
    }

    return leaves->insert( newnode );
}

bool MerkleTree::remove( string hash )
{
    MerkleTreeNode * node = search( hash );
    if( node )
    {
        MerkleTreeNode * parent = node->getParent();
        while( parent->getNumChidren() < 2 )
        {
            MerkleTreeNode * aux = parent;
            parent = aux->getParent();
            delete aux;
        }
        return leaves->remove( node );
    }
    return false;
}

MerkleTreeNode * MerkleTree::search( string hash )
{
    return leaves->search( hash );
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

    MerkleTreeNode * node = search( hash );

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

