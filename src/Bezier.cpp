#include <Bezier.h>

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
    return this->c();
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

// logic defined here is slightly different from the one defined in evaluate(t)
// because we dont really have to account for cases where t is too big
// if t is too big here then the game is broken anyways 

void Bezier::tick(float deltaTime){
    increasing ? current_t += deltaTime: current_t -= deltaTime;

    if(current_t > 1.0f){
        increasing = false;
        current_t = 1.0f - (current_t - 1.0f);
    }
    else if(current_t < 0){
        increasing = true;
        current_t = fabs(current_t);
    }
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
