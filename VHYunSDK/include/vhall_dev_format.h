#ifdef _MSC_VER
#pragma once
#endif // _MSC_VER

#ifndef __VHALL_DEV_FORMAT__
#define __VHALL_DEV_FORMAT__
#endif

#include <string>
#include <vector>
#include <memory>

namespace vhall {

  class WaveFormat
  {
  public:
    WaveFormat() {
      wFormatTag = 0;
      nChannels = 0;
      nSamplesPerSec = 0;
      nAvgBytesPerSec = 0;
      nBlockAlign = 0;
      wBitsPerSample = 0;
      cbSize = 0;
    };
    unsigned short        wFormatTag;         /* format type */
    unsigned short        nChannels;          /* number of channels (i.e. mono, stereo...) */
    unsigned long         nSamplesPerSec;     /* sample rate */
    unsigned long         nAvgBytesPerSec;    /* for buffer estimation */
    unsigned short        nBlockAlign;        /* block size of data */
    unsigned short        wBitsPerSample;     /* number of bits per sample of mono data */
    unsigned short        cbSize;             /* the count in bytes of the size of */
                                    /* extra information (after cbSize) */
  };

  class AudioDevProperty {
  public:
    AudioDevProperty() {};
    AudioDevProperty(int index, std::string devName, std::string devGuid, WaveFormat& format) {
      mIndex = index;
      mDevName = devName;
      mDevGuid = devGuid;
      mFormat = format;
    };
    virtual ~AudioDevProperty() {};
  public:
    int mIndex = -1;
    bool isDefaultDevice = false;
    bool isDefalutCommunicationDevice = false;
    std::string mDevName = "";
    std::string mDevGuid = "";
    WaveFormat mFormat;
  };


  class VideoFormat {
  public:
    VideoFormat() {
      width = 0;
      height = 0;
      maxFPS = 0;
      videoType = 0;
      interlaced = false;
    };
    int width;  // Number of pixels.
    int height;  // Number of pixels.
    int maxFPS;
    int videoType;
    bool interlaced;
  };

  class VideoDevProperty {
  public:
    VideoDevProperty() {};
    virtual ~VideoDevProperty() {};
  public:
    uint32_t mIndex = 0;
    std::string mDevName = "";
    std::string mDevId = "";
    std::vector<std::shared_ptr<VideoFormat>> mDevFormatList;
  };

}