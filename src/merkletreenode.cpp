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
    if( this->getLeft() )
        delete this->left;
    if( this->getRight() )
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
    this->getLeft()->setParent( this );
    return setHash( calculateHash() );
}

bool MerkleTreeNode::setRight( MerkleTreeNode * right )
{
    if( !right )
        return false;
    this->right = right;
    this->getRight()->setParent( this );
    return setHash( calculateHash() );
}

bool MerkleTreeNode::setChilden( MerkleTreeNode * left, MerkleTreeNode * right )
{
    if( setLeft( left ) )
        if( setRight( right ) )
        {
            left->setParent( this );
            right->setParent( this );
            return setHash( calculateHash() );
        }
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
        tohash.append( this->getLeft()->getData() );
        tohash.append( this->getRight()->getData() );
        return Hash::hash( tohash );
    }
    else if( getLeft() )
    {
        tohash.append( this->getLeft()->getData() );
        tohash.append( this->getLeft()->getData() );
        return Hash::hash( tohash );
    }
    else
    {
        tohash.append( this->getRight()->getData() );
        tohash.append( this->getRight()->getData() );
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

bool MerkleTreeNode::sync( MerkleTreeNode * newNode )
{
    return true;
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
