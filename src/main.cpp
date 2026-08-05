#include "hiddev/hiddevreader.h"
#include "hiddev/hiddevfinder.h"
#include "sdgyrodsu/sdhidframe.h"
#include "sdgyrodsu/presenter.h"
#include "cemuhook/cemuhookprotocol.h"
#include "cemuhook/cemuhookserver.h"
#include "sdgyrodsu/cemuhookadapter.h"
#include "log/log.h"
#include <iostream>
#include <future>
#include <thread>
#include <csignal>

using namespace kmicki::sdgyrodsu;
using namespace kmicki::hiddev;
using namespace kmicki::log;
using namespace kmicki::cemuhook::protocol;
using namespace kmicki::cemuhook;

const LogLevel cLogLevel = LogLevelDebug; // change to Default when configuration is possible
const bool cRunPresenter = false;
const bool cUseHiddevFile = false;
const bool cTestRun = false;

const std::string cVersion = "2.2";   // Release version

bool stop = false;
std::mutex stopMutex = std::mutex();
std::condition_variable stopCV = std::condition_variable();

void SignalHandler(int signal)
{
    {
        LogF msg;
        msg << "Incoming signal: ";
        bool stopCmd = true;
        switch(signal)
        {
            case SIGINT:
                msg << "SIGINT";
                break;
            case SIGTERM:
                msg << "SIGTERM";
                break;
            default:
                msg << "Other";
                stopCmd = false;
                break;
        }
        if(!stopCmd)
        {
            msg << ". Unhandled, ignoring...";
            return;
        }
        msg << ". Stopping...";
    }

    {
        std::lock_guard lock(stopMutex);
        stop = true;
    }
    stopCV.notify_all();
}

void PresenterRun(HidDevReader * reader, kmicki::sdgyrodsu::DeviceProfile const& profile)
{
    reader->Start();
    auto & frameServe = reader->GetServe();
    auto const& data = frameServe.GetPointer();
    int temp;
    void* tempPtr = reinterpret_cast<void*>(&temp);
    Presenter::Initialize(profile.acc1g, profile.gyro1dps, profile.scanTimeUs);
    while(true)
    {
        auto lock = frameServe.GetConsumeLock();
        Presenter::Present(GetSdFrame(*data));
    }
    Presenter::Finish();
}

int main()
{
    signal(SIGINT,SignalHandler);
    signal(SIGTERM,SignalHandler);

    stop = false;

    if(cRunPresenter)
        SetLogLevel(LogLevelNone);
    else
        SetLogLevel(cLogLevel);

    { LogF() << "SteamDeckGyroDSU Version: " << cVersion; }

    static const kmicki::sdgyrodsu::DeviceProfile cDeckProfile = { "Steam Deck", 0x28de, 0x1205, 2, 64, 4000, 16384.0f, 16.0f, true };
    kmicki::sdgyrodsu::DeviceProfile const* profile = &cDeckProfile;

    std::unique_ptr<HidDevReader> readerPtr;

    if(cUseHiddevFile)
    {
        const int cVID = 0x28de;
        const int cPID = 0x1205;
        int hidno = FindHidDevNo(cVID,cPID);
        if(hidno < 0)
        {
            Log("Steam Deck Controls' HID device not found.");
            return 0;
        }
        { LogF() << "Found Steam Deck Controls' HID device at /dev/usb/hiddev" << hidno; }
        readerPtr.reset(new HidDevReader(hidno,profile->frameLen,profile->scanTimeUs));
    }
    else
    {
        profile = FindDeviceProfile();
        if(profile == nullptr)
        {
            Log("No compatible device found (Steam Deck / Legion Go S). Retrying on restart.");
            return 0;
        }
        { LogF() << "Detected device: " << profile->name; }
        readerPtr.reset(new HidDevReader(profile->vid, profile->pid, profile->interface, profile->frameLen, profile->scanTimeUs));
    }

    HidDevReader &reader = *readerPtr;

    reader.SetStartMarker({ 0x01, 0x00, 0x09, 0x40 }); // Beginning of every Steam Decks' HID frame
    reader.SetEnableGyro(profile->needsGyroEnable);

    CemuhookAdapter adapter(reader, *profile);
    reader.SetNoGyro(adapter.NoGyro);
    Server server(adapter);

    uint32_t lastInc = 0;
    int stopping = 0;

    std::unique_ptr<std::thread> presenter;
    if(cRunPresenter)
        presenter.reset(new std::thread(PresenterRun,&reader,*profile));

    if(cTestRun && !cRunPresenter)
        reader.Start();

    {
        std::unique_lock lock(stopMutex);
        stopCV.wait(lock,[]{ return stop; });
    }

    Log("SteamDeckGyroDSU exiting.");

    return 0;
}