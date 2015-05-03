#version 330 core

in vec2 uv;

out vec3 color;

uniform sampler2D tex;
uniform mat4 modelView;
uniform mat4 inverseModelView;

void main() {
    float height0 = texture(tex, vec2(uv.x, uv.y)).r;
    float height1 = texture(tex, vec2(uv.x + 1.0 , uv.y)).r;
    float height2 = texture(tex, vec2(uv.x - 1.0 , uv.y)).r;
    float height3 = texture(tex, vec2(uv.x, uv.y + 1.0 )).r;
    float height4 = texture(tex, vec2(uv.x, uv.y - 1.0)).r;

    vec2 position = uv * 2 - vec2(1.0, 1.0);
    vec3 pos0 = vec3(position.x, height0, -position.y);
    vec3 pos1 = vec3(position.x, height1, -position.y);
    vec3 pos2 = vec3(position.x, height2, -position.y);
    vec3 pos3 = vec3(position.x, height3, -position.y);
    vec3 pos4 = vec3(position.x, height4, -position.y);

    //vec3 normal = normalize(inverseModelView * vec4(cross(pos1 - pos0, pos3 - pos0),1)).xyz;
    
    // http://stackoverflow.com/questions/21079623/how-to-calculate-the-normal-matrix

    vec3 diffuse = ((pos1 - pos0) + (pos2 - pos0) + (pos3 - pos0) + (pos4 - pos0)) / 4.0;
    float diffuseLength = length(diffuse);
	

    if (height0 <= 0.39) {
    	 color = vec3(0.0, 0.0, 1.0);
    } else if (height0 <= 0.47) {
        color = vec3(1.0, 1.0, 0.0);
    } else if (height0 <= 0.7) {
        color = vec3(0, 1.0, 0.0);
    } else {
        color = vec3(1.0, 1.0, 1.0);
    }

    //color = vec3(height0, height0, height0);

    //color *= diffuseLength * 5.0;

    //color = vec3(1.0, 0.0, 0.0);
}
