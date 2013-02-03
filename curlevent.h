#ifndef CURLEVENT_H
#define CURLEVENT_H

#include "../genlib/curl.h"


class CurlEvent : public Curl
{
public:
    CurlEvent(QWidget *parent = 0);
};

#endif // CURLEVENT_H
