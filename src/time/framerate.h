#ifndef CIVILARIUM_TIME_FRAMERATE_H_
#define CIVILARIUM_TIME_FRAMERATE_H_

#include "time/timer.h"

namespace civilarium::time::framerate {

namespace impl {

namespace timer = civilarium::time::timer;

}  // namespace impl

class FrameRateAdjuster {
public:
    explicit FrameRateAdjuster(int frame_rate)
    {
        SetFrameRate(frame_rate);
        SetTimer();
    }

    void SetFrameRate(int frame_rate);

    void SetTimer();
    void Delay() const;

    void Adjust();

private:
    int frame_rate_;         // s-1
    double frame_duration_;  // ms

    impl::timer::SimpleTimer timer_;
};

class FrameRateMeasurer {
public:
    FrameRateMeasurer() : elapsed_frames_(0)
    {
        SetTimer();
    }

    void SetTimer();
    bool MeasureFrameRate(double& measured_frame_rate);

private:
    int elapsed_frames_;

    impl::timer::SimpleTimer timer_;
};

}  // namespace civilarium::time::framerate

#endif  // CIVILARIUM_TIME_FRAMERATE_H_
