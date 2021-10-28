#ifndef DEVICE_H
#define DEVICE_H


/*
    TO DO
    Since QDesktopWidget was already deprecated in Qt 5, and has been removed in Qt 6, together with QApplication::desktop().
    New methods to keep screen aspect ratio are needed.
*/

class Device
{
public:
    Device();
    ~Device();

    int static width()
    {
        return 0;
    }

    int static widthMM()
    {
        return 0;
    }
};

#endif // DEVICE_H
