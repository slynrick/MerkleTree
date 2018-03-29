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

    if( !getRoot() )
    {
        this->root = newNode;
        return true;
    }

    CustomAVLNode * currNode = getRoot();

    while( currNode )
    {
        if( strcmp( mt_node->getHash().c_str(), currNode->get_mt_node()->getHash().c_str() ) <= 0 )
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
            if( strcmp( mt_node->getHash().c_str(), currNode->getLeft()->get_mt_node()->getHash().c_str() ) <= 0 ) // ll case
                currNode = rotate_right( currNode );

            if( strcmp( mt_node->getHash().c_str(), currNode->getRight()->get_mt_node()->getHash().c_str() ) > 0 ) // lr case
            {
                currNode->setLeft( rotate_left( currNode->getLeft() ) );
                currNode = rotate_right( currNode );
            }
        }

        if( bf < -1 )
        {
            if( strcmp( mt_node->getHash().c_str(), currNode->getRight()->get_mt_node()->getHash().c_str() ) > 0 ) // rr case
                currNode = rotate_left( currNode );

            if( strcmp( mt_node->getHash().c_str(), currNode->getLeft()->get_mt_node()->getHash().c_str() ) <= 0 ) // rl case
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

    return true;

    CustomAVLNode * node = findNode( mt_node->getHash() );

    if( node )
    {
        if( node->isLeaf() )
        {
            delete node;
            return true;
        }

        if( node->getNumChidren() == 1 )
        {
            CustomAVLNode * parent = node->getParent();
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
                CustomAVLNode * child = node->getLeft() ? node->getLeft() : node->getRight();
                this->root = child;
            }
            node->setChilden( NULL, NULL );
            node->setParent( NULL );
            delete node;
            return true;
        }

        if( node->getNumChidren() == 2 )
        {

            delete node;
            return true;
        }
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
