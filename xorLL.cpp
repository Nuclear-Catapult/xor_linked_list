#include <cstdio>
#include <cstdlib>
#include <cstdint>

#include "xorLL.h"

List::List()
{
    this->cursorCount = (uint64_t*) malloc(sizeof(uint64_t));
    *this->cursorCount = 1;
    this->hanger.ptr = (Node*) calloc(sizeof(struct List), 1);
    this->current.ptr = this->hanger.ptr;
    this->previous.ptr = this->current.ptr;
}

List::List(const List &c)
{
    this->cursorCount = c.cursorCount;
    (*cursorCount)++;

    this->hanger.ptr = c.hanger.ptr;
    this->current.ptr = c.current.ptr;
    this->previous.ptr = c.previous.ptr;
}

List::~List()
{
    if (--(*cursorCount) == 0)
        while(deleteNode())
        { /* keep deleting until empty */ }
}

void List::reverse()
{
    this->previous.uintptr ^= this->current.ptr->PxorN;
}

#define SWAP(a, b) a ^= b, b ^= a, a ^= b

void List::moveLeft()
{
    SWAP(this->previous.uintptr, this->current.uintptr);
    reverse();
}

void List::moveRight()
{
    reverse();
    SWAP(this->previous.uintptr, this->current.uintptr);
}

void List::insert(uint64_t data)
{
    this->hanger.ptr->data = data;

    this->hanger.ptr->PxorN = this->previous.uintptr ^ this->current.uintptr;
    this->current.ptr->PxorN ^= this->previous.uintptr ^ this->hanger.uintptr;
    SWAP(this->previous.uintptr, this->hanger.uintptr);
    this->hanger.ptr->PxorN ^= this->current.uintptr ^ this->previous.uintptr;

    this->hanger.ptr = (Node*) malloc(sizeof(struct Node));
}

bool List::isEmpty()
{
    return (this->hanger.ptr == this->current.ptr);
}

uint8_t List::deleteNode()
{
    if (isEmpty())
        return 0;

    free(this->hanger.ptr);
    this->hanger.ptr = this->current.ptr;
    this->current.uintptr = this->current.ptr->PxorN ^ this->previous.uintptr;
    this->current.ptr->PxorN ^= this->hanger.uintptr ^ this->previous.uintptr;
    this->previous.ptr->PxorN ^= this->hanger.uintptr ^ this->current.uintptr;

    return 1;
}

uint64_t List::getData() const
{
    return this->current.ptr->data;
}

