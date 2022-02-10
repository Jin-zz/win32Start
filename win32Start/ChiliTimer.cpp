#include "ChiliTimer.h"

using namespace std::chrono;

ChiliTimer::ChiliTimer() noexcept
{
    last = steady_clock::now();
}

// �����ϴε���mark��������������ʱ��
float ChiliTimer::Mark() noexcept
{
    const auto old = last;
    last = steady_clock::now();  // ����
    const duration<float> frameTime = last - old;
    return frameTime.count();
}
// �����ϴε���mark��������������ʱ�� ��������
float ChiliTimer::Peek() const noexcept
{
    return duration<float>(steady_clock::now() - last).count();
}