#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
   public:
    Camera() : pos_{0.0, 0.0}, zoom_{1.2}, ratio_{1.0} {
    }

    void shift(const float dx, const float dy, const float scale = 40.0) {
        pos_ += scale * zoom_ * glm::vec2(-dx * ratio_, dy);
    }

    void zoom_in() {
        zoom_ = std::min(zoom_ * 1.1, 20.0);
    }

    void zoom_out() {
        zoom_ = std::max(zoom_ / 1.1, 0.05);
    }

    void set_ratio(const float w, const float h) {
        if (h == 0.0) {
            ratio_ = 1.0;
            return;
        }
        ratio_ = w / h;
    }

    auto matrix() const {
        return glm::ortho(-20.0 * ratio_ * zoom_ + pos_.x,
                          20.0 * ratio_ * zoom_ + pos_.x,
                          -20.0 * zoom_ + pos_.y,
                          20.0 * zoom_ + pos_.y,
                          0.0,
                          1.0);
    }

   private:
    glm::vec2 pos_;
    float zoom_;
    float ratio_;
};

#endif
