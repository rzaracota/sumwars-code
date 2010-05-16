///////////////////////////////////////////////////////////
//                                                       //
// PC MAGAZIN - PC Underground                           //
//                                                       //
// Direct3DX Basisprogramm und MD2Model Viewer           //
//                                                       //
///////////////////////////////////////////////////////////
#include <stdio.h>
//#include <iostream.h>
#include <commctrl.h>
#include <math.h>

#include "string.h"
#include "Base3D.h"

#ifndef _MD2Model_H
#define _MD2Model_H

#include "MD2Constants.h"

// Template Linked list class
#include "ArrayList.h"

//used to identify an animation
struct SAnimation
{
	//start and end of an animation
	int start, end;
	//Name
	char name[16];
};

class MD2Model : public Base3D
{
private :
	struct make_index_list
	{
		int a, b, c;
		float	a_s, a_t,
				b_s, b_t,
				c_s, c_t;
	};

	struct make_vertex_list
	{
		float x, y, z;
	};

	struct make_frame_list
	{
		make_vertex_list *vertex;
	};

	typedef unsigned char byte;

	struct vec3_t
	{
		float v[3];
	};


	struct dstvert_t
	{
		short s;
		short t;
	};

	struct dtriangle_t
	{
		  short index_xyz[3];
		  short index_st[3];
	};


	struct dtrivertx_t
	{
		byte v[3];
		byte lightnormalindex;
	};

	//Identify frames
	struct daliasframe_t
	{
		float scale[3];
		float translate[3];
		char name[16];
		dtrivertx_t verts[1];
	};

	struct SMD2Header
	{
	  int ident;
	  int version;

	  int skinwidth;			//Width and height of frame
	  int skinheight;			//Should be dividable by 8

	  int framesize;

	  int num_skins;			//Number of skintextures
	  int num_xyz;			
	  int num_st;
	  int num_tris;				//Count of triangles
	  int num_glcmds;
	  int num_frames;			//Count of fames

	//Some file offsets
	  int ofs_skins;
	  int ofs_st;
	  int ofs_tris;
	  int ofs_frames;
	  int ofs_glcmds; 
	  int ofs_end;

	} ;

	typedef struct
	{
			vec3_t          v;
			int   lightnormalindex;
	} trivert_t;

	typedef struct
	{
			vec3_t          mins, maxs;
			char            name[16];
			trivert_t       v[MAX_VERTS];
	} frame_t;

	make_index_list* m_index_list;
	make_frame_list* m_frame_list;
	long m_frames;

	void NumberChop(char *str);

	//Called by Load
	int Init(D3DXVECTOR3 &axisToRotateAround, float amountToRotateInRadians);
public:
	// The maximum and minimum vertex coordinate values of this object for each keyframe, in local space of course.
	// From my calculations md2 models are not centered around 0,0,0 in local space, so you'll need to use these to set them at the right location
	float xMax[MAX_FRAMES], xMin[MAX_FRAMES], yMax[MAX_FRAMES], yMin[MAX_FRAMES], zMax[MAX_FRAMES], zMin[MAX_FRAMES];
	float xMaxAbsolute,xMinAbsolute,yMaxAbsolute,yMinAbsolute,zMaxAbsolute,zMinAbsolute;

	//Save vertice
	SMesh			 MeshData [MAX_FRAMES];

	MD2Model();
	~MD2Model();
	
	//Loads the file 
	BOOL				Load (const char*, D3DXVECTOR3 &axisToRotateAround, float amountToRotateInRadians);
	//Frees memory
	void				Destroy ();


	// If the elapsed time for the current animation is such that the current keyframe is no longer the current keyframe, then call UpdateKeyframe
	// Then render based on keyframe values
	BOOL Render(unsigned long elapsedTime);

	inline int			GetFrameCount()		{ return    m_frames;	}
	inline int			GetVertexCount()	{ return	m_vertices; }
	inline int			GetTriangleCount()	{ return	m_triangles;}

	BasicDataStructures::List<SAnimation*> animationDataArray;

};

#endif