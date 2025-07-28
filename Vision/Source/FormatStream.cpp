#include <stdio.h>
#include "FormatStream.h"

//*** CMatheStream ***
CFormatStream::CFormatStream():ofstream() {
}

CFormatStream& CFormatStream::operator<<(double x) {
  char buf[20];
  sprintf(buf,"%012 g",x);
  *this << buf;
  return *this;
}


/*CFormatStream& CFormatStream::operator<<(char *c) {
  char buf[20];
  sprintf(buf,"%012 s",c);
  buf[13]=0;
  *this << buf;
  return *this;
} */


