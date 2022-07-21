#include "PathFollower.h"

void PathFollower::Update(float deltaTime)
{
    int segment;
    glm::vec2 closestPoint = GetClosestPointOnPath(segment);
    float distAhead = glm::length(m_velocity);
    glm::vec2 target = GetPointAlongPath(closestPoint, segment, distAhead);

    // update debug details
    m_debugClosest = closestPoint;
    m_debugTarget = target;

    // find the unit vector direction to the target
    glm::vec2 toTarget = target - m_position;
    toTarget = glm::normalize(toTarget);

    // friction - we want this to balance out acceleration when we're moving at
    // speed. This should provide a force equal to -acceleration when moving at top    
    // speed.
    m_velocity *= (1 - m_acceleration / m_speed * deltaTime);

    // accelerate towards target point
    m_velocity += toTarget * m_acceleration * deltaTime;

    // apply velocity
    m_position += m_velocity * deltaTime;
}

// returns the closest point, and the segment index such that point lies between points [segment] and [segment + 1]
glm::vec2 PathFollower::GetClosestPointOnPath(int& segment)
{
    float closestDistance = FLT_MAX;
    glm::vec2 closestPoint = m_position;
    segment = -1;

    // loop over each segment
    for (int i = 0; i < (int)m_path.size(); i++)
    {
        glm::vec2 segmentStart = m_path[i];
        glm::vec2 segmentEnd = m_path[i == (int)m_path.size() - 1 ? 0 : i + 1];

        glm::vec2 toPosition = m_position - segmentStart;
        glm::vec2 toEnd = segmentEnd - segmentStart;

        // project along the segment to get a value where 0 = level with start and 1 = level with end.
        float alpha = glm::dot(toPosition, toEnd) / glm::dot(toEnd, toEnd);

        // clamp that value
        if (alpha < 0) alpha = 0;
        if (alpha > 1) alpha = 1;

        // closest point on the line segment comes from lerping from start to end with our clamped alpha value
        glm::vec2 point = segmentStart * (1 - alpha) + segmentEnd * alpha;
        float distance = glm::distance(point, m_position);

        if (closestDistance > distance)
        {
            closestDistance = distance;
            closestPoint = point;
            segment = i;
        }
    }
    return closestPoint;
}

// pos is a position on the path, preferably calculated using GetClosestPoint()
// we step along the path segment by segment until we've reached the end or
// travelled (distanceAhead) metres
glm::vec2 PathFollower::GetPointAlongPath(glm::vec2 pos, int segment, float distanceAhead)
{
    glm::vec2 start = pos;
    glm::vec2 end = m_path[segment == m_path.size() - 1 ? 0 : segment + 1];
    while (distanceAhead > 0)
    {
        float remainingDistance = glm::distance(start, end);
        if (distanceAhead <= remainingDistance)
        {
            // we won't reach the end node of this segment with our remaining distance, so lerp towards the end
            float alpha = distanceAhead / remainingDistance;
            return start * (1 - alpha) + end * alpha;
        }
        else
        {
            distanceAhead -= remainingDistance;
            segment++;
            start = end;
            if (segment + 1 >= (int)m_path.size())
                segment = -1;
            end = m_path[segment + 1]; // move on to the next segment and try again
        }
    }
    return end;
}

void PathFollower::SetPosition(float x, float y)
{
    m_position = glm::vec2(x, y);
}

void PathFollower::SetSpeed(float acceleration, float topSpeed)
{
    m_acceleration = acceleration;
    m_speed = topSpeed;
}

void PathFollower::AddPathPoint(float x, float y)
{
    glm::vec2 pathPoint = glm::vec2(x, y);
    m_path.push_back(pathPoint);
}

void PathFollower::Draw()
{
    // draw the path in black
    for (int i = 0; i < (int)m_path.size(); i++)
    {
        glm::vec2 start = m_path[i == 0 ? (int)m_path.size() - 1 : i - 1];
        glm::vec2 end = m_path[i];
        DrawLine((int)start.x, (int)start.y, (int)end.x, (int)end.y, DARKGREEN);
        DrawCircle((int)start.x, (int)start.y, 4, BLACK);
    }

    // draw the agent as a blue circle
    DrawCircle((int)m_position.x, (int)m_position.y, 8, BLUE);

    // draw debug points
    DrawCircle((int)m_debugClosest.x, (int)m_debugClosest.y, 4, { 128,128,128,255 });
    DrawCircle((int)m_debugTarget.x, (int)m_debugTarget.y, 6, { 128,128,128,255 });
}
