#include "markletree.h"

#include <sstream>
#include <vector>
#include <cmath>

#define MAX_LEAF_SIZE 256 //256 bytes

MarkleTree::MarkleTree()
{
    this->height = 0;
}

MarkleTree::~MarkleTree()
{
    if( getRoot() )
        delete this->root;
}

bool MarkleTree::insert( const char * data , int len )
{
    if(len > MAX_LEAF_SIZE )
        return false;

    MarkleTreeNode * newnode = new MarkleTreeNode();
    if( this->leaves.size() % 2 != 0 )
    {
        MarkleTreeNode * parent = this->leaves.back()->getParent();
        newnode->setData( data );
        newnode->setParent( parent );
        parent->setRight( newnode );
    }
    else if( this->leaves.size() < (int)pow( 2, this->height ) )
    {
        MarkleTreeNode * parent = this->leaves.back()->getParent()->getParent()->getRight();
        newnode->setData( data );
        newnode->setParent( parent );
        parent->setRight( newnode );
    }
    else
    {
        MarkleTreeNode * parent = new MarkleTreeNode();
        parent->setParent( this->leaves.front()->getParent() );

        MarkleTreeNode * left = new MarkleTreeNode();
        left->setData( this->leaves.front()->getData() );
        left->setParent( parent );

        newnode->setData( data );
        newnode->setParent( parent );

        parent->setChilden( left, newnode );

        this->leaves.pop_front();
        this->leaves.push_back( left );
        ++this->height;
    }

    this->leaves.push_back( newnode );

    MarkleTreeNode * parent = newnode->getParent();
    while( parent != NULL )
    {
        if( !parent->setHash() )
            return false;
        parent = parent->getParent();
    }

    return true;
}

bool MarkleTree::remove( string hash )
{
    for ( deque<MarkleTreeNode*>::iterator it = this->leaves.begin() ; it != this->leaves.end(); ++it )
    {
        if( (*it)->getHash() == hash )
        {
            if( (*it)->getParent()->getLeft()->getHash() == hash )
                (*it)->getParent()->setLeft( NULL );
            else
                (*it)->getParent()->setRight( NULL );

            MarkleTreeNode * node = (*it)->getParent();
            while( node != NULL )
            {
                if( !node->setHash() )
                    return false;
                node = node->getParent();
            }

            this->leaves.erase( it );
            return true;
        }
    }

    return false;
}

MarkleTreeNode * MarkleTree::search( string hash )
{
    for ( deque<MarkleTreeNode*>::iterator it = this->leaves.begin() ; it != this->leaves.end(); ++it )
    {
        if( (*it)->getHash() == hash )
        {
            return (*it);
        }
    }

    return NULL;
}

bool MarkleTree::build( const char * data , int len )
{
    stringstream ss;
    for( int i = 0, count = 0; i < len; ++i, ++count )
    {
        if( count == MAX_LEAF_SIZE )
        {
            MarkleTreeNode * leaf = new MarkleTreeNode();
            leaf->setData( ss.str() );
            this->leaves.push_back( leaf );
            count = 0;
            ss.str("");
        }
        ss << &data[i];
    }

    if( len % MAX_LEAF_SIZE != 0 )
    {
        MarkleTreeNode * leaf = new MarkleTreeNode();
        leaf->setData( ss.str() );
        this->leaves.push_back( leaf );
    }

    ++this->height;
    return populate( this->leaves ).empty();
}

deque<MarkleTreeNode *> MarkleTree::populate( deque<MarkleTreeNode *> data )
{
    deque<MarkleTreeNode*> aux;
    if( data.size() == 2 )
    {
        root = new MarkleTreeNode();
        root->setChilden( data[0], data[1] );
        return aux;
    }
    if( data.size() == 1 )
    {
        root = new MarkleTreeNode();
        root->setLeft( data[0] );
        return aux;
    }
    if( data.empty() )
        return aux;

    for( int i = 0; i < data.size(); i += 2 )
    {
        MarkleTreeNode * parent = new MarkleTreeNode();
        if( i + 1 == data.size() )
        {
            parent->setLeft( data[i] );
            data[i]->setParent( parent );
        }
        else
        {
            parent->setChilden( data[i], data[i+1] );
            data[i]->setParent( parent );
            data[i]->setParent( parent );
        }
        aux.push_back( parent );
    }

    ++this->height;
    return populate( aux );
}

bool MarkleTree::isValid()
{
    if( !getRoot() )
        return false;

    if( getRoot()->isValid() )
    {
        deque<MarkleTreeNode*> aux;
        if( getRoot()->getLeft() )
            aux.push_back( getRoot()->getLeft() );
        if( getRoot()->getRight() )
            aux.push_back( getRoot()->getRight() );

        return childrenIsValid( aux );
    }

    return false;
}

bool MarkleTree::childrenIsValid( deque<MarkleTreeNode*> data )
{
    if( data.empty() )
        return true;

    deque<MarkleTreeNode*> aux;
    for( int i = 0; i < data.size(); ++i )
    {
        if( data[i]->isValid() )
        {
            if( data[i]->getLeft() )
                aux.push_back( data[i]->getLeft() );
            if( data[i]->getRight() )
                aux.push_back( data[i]->getRight() );
        }
        else
            return false;
    }

    return childrenIsValid( aux );
}

bool MarkleTree::syncronize( MarkleTree * tree )
{
    if( !getRoot() || !tree->getRoot() )
        return false;

    this->height = tree->getHeight();

    if( getRoot()->getHash() != tree->getRoot()->getHash() )
    {
        deque<MarkleTreeNode*> aux1, aux2;
        if( !addremoveNodes( getRoot(), tree->getRoot(), true ) )
        {
            if( getRoot()->getLeft()->getHash() != tree->getRoot()->getLeft()->getHash() )
            {
                aux1.push_back( getRoot()->getLeft() );
                aux2.push_back( tree->getRoot()->getLeft() );
            }
        }

        if( !addremoveNodes( getRoot(), tree->getRoot(), false ) )
        {
            if( getRoot()->getRight()->getHash() != tree->getRoot()->getRight()->getHash() )
            {
                aux1.push_back( getRoot()->getRight() );
                aux2.push_back( tree->getRoot()->getRight() );
            }
        }

        getRoot()->setHash( tree->getRoot()->getHash() );

        return syncronizeChildren( aux1, aux2 );
    }
}

bool MarkleTree::syncronizeChildren( deque<MarkleTreeNode *> owndata, deque<MarkleTreeNode *> treedata )
{
    if( owndata.empty() && treedata.empty() )
        return true;

    deque<MarkleTreeNode*> aux1, aux2;
    for( int i = 0; i < owndata.size(); ++i )
    {
        if( owndata[i]->getHash() != treedata[i]->getHash() )
        {
            if( owndata[i]->isLeaf() )
            {
                owndata[i]->setHash( treedata[i]->getHash() );
                owndata[i]->setData( treedata[i]->getData() );
            }
            else
            {

                if( !addremoveNodes( owndata[i], treedata[i], true ) )
                {
                    if( owndata[i]->getLeft()->getHash() != treedata[i]->getLeft()->getHash() )
                    {
                        aux1.push_back( owndata[i]->getLeft() );
                        aux2.push_back( treedata[i]->getLeft() );
                    }
                }

                if( !addremoveNodes( owndata[i], treedata[i], false ) )
                {
                    if( owndata[i]->getRight()->getHash() != treedata[i]->getRight()->getHash() )
                    {
                        aux1.push_back( owndata[i]->getRight() );
                        aux2.push_back( treedata[i]->getRight() );
                    }
                }

                owndata[i]->setHash( treedata[i]->getHash() );
            }
        }
    }

    return syncronizeChildren( aux1, aux2 );
}

bool MarkleTree::addremoveNodes( MarkleTreeNode * ownNode, MarkleTreeNode * treeNode, bool isleft )
{
    if( isleft )
    {
        if( ownNode->getLeft() && !treeNode->getLeft() )
        {
            updateLeaves( true );
            delete ownNode->getLeft();
        }
        else if( !ownNode->getLeft() && treeNode->getLeft() )
        {
            ownNode->setLeft( treeNode->getLeft() );
            updateLeaves( false, ownNode->getLeft() );
        }
        else
        {
            return false;
        }
    }
    else
    {
        if( ownNode->getRight() && !treeNode->getRight() )
        {
            delete ownNode->getRight();
            updateLeaves( true );
        }
        else if( !ownNode->getRight() && treeNode->getRight() )
        {
            ownNode->setRight( treeNode->getRight() );
            updateLeaves( false, ownNode->getRight() );
        }
        else
        {
            return false;
        }
    }

    return true;
}

void MarkleTree::updateLeaves( bool remove , MarkleTreeNode *node )
{
    if( remove )
    {
        for ( deque<MarkleTreeNode*>::iterator it = this->leaves.end() ; it != this->leaves.begin(); --it )
            if( !(*it) )
                this->leaves.erase( it );
        return;
    }

    deque<MarkleTreeNode*> leaves;
    leaves.push_back( node );
    leaves = getLeavesFromNodes( leaves );
    for( int i = 0; i < leaves.size(); ++i )
    {
        if( !search( leaves[i]->getHash() ) && !remove )
            this->leaves.push_back( leaves[i] );
    }
}

deque<MarkleTreeNode*> MarkleTree::getLeavesFromNodes( deque<MarkleTreeNode *> nodes )
{
    if( nodes.front()->isLeaf() )
    {
        return nodes;
    }

    deque<MarkleTreeNode*> aux;
    for( int i = 0; i < nodes.size(); ++i )
    {
        if( nodes[i]->getLeft() )
            aux.push_back( nodes[i]->getLeft() );
        if( nodes[i]->getRight() )
            aux.push_back( nodes[i]->getRight() );
    }

    return getLeavesFromNodes( aux );
}

