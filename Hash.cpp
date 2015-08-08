#include "Hash.h"
#include "body.h"
bool BodyPair::operator ==(const BodyPair &bp)const
{
	return (body1->operator ==(bp.body1)) && (body2->operator ==(bp.body2));
}

bool BodyPair::operator <(const BodyPair&bp)const
{
	return ((bp.body1->getmass()) < (body1->getmass()))&&((bp.body2->getmass())<(body2->getmass()));
}

size_t KeyHasher::operator()(const BodyPair &k)const
{
	return size_t(k.body1->getmass()) + size_t(k.body2->getmass());
}