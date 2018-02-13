#include "markletreenode.h"

MarkleTreeNode::MarkleTreeNode()
{
    this->left = NULL;
    this->right = NULL;
    this->parent = NULL;
    this->data = string();
}

MarkleTreeNode::~MarkleTreeNode()
{
    if( this->getLeft() )
        delete this->left;
    if( this->getRight() )
        delete this->right;
    this->data = string();
    this->parent = NULL;
}


bool MarkleTreeNode::setData( string data )
{
    if( isLeaf() )
    {
        this->data = data;
        return setHash();
    }
    return false;
}

bool MarkleTreeNode::setLeft( MarkleTreeNode * left )
{
    if( !left )
        return false;
    this->left = left;
    this->getLeft()->setParent( this );
    return setHash();
}

bool MarkleTreeNode::setRight( MarkleTreeNode * right )
{
    if( !right )
        return false;
    this->right = right;
    this->getRight()->setParent( this );
    return setHash();
}

bool MarkleTreeNode::setChilden( MarkleTreeNode * left, MarkleTreeNode * right )
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

void MarkleTreeNode::setParent( MarkleTreeNode * parent )
{
    this->parent = parent;
}

string MarkleTreeNode::calculateHash()
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

bool MarkleTreeNode::setHash()
{
    string h = calculateHash();
    if( h == string() )
        return false;

    this->hash = h;
    return true;
}

bool MarkleTreeNode::isValid()
{
    if( calculateHash() != this->hash )
        return false;
    return true;
}
