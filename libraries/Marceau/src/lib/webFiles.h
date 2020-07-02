#ifndef __webFiles_h__
#define __webFiles_h__

struct t_webfiles {
  const char* filename;
  const char* mime;
  const unsigned int len;
  const uint8_t* content;
};

#endif
