#include "customavlnode.h"

CustomAVLNode::CustomAVLNode()
{
    this->left = NULL;
    this->right = NULL;
    this->parent = NULL;
    this->mt_node = NULL;
}

CustomAVLNode::~CustomAVLNode()
{
    if( this->getLeft() )
        delete this->left;
    if( this->getRight() )
        delete this->right;
    this->mt_node = NULL;
    this->parent = NULL;
}

bool CustomAVLNode::set_mt_node( MerkleTreeNode * node )
{
    if( node )
    {
        this->mt_node = node;
        return true;
    }
    return false;
}

bool CustomAVLNode::setLeft( CustomAVLNode * left )
{
    if( !left )
        return false;
    this->left = left;
    return true;
}

bool CustomAVLNode::setRight( CustomAVLNode * right )
{
    if( !right )
        return false;
    this->right = right;
    return true;
}

bool CustomAVLNode::setChilden( CustomAVLNode * left, CustomAVLNode * right )
{
    if( setLeft( left ) )
        if( setRight( right ) )
        {
            left->setParent( this );
            right->setParent( this );
        }
            return true;
    return false;
}

void CustomAVLNode::setParent( CustomAVLNode * parent )
{
    this->parent = parent;
}

unsigned CustomAVLNode::getHeight()
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
