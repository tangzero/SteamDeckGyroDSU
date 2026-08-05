#ifndef _KMICKI_SDGYRODSU_PRESENTER_H_
#define _KMICKI_SDGYRODSU_PRESENTER_H_

#include <vector>
#include "sdhidframe.h"

namespace kmicki::sdgyrodsu
{
    class Presenter
    {
        public:
        static void Initialize(float acc1g = 16384.0f, float gyro1dps = 16.0f, int scanTimeUs = 4000);
        static void Present(SdHidFrame const& frame);
        static void Finish();
    };
}

#endif