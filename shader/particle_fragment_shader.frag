#version 330 core

out vec4 vFragColor;

// code from http://mmmovania.blogspot.com/2011/01/point-sprites-as-spheres-in-opengl33.html

uniform vec3 lightDir;
float Ns = 250;
vec4 mat_specular=vec4(1);
vec4 light_specular=vec4(1);
vec3 Color = vec3(1.0, 1.0, 1.0);

void main(void)
{
    // calculate normal from texture coordinates
    vec3 N;
    N.xy = gl_PointCoord* 2.0 - vec2(1.0);
    float mag = dot(N.xy, N.xy);
    if (mag > 1.0) discard;   // kill pixels outside circle
    N.z = sqrt(1.0-mag);

    // calculate lighting
    float diffuse = max(0.0, dot(lightDir, N));

    vec3 eye = vec3 (1.0, 1.0, 1.0);
    vec3 halfVector = normalize( eye + lightDir);
    float spec = max( pow(dot(N,halfVector), Ns), 0.);
    vec4 S = light_specular*mat_specular* spec;
    vFragColor = vec4(Color,1) * diffuse + S;
}

//out vec4 color;

//void main(void)
//{

//    color = vec4(gl_PointCoord.x, 0.1f,  0.9f, 1.0f);
//}


//out vec4 vFragColor;

////uniform vec3 lightDir;



//void main(void)
//{
//    vec3 lightDir = normalize(vec3(-1, -1, -1));

//    // calculate normal from texture coordinates
//    vec3 N;
//    N.xy = gl_PointCoord* 2.0 - vec2(1.0);
//    float mag = dot(N.xy, N.xy);
//    if (mag > 1.0) discard;   // kill pixels outside circle
//    N.z = sqrt(1.0-mag);

//    // calculate lighting
//    float diffuse = max(0.0, dot(lightDir, N));

//    vFragColor = vec4(0.1, 0.1, 0.8, 1) * diffuse;
//}

