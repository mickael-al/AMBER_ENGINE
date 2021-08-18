#ifndef __ENGINE_TRANSFORM__
#define __ENGINE_TRANSFORM__

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/euler_angles.hpp"
#include "glm/common.hpp"
#include "Vector2.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"

struct Transform
{
    glm::mat4 rotationMatrix;
    glm::mat4 translateMatrix;
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 target;
    glm::quat rotation;
    glm::vec3 eulerAngles;
    glm::vec3 scale;
    Vector3 pos;
    Vector4 rot;
    Vector3 eul;
    Vector3 dir;
    Vector3 size;
};

#endif //__ENGINE_TRANSFORM__