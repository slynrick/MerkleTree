#include "merkletreenode.h"

MerkleTreeNode::MerkleTreeNode()
{
    this->left = NULL;
    this->right = NULL;
    this->parent = NULL;
    this->data = string();
}

MerkleTreeNode::~MerkleTreeNode()
{
    if( getLeft() )
        delete this->left;
    if( getRight() )
        delete this->right;
    this->data = string();
    this->parent = NULL;
}


bool MerkleTreeNode::setData( string data )
{
    if( isLeaf() )
    {
        this->data = data;
        return setHash( calculateHash() );
    }
    return false;
}

bool MerkleTreeNode::setLeft( MerkleTreeNode * left )
{
    if( !left )
        return false;
    this->left = left;
    getLeft()->setParent( this );
    return setHash( calculateHash() );
}

bool MerkleTreeNode::setRight( MerkleTreeNode * right )
{
    if( !right )
        return false;
    this->right = right;
    getRight()->setParent( this );
    return setHash( calculateHash() );
}

bool MerkleTreeNode::setChilden( MerkleTreeNode * left, MerkleTreeNode * right )
{
    if( setLeft( left ) )
        if( setRight( right ) )
            return setHash( calculateHash() );
    return false;
}

void MerkleTreeNode::setParent( MerkleTreeNode * parent )
{
    this->parent = parent;
}

string MerkleTreeNode::calculateHash()
{

    if( isLeaf() )
    {
        return Hash::hash( getData() );
    }

    string tohash = string();
    if( getLeft() && getRight() )
    {
        tohash.append( getLeft()->getData() );
        tohash.append( getRight()->getData() );
        return Hash::hash( tohash );
    }
    else if( getLeft() )
    {
        tohash.append( getLeft()->getData() );
        tohash.append( getLeft()->getData() );
        return Hash::hash( tohash );
    }
    else
    {
        tohash.append( getRight()->getData() );
        tohash.append( getRight()->getData() );
        return Hash::hash( tohash );
    }

    return tohash;
}

bool MerkleTreeNode::isValid()
{
    MerkleTreeNode * node = this;

    if( node->calculateHash() != node->getHash() )
        return false;


    bool validl = node->getLeft() ? node->getLeft()->isValid() : true;
    bool validr = node->getRight() ? node->getRight()->isValid() : true;

    return ( validl && validr );
}

unsigned MerkleTreeNode::getHeight()
{
    if( isLeaf() )
        return 0;

    unsigned l = 0, r = 0;
    if( getLeft() )
        l = getLeft()->getHeight();
    if( getRight() )
        r = getRight()->getHeight();

    unsigned h =  l >= r ? l : r;
    ++h;

    return h;

}
