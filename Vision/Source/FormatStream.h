#if !defined(__FormatStream_h)
#define __FormatStream_h

#include <fstream.h>

class CFormatStream : public ofstream {
public:
  CFormatStream();
  CFormatStream& operator<<(double x);
//  CFormatStream& operator<<(char *c);
};

#endif
