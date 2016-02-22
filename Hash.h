#ifndef HASH_H
#define HASH_H
#include <iostream>
class Body;
using namespace std;
struct BodyPair
{
public:

    const Body *body1, *body2;
    BodyPair(const Body *b1,const Body *b2):body1(b1), body2(b2){}
    bool operator ==(const BodyPair&)const;
	bool operator <(const BodyPair&)const;
};
struct KeyHasher
{
    size_t operator()(const BodyPair &k)const;
};
#endif

