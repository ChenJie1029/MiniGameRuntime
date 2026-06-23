#include "Components/VelocityComponent.h"

VelocityComponent::VelocityComponent()
    :velocity(0.0f, 0.0f){}

VelocityComponent::VelocityComponent(const Vec2& velocityValue)
    :velocity(velocityValue){}