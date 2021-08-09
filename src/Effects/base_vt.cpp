#include "base_vt.hpp"

namespace sffx
{
    const char* base_vt = 

R"(
void main(){
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
    gl_FrontColor = gl_Color;
}
)";


} // namespace sffx
