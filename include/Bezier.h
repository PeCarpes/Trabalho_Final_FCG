#pragma once

#include <glm/glm.hpp>

/*      This class represents a cubic bezier curve

    Use the tick() method to advance in time with the curve.
    Once current t 
*/

class Bezier
{

private:
    glm::vec4 p0, p1, p2, p3;
    bool increasing = true;

    float b03(float t) const;
    float b13(float t) const;
    float b23(float t) const;
    float b33(float t) const;

    glm::vec4 c() const;
    glm::vec4 c(float t) const;

    float current_t = 0.0f;

public:
    Bezier(const glm::vec4 &p0, const glm::vec4 &p1, const glm::vec4 &p2, const glm::vec4 &p3)
        : p0(p0), p1(p1), p2(p2), p3(p3) {}

    void setP0(const glm::vec4 &p0);
    void setP1(const glm::vec4 &p1);
    void setP2(const glm::vec4 &p2);
    void setP3(const glm::vec4 &p3);

    void reset();
    void tick(float deltaTime);

    glm::vec4 evaluate();        // Get value at current time
    glm::vec4 evaluate(float t); // Get value at an arbitrary time
};