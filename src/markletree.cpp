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
        parent->setRight( newnode );
    }
    else if( this->leaves.size() < (int)pow( 2, this->height ) )
    {
        MarkleTreeNode * parent = this->leaves.back()->getParent()->getParent()->getRight();
        newnode->setData( data );
        parent->setRight( newnode );
    }
    else
    {
        MarkleTreeNode * parent = new MarkleTreeNode();
        MarkleTreeNode * left = new MarkleTreeNode();
        left->setData( this->leaves.front()->getData() );
        newnode->setData( data );
        parent->setChilden( left, newnode );

        this->leaves.pop_front();
        this->leaves.push_back( left );
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
    bool isThereRemainder = len % MAX_LEAF_SIZE != 0;
    int numChildren = len/MAX_LEAF_SIZE;
    if( isThereRemainder )
        numChildren += 1;
    this->height = (unsigned)ceil( log2( numChildren ) );

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

    if( isThereRemainder )
    {
        MarkleTreeNode * leaf = new MarkleTreeNode();
        leaf->setData( ss.str() );
        this->leaves.push_back( leaf );
        numChildren +=1;
    }

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

bool MarkleTree::appendTree( MarkleTree * tree )
{
    MarkleTreeNode * newRoot = new MarkleTreeNode();
    if( newRoot->setChilden( getRoot(), tree->getRoot() ) )
    {
        if( this->getRoot()->getRight()->getHash() != tree->getRoot()->getHash() )
            return false;

        unsigned hdiff = tree->getHeight() - this->getHeight();

        this->root = newRoot;

        if( hdiff > 0 )
            this->height += hdiff + 1;
        else
            this->height += 1;

        deque<MarkleTreeNode*> newleaves;
        newleaves.push_back( this->getRoot()->getRight() );
        newleaves = getLeavesFromNodes( newleaves );

        for ( deque<MarkleTreeNode*>::iterator it = newleaves.begin() ; it != newleaves.end(); ++it )
        {
            this->leaves.push_back( (*it) );
        }

        return true;
    }
    return false;
}

vector<string> MarkleTree::auditProof( string hash )
{
    vector<string> data;
    for ( deque<MarkleTreeNode*>::iterator it = this->leaves.begin() ; it != this->leaves.end(); ++it )
    {
        if( (*it)->getHash() == hash )
        {
            MarkleTreeNode * node = (*it);
            while( !node->isRoot() )
            {
                if( node->getParent()->getLeft() )
                {
                    if( node->getParent()->getLeft()->getHash() != node->getHash() )
                    {
                        data.push_back( node->getParent()->getLeft()->getHash() );
                    }
                    else
                    {
                        if( node->getParent()->getRight() )
                            data.push_back( node->getParent()->getRight()->getHash() );
                        else
                            data.push_back( node->getParent()->getLeft()->getHash() );
                    }
                }

                if( node->getParent()->getRight() )
                {
                    if( node->getParent()->getRight()->getHash() != node->getHash() )
                    {
                        data.push_back( node->getParent()->getRight()->getHash() );
                    }
                    else
                    {
                        if( node->getParent()->getLeft() )
                            data.push_back( node->getParent()->getLeft()->getHash() );
                        else
                            data.push_back( node->getParent()->getRight()->getHash() );
                    }
                }
                node = node->getParent();
            }

            break;
        }
    }

    return data;
}

