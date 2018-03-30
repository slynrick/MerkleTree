#include "merkletree.h"

#include <deque>

MerkleTree::MerkleTree()
{
    this->root = NULL;
    this->leaves = new CustomAVL();
}

MerkleTree::~MerkleTree()
{
    if( getRoot() )
        delete this->root;
    if( getLeaves() )
        delete this->leaves;
}

bool MerkleTree::insert( string data )
{
    if( data.length() <= 0 )
        return false;

    MerkleTreeNode * newnode = new MerkleTreeNode();
    newnode->setData( data );

    if( !getRoot() )
    {
        this->root = newnode;
        return getLeaves()->insert( newnode );
    }

    MerkleTreeNode * node = getRoot();

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

    return getLeaves()->insert( newnode );
}

bool MerkleTree::remove( string hash )
{
    MerkleTreeNode * node = search( hash );
    if( node )
    {
        if( node->isRoot() )
            return getLeaves()->remove( node );

        MerkleTreeNode * parent = node->getParent();
        while( parent->getNumChidren() < 2 )
        {
            MerkleTreeNode * aux = parent;
            parent = aux->getParent();
            delete aux;
        }
        return getLeaves()->remove( node );
    }
    return false;
}

MerkleTreeNode * MerkleTree::search( string hash )
{
    return getLeaves()->search( hash );
}

bool MerkleTree::build( vector<string> data )
{
    if( data.size() == 0 )
        return false;

    deque<MerkleTreeNode*> d;
    for( int i = 0; i < data.size(); ++i )
    {
        MerkleTreeNode * node = new MerkleTreeNode();
        node->setData( data[i] );
        d.push_back( node );
    }

    while( d.size() > 1 )
    {
        bool even = d.size() % 2 == 0;
        for( int i = 0; i < d.size(); i += 2 )
        {
            MerkleTreeNode * node = new MerkleTreeNode();
            node->setLeft( d.front() );
            d.pop_front();
            node->setRight( d.front() );
            d.pop_front();
            d.push_back( node );
        }

        if( !even )
        {
            MerkleTreeNode * node = new MerkleTreeNode();
            node->setLeft( d.front() );
            d.pop_front();
            d.push_back( node );
        }
    }

    this->root = d.front();
    d.clear();

    return true;
}

bool MerkleTree::isValid()
{
    if( !getRoot() )
        return false;

    return getRoot()->isValid();
}

void MerkleTree::syncNode( MerkleTreeNode * oldNode, MerkleTreeNode * newNode )
{
    if( !oldNode )
        if( newNode )
        {
            if( newNode->isLeaf() )
                getLeaves()->insert( newNode );
            oldNode = newNode;
        }

    if( !newNode )
        if( oldNode )
        {
            if( oldNode->isLeaf() )
                getLeaves()->remove( oldNode );
            oldNode = NULL;
            delete oldNode;
        }

    if( !oldNode || !newNode )
        return;

    if( oldNode->getHash() == newNode->getHash() )
        return;

    oldNode->setData( newNode->getData() );
    oldNode->setHash( newNode->getHash() );

    syncNode( oldNode->getLeft(), newNode->getLeft() );
    syncNode( oldNode->getRight(), newNode->getRight() );

    oldNode->setChilden( newNode->getLeft(), newNode->getRight() );
}

void MerkleTree::syncronize( MerkleTree * tree )
{
    syncNode( getRoot(), tree->getRoot() );
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

