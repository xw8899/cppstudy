/*
 * index_list.h
 *
 *  Created on: 2017Äê4ÔÂ21ÈÕ
 *      Author: 10031827
 */

#ifndef INDEX_LIST_H_
#define INDEX_LIST_H_

typedef unsigned long WORD32;
#define INVALID_NODE 0xFFFFFFFF

namespace tool
{

template<typename T, WORD32 MAX_NODE_NUM>
class List
{
public:
    struct Node
    {
        WORD32 next;
        T* data;
    };

public:
    List() :
            size(0), headNode(INVALID_NODE), tailNode(INVALID_NODE), freeHeadNode(0)
    {
        Init();
    }

    ~List()
    {
        WORD32 node = headNode;
        while (node != INVALID_NODE)
        {
            delete nodes[node].data;
            node = nodes[node].next;
        }
    }

    void Init()
    {
        for (WORD32 i = 0; i < MAX_NODE_NUM - 1; i++)
        {
            nodes[i].next = i + 1;
        }
        nodes[MAX_NODE_NUM - 1].next = INVALID_NODE;
    }

    WORD32 GetFirstNode()
    {
        return headNode;
    }

    T* GetNextNode(WORD32& iter)
    {
        if (iter != INVALID_NODE)
        {
            T* item = nodes[iter].data;
            iter = nodes[iter].next;
            return item;
        }
        return NULL;
    }

    WORD32 PushFront(T* item)
    {
        if (size >= MAX_NODE_NUM)
        {
            return INVALID_NODE;
        }
        if (headNode == INVALID_NODE)
        {
            headNode = freeHeadNode;
            tailNode = headNode;
            freeHeadNode = nodes[freeHeadNode].next;
            nodes[headNode].next = INVALID_NODE;
            nodes[headNode].data = item;
        }
        else
        {
            WORD32 newNode = freeHeadNode;
            freeHeadNode = nodes[freeHeadNode].next;
            nodes[newNode].next = headNode;
            nodes[newNode].data = item;
            headNode = newNode;
        }

        size++;

        return headNode;
    }

    WORD32 PushBack(T* item)
    {
        if (size >= MAX_NODE_NUM)
        {
            return INVALID_NODE;
        }
        if (tailNode == INVALID_NODE)
        {
            tailNode = freeHeadNode;
            headNode = tailNode;
            freeHeadNode = nodes[freeHeadNode].next;
            nodes[headNode].next = INVALID_NODE;
            nodes[headNode].data = item;
        }
        else
        {
            WORD32 newNode = freeHeadNode;
            freeHeadNode = nodes[freeHeadNode].next;

            nodes[tailNode].next = newNode;
            nodes[newNode].next = INVALID_NODE;
            nodes[newNode].data = item;
            tailNode = newNode;
        }

        size++;

        return tailNode;
    }

    template<typename Pred>
    WORD32 FindNode(Pred pred)
    {
        if (IsEmpty())
            return INVALID_NODE;
        WORD32 node = headNode;
        while (node != INVALID_NODE)
        {
            if (pred(nodes[node].data))
            {
                return node;
            }
            node = nodes[node].next;
        }
        return INVALID_NODE;
    }

    T* GetNode(WORD32 index)
    {
        if (index < MAX_NODE_NUM)
        {
            return nodes[index].data;
        }
        return NULL;
    }

    void RemoveNode(WORD32 index)
    {
        if (IsEmpty())
            return;
        WORD32 node = headNode;
        WORD32 parent = INVALID_NODE;
        while (node != INVALID_NODE)
        {
            if (node == index)
            {
                delete nodes[index].data;
                nodes[index].data = NULL;
                WORD32 nextNode = nodes[index].next;
                nodes[index].next = freeHeadNode;
                freeHeadNode = index;
                if (parent == INVALID_NODE)
                {
                    headNode = nextNode;
                }
                else
                {
                    nodes[parent].next = nextNode;
                }
                --size;
                return;
            }
            parent = node;
            node = nodes[node].next;
        }
    }

    WORD32 Size()
    {
        return size;
    }

    bool IsEmpty()
    {
        return (Size() == 0);
    }

private:
    Node nodes[MAX_NODE_NUM];
    WORD32 size;
    WORD32 headNode;
    WORD32 tailNode;
    WORD32 freeHeadNode;
};
}

#endif /* INDEX_LIST_H_ */
