/*
 ============================================================================
 Name        : fragment_shader_raycasting.c
 Author      : Lionis de Souza Watanabe
 Version     :
 Copyright   :
 Description : Hello World in C, Ansi-style
 ============================================================================
 */
//#define DELTA 0.004
//#define THRESHOLD 0.2
#define STEPSIZE 0.01
#define AXIAL 0 //plane of visualization planeVis
#define SAGITTAL 1 //plane of visualization planeVis
#define CORONAL 2 //plane of visualization planeVis

uniform sampler1D TF;
uniform sampler3D VOLUME;
uniform vec4 cursorPosition;
uniform vec4 cameraPosition;
uniform bvec3 drawPlane;
uniform bvec3 clippingPlane;

void main(){
	int i;
	vec3 volExtentMin, volExtentMax;
	vec3 direction, position, temp1, temp2, posIncr;
	vec4 dst, src, planeColor;
	float scalar, inside;

	vec4 axialColor = vec4(0.0, 0.08, 0.08, 0.0);
	vec4 sagittalColor = vec4(0.08, 0.0, 0.08, 0.0);
	vec4 coronalColor = vec4(0.08, 0.08, 0.0, 0.0);

	volExtentMin = vec3(0.0, 0.0, 0.0);
	volExtentMax = vec3(1.0, 1.0, 1.0);

	// Initialize accumulated color and opacity
	dst = vec4(0.0, 0.0, 0.0, 0.0);

	// Determine volume entry position
	position = gl_TexCoord[0].xyz;

	// Compute camera position and ray direction, normalizing it
	//camera = (position*gl_ModelViewMatrixInverse).xyz;
  	//cameraPosition = vec4(0.0, 0.0, 1.0, 0.0);
	direction = normalize((gl_ModelViewProjectionMatrixTranspose*cameraPosition).xyz);//=gl_ModelViewMatrix
	posIncr = direction*STEPSIZE;

	// Loop for ray traversal
	for(i=0; i<200; i++){
		// Data access to scalar value in 3D volume texture
		//scalar = texture3D(VOLUME, position+vec3(0,0,0.5)).r;
		scalar = texture3D(VOLUME, position).r;
		// Apply transfer function
		src = texture1D(TF, scalar);

		//Calculate its associated color, by weighting it by its opacity (pg 55)
		src *= src.a;

		//Front-to-back composition
		dst += (1.0-dst.a)*src;
		dst.a += src.a;

		//Drawing planes
		if(drawPlane[AXIAL] == true && position.z > cursorPosition.z && position.z < cursorPosition.z+STEPSIZE){
			dst += axialColor;
		}
		if(drawPlane[SAGITTAL] == true && position.x > cursorPosition.x && position.x < cursorPosition.x+STEPSIZE){
			dst += sagittalColor;
		}
		if(drawPlane[CORONAL] == true && position.y > cursorPosition.y && position.y < cursorPosition.y+STEPSIZE){
			dst += coronalColor;
		}

		//Clipping planes
		if(clippingPlane[AXIAL] == true && position.z < cursorPosition.z){
			dst = 0;
		}
		if(clippingPlane[SAGITTAL] == true && position.x < cursorPosition.x){
			dst = 0;
		}
		if(clippingPlane[CORONAL] == true && position.y < cursorPosition.y){
			dst = 0;
		}

		// Advance ray position along ray direction
		position += posIncr;
		// Ray termination: Test if outside volume...
		temp1 = sign(position - volExtentMin);
		temp2 = sign(volExtentMax - position);
		inside = dot(temp1, temp2);

		//is not in the volume, exit
		if (inside<3.0 || dst.a >= 1.0)
			break;
	}

	gl_FragColor = dst;
	//gl_FragColor = texture1D(TF, 3378);

	//gl_FragColor.rgb = texture3D(VOLUME, position+vec3(0.0,0.0,0.2)).r;
	//gl_FragColor.rgb = posIncr;
	//gl_FragColor.rgb = scalar;
	//gl_FragColor.a = dst.a;
}
