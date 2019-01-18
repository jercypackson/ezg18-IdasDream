#version 460 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

out VertexData {
	vec3 position_world;
	vec3 normal_world;
	vec2 uv;
    flat int drawID;
    vec3 debugColor;
} vert;

uniform mat4 viewProjMatrix;

struct VertData {
    mat4 modelMatix;
    mat4 normalMatrix;
    float twistParam;
    float waveParam;
};

layout(std430, binding = 0) buffer dataBuffer {
	VertData data[];
};

layout(std430, binding = 1) buffer bonesBuffer {
	mat4 bones[];
};

const int NUM_BONES_PER_VEREX = 7;
struct BoneData {
    float weigth[NUM_BONES_PER_VEREX];
};

layout(std430, binding = 2) buffer boneDataBuffer {
	BoneData boneData[];
};

layout(std430, binding = 3) buffer boneDataStartBuffer {
	int boneDataStartIdx[];
};

mat4 createTwistMat(float k, float z) {
    mat4 twist = mat4(1.0f);

    twist[0][0] = cos(k*z);
    twist[0][1] = sin(k*z);

    twist[1][0] = -sin(k*z);
    twist[1][1] = cos(k*z);

    return twist;
}

void main() {

    vec4 NormalL = vec4(normal, 0.0);
    vec4 PosL = vec4(position, 1.0);

    VertData d = data[gl_DrawID];

    //twist
    mat4 twist = createTwistMat(d.twistParam, PosL.z);

    NormalL = twist * NormalL;  //no non-uniform scaling, so no need to transpose and invert
    PosL = twist * PosL;

    if (d.waveParam >= 0) {
        float t = d.waveParam;
        float u = PosL.y;
        PosL = vec4(PosL.x, PosL.y, (sin(t + u) - sin(t + 2)) * 0.3, 1);
        NormalL = vec4(0, NormalL.z * -cos(t+u) * 0.3, 1, 0);
    }


    int startIdx = boneDataStartIdx[gl_DrawID];

    if (startIdx < 0) {
        //no vertex skinning

    } else {
        int boneDataIdx = startIdx + gl_VertexID - gl_BaseVertex;
        
        BoneData bd = boneData[boneDataIdx];
        
        mat4 boneTransform = mat4(0.0f);
        
        for (int i = 0; i < NUM_BONES_PER_VEREX; i++){
            boneTransform += bones[i] * bd.weigth[i];
        }
        
        NormalL = boneTransform * NormalL;
        PosL = boneTransform * PosL;
    }


    
	vert.normal_world = (d.normalMatrix * NormalL).xyz;

	vec4 position_world_ = d.modelMatix * PosL;
	vert.position_world = position_world_.xyz;
	gl_Position = viewProjMatrix * position_world_;

    vert.uv = uv;
    vert.drawID = gl_DrawID;
}