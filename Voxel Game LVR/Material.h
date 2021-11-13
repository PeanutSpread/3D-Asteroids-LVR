#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "Texture.h"

struct Material {
    const Texture*  tex;
    glm::vec4       tint;

    glm::vec3       emissive;    // color/intensity of emissive light
    glm::vec3       specular;    // color/intensity of specular highlights
    float           shininess;   // determines specular highlight "focus"

    Material(const Texture* tex, const glm::vec4& tint = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f))
        : tex(tex)
        , tint(tint)
        , emissive(0, 0, 0)  // does not emit light
        , specular(0, 0, 0)  // no specular hightligts
        , shininess(16)      // medium shininess (meaningless unless specular color is set to something other than black)
    { }
};

#endif
