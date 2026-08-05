#include "sdgyrodsu/deviceprofile.h"
#include <hidapi/hidapi.h>

namespace kmicki::sdgyrodsu
{
    static const DeviceProfile cProfiles[] =
    {
        { "Steam Deck",   0x28de, 0x1205, 2, 64, 4000, 16384.0f, 16.0f,   true  },
        { "Legion Go S",  0x28de, 0x12ff, -1, 64, 2766,   102.0f, 16.0f, false  },
    };

    static_assert(sizeof(cProfiles)/sizeof(cProfiles[0]) == 2, "profile table drift");

    std::vector<DeviceProfile const*> const& DeviceProfiles()
    {
        static const std::vector<DeviceProfile const*> profiles =
            { &cProfiles[0], &cProfiles[1] };
        return profiles;
    }

    DeviceProfile const* FindDeviceProfile()
    {
        for (auto const& profile : DeviceProfiles())
        {
            auto info = hid_enumerate(profile->vid, profile->pid);
            for (auto dev = info; dev != nullptr; dev = dev->next)
            {
                if (dev->interface_number == profile->interface)
                {
                    hid_free_enumeration(info);
                    return profile;
                }
            }
            if (info != nullptr)
                hid_free_enumeration(info);
        }
        return nullptr;
    }
}
