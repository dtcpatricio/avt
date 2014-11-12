#version 330

uniform sampler2D texmap;
uniform sampler2D texmap1;
uniform sampler2D texmap2;
uniform sampler2D texmap3;
uniform sampler2D texmap4;
uniform int texMode;

out vec4 outFrag;

struct Materials {
	vec4 diffuse;
	vec4 ambient;
	vec4 specular;
	vec4 emissive;
	float shininess;
	int texCount;
};
uniform Materials mat;

struct LightSource {
	vec4 spotDir;
	float cutOff;
};
uniform LightSource li;

in Data {
	float stateGlobal;
	float stateLamp;
	vec3 normal;
	vec3 lightDir;
	vec3 lamps[6];
	vec3 eye;
	vec2 tex_coord;
} DataIn;

void main(void)
{
	vec4 dirLight = vec4(0.0); 
	vec4 pointLight = vec4(0.0);
	vec4 spotLight = vec4(0.0);
	vec4 texel;

	vec3 n = vec3(0.0);
	vec3 e = vec3(0.0);

	// Constants for attenuation
	float a = .01;
	float b = .025;
	float c = 0.055;

	n = normalize(DataIn.normal);
	e = normalize(DataIn.eye);


	////////////////////////////////////////
	// Directional Light

	vec3 l_dir = vec3(0.0);
	vec3 h_dir = vec3(0.0);
	vec4 spec_dir = vec4(0.0);
	vec4 l_dif_dir = vec4(0.0);
	float intensity_dir = 0.0;
	float intSpec_dir = 0.0;
	vec4 texelDir = vec4(0.0);
	if(DataIn.stateGlobal == 1){

		l_dir = normalize(DataIn.lightDir);		

		intensity_dir = max(dot(n,l_dir), 0.0);
	
		if (intensity_dir > 0.0) {
			l_dif_dir = intensity_dir * mat.diffuse;

			h_dir = normalize(l_dir + e);
			intSpec_dir = max(dot(h_dir,n), 0.0);
			spec_dir = mat.specular * pow(intSpec_dir, mat.shininess);

			if(texMode == 1)
			{
				texel = texture(texmap1, DataIn.tex_coord);
				texelDir = intensity_dir*texel + spec_dir;
				dirLight = texelDir;
			}
			else
			{
				dirLight = (spec_dir + l_dif_dir);
			}
		}	
	}
	else
	{
		vec3 ld = normalize(DataIn.lightDir + DataIn.eye);
		vec3 sd = normalize(vec3(-li.spotDir));
		float intensity_spot = 0.0;
		if(dot(sd, ld) > .7) {
			intensity_spot = max(dot(n,ld), 0.0);
			if(intensity_spot > 0.0) {
				vec3 h = normalize(ld + e);
				float intSpecSpot = max(dot(h, n), 0.0);
				spec_dir = mat.specular * pow(intSpecSpot, mat.shininess);

				if(texMode == 1)
				{
					texel = texture(texmap1, DataIn.tex_coord);
					texelDir = intensity_spot*texel + spec_dir;
					spotLight = texelDir;
				}
				else
				{
					spotLight = intensity_spot * mat.diffuse + spec_dir;
				}
			}
		}
	}

	///////////////////////////////////////
	// Point Light
	
	vec3 l_pt = vec3(0.0);
	vec3 h_pt = vec3(0.0);
	vec4 spec_pt = vec4(0.0);
	vec4 l_dif_pt = vec4(0.0);
	float intensity_pt = 0.0;
	float intSpec_pt = 0.0;
	float d = 0.0;
	vec4 texelLight = vec4(0.0);

	if(DataIn.stateLamp == 1) {
		
		for(int i = 0; i < 6; i++) {
			d = length(DataIn.lamps[i]);
			l_pt = normalize(DataIn.lamps[i]);
			intensity_pt = max(dot(n, l_pt), 0.0);

			if(intensity_pt > 0.0) {
				l_dif_pt = intensity_pt * mat.diffuse;
			
				h_pt = normalize(l_pt + e);
			
				intSpec_pt = max(dot(h_pt, n), 0.0);
				spec_pt = mat.specular * pow(intSpec_pt, mat.shininess);
				
				float bd = b * d;
				if(texMode == 1)
				{
					texel = texture(texmap1, DataIn.tex_coord);
					texelLight = intensity_pt*texel + spec_pt;
					pointLight += texelLight / (a + bd + c * pow(d,2));
				}
				/*if(texMode == 2)
				{
					texel = texture(texmap3, DataIn.tex_coord);
					texelLight = intensity_pt*texel + spec_pt;
					pointLight += texelLight / (a + bd + c * pow(d,2));
				}*/
				else
				{
					pointLight += max(l_dif_pt + spec_pt, mat.ambient) / (a + bd + c * pow(d,2));
				}
			}
		}
	}
	if(texMode == 1)
	{
		texel = texture(texmap1, DataIn.tex_coord);
		outFrag = max(dirLight + pointLight + spotLight, 0.1*texel);
	}

	if(texMode == 2)
	{
		texel = texture(texmap3, DataIn.tex_coord);
		outFrag = max(dirLight + pointLight + spotLight, 0.5*texel);
	}

	if(texMode == 4)
	{
		texel = texture(texmap4, DataIn.tex_coord);
		if(texel.a < 0.3) {
			discard;
		}
		outFrag = max(dirLight + pointLight + spotLight, 0.5*texel);
	}

	else
	{
		outFrag = max(dirLight + pointLight + spotLight, mat.ambient);
	}

	float dst = length(abs(DataIn.eye.z));
	float fogAmount = exp( -dst*b );
	vec4 fogColor = vec4(0.3,0.4,0.5,1.0);

	outFrag = mix(outFrag, fogColor, fogAmount );

}