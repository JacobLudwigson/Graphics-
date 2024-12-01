    // Process spotlights using dirLights
    for (int i = 0; i < NUMDIRLIGHTS; i++) {
        vec3 spotlightPos = dirLights[i].position;
        vec3 spotlightDir = normalize(-dirLights[i].direction); // Spotlight direction (toward the object)

        // Compute ambient component
        vec3 ambient = dirLights[i].ambientComponent * dirLights[i].lightColor * mat.ambientReflect;

        // Compute light direction and distance
        vec3 lightDir = normalize(spotlightPos - FragPos);
        float distance = length(spotlightPos - FragPos);

        // Check if within spotlight cone
        float theta = dot(lightDir, spotlightDir);
        float epsilon = dirLights[i].innerCutoff - dirLights[i].outerCutoff;
        float spotlightIntensity = clamp((theta - dirLights[i].outerCutoff) / epsilon, 0.0, 1.0);

        // Diffuse lighting
        vec3 norm = normalize(Normal);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * dirLights[i].diffuseComponent * dirLights[i].lightColor * mat.diffuseReflect;

        // Specular lighting
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0); // 32 is the shininess
        vec3 specular = spec * dirLights[i].specularComponent * dirLights[i].lightColor * mat.specularReflect;

        // Combine components, scale by spotlight intensity, and apply attenuation
        float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * (distance * distance)); // Quadratic attenuation
        vec3 spotlight = (ambient + diffuse + specular) * spotlightIntensity * attenuation;

        lighting += spotlight * mat.objectColor;
    }