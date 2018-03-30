#include "customavl.h"

#include "merkletreenode.h"

#include <cstring>

CustomAVL::CustomAVL()
{
    this->root = NULL;
}

CustomAVL::~CustomAVL()
{
    if( getRoot() )
        delete this->root;
}

CustomAVLNode * CustomAVL::rotate_left( CustomAVLNode * node )
{
    CustomAVLNode * aux = node->getRight();
    CustomAVLNode * aux2 = aux->getLeft();

    aux->setLeft( node );
    node->setRight( aux2 );

    return aux;
}

CustomAVLNode * CustomAVL::rotate_right( CustomAVLNode * node )
{
    CustomAVLNode * aux =  node->getLeft();
    CustomAVLNode * aux2 = aux->getRight();

    aux->setRight( node );
    node->setLeft( aux2 );

    return aux;
}
bool CustomAVL::insert( MerkleTreeNode * mt_node )
{
    CustomAVLNode * newNode = new CustomAVLNode();
    newNode->set_mt_node( mt_node );
    return insert( newNode );
}

bool CustomAVL::insert( CustomAVLNode * newNode )
{
    if( !getRoot() )
    {
        this->root = newNode;
        return true;
    }

    CustomAVLNode * currNode = getRoot();

    while( currNode )
    {
        if( strcmp( newNode->get_mt_node()->getHash().c_str(), currNode->get_mt_node()->getHash().c_str() ) <= 0 )
        {
            if( !currNode->getLeft() )
            {
                currNode->setLeft( newNode );
                break;
            }
            currNode = currNode->getLeft();
        }
        else
        {
            if( !currNode->getRight() )
            {
                currNode->setRight( newNode );
                break;
            }
            currNode = currNode->getRight();
        }
    }

    while( !currNode->isRoot() )
    {
        int bf = currNode->balanceFactor();

        if( bf > 1 )
        {
            if( strcmp( newNode->get_mt_node()->getHash().c_str(), currNode->getLeft()->get_mt_node()->getHash().c_str() ) <= 0 ) // ll case
                currNode = rotate_right( currNode );

            if( strcmp( newNode->get_mt_node()->getHash().c_str(), currNode->getRight()->get_mt_node()->getHash().c_str() ) > 0 ) // lr case
            {
                currNode->setLeft( rotate_left( currNode->getLeft() ) );
                currNode = rotate_right( currNode );
            }
        }

        if( bf < -1 )
        {
            if( strcmp( newNode->get_mt_node()->getHash().c_str(), currNode->getRight()->get_mt_node()->getHash().c_str() ) > 0 ) // rr case
                currNode = rotate_left( currNode );

            if( strcmp( newNode->get_mt_node()->getHash().c_str(), currNode->getLeft()->get_mt_node()->getHash().c_str() ) <= 0 ) // rl case
            {
                currNode->setRight( rotate_right( currNode->getRight() ) );
                currNode = rotate_left( currNode );
            }
        }

        currNode = currNode->getParent();
    }

    return true;
}

bool CustomAVL::remove( MerkleTreeNode * mt_node )
{
    if( !mt_node )
        return false;

    CustomAVLNode * node = findNode( mt_node->getHash() );
    return remove( node );
}

bool CustomAVL::remove( CustomAVLNode * node )
{
    if( node )
    {
        if( !node->isLeaf() )
        {
            CustomAVLNode * parent = node->getParent();

            if( node->getNumChidren() == 1 )
            {
                CustomAVLNode * child = node->getLeft() ? node->getLeft() : node->getRight();
                if( parent )
                {
                    if( parent->getLeft() == node )
                        parent->setLeft( child );
                    else
                        parent->setRight( child );
                }
                else
                {
                    this->root = child;
                }
            }

            if( node->getNumChidren() == 2 )
            {
                CustomAVLNode * rchild = node->getRight();
                if( parent->getLeft() == node )
                    parent->setLeft( rchild );
                else
                    parent->setRight( rchild );

                insert( node->getLeft() );
            }

        }

        node->setChilden( NULL, NULL );
        node->setParent( NULL );
        node = NULL;
        delete node;
        return true;
    }

    return false;
}


MerkleTreeNode * CustomAVL::search( string hash )
{
    CustomAVLNode * node = findNode( hash );
    if( node )
        return node->get_mt_node();
    return NULL;
}

CustomAVLNode * CustomAVL::findNode( string value )
{
    CustomAVLNode * node = getRoot();
    while( node )
    {
        if( strcmp( value.c_str(), node->get_mt_node()->getHash().c_str() ) > 0 )
            node = node->getRight();
        else if( strcmp( value.c_str(), node->get_mt_node()->getHash().c_str() ) < 0 )
            node = node->getLeft();
        else
            return node;
    }

    return NULL;
}
