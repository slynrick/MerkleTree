#include "customavl.h"

CustomAVL::CustomAVL()
{
    this->root = NULL;
}

CustomAVL::~CustomAVL()
{
    delete root;
}

CustomAVLNode * CustomAVL::rr_rotation( CustomAVLNode * parent )
{
    CustomAVLNode * aux = parent->getRight();
    parent->setRight( aux->getLeft() );
    parent->getRight()->setParent( parent );
    aux->setLeft( parent );
    aux->getLeft()->setParent( aux );
    return aux;
}

CustomAVLNode * CustomAVL::ll_rotation( CustomAVLNode * parent )
{
    CustomAVLNode * aux =  parent->getLeft();
    parent->setLeft( aux->getRight() );
    parent->getLeft()->setParent( parent );
    aux->setRight( parent );
    aux->getRight()->setParent( aux );
    return aux;
}

CustomAVLNode * CustomAVL::lr_rotation( CustomAVLNode * parent )
{
    CustomAVLNode * aux = parent->getLeft();
    parent->setLeft( rr_rotation( aux ) );
    parent->getLeft()->setParent( parent );
    return ll_rotation( parent );
}

CustomAVLNode * CustomAVL::rl_rotation( CustomAVLNode * parent )
{
    CustomAVLNode * aux = parent->getRight();
    parent->setRight( ll_rotation( aux ) );
    parent->getRight()->setParent( parent );
    return rr_rotation( parent );
}

CustomAVLNode * CustomAVL::balance( CustomAVLNode * node )
{

    int hl = node->getLeft() ? node->getLeft()->getHeight() : 0;
    int hr = node->getRight() ? node->getRight()->getHeight() : 0;
    int balance = hl - hr;

    if( balance > 1 )
    {
        int hl2 = node->getLeft()->getLeft() ? node->getLeft()->getLeft()->getHeight() : 0;
        int hr2 = node->getLeft()->getRight() ? node->getLeft()->getRight()->getHeight() : 0;
        int balance2 = hl2 - hr2;

        if( balance2 > 0 )
            node = ll_rotation( node );
        else
            node = lr_rotation( node );
    }
    else if( balance < -1 )
    {
        int hl2 = node->getRight()->getLeft() ? node->getRight()->getLeft()->getHeight() : 0;
        int hr2 = node->getRight()->getRight() ? node->getRight()->getRight()->getHeight() : 0;
        int balance2 = hl2 - hr2;

        if( balance2 > 0 )
            node = rl_rotation( node );
        else
            node = rr_rotation( node );
    }

    return node;
}

bool CustomAVL::insert( MerkleTreeNode * mt_node )
{
    if( !getRoot() )
    {
        this->root = new CustomAVLNode();
        this->root->set_mt_node( mt_node );
        return true;
    }
    else
    {
        CustomAVLNode * node = getRoot();
        while( node )
        {
            if( node->get_mt_node()->getHash().compare( mt_node->getHash() ) < 0 )
            {
                if( !node->getLeft() )
                {
                    CustomAVLNode * new_node = new CustomAVLNode();
                    new_node->set_mt_node( mt_node );
                    new_node->setParent( node );
                    node->setLeft( new_node );
                    node = balance( node );
                    return true;
                }
                node = node->getLeft();
            }
            else
            {
                if( !node->getRight() )
                {
                    CustomAVLNode * new_node = new CustomAVLNode();
                    new_node->set_mt_node( mt_node );
                    new_node->setParent( node );
                    node->setRight( new_node );
                    node = balance( node );
                    return true;
                }
                node = node->getRight();
            }
        }
    }

    return false;
}

bool CustomAVL::remove( MerkleTreeNode * mt_node )
{
    if( !mt_node )
        return false;

    CustomAVLNode * node = getRoot();
    while( node && node->get_mt_node()->getHash().compare( mt_node->getHash() ) != 0 )
    {
        if( node->get_mt_node()->getHash().compare( mt_node->getHash() ) < 0 )
            node = node->getLeft();
        else if( node->get_mt_node()->getHash().compare( mt_node->getHash() ) > 0 )
            node = node->getRight();
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
        if( node->get_mt_node()->getHash().compare( hash ) < 0 )
            node = node->getLeft();
        else if( node->get_mt_node()->getHash().compare( hash ) > 0 )
            node = node->getRight();
    }

    if( node )
        return node->get_mt_node();

    return NULL;
}


