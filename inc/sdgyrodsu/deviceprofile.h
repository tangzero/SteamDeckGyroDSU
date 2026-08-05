#ifndef _KMICKI_SDGYRODSU_DEVICEPROFILE_H_
#define _KMICKI_SDGYRODSU_DEVICEPROFILE_H_

#include <cstdint>
#include <vector>

namespace kmicki::sdgyrodsu
{
    struct DeviceProfile
    {
        const char* name;
        uint16_t vid;
        uint16_t pid;
        int interface;
        int frameLen;
        int scanTimeUs;
        float acc1g;
        float gyro1dps;
        bool needsGyroEnable;
    };

    std::vector<DeviceProfile const*> const& DeviceProfiles();
    DeviceProfile const* FindDeviceProfile();
}

#endif
