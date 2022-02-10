#include "ChiliTimer.h"

using namespace std::chrono;

ChiliTimer::ChiliTimer() noexcept
{
    last = steady_clock::now();
}

// 返回上次调用mark起到现在所经历的时间
float ChiliTimer::Mark() noexcept
{
    const auto old = last;
    last = steady_clock::now();  // 重置
    const duration<float> frameTime = last - old;
    return frameTime.count();
}
// 返回上次调用mark起到现在所经历的时间 但不重置
float ChiliTimer::Peek() const noexcept
{
    return duration<float>(steady_clock::now() - last).count();
}