/* Copyright (C) 2001-2003 W.P. van Paassen - peter@paassen.tmfweb.nl

   This program is free software; you can redistribute it and/or modify it under
   the terms of the GNU General Public License as published by the Free
   Software Foundation; either version 2 of the License, or (at your
   option) any later version.

   This program is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
   for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to the Free
   Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  */

#include <iostream>
#include <fstream>
#include <list>
#include <string>
#include <cmath>
#include "WP_GLState.h"
#include "WP_Camera.h"
#include "WP_TextureManager.h"
#include "WP_AnimationManager.h"
#include "WP_Model.h"

namespace WPCG
{
/////////////////////////////WP_Model//////////////////////////////////

WP_Model::WP_Model(){}

WP_Model::~WP_Model()
{
  WP_TextureManager::getInstance()->removeTextures(this);
  delete[] triangles;
}

//COPY CONSTRUCTOR
WP_Model::WP_Model(const WP_Model &model)
{
  model_name = model.model_name;
  count = model.count;
  tex_id = model.tex_id;

  int i = 0;
  numberTriangles = model.numberTriangles;
  triangles = new unsigned int[numberTriangles * 3];
  material = model.material;

  for (; i < numberTriangles * 3; ++i)
    triangles[i] = model.triangles[i];
}

WP_Model& WP_Model::operator=(const WP_Model &model)
{
  if (this == &model)
    return *this;

  model_name = model.model_name;
  count = model.count;
  tex_id = model.tex_id;

  delete[] triangles;
  int i = 0;
  numberTriangles = model.numberTriangles;
  triangles = new unsigned int[numberTriangles * 3];
  material = model.material;

  for (; i < numberTriangles * 3; ++i)
    triangles[i] = model.triangles[i];

  return *this;
}

bool WP_Model::init()
{
  if (!initModel())
    {
      return false;
    }
  
  return finalizeAll();
}

////////////////////////////////// WP_FRAME ////////////////////////////////////////////

WP_Model::WP_Frame::WP_Frame(const WP_Model::WP_Frame &frame)
{
  model = frame.model;
  int i = 0;
  vertices = new WP_Vertex[model->numberVertices];
  for (; i < model->numberVertices; ++i)
      vertices[i] = frame.vertices[i];
  
  collision_model = frame.collision_model;
  name = frame.name;
}

WP_Model::WP_Frame& WP_Model::WP_Frame::operator=(const WP_Model::WP_Frame &frame)
{
  if (this == &frame)
    return *this;

  delete[] vertices;
  vertices = new WP_Vertex[model->numberVertices];
  
  model = frame.model;
  int i = 0;
  for (; i < model->numberVertices; ++i)
      vertices[i] = frame.vertices[i];
  
  collision_model = frame.collision_model;
  name =  frame.name;
  return *this;
}

////////////////////////////////// WP_AnimatedModel ////////////////////////////////////

//COPY CONSTRUCTOR
WP_AnimatedModel::WP_AnimatedModel(const WP_AnimatedModel& amodel):WP_Model(amodel)
{
  numberFrames = amodel.numberFrames;
  frames = new WP_Frame[numberFrames];

  int i = 0;
  for (; i < numberFrames; ++i)
    frames[i] = amodel.frames[i];

  map<string, WP_FrameCategory*>::const_iterator iter = amodel.categories.begin();
  while(iter != amodel.categories.end())
    {
      WP_FrameCategory *c = new WP_FrameCategory(*(*iter).second);
      categories.insert(map<string, WP_FrameCategory*>::value_type((*iter).first, c));
      iter++;
    }
}

WP_AnimatedModel& WP_AnimatedModel::operator=(const WP_AnimatedModel &amodel)
{
  if (this == &amodel)
    return *this;

  WP_Model::operator=(amodel);

  delete[] frames;
  numberFrames = amodel.numberFrames;
  frames = new WP_Frame[numberFrames];

  int j = 0;
  for (; j < numberFrames; ++j)
    frames[j] = amodel.frames[j];

  map<string, WP_FrameCategory*>::iterator i = categories.begin();
  while(i != amodel.categories.end())
    {
      delete (*i).second;
      i++;
    }

  categories.clear();

  map<string, WP_FrameCategory*>::const_iterator iter = amodel.categories.begin();
  while(iter != amodel.categories.end())
    {
      WP_FrameCategory *c = new WP_FrameCategory(*(*iter).second);
      categories.insert(map<string, WP_FrameCategory*>::value_type((*iter).first, c));
      iter++;
    }
  
  return *this;
}

bool 
WP_AnimatedModel::finalizeAll()
{
  OPCODECREATE OPCC;
  
  OPCC.NbTris = numberTriangles;
  OPCC.NbVerts = numberVertices;
  OPCC.Rules = SPLIT_COMPLETE | SPLIT_SPLATTERPOINTS | SPLIT_GEOMCENTER;
  OPCC.NoLeaf = true;
  OPCC.Quantized = true;
  OPCC.Tris = triangles;

  int k;
  Point* points = new Point[numberVertices];
  OPCC.Verts = points;

  cout << "Creating collision models" << endl;

  for (k = 0; k < numberFrames; ++k)
    {
      int i = 0;
      for (; i < numberVertices; ++i)
	points[i].Set(frames[k].vertices[i].point.data);

      frames[k].collision_model.Build(OPCC);
    }

  delete[] points;
  return true;
}


/////////////////////////// WP_NonAnimatedModel //////////////////////////////////

bool 
WP_NonAnimatedModel::finalizeAll()
{
 //init OPCODE_Model for only frame

  OPCODECREATE OPCC;

  OPCC.NbTris = numberTriangles;
  OPCC.NbVerts = numberVertices;

  OPCC.Tris = triangles;

  //convert WP_Vertex to Point of OPC library

  Point* points = new Point[numberVertices];

  int i = 0;

  for (; i < numberVertices; ++i)
      points[i].Set(frame->vertices[i].point.data);

  OPCC.Verts = points;

  OPCC.Rules = SPLIT_COMPLETE | SPLIT_SPLATTERPOINTS | SPLIT_GEOMCENTER;
  OPCC.NoLeaf = false;
  OPCC.Quantized = true;

  cout << "Creating collision model" << endl;
  frame->collision_model.Build(OPCC);

  delete[] points;

  return true;
}

/////////////////////////// WP_Model_MD2 //////////////////////////////////////////
// Quake 2 file format model loader

WP_Vector3D WP_Model_MD2::quake2_normals[] = 
  {
    WP_Vector3D(-0.525731, 0.000000, 0.850651), 
    WP_Vector3D(-0.442863, 0.238856, 0.864188),
    WP_Vector3D(-0.295242, 0.000000, 0.955423), 
    WP_Vector3D(-0.309017, 0.500000, 0.809017), 
    WP_Vector3D(-0.162460, 0.262866, 0.951056), 
    WP_Vector3D(0.000000, 0.000000, 1.000000), 
    WP_Vector3D(0.000000, 0.850651, 0.525731), 
    WP_Vector3D(-0.147621, 0.716567, 0.681718), 
    WP_Vector3D(0.147621, 0.716567, 0.681718), 
    WP_Vector3D(0.000000, 0.525731, 0.850651), 
    WP_Vector3D(0.309017, 0.500000, 0.809017), 
    WP_Vector3D(0.525731, 0.000000, 0.850651), 
    WP_Vector3D(0.295242, 0.000000, 0.955423), 
    WP_Vector3D(0.442863, 0.238856, 0.864188), 
    WP_Vector3D(0.162460, 0.262866, 0.951056), 
    WP_Vector3D(-0.681718, 0.147621, 0.716567), 
    WP_Vector3D(-0.809017, 0.309017, 0.500000), 
    WP_Vector3D(-0.587785, 0.425325, 0.688191), 
    WP_Vector3D(-0.850651, 0.525731, 0.000000), 
    WP_Vector3D(-0.864188, 0.442863, 0.238856), 
    WP_Vector3D(-0.716567, 0.681718, 0.147621), 
    WP_Vector3D(-0.688191, 0.587785, 0.425325), 
    WP_Vector3D(-0.500000, 0.809017, 0.309017), 
    WP_Vector3D(-0.238856, 0.864188, 0.442863), 
    WP_Vector3D(-0.425325, 0.688191, 0.587785), 
    WP_Vector3D(-0.716567, 0.681718, -0.147621), 
    WP_Vector3D(-0.500000, 0.809017, -0.309017), 
    WP_Vector3D(-0.525731, 0.850651, 0.000000), 
    WP_Vector3D(0.000000, 0.850651, -0.525731), 
    WP_Vector3D(-0.238856, 0.864188, -0.442863), 
    WP_Vector3D(0.000000, 0.955423, -0.295242), 
    WP_Vector3D(-0.262866, 0.951056, -0.162460), 
    WP_Vector3D(0.000000, 1.000000, 0.000000), 
    WP_Vector3D(0.000000, 0.955423, 0.295242), 
    WP_Vector3D(-0.262866, 0.951056, 0.162460), 
    WP_Vector3D(0.238856, 0.864188, 0.442863), 
    WP_Vector3D(0.262866, 0.951056, 0.162460), 
    WP_Vector3D(0.500000, 0.809017, 0.309017), 
    WP_Vector3D(0.238856, 0.864188, -0.442863), 
    WP_Vector3D(0.262866, 0.951056, -0.162460), 
    WP_Vector3D(0.500000, 0.809017, -0.309017), 
    WP_Vector3D(0.850651, 0.525731, 0.000000), 
    WP_Vector3D(0.716567, 0.681718, 0.147621), 
    WP_Vector3D(0.716567, 0.681718, -0.147621), 
    WP_Vector3D(0.525731, 0.850651, 0.000000), 
    WP_Vector3D(0.425325, 0.688191, 0.587785), 
    WP_Vector3D(0.864188, 0.442863, 0.238856), 
    WP_Vector3D(0.688191, 0.587785, 0.425325), 
    WP_Vector3D(0.809017, 0.309017, 0.500000), 
    WP_Vector3D(0.681718, 0.147621, 0.716567), 
    WP_Vector3D(0.587785, 0.425325, 0.688191), 
    WP_Vector3D(0.955423, 0.295242, 0.000000), 
    WP_Vector3D(1.000000, 0.000000, 0.000000), 
    WP_Vector3D(0.951056, 0.162460, 0.262866), 
    WP_Vector3D(0.850651, -0.525731, 0.000000), 
    WP_Vector3D(0.955423, -0.295242, 0.000000), 
    WP_Vector3D(0.864188, -0.442863, 0.238856), 
    WP_Vector3D(0.951056, -0.162460, 0.262866), 
    WP_Vector3D(0.809017, -0.309017, 0.500000), 
    WP_Vector3D(0.681718, -0.147621, 0.716567), 
    WP_Vector3D(0.850651, 0.000000, 0.525731), 
    WP_Vector3D(0.864188, 0.442863, -0.238856), 
    WP_Vector3D(0.809017, 0.309017, -0.500000), 
    WP_Vector3D(0.951056, 0.162460, -0.262866), 
    WP_Vector3D(0.525731, 0.000000, -0.850651), 
    WP_Vector3D(0.681718, 0.147621, -0.716567), 
    WP_Vector3D(0.681718, -0.147621, -0.716567), 
    WP_Vector3D(0.850651, 0.000000, -0.525731), 
    WP_Vector3D(0.809017, -0.309017, -0.500000), 
    WP_Vector3D(0.864188, -0.442863, -0.238856), 
    WP_Vector3D(0.951056, -0.162460, -0.262866), 
    WP_Vector3D(0.147621, 0.716567, -0.681718), 
    WP_Vector3D(0.309017, 0.500000, -0.809017), 
    WP_Vector3D(0.425325, 0.688191, -0.587785), 
    WP_Vector3D(0.442863, 0.238856, -0.864188), 
    WP_Vector3D(0.587785, 0.425325, -0.688191), 
    WP_Vector3D(0.688191, 0.587785, -0.425325), 
    WP_Vector3D(-0.147621, 0.716567, -0.681718), 
    WP_Vector3D(-0.309017, 0.500000, -0.809017), 
    WP_Vector3D(0.000000, 0.525731, -0.850651), 
    WP_Vector3D(-0.525731, 0.000000, -0.850651), 
    WP_Vector3D(-0.442863, 0.238856, -0.864188), 
    WP_Vector3D(-0.295242, 0.000000, -0.955423), 
    WP_Vector3D(-0.162460, 0.262866, -0.951056), 
    WP_Vector3D(0.000000, 0.000000, -1.000000), 
    WP_Vector3D(0.295242, 0.000000, -0.955423), 
    WP_Vector3D(0.162460, 0.262866, -0.951056), 
    WP_Vector3D(-0.442863, -0.238856, -0.864188), 
    WP_Vector3D(-0.309017, -0.500000, -0.809017), 
    WP_Vector3D(-0.162460, -0.262866, -0.951056), 
    WP_Vector3D(0.000000, -0.850651, -0.525731), 
    WP_Vector3D(-0.147621, -0.716567, -0.681718), 
    WP_Vector3D(0.147621, -0.716567, -0.681718), 
    WP_Vector3D(0.000000, -0.525731, -0.850651), 
    WP_Vector3D(0.309017, -0.500000, -0.809017), 
    WP_Vector3D(0.442863, -0.238856, -0.864188), 
    WP_Vector3D(0.162460, -0.262866, -0.951056), 
    WP_Vector3D(0.238856, -0.864188, -0.442863), 
    WP_Vector3D(0.500000, -0.809017, -0.309017), 
    WP_Vector3D(0.425325, -0.688191, -0.587785), 
    WP_Vector3D(0.716567, -0.681718, -0.147621), 
    WP_Vector3D(0.688191, -0.587785, -0.425325), 
    WP_Vector3D(0.587785, -0.425325, -0.688191), 
    WP_Vector3D(0.000000, -0.955423, -0.295242), 
    WP_Vector3D(0.000000, -1.000000, 0.000000), 
    WP_Vector3D(0.262866, -0.951056, -0.162460), 
    WP_Vector3D(0.000000, -0.850651, 0.525731), 
    WP_Vector3D(0.000000, -0.955423, 0.295242), 
    WP_Vector3D(0.238856, -0.864188, 0.442863), 
    WP_Vector3D(0.262866, -0.951056, 0.162460), 
    WP_Vector3D(0.500000, -0.809017, 0.309017), 
    WP_Vector3D(0.716567, -0.681718, 0.147621), 
    WP_Vector3D(0.525731, -0.850651, 0.000000), 
    WP_Vector3D(-0.238856, -0.864188, -0.442863), 
    WP_Vector3D(-0.500000, -0.809017, -0.309017), 
    WP_Vector3D(-0.262866, -0.951056, -0.162460), 
    WP_Vector3D(-0.850651, -0.525731, 0.000000), 
    WP_Vector3D(-0.716567, -0.681718, -0.147621), 
    WP_Vector3D(-0.716567, -0.681718, 0.147621), 
    WP_Vector3D(-0.525731, -0.850651, 0.000000), 
    WP_Vector3D(-0.500000, -0.809017, 0.309017), 
    WP_Vector3D(-0.238856, -0.864188, 0.442863), 
    WP_Vector3D(-0.262866, -0.951056, 0.162460), 
    WP_Vector3D(-0.864188, -0.442863, 0.238856), 
    WP_Vector3D(-0.809017, -0.309017, 0.500000), 
    WP_Vector3D(-0.688191, -0.587785, 0.425325), 
    WP_Vector3D(-0.681718, -0.147621, 0.716567), 
    WP_Vector3D(-0.442863, -0.238856, 0.864188), 
    WP_Vector3D(-0.587785, -0.425325, 0.688191), 
    WP_Vector3D(-0.309017, -0.500000, 0.809017), 
    WP_Vector3D(-0.147621, -0.716567, 0.681718), 
    WP_Vector3D(-0.425325, -0.688191, 0.587785), 
    WP_Vector3D(-0.162460, -0.262866, 0.951056), 
    WP_Vector3D(0.442863, -0.238856, 0.864188), 
    WP_Vector3D(0.162460, -0.262866, 0.951056), 
    WP_Vector3D(0.309017, -0.500000, 0.809017), 
    WP_Vector3D(0.147621, -0.716567, 0.681718), 
    WP_Vector3D(0.000000, -0.525731, 0.850651), 
    WP_Vector3D(0.425325, -0.688191, 0.587785), 
    WP_Vector3D(0.587785, -0.425325, 0.688191), 
    WP_Vector3D(0.688191, -0.587785, 0.425325), 
    WP_Vector3D(-0.955423, 0.295242, 0.000000), 
    WP_Vector3D(-0.951056, 0.162460, 0.262866), 
    WP_Vector3D(-1.000000, 0.000000, 0.000000), 
    WP_Vector3D(-0.850651, 0.000000, 0.525731), 
    WP_Vector3D(-0.955423, -0.295242, 0.000000), 
    WP_Vector3D(-0.951056, -0.162460, 0.262866), 
    WP_Vector3D(-0.864188, 0.442863, -0.238856), 
    WP_Vector3D(-0.951056, 0.162460, -0.262866), 
    WP_Vector3D(-0.809017, 0.309017, -0.500000), 
    WP_Vector3D(-0.864188, -0.442863, -0.238856), 
    WP_Vector3D(-0.951056, -0.162460, -0.262866), 
    WP_Vector3D(-0.809017, -0.309017, -0.500000), 
    WP_Vector3D(-0.681718, 0.147621, -0.716567), 
    WP_Vector3D(-0.681718, -0.147621, -0.716567), 
    WP_Vector3D(-0.850651, 0.000000, -0.525731), 
    WP_Vector3D(-0.688191, 0.587785, -0.425325), 
    WP_Vector3D(-0.587785, 0.425325, -0.688191), 
    WP_Vector3D(-0.425325, 0.688191, -0.587785), 
    WP_Vector3D(-0.425325, -0.688191, -0.587785), 
    WP_Vector3D(-0.587785, -0.425325, -0.688191), 
    WP_Vector3D(-0.688191, -0.587785, -0.425325), 
  };

WP_Model_MD2::WP_Model_MD2(const string& name): WP_AnimatedModel(name)		
{}

//COPY CONSTRUCTOR
//FIXME get rid of dynamic cast !!!
WP_Model_MD2::WP_Model_MD2(const WP_Model_MD2 &md2model):WP_AnimatedModel(md2model)					
{
  list<WP_TriangleGroup*>::const_iterator j = md2model.triangle_groups.begin();
  while (j != md2model.triangle_groups.end())
    {
      if (WP_TriangleFan *fan = dynamic_cast<WP_TriangleFan*>(*j))
	triangle_groups.push_back(new WP_TriangleFan(*fan));
      else if (WP_TriangleStrip* strip = dynamic_cast<WP_TriangleStrip*>(*j))
	triangle_groups.push_back(new WP_TriangleStrip(*strip));
      j++;
    }  
}

//ASSIGNMENT OPERATOR
WP_Model_MD2& WP_Model_MD2::operator=(const WP_Model_MD2 &md2model)
{
  if (this == &md2model)
    return *this;

  WP_AnimatedModel::operator=(md2model);

  list<WP_TriangleGroup*>::iterator i = triangle_groups.begin();
  while (i != triangle_groups.end())
    {
      delete *i;
      i++;
    } 

  triangle_groups.clear();

  list<WP_TriangleGroup*>::const_iterator j = md2model.triangle_groups.begin();
  while (j != md2model.triangle_groups.end())
    {
      if (WP_TriangleFan *fan = dynamic_cast<WP_TriangleFan*>(*j))
	triangle_groups.push_back(new WP_TriangleFan(*fan));
      else if (WP_TriangleStrip* strip = dynamic_cast<WP_TriangleStrip*>(*j))
	triangle_groups.push_back(new WP_TriangleStrip(*strip));
      j++;
    }  

  return *this;
}

bool WP_Model_MD2::initModel()
{
  int i = 0;
  int j = 0;
  int k = 0;

  WP_Endian* endian = WP_Endian::getInstance();

  try
    {
      ifstream input(model_name.c_str(), ios::in | ios::binary);
      if (!input)
	{
	  throw("Error opening model");
	}

      //read file to memory

      int size = 0;
      char c;
      while(input.get(c))
	{
	  size++;
	}

      byte* buffer = new byte[size];
      if (!buffer)
	{
	  input.close();
	  throw("WP_Model_MD2::initModel: Error while allocating memory");
	}

      input.clear(); //!!!
      input.seekg(ios_base::beg);

      byte* p = buffer;

      while(input.get(c))
	{
	  *p = c;
	  p++;
	}

      input.close();
      p = buffer;  

      WP_MD2_HEADER header;

      endian->getTypedData(&header.magic, p, sizeof(int));
      endian->getTypedData(&header.version, p, sizeof(int));
      endian->getTypedData(&header.skinWidth, p, sizeof(int));
      endian->getTypedData(&header.skinHeight, p, sizeof(int));
      endian->getTypedData(&header.frameSize, p, sizeof(int));
      endian->getTypedData(&header.numSkins, p, sizeof(int));
      endian->getTypedData(&header.numVertices, p, sizeof(int));
      endian->getTypedData(&header.numTexCoords, p, sizeof(int));
      endian->getTypedData(&header.numTriangles, p, sizeof(int));
      endian->getTypedData(&header.numGlCommands, p, sizeof(int));
      endian->getTypedData(&header.numFrames, p, sizeof(int));
      endian->getTypedData(&header.offsetSkins, p, sizeof(int));
      endian->getTypedData(&header.offsetTexCoords, p, sizeof(int));
      endian->getTypedData(&header.offsetTriangles, p, sizeof(int));
      endian->getTypedData(&header.offsetFrames, p, sizeof(int));
      endian->getTypedData(&header.offsetGlCommands, p, sizeof(int));
      endian->getTypedData(&header.offsetEnd, p, sizeof(int));
      
      //get skin
      
      p = buffer + header.offsetSkins;

      char name[64];
      for (i = 0; i < 64; i++)
	{
	  name[i] = *p;
	  p++;
	}

      string sname(name);
  
      int last = sname.find_last_of('/');
      if (last != -1)
	{
	  sname = sname.substr(last + 1, sname.length());
	}

      WP_TextureManager* tex_manager = WP_TextureManager::getInstance();
      tex_id = tex_manager->getTexture(sname, this,true);

      numberFrames = header.numFrames;
      frames = new WP_Frame[numberFrames];
      
      numberTriangles = header.numTriangles;
      triangles = new unsigned int[numberTriangles * 3];

      numberVertices = header.numVertices;

      p = buffer + header.offsetTriangles;
      texCoords = new short[numberTriangles * 3];


      for (k = 0; k < numberTriangles; ++k)
	{
	  short index1, index2, index3;
	  endian->getTypedData(&index1, p, sizeof(short));
	  endian->getTypedData(&index2, p, sizeof(short));
	  endian->getTypedData(&index3, p, sizeof(short));
	  triangles[k * 3] = index1;
	  triangles[k * 3 + 1] = index3;
	  triangles[k * 3 + 2] = index2;
	  endian->getTypedData(&index1, p, sizeof(short));
	  endian->getTypedData(&index2, p, sizeof(short));
	  endian->getTypedData(&index3, p, sizeof(short));
	  texCoords[k * 3] = index1;
	  texCoords[k * 3 + 1] = index3;
	  texCoords[k * 3 + 2] = index2;
	}

      p = buffer + header.offsetFrames;

      /* create matrix which will orientate model into internal objectmanager orientation, being facing north and standing straight up*/
      
      WP_Matrix3D internal_orientation = WP_Matrix3D(Y_ROTATION_MATRIX, 90.0) * WP_Matrix3D(X_ROTATION_MATRIX, -90.0);

      string numerics("0123456789");
      for (k = 0; k < header.numFrames; ++k)
	{
	  (frames + k)->model = this;
	  (frames + k)->vertices = new WP_Vertex[header.numVertices];

	  //(frames + k)->vertexdata = new scalar[header.numVertices * 8]; //vertex position + vertex normal + vertex texture coords
      
	  float x,y,z;
	  endian->getTypedData(&x, p, sizeof(float));
	  endian->getTypedData(&y, p, sizeof(float));
	  endian->getTypedData(&z, p, sizeof(float));
      
	  WP_Matrix3D scale(SCALING_MATRIX, x, y, z);
	  WP_Matrix3D internal_scale(SCALING_MATRIX, 0.1, 0.1, 0.1);
	  
	  endian->getTypedData(&x, p, sizeof(float));
	  endian->getTypedData(&y, p, sizeof(float));
	  endian->getTypedData(&z, p, sizeof(float));
      
	  WP_Matrix3D translate(TRANSLATION_MATRIX, x, y, z);
      
	  for (i = 0; i < 16; i++)
	      (frames + k)->name += *p++;

	  string::size_type pos = (frames + k)->name.find_last_of(numerics);
	  string category = (frames + k)->name.substr(0, pos - 1);

	  WP_FrameCategory *frame_cat;

	  //check if animation category already exist, if yes get it, if no create it	
	  if (categories.count(category))
	    {
	      // exists, so get it and increase total frames
	      frame_cat = categories[category];
	      frame_cat->number_frames++;
	    }
	  else
	    {
	      //does not exist, create it,set start frame and insert it into categories map
	      frame_cat = new WP_FrameCategory(k);
	      categories.insert(map<string, WP_FrameCategory*>::value_type(category, frame_cat));
	    }
	
	WP_Matrix3D normalTransform = internal_orientation;
	normalTransform.inverse();
	normalTransform.transpose();

	  for (j = 0; j < header.numVertices; j++)
	    {
	      byte x,y,z;
	      endian->getTypedData(&x, p, sizeof(byte));
	      endian->getTypedData(&y, p, sizeof(byte));
	      endian->getTypedData(&z, p, sizeof(byte));

	      WP_Point3D point(x, y, z);
	      point *= scale;
	      point *= translate;
	      point *= internal_orientation;
	      point *= internal_scale;
	      endian->getTypedData(&x, p, sizeof(byte));

	      WP_Vector3D normal(quake2_normals[x]);    
	      normal *= normalTransform; //multiply a normal by the inverse transposed of the transformation matrix
	  
	      (frames + k)->vertices[j] = WP_Vertex(point, normal);
	      /*(frames + k)->vertexdata[j * 8 + 2] = point.data[0]; //FIXME use memcpy
	      (frames + k)->vertexdata[j * 8 + 3] = point.data[1];
	      (frames + k)->vertexdata[j * 8 + 4] = point.data[2];
	      (frames + k)->vertexdata[j * 8 + 5] = point.data[0];
	      (frames + k)->vertexdata[j * 8 + 6] = point.data[1];
	      (frames + k)->vertexdata[j * 8 + 7] = point.data[2];*/
	    }
	}  

	      p = buffer + header.offsetTexCoords;
      		texs = new short[header.numTexCoords*2];
	      for (j = 0; j < header.numTexCoords; ++j)
		{
	  	short index1;
	  	endian->getTypedData(&index1, p, sizeof(short));
		texs[j*2] = index1;
	  	endian->getTypedData(&index1, p, sizeof(short));
		texs[j*2+1] = index1;
		
		}

      p = buffer + header.offsetGlCommands;
      
      int command;
      int vertex_index;
      float s,t;

      while(1)
      {
	endian->getTypedData(&command, p, sizeof(int));
      
	if (command == 0)
	  {
	    break; //end of list
	  }
      
	if (command < 0)
	  {
	    //triangle fan
	    command = -command;
	    WP_TriangleFan* fan = new WP_TriangleFan();
	    fan->numberIndices = command;
	    fan->indices = new unsigned int[command];
	    fan->texCoords = new float[command << 1];

	    unsigned int i;
	    for (i = 0; i < command; i++)
	      {
		endian->getTypedData(&s, p, sizeof(float));
		endian->getTypedData(&t, p, sizeof(float));
		endian->getTypedData(&vertex_index, p, sizeof(int));
		
		*(fan->indices + i) = vertex_index;
		*(fan->texCoords + (i << 1)) = s;
		*(fan->texCoords + (i << 1) + 1) = t;
	      }
	    triangle_groups.push_back(fan);
	  }
	else
	  {
	    //triangle strip
	    WP_TriangleStrip* strip = new WP_TriangleStrip();
	    strip->numberIndices = command;
	    strip->indices = new unsigned int[command];
	    strip->texCoords = new float[command << 1];

	    unsigned int i;
	    for (i = 0; i < command; i++)
	      {
		endian->getTypedData(&s, p, sizeof(float));
		endian->getTypedData(&t, p, sizeof(float));
		endian->getTypedData(&vertex_index, p, sizeof(int));

		*(strip->indices + i) = vertex_index;
		*(strip->texCoords + (i << 1)) = s;
		*(strip->texCoords + (i << 1) + 1) = t;
	      }
	    triangle_groups.push_back(strip);
	  }
      }
      
      delete[] buffer;
      
      return true;
    }
  catch(char* s)
    {
      cerr << s << endl;
      return false;
    }
  catch(...)
    {
      return false;
    }
}

void 
WP_Model_MD2::drawOpenGL(const WP_Matrix3D& matrix, WP_Object *object) 
{
  static WP_AnimationManager *ani = WP_AnimationManager::getInstance();
  static WP_GLState *state = WP_GLState::getInstance();

  list<WP_TriangleGroup*>::const_iterator i = triangle_groups.begin();
  unsigned short start_frame, next_frame;
  scalar interpolate;

  glPushMatrix();
  glMultMatrixf(matrix.data); 
  
  glFrontFace(GL_CW); //FIXME 

  glBindTexture(GL_TEXTURE_2D, tex_id);
  state->enableTexture2D(); 

  if (ani->animateObject(object, &start_frame, &next_frame, &interpolate))
    {
      while (i != triangle_groups.end())
	{
	  (*i)->drawOpenGL((frames + start_frame)->vertices, (frames + next_frame)->vertices, interpolate);
	  i++;
	}
    }
  else
    {
      while (i != triangle_groups.end())
	{
	  (*i)->drawOpenGL((frames + start_frame)->vertices);
	  i++;
	}
    }

  state->disableTexture2D();
  glPopMatrix();
  glFrontFace(GL_CCW);
}

////////////////////////// WP_Square ////////////////////

WP_Square::WP_Square(const string& name):WP_NonAnimatedModel(name){}

WP_Square::WP_Square(const WP_Square &s):WP_NonAnimatedModel(s)
{
  center = s.center;
}

WP_Square& 
WP_Square::operator=(const WP_Square &s)
{
  if (this == &s)
    return *this;

  WP_Model::operator=(s);
  center = s.center;

  return *this;
}

void 
WP_Square::drawOpenGL(const WP_Matrix3D& matrix, WP_Object *object)
{
  int i = 0;
  glPushMatrix();
  glMultMatrixf(matrix.data); 

  for (; i < numberTriangles; ++i)
    {
      glBegin(GL_TRIANGLES);
	glColor3f(0.5,0.5,0.5);
      glNormal3fv(frame->vertices[triangles[i * 3]].normal.data);
      glVertex3fv(frame->vertices[triangles[i * 3]].point.data);
	glColor3f(0.5,0.5,0.5);
      glNormal3fv(frame->vertices[triangles[i * 3 + 1]].normal.data);
      glVertex3fv(frame->vertices[triangles[i * 3 + 1]].point.data);
	glColor3f(0.5,0.5,0.5);
      glNormal3fv(frame->vertices[triangles[i * 3 + 2]].normal.data);
      glVertex3fv(frame->vertices[triangles[i * 3 + 2]].point.data);
      glEnd();
    }
  
  glPopMatrix();
}

/**
 * this function is used for reading the model file and initializing the model by filling the variables of the base class with the appropriate read values. This function is automatically called by the base class WP_Model by a call to its <i>init</i> function
 */
bool 
WP_Square::initModel()
{
  //create test_model for collision detection test

  numberVertices = 4;
  numberTriangles = 2;
  triangles = new unsigned int[6];
  frame = new WP_Frame(this);
  frame->vertices = new WP_Vertex[4];

  frame->vertices[0].point = WP_Point3D(10.0, 10.0, 0.0);
  frame->vertices[1].point = WP_Point3D(10.0, -10.0, 0.0);
  frame->vertices[2].point = WP_Point3D(-10.0, 10.0, 0.0);
  frame->vertices[3].point = WP_Point3D(-10.0, -10.0, 0.0);
  frame->vertices[0].normal = WP_Vector3D(0.0,0.0,1.0);
  frame->vertices[1].normal = WP_Vector3D(0.0,0.0,1.0);
  frame->vertices[2].normal = WP_Vector3D(0.0,0.0,1.0);
  frame->vertices[3].normal = WP_Vector3D(0.0,0.0,1.0);
  //frame->vertices[0].normal.normalize();
  //frame->vertices[1].normal.normalize();
  //frame->vertices[2].normal.normalize();
  //frame->vertices[3].normal.normalize();
 

  triangles[0] = 3;
  triangles[1] = 1;
  triangles[2] = 0;
  triangles[3] = 0;
  triangles[4] = 2;
  triangles[5] = 3;

  return true;
}
}
