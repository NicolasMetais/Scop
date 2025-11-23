#include <Renderer.hpp>

const char* vertexShaderSource = R"(
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in vec2 aTexCoord;

out vec3 vColor;
out vec3 vNormal;
out vec2 TexCoord;
out vec3 FragPos; //position du vertex a envoyer dans le fragment shader

uniform mat4 MVP;
uniform mat4 model;

void main() {
    gl_Position = MVP * vec4(aPos, 1.0);
    vColor = aColor;
    TexCoord = aTexCoord;

    FragPos = vec3(model * vec4(aPos, 1.0));
    vNormal = mat3(transpose(inverse(model))) * aNormal;
}
)";

const char* fragmentShaderSource = R"(
#version 330 core

in vec3 vColor;
in vec2 TexCoord;
in vec3 FragPos;
in vec3 vNormal;

out vec4 FragColor;

uniform sampler2D texture1;
uniform bool useTexture;
uniform bool hasMtl;
uniform vec3 Kd; //couleur diffuse
uniform vec3 Ka; //couleure ambiante
uniform vec3 lightDir; //direction de la lumiere

void main() {
    vec3 baseColor;
    if (useTexture)
        baseColor = texture(texture1, TexCoord).rgb;
    else if (!hasMtl)
        baseColor = vColor;
    else
        baseColor = Kd;

    vec3 norm = normalize(vNormal);
    vec3 light = normalize(-lightDir);

    float diff = max(dot(norm, light), 0.0);

    float ambientStrength = 0.8;   // augmente => ombres plus claires
    float diffuseStrength = 1.2;   // diminue => ombres plus douces

    vec3 ambient = Ka * ambientStrength;
    vec3 diffuse = baseColor * diff * diffuseStrength;

    vec3 finalColor = ambient + diffuse;

    FragColor = vec4(finalColor, 1.0);
}
)";


GLuint compileShader(GLenum type, const char* source)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    // Vérifier compilation
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Error in shader compilation: " << infoLog << std::endl;
    }
    return shader;
}

GLuint createShaderProgram(const char* vertexSrc, const char* fragmentSrc)
{
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexSrc);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSrc);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    // Vérifier le link
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cerr << "Error in the linking program: " << infoLog << std::endl;
    }

    // Supprimer shaders après link
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}