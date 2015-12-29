#pragma once

#include "common_imports.h"

enum class ResourceType {
    NOT_DEFINED,
    MESH,
    TEXTURE
};

struct Resource {
    S32 refCount = 0;
    ResourceType type = ResourceType::NOT_DEFINED;
    S32 handle = -1;
};