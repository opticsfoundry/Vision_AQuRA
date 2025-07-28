#if !defined(__MessPoint_h)
#define __MessPoint_h

#include "point.h"

const MaxDataArray=50;

//*** CMessPoint ***
class CMessPoint: public CMyPoint {
public:
  CMessPoint();
  CMessPoint(ifstream *in,int &err);
  CMessPoint(CMyPoint *last);
};

#endif
