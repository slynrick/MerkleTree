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
        return setHash();
    }
    return false;
}

bool MerkleTreeNode::setLeft( MerkleTreeNode * left )
{
    if( !left )
        return false;
    this->left = left;
    this->getLeft()->setParent( this );
    return setHash();
}

bool MerkleTreeNode::setRight( MerkleTreeNode * right )
{
    if( !right )
        return false;
    this->right = right;
    this->getRight()->setParent( this );
    return setHash();
}

bool MerkleTreeNode::setChilden( MerkleTreeNode * left, MerkleTreeNode * right )
{
    if( setLeft( left ) )
        if( setRight( right ) )
        {
            left->setParent( this );
            right->setParent( this );
        }
            return setHash();
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

bool MerkleTreeNode::setHash()
{
    string h = calculateHash();
    if( h == string() )
        return false;

    this->hash = h;
    return true;
}

bool MerkleTreeNode::isValid()
{
    MerkleTreeNode * node = this;
    while( node )
    {
        if( node->calculateHash() != node->getHash() )
            break;
        node = node->getParent();
    }

    if( !node->isRoot() )
        return false;
    return true;
}
