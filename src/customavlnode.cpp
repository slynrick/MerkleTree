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
    if( getLeft() )
        delete this->left;
    if( getRight() )
        delete this->right;
    this->mt_node = NULL;
    this->parent = NULL;
}

int CustomAVLNode::balanceFactor()
{
        int lh = 0, rh = 0;

    if( getLeft() )
        lh = getLeft()->getHeight();
    if( getRight() )
        rh = getRight()->getHeight();

    int bf = lh - rh;

    return bf;
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
    getLeft()->setParent( this );
    return true;
}

bool CustomAVLNode::setRight( CustomAVLNode * right )
{
    if( !right )
        return false;
    this->right = right;
    getRight()->setParent( this );
    return true;
}

bool CustomAVLNode::setChilden( CustomAVLNode * left, CustomAVLNode * right )
{
    if( setLeft( left ) )
        if( setRight( right ) )
        {
            left->setParent( this );
            right->setParent( this );
            return true;
        }
    return false;
}

void CustomAVLNode::setParent( CustomAVLNode * parent )
{
    this->parent = parent;
}

unsigned CustomAVLNode::getHeight()
{

    unsigned l = 0, r = 0;
    if( getLeft() )
        l = getLeft()->getHeight();
    if( getRight() )
        r = getRight()->getHeight();

    unsigned h =  l >= r ? l : r;

    if( h == 0 )
        return 0;
    else
        return ++h;
}
