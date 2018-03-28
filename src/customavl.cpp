#include "customavl.h"

#include "merkletreenode.h"

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
        if( mt_node->getHash() <= currNode->get_mt_node()->getHash() )
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
            if( mt_node->getHash() <= currNode->getLeft()->get_mt_node()->getHash() ) // ll case
                currNode = rotate_right( currNode );

            if( mt_node->getHash() > currNode->getRight()->get_mt_node()->getHash() ) // lr case
            {
                currNode->setLeft( rotate_left( currNode->getLeft() ) );
                currNode = rotate_right( currNode );
            }
        }

        if( bf < -1 )
        {
            if( mt_node->getHash() > currNode->getRight()->get_mt_node()->getHash() ) // rr case
                currNode = rotate_left( currNode );

            if( mt_node->getHash() <= currNode->getLeft()->get_mt_node()->getHash() ) // rl case
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

    CustomAVLNode * node = getRoot();
    while( node && node->get_mt_node()->getHash().compare( mt_node->getHash() ) != 0 )
    {
        if( node->get_mt_node()->getHash() < mt_node->getHash() )
            node = node->getRight();
        else if( node->get_mt_node()->getHash() > mt_node->getHash() )
            node = node->getLeft();
    }

    if( node )
        delete node;

    return true;
}

MerkleTreeNode * CustomAVL::search( string hash )
{
    CustomAVLNode * node = getRoot();
    while( node && node->get_mt_node()->getHash().compare( hash ) != 0 )
    {
        if( node->get_mt_node()->getHash() < hash )
            node = node->getRight();
        else if( node->get_mt_node()->getHash() > hash )
            node = node->getLeft();
    }

    if( node )
        return node->get_mt_node();

    return NULL;
}
