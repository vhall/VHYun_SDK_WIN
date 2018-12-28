#pragma once
#include <string>
#include <memory>
#include <unordered_map>

class BroadCastProfile {
public:
  BroadCastProfile(int profileIndex, int width, int height, int ratio_num, int ratio_den, int frameRate, int bitRate);
  int mIndex;
  int mWidth;
  int mHeight;
  int mRatioNum;
  int mRatioDen;
  int mFrameRate;
  int mBitRate;
};

enum BroadCastVideoProfileIndex {
  BROADCAST_VIDEO_PROFILE_UNDEFINED = -1,
  BROADCAST_VIDEO_PROFILE_480P_0 = 0,     // 640 x 480
  BROADCAST_VIDEO_PROFILE_480P_1,         // 852 x 480
  BROADCAST_VIDEO_PROFILE_540P_0,         // 720 x 540
  BROADCAST_VIDEO_PROFILE_540P_1,         // 960 x 540
  BROADCAST_VIDEO_PROFILE_720P_0,         // 960 x 720
  BROADCAST_VIDEO_PROFILE_720P_1,         // 1280 x 720
  BROADCAST_VIDEO_PROFILE_960P_0,         // 1280 x 960
  BROADCAST_VIDEO_PROFILE_1080P_0,        // 1440 x 1080
  BROADCAST_VIDEO_PROFILE_1080P_1         // 1920 x 1080
};
class BroadCastProfileList {
public:
  BroadCastProfileList();
  std::shared_ptr<BroadCastProfile> GetProfile(BroadCastVideoProfileIndex index);
private:
  std::unordered_map<BroadCastVideoProfileIndex, std::shared_ptr<BroadCastProfile>> mList;
};


class VideoProfile {
public:
  VideoProfile(int profileIndex, int width, int height, int ratio_num, int ratio_den, int minFrameRate, int maxFrameRate, int minBitRate, int maxBitRate);
  int mIndex;
  int mWidth;
  int mHeight;
  int mRatioNum;
  int mRatioDen;
  int mMaxFrameRate;
  int mMinFrameRate;
  int mMaxBitRate;
  int mMinBitRate;
};

enum VideoProfileIndex {
  VIDEO_PROFILE_UNDEFINED = -1,
  VIDEO_PROFILE_120P_0 = 0,     // 160 x 120
  VIDEO_PROFILE_120P_1,         // 160 x 90
  VIDEO_PROFILE_180P_0,         // 240 x 180
  VIDEO_PROFILE_180P_1,         // 320 x 180
  VIDEO_PROFILE_240P_0,         // 320 x 240
  VIDEO_PROFILE_240P_1,         // 424 x 240
  VIDEO_PROFILE_360P_0,         // 480 x 360
  VIDEO_PROFILE_360P_1,         // 640 x 360
  VIDEO_PROFILE_480P_0,         // 640 x 480
  VIDEO_PROFILE_480P_1,         // 848 x 480
  VIDEO_PROFILE_720P_0,         // 960 x 720
  VIDEO_PROFILE_720P_1,         // 1280 x 720
  VIDEO_PROFILE_1080P_0,        // 1440 x 1080
  VIDEO_PROFILE_1080P_1,        // 1920 x 1080
  VIDEO_PROFILE_720P_DESKTOP,   // 1280 x 720
  VIDEO_PROFILE_1080P_DESKTOP,  // 1920 x 1080
};

class VideoProfileList {
public:
  VideoProfileList();
  std::shared_ptr<VideoProfile> GetProfile(VideoProfileIndex index);
private:
  std::unordered_map<VideoProfileIndex, std::shared_ptr<VideoProfile>> mProfileList;
};

enum LayoutMode {
  /* 均匀表格布局 */
  CANVAS_LAYOUT_PATTERN_GRID_1 = 0,    // 一人铺满
  CANVAS_LAYOUT_PATTERN_GRID_2_H,      // 左右两格
  CANVAS_LAYOUT_PATTERN_GRID_3_E,      // 正品字
  CANVAS_LAYOUT_PATTERN_GRID_3_D,      // 倒品字
  CANVAS_LAYOUT_PATTERN_GRID_4_M,      // 2行x2列
  CANVAS_LAYOUT_PATTERN_GRID_5_D,      // 2行，上3下2
  CANVAS_LAYOUT_PATTERN_GRID_6_E,      // 2行x3列
  CANVAS_LAYOUT_PATTERN_GRID_9_E,      // 3行x3列
  /* 主次屏浮窗布局 */
  CANVAS_LAYOUT_PATTERN_FLOAT_2_1DR,   // 大屏铺满，小屏悬浮右下角
  CANVAS_LAYOUT_PATTERN_FLOAT_2_1DL,   // 大屏铺满，小屏悬浮左下角
  CANVAS_LAYOUT_PATTERN_FLOAT_3_2DL,   // 大屏铺满，2小屏悬浮左下角
  CANVAS_LAYOUT_PATTERN_FLOAT_6_5D,    // 大屏铺满，一行5个悬浮于下面
  CANVAS_LAYOUT_PATTERN_FLOAT_6_5T,    // 大屏铺满，一行5个悬浮于上面
  /* 主次屏平铺布局 */
  CANVAS_LAYOUT_PATTERN_TILED_5_1T4D,  // 主次平铺，一行4个位于底部
  CANVAS_LAYOUT_PATTERN_TILED_5_1D4T,  // 主次平铺，一行4个位于顶部
  CANVAS_LAYOUT_PATTERN_TILED_5_1L4R,  // 主次平铺，一列4个位于右边
  CANVAS_LAYOUT_PATTERN_TILED_5_1R4L,  // 主次平铺，一列4个位于左边
  CANVAS_LAYOUT_PATTERN_TILED_6_1T5D,  // 主次平铺，一行5个位于底部
  CANVAS_LAYOUT_PATTERN_TILED_6_1D5T,  // 主次平铺，一行5个位于顶部
  CANVAS_LAYOUT_PATTERN_TILED_9_1L8R,  // 主次平铺，右边为（2列x4行=8个块）
  CANVAS_LAYOUT_PATTERN_TILED_9_1R8L,  // 主次平铺，左边为（2列x4行=8个块）
  CANVAS_LAYOUT_PATTERN_TILED_13_1L12R,  // 主次平铺，右边为（3列x4行=12个块）
  CANVAS_LAYOUT_PATTERN_TILED_17_1TL16GRID,  // 主次平铺，1V16，主屏在左上角
  CANVAS_LAYOUT_PATTERN_TILED_9_1D8T,  // 主次平铺，主屏在下，8个（2行x4列）在上
  CANVAS_LAYOUT_PATTERN_TILED_13_1TL12GRID,  // 主次平铺，主屏在左上角，其余12个均铺于其他剩余区域
  CANVAS_LAYOUT_PATTERN_TILED_17_1TL16GRID_E,  // 主次平铺，主屏在左上角，其余16个均铺于其他剩余区域
  /* 自定义坐标布局 */
  CANVAS_LAYOUT_PATTERN_CUSTOM        // 自定义，当使用坐标布局接口时，请使用此
};