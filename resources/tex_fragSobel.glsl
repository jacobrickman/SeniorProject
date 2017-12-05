#version 330 core
in vec2 texCoord;
out vec4 color;
uniform sampler2D texBuf;

void main()
{
	vec4 top         = texture(texBuf, vec2(texCoord.x, texCoord.y + 1.0 / 200.0));
	vec4 bottom      = texture(texBuf, vec2(texCoord.x, texCoord.y - 1.0 / 200.0));
	vec4 left        = texture(texBuf, vec2(texCoord.x - 1.0 / 300.0, texCoord.y));
	vec4 right       = texture(texBuf, vec2(texCoord.x + 1.0 / 300.0, texCoord.y));
	vec4 topLeft     = texture(texBuf, vec2(texCoord.x - 1.0 / 300.0, texCoord.y + 1.0 / 200.0));
	vec4 topRight    = texture(texBuf, vec2(texCoord.x + 1.0 / 300.0, texCoord.y + 1.0 / 200.0));
	vec4 bottomLeft  = texture(texBuf, vec2(texCoord.x - 1.0 / 300.0, texCoord.y - 1.0 / 200.0));
	vec4 bottomRight = texture(texBuf, vec2(texCoord.x + 1.0 / 300.0, texCoord.y - 1.0 / 200.0));
	vec4 sx = -topLeft - 2 * left - bottomLeft + topRight   + 2 * right  + bottomRight;
	vec4 sy = -topLeft - 2 * top  - topRight   + bottomLeft + 2 * bottom + bottomRight;
	vec4 sobel = sqrt(sx * sx + sy * sy);
	color = sobel;
}