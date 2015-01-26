#version 120

varying vec3 color;
varying vec4 line_data;

void main() 
{
    float width_alpha = mix(0.0, 1.0, line_data.z - abs(line_data.x));
    float length_alpha = mix(0.0, 1.0, line_data.w - abs(line_data.y));
    float alpha = min(width_alpha, length_alpha);
    gl_FragColor = vec4(color, alpha);
}
