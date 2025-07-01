#include <Bezier.h>

#include <iostream>

/* ======== Bernstein polynomials ======== */

float Bezier::b03(float t) const
{
    return pow((1 - t), 3);
}

float Bezier::b13(float t) const
{
    return 3 * t * pow((1 - t), 2);
}

float Bezier::b23(float t) const
{
    return 3 * pow(t, 2) * (1 - t);
}

float Bezier::b33(float t) const
{
    return pow(t, 3);
}
/* ========================================= */
glm::vec4 Bezier::c(float t) const
{
    return b03(t) * p0 + b13(t) * p1 + b23(t) * p2 + b33(t) * p3;
}

glm::vec4 Bezier::c() const
{
    return this->c(current_t);
}

/* ========================================= */

glm::vec4 Bezier::evaluate()
{
    return this->evaluate(current_t);
}

glm::vec4 Bezier::evaluate(float t)
{
    // If floor(t) is EVEN, the curve is going backwards at the given moment t
    bool backwards = int(t) % 2 == 0;

    if (backwards)
    {
        t = 1 - (t - int(t));
    }
    else
    {
        t = t - int(t);
    }

    return c(t);
}
/* ========================================= */

void Bezier::tick(float deltaTime){

    current_t += deltaTime;

}

float Bezier::getT() const
{
    return current_t;
}

/* ============= Setters =================== */

void Bezier::setP0(const glm::vec4 &p0)
{
    this->p0 = p0;
}
void Bezier::setP1(const glm::vec4 &p1)
{
    this->p1 = p1;
}
void Bezier::setP2(const glm::vec4 &p2)
{
    this->p2 = p2;
}
void Bezier::setP3(const glm::vec4 &p3)
{
    this->p3 = p3;
}

/* ========================================= */
