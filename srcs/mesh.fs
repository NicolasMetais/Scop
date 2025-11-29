#version 330 core

in vec3 vColor;
in vec2 TexCoord;
in vec3 FragPos;
in vec3 vNormal;

out vec4 FragColor;

uniform sampler2D texture1;
uniform bool useTexture;
uniform bool hasMtl;
uniform vec3 Kd; //Kouleur diffuse
uniform vec3 Ka; //Kouleur ambiante
uniform vec3 Ks; //Kouleur speculaire
uniform float Ns; //shininess
uniform int illum;
uniform float d;


uniform vec3 lightDir; //direction de la lumiere
uniform vec3 viewPos;   //pos de la camera
uniform vec3 lightColor;

void main() {
    vec3 baseColor;
    if (useTexture)
        baseColor = texture(texture1, TexCoord).rgb;
    else if (!hasMtl)
    {
        baseColor = vColor;
    }
    else
        baseColor = Kd;

    //setup normized vector
    vec3 norm = normalize(vNormal);
    vec3 light = normalize(-lightDir);
    vec3 viewDir = normalize(viewPos - FragPos);

    //Ambiant (Ka)
    float ambientStrength = 0.8;   // augmente => ombres plus claires
    vec3 ambient = Ka * ambientStrength;

    //Diffuse (Kd)
    float diff = max(dot(norm, light), 0.0);
    float diffuseStrength = 1.2;   // diminue => ombres plus douces
    vec3 diffuse = baseColor * diff * diffuseStrength;

    //Speculaire (Ks)
    vec3 reflectDir = reflect(-light, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), Ns);
    vec3 specular = Ks * spec * lightColor;

    vec3 finalColor = vec3(0.0, 0.0, 0.0);
    //Couleur
    switch(illum){
        case 3:
            finalColor += specular;
        case 2:
            finalColor += diffuse;
        case 1:
            finalColor += ambient;
        break ;
        case 0:
            FragColor = vec4(vColor, d);
        return;
        default: finalColor = baseColor;
    }
    FragColor = vec4(finalColor, d);
}