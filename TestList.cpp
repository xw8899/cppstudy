#include "gtest/gtest.h"
#include <index_list.h>

using namespace tool;

#define MAX_NODES 20
namespace
{
static int clean = 0;
struct Foo
{
    Foo(int a) :
            val(a)
    {

    }
    ~Foo()
    {
        clean++;
        printf("clean foo : %d\n", val);
    }
    int val;
};
}
class ListTest: public testing::Test
{
public:
    ListTest()
    {
    }
    virtual ~ListTest()
    {
        clean = 0;
    }
    List<Foo, MAX_NODES> elems;
};

TEST_F(ListTest, should_be_empty_when_init)
{
    ASSERT_TRUE(elems.IsEmpty());
    ASSERT_EQ(elems.Size(), 0);
    ASSERT_TRUE(elems.GetFirstNode() == INVALID_NODE);
}

TEST_F(ListTest, should_get_elem_when_list_is_not_empty)
{
    Foo *elem = new Foo(1);
    elems.PushFront(elem);
    ASSERT_EQ(elems.Size(), 1);
    WORD32 node = elems.GetFirstNode();
    ASSERT_EQ(elems.GetNode(node)->val, 1);
    ASSERT_EQ(node, 0);
    elems.GetNextNode(node);
    ASSERT_TRUE(node == INVALID_NODE);
}

bool Fun(Foo *f)
{
    if (f->val == 2)
        return true;
    return false;
}
TEST_F(ListTest, should_act_right_when_push_front)
{
    Foo *elem = new Foo(1);
    Foo *elem2 = new Foo(2);
    Foo *elem3 = new Foo(3);
    elems.PushFront(elem);
    elems.PushFront(elem2);
    elems.PushFront(elem3);

    ASSERT_EQ(elems.Size(), 3);
    WORD32 node = elems.GetFirstNode();
    ASSERT_EQ(elems.GetNode(node)->val, 3);
    ASSERT_EQ(node, 2);
    elems.GetNextNode(node);
    //Fun = [x](int n) { return n < x; }
    auto pred = [elem2](Foo* n) { return (n->val == elem2->val); };
    node = elems.FindNode(pred);
    ASSERT_EQ(node, 1);

    elems.RemoveNode(node);
    ASSERT_EQ(elems.Size(), 2);
}

TEST_F(ListTest, should_act_right_when_push_back)
{
    Foo *elem = new Foo(1);
    Foo *elem2 = new Foo(2);
    Foo *elem3 = new Foo(3);
    elems.PushBack(elem);
    elems.PushBack(elem2);
    elems.PushBack(elem3);

    ASSERT_EQ(elems.Size(), 3);
    WORD32 node = elems.GetFirstNode();
    ASSERT_EQ(elems.GetNode(node)->val, 1);
    ASSERT_EQ(node, 0);
    elems.GetNextNode(node);
    ASSERT_EQ(node, 1);

    node = elems.FindNode(Fun);
    ASSERT_EQ(node, 1);

    elems.RemoveNode(node);
    ASSERT_EQ(elems.Size(), 2);
}

TEST_F(ListTest, should_act_right_when_iter)
{
    Foo *elem = new Foo(1);
    Foo *elem2 = new Foo(2);
    Foo *elem3 = new Foo(3);
    elems.PushBack(elem);
    elems.PushBack(elem2);
    elems.PushBack(elem3);

    WORD32 iter = elems.GetFirstNode();
    int actVal = 0;
    while(iter != INVALID_NODE)
    {
        Foo *temp = elems.GetNextNode(iter);
        actVal++;
        ASSERT_EQ(temp->val, actVal);
    }
    ASSERT_EQ(actVal,3);


}

TEST_F(ListTest, should_act_right_when_push_too_many_item)
{
    for (int i = 0; i < MAX_NODES + 2; i++)
    {
        Foo *elem = new Foo(i);
        if (elems.PushBack(elem) == INVALID_NODE)
        {
           delete elem;
        }
    }

    ASSERT_EQ(elems.Size(), MAX_NODES);

    WORD32 node = elems.GetFirstNode();
    int actVal = 0;
    while(node != INVALID_NODE)
    {
        Foo *item = elems.GetNode(node);
        ASSERT_TRUE(item != NULL);
        ASSERT_EQ(item->val, actVal);
        elems.GetNextNode(node);
        actVal++;
    }

    elems.RemoveNode(2);
    elems.RemoveNode(6);
    elems.RemoveNode(9);
    ASSERT_EQ(elems.Size(), 17);
    for (int i = 0; i < MAX_NODES; i++)
    {
        Foo *elem = new Foo(i+100);
        if (elems.PushBack(elem) == INVALID_NODE)
        {
           delete elem;
        }
    }
    ASSERT_EQ(elems.Size(), 20);
}
//
//	TEST("should travel the list")
//	{
//	    Foo elem1(1), elem2(2), elem3(3);
//
//	    elems.pushBack(elem1);
//	    elems.pushBack(elem2);
//	    elems.pushBack(elem3);
//
//	    int i = 1;
//	    LIST_FOREACH(Foo, elem, elems)
//	    {
//	        ASSERT_THAT(elem->getValue(), eq(i++));
//	    }
//	}
//
//	TEST("should point to the correct addr when get next")
//	{
//	    Foo elem(1);
//	    elems.pushBack(elem);
//
//	    ASSERT_THAT(elems.begin().getValue(), eq(&elem));
//	    ASSERT_THAT(elems.end().getValue(), ne(&elem));
//	    List<Foo>::Iterator p = elems.begin();
//	    ASSERT_THAT(elems.getNext(p), eq(elems.end()));
//	}

