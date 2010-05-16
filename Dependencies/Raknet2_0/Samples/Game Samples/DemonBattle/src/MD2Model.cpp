//#include <iostream.h>
#include <windows.h>
#include <commctrl.h>
#include <windowsx.h>
#include <math.h>
#include <assert.h>

// Gives me the external m_pd3dDevice pointer.  Remove this and put your own header with your direct3d device pointer
#include "..\Common\Include\d3dutil.h"

#include "MD2Model.h"

// External memory manager.  Remove this header
//#include "MemoryManager.h"

// Constructor
MD2Model::MD2Model () 
{
	m_index_list = NULL;
	m_frame_list = NULL;
	m_frames = m_vertices = m_triangles = 0;
}

// Destructor
MD2Model::~MD2Model () 
{
	Destroy();
}


void MD2Model::Destroy (void) 
{
	if( m_frame_list != NULL ) {
		
		for( int i = 0; i < m_frames; i++ )
			delete [] m_frame_list[i].vertex;
		
		delete [] m_frame_list;
		m_frame_list = NULL;
	}
	
	if( m_index_list != NULL ) {
		delete [] m_index_list;
		m_index_list = NULL;
	}

	int size = animationDataArray.size();
	for (int i=0; i < size; i++)
		delete animationDataArray[i];
	animationDataArray.clear();

	for (int i=0; i < MAX_FRAMES; i++)
		MeshData[i].vertex.clear();
}

// Define this to write the md2 model out as you read it
//#define _WRITEMODEL

int MD2Model::Load(const char *filename, D3DXVECTOR3 &axisToRotateAround, float amountToRotateInRadians) 
{
	FILE		*modelfile = NULL;
	char		g_skins[MAX_MD2SKINS][64];
	dstvert_t	base_st[MAX_VERTS];
	byte		buffer[MAX_VERTS*4+128];
	SMD2Header	modelheader;

#ifdef _WRITEMODEL
	// Added to modify files
	FILE *outfile;
#endif
	
	dtriangle_t     tri;
	daliasframe_t	*out;
	
	if( (modelfile = fopen (filename, "rb")) == NULL )
		return 0;

	#ifdef _WRITEMODEL
	if( (outfile = fopen ("output.md2", "wb")) == NULL )
		return 0;
#endif
	
	// Read the header
	fread( &modelheader, 1, sizeof(modelheader), modelfile );
	#ifdef _WRITEMODEL
	fwrite( &modelheader, 1, sizeof(modelheader), outfile );
#endif
	
	modelheader.framesize = (int)&((daliasframe_t *)0)->verts[modelheader.num_xyz];
	
	//copy some data
	m_frames     = modelheader.num_frames;
	m_vertices   = modelheader.num_xyz;
	m_triangles  = modelheader.num_tris;
	
	m_index_list = new make_index_list [modelheader.num_tris];
	m_frame_list = new make_frame_list [modelheader.num_frames];
	
	for( int i = 0; i < modelheader.num_frames; i++)
		m_frame_list[i].vertex = new make_vertex_list [modelheader.num_xyz];
	
	//read skin information
	fread( g_skins, 1, modelheader.num_skins * MAX_SKINNAME, modelfile );
	#ifdef _WRITEMODEL
	fwrite( g_skins, 1, modelheader.num_skins * MAX_SKINNAME, outfile );
#endif
	
	// read indice of the polygon meshes
	fread( base_st, 1, modelheader.num_st * sizeof(base_st[0]), modelfile );
	#ifdef _WRITEMODEL
	fwrite( base_st, 1, (modelheader.num_st) * sizeof(base_st[0]), outfile );
#endif
	
	
	int	max_tex_u = 0, max_tex_v = 0;

	for( i = 0; i < modelheader.num_tris; i++ ) 
	{
		// read vertice
		fread( &tri, 1, sizeof(dtriangle_t), modelfile);
		#ifdef _WRITEMODEL
		fwrite( &tri, 1, sizeof(dtriangle_t), outfile);
#endif
		
		(m_index_list)[i].a = tri.index_xyz[2];
		(m_index_list)[i].b = tri.index_xyz[1];
		(m_index_list)[i].c = tri.index_xyz[0];
	
		// read t&u
		(m_index_list)[i].a_s = base_st[tri.index_st[2]].s;
		(m_index_list)[i].a_t = base_st[tri.index_st[2]].t;
		(m_index_list)[i].b_s = base_st[tri.index_st[1]].s;
		(m_index_list)[i].b_t = base_st[tri.index_st[1]].t;
		(m_index_list)[i].c_s = base_st[tri.index_st[0]].s;
		(m_index_list)[i].c_t = base_st[tri.index_st[0]].t;
		max_tex_u = max( max_tex_u, base_st[tri.index_st[0]].s );
		max_tex_u = max( max_tex_u, base_st[tri.index_st[1]].s );
		max_tex_u = max( max_tex_u, base_st[tri.index_st[2]].s );
		max_tex_v = max( max_tex_v, base_st[tri.index_st[0]].t );
		max_tex_v = max( max_tex_v, base_st[tri.index_st[1]].t );
		max_tex_v = max( max_tex_v, base_st[tri.index_st[2]].t );
	}

	//convert t&u to be valid
	for ( i = 0; i < modelheader.num_tris; i++ ) 
	{
		m_index_list[ i ].a_s /= max_tex_u;
		m_index_list[ i ].b_s /= max_tex_u;
		m_index_list[ i ].c_s /= max_tex_u;
		m_index_list[ i ].a_t /= max_tex_v;
		m_index_list[ i ].b_t /= max_tex_v;
		m_index_list[ i ].c_t /= max_tex_v;
	}

//	m_toolz.FTrace ("Animation-names for : ");
//	m_toolz.FTrace (filename);
//	m_toolz.FTrace ("\n\n");

	SAnimation* ap=0;
	//xMin=9999.999f;
	char thisName[16];

	// Read vertexdata from all animations
	for( i = 0; i < modelheader.num_frames; i++ ) 
	{
		out = (daliasframe_t *)buffer;
		fread( out, 1, modelheader.framesize, modelfile );
		#ifdef _WRITEMODEL
		fwrite( out, 1, modelheader.framesize, outfile );
#endif
		assert(out->name);
		// Chop off the number at the end
		strcpy(thisName, out->name);
		NumberChop(thisName);

		if (i==0 || strcmp(thisName, ap->name)!=0)
		{
			ap = new SAnimation;
			animationDataArray.insert(ap);
			ap->start=i;
			//m_toolz.FTrace (out->name);
			strcpy(ap->name, thisName);
		}

		ap->end=i;
		//m_toolz.FTrace ("\n");
		
		for( int j = 0; j < modelheader.num_xyz; j++ ) 
		{
			(m_frame_list)[i].vertex[j].x = out->verts[j].v[0] * out->scale[0] + out->translate[0];
			(m_frame_list)[i].vertex[j].y = out->verts[j].v[1] * out->scale[1] + out->translate[1];
			(m_frame_list)[i].vertex[j].z = out->verts[j].v[2] * out->scale[2] + out->translate[2];

			// Moved to Init
			/*
			if (xMin==9999.999f) // First values
			{
				xMin=xMax=(m_frame_list)[i].vertex[j].x;
				yMin=yMax=(m_frame_list)[i].vertex[j].y;
				zMin=zMax=(m_frame_list)[i].vertex[j].z;
			}
			else
			{
				if ((m_frame_list)[i].vertex[j].x < xMin)
					xMin = (m_frame_list)[i].vertex[j].x;
				if ((m_frame_list)[i].vertex[j].y < yMin)
					yMin = (m_frame_list)[i].vertex[j].y;
				if ((m_frame_list)[i].vertex[j].z < zMin)
					zMin = (m_frame_list)[i].vertex[j].z;

				if ((m_frame_list)[i].vertex[j].x > xMax)
					xMax = (m_frame_list)[i].vertex[j].x;
				if ((m_frame_list)[i].vertex[j].y > yMax)
					yMax = (m_frame_list)[i].vertex[j].y;
				if ((m_frame_list)[i].vertex[j].z > zMax)
					zMax = (m_frame_list)[i].vertex[j].z;

			}
			*/
			
		}
	}
	

	fclose (modelfile);
	#ifdef _WRITEMODEL
	fclose(outfile);
#endif
	return Init(axisToRotateAround, amountToRotateInRadians);
}

int MD2Model::Init(D3DXVECTOR3 &axisToRotateAround, float amountToRotateInRadians) 
{
	
	MODELVERTEX pVertex;
	
	D3DXCOLOR	LightColor(1.0f, 1.0f, 1.0f, 1.0f );

	// For each animation we use one SMesh
	for ( int i = 0; i < GetFrameCount(); i++ )
	{
		for( int j = 0; j < GetTriangleCount(); j++) 
		{
			pVertex.m_vecPos.x = m_frame_list[i].vertex[m_index_list[j].a].x;
			pVertex.m_vecPos.y = m_frame_list[i].vertex[m_index_list[j].a].z;
			pVertex.m_vecPos.z = m_frame_list[i].vertex[m_index_list[j].a].y;

			pVertex.m_vecTex.x = m_index_list[j].a_s;
			pVertex.m_vecTex.y = m_index_list[j].a_t;
			pVertex.m_dwDiffuse = LightColor;
			MeshData[i].vertex.push_back (pVertex);
			
			
			
			pVertex.m_vecPos.x = m_frame_list[i].vertex[m_index_list[j].b].x;
			pVertex.m_vecPos.y = m_frame_list[i].vertex[m_index_list[j].b].z;
			pVertex.m_vecPos.z = m_frame_list[i].vertex[m_index_list[j].b].y;

			pVertex.m_vecTex.x = m_index_list[j].b_s;
			pVertex.m_vecTex.y = m_index_list[j].b_t;
			pVertex.m_dwDiffuse = LightColor;
			MeshData[i].vertex.push_back (pVertex);
			
			
			
			pVertex.m_vecPos.x = m_frame_list[i].vertex[m_index_list[j].c].x;
			pVertex.m_vecPos.y = m_frame_list[i].vertex[m_index_list[j].c].z;
			pVertex.m_vecPos.z = m_frame_list[i].vertex[m_index_list[j].c].y;
			
			pVertex.m_vecTex.x = m_index_list[j].c_s;
			pVertex.m_vecTex.y = m_index_list[j].c_t;
			pVertex.m_dwDiffuse = LightColor;
			MeshData[i].vertex.push_back (pVertex);
		}
	}

	// Rotate the model in model space if the user desires so we can save a step when making the view matrix tranform
	D3DXMATRIX axisRotationMatrix;
	D3DXVECTOR4 vec4;
	D3DXMatrixRotationAxis (&axisRotationMatrix, &axisToRotateAround, amountToRotateInRadians);

	if (amountToRotateInRadians != 0.0f)
	{
		// Rotate the model and recalculate the dimensions
		for ( int i = 0; i < GetFrameCount(); i++ )
			for( int j = 0; j < GetTriangleCount() * 3; j++) 
			{
				D3DXVec3Transform(&vec4,&(MeshData[i].vertex[j].m_vecPos),&axisRotationMatrix);
				MeshData[i].vertex[j].m_vecPos.x=vec4.x;
				MeshData[i].vertex[j].m_vecPos.y=vec4.y;
				MeshData[i].vertex[j].m_vecPos.z=vec4.z;

				if (i==0)
				{
					xMinAbsolute=xMaxAbsolute=xMin[i]=xMax[i]=vec4.x;
					xMinAbsolute=xMaxAbsolute=yMin[i]=yMax[i]=vec4.y;
					xMinAbsolute=xMaxAbsolute=zMin[i]=zMax[i]=vec4.z;
				}
				else
				{				
					if (vec4.x < xMin[i])
					{
						xMin[i]=vec4.x;
						if (vec4.x < xMinAbsolute)
							xMinAbsolute = vec4.x;
					}
					else if (vec4.x > xMax[i])
					{
						xMax[i]=vec4.x;
						if (vec4.x > xMaxAbsolute)
							xMaxAbsolute = vec4.x;
					}

					if (vec4.y < yMin[i])
					{
						yMin[i]=vec4.y;
						if (vec4.y < yMinAbsolute)
							yMinAbsolute = vec4.y;
					}
					else if (vec4.y > yMax[i])
					{
						yMax[i]=vec4.y;
						if (vec4.y > yMaxAbsolute)
							yMaxAbsolute = vec4.y;
					}

					if (vec4.z < zMin[i])
					{
						zMin[i]=vec4.z;
						if (vec4.z < zMinAbsolute)
							zMinAbsolute = vec4.z;
					}
					else if (vec4.z > zMax[i])
					{
						zMax[i]=vec4.z;
						if (vec4.z > zMaxAbsolute)
							zMaxAbsolute = vec4.z;
					}
				}
			}
	}

	return 1;
}



// If the elapsed time for the current animation is such that the current keyframe is no longer the current keyframe, then call UpdateKeyframe
// Then render based on keyframe values
BOOL MD2Model::Render(unsigned long elapsedTime)
{
	return (SUCCEEDED(m_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, //Typ	
													GetTriangleCount (),		  //Anzahl
													(BYTE**)&MeshData[/*currentKeyframeIndex*/ 0].vertex[0],		  //Pointer auf Daten
													sizeof(MODELVERTEX))));  //Größe Vertex
}

void MD2Model::NumberChop(char *str)
{
	for (int k=0; str[k]; k++)
		;

	assert(k>2);
	*(str+k-2)=0;
}
