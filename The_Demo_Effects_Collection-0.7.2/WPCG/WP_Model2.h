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

#ifndef WP_MODEL_H
#define WP_MODEL_H

#include <string>
#include <list>
#include <map>

#include "WPCG.h"//FIXME don't include all headers!

namespace WPCG
{
/**
 * this abstract class represents a 3D model which is used in WP_Object to composite a 3D entity. The model is only the 3D shape, its textures, its bounding hull for collision detection. A WP_Object contains far more, it can contain for instance among others mass, heading, velocity etc etc\n
 */

class WP_Model
 {
   friend class WP_Frame;
 public:
  WP_Model();
  
  /**
   * @param the name of the model to load
   */
  WP_Model(const string& name): model_name(name) ,numberTriangles(0), triangles(0), numberVertices(0) 
    {
    };

  //COPY CONSTRUCTOR
  WP_Model(const WP_Model &model);

  virtual ~WP_Model();

  //ASSIGNMENT OPERATOR
  WP_Model& operator=(const WP_Model &model);
  
  /**
   * this function draws the model according to its world matrix into the 3D scene 
   * @param matrix a WP_Matrix3D object representing the world matrix of the model indicating where and how the model is rendered into the scene
   */
  virtual void drawOpenGL(const WP_Matrix3D& matrix, WP_Object *object) = 0;
  
  /**
   * this virtual function should be overriden by child objects. In this function the model must be read and the model's internals must be filled (like for example the meshes, the material etc). Every 3d file format stores this information in a different way there this function can be used to substract the necessary data from it.
   */
  virtual bool initModel() = 0;
  
  /**
   * this function must be called to init the model. This is based on the Template design pattern.
   */
  bool init();

  /**
   * the name of the model
   */
  string model_name;
  
  int numberTriangles;

  virtual OPCODE_Model* getCollisionModel(const WP_Object *object) = 0;

  virtual WP_Vertex* getVertex(const WP_Object *object, unsigned int index) = 0;

  /**
   * indices into the vertices array, so every three indices make up a triangle 
   */
  unsigned int *triangles; 

  /**
   * the number of vertices in the model
   */
  int numberVertices;

  /**
   * the number of objects using this model, this is used by the WP_ObjectManager object. The model is only discarted by the object manager if this value is zero. This is a so called reference count
   */
  int count;

  /**
   * the OpenGL texture ID of the texture belonging to this model
   */
  GLint tex_id;

 protected:

  /**
   * this function finalizes all after the model was read and everything was initialized. It for instance builds the collision mesh
   */
  virtual bool finalizeAll() = 0;  

 /**
   * this nested class represents a frame. 
   */
  class WP_Frame
    {
    public:
      WP_Frame():vertices(0){};
      WP_Frame(WP_Model* m):model(m), vertices(0){};

      //copy constructor
      WP_Frame(const WP_Frame &frame);

      ~WP_Frame()
	{
	  delete[] vertices;
	}

      //assignment operator
      WP_Frame& operator=(const WP_Frame &frame);

      OPCODE_Model collision_model;

      WP_Model *model;

      /**
       * an array of <i>numberVertices</i> WP_Vertex objects representing the vertices of the frame
       */
      WP_Vertex* vertices;

      /**
       * a WP_Material object holding the OpenGL material of the frame
       */
      WP_Material material;

      /**
       * the name of the frame
       */
      string name;
    };
};

////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * this abstract class represents a non_animated model
 */
 
class WP_NonAnimatedModel: public WP_Model
{
 public:
  WP_NonAnimatedModel(const string& name):WP_Model(name)
    {
      frame = new WP_Frame(this);
    };
  
  virtual ~WP_NonAnimatedModel()
    {
      delete frame;
    };
  
  //COPY CONSTRUCTOR
  WP_NonAnimatedModel(const WP_NonAnimatedModel& namodel):WP_Model(namodel)
    {
      frame = new WP_Frame(*namodel.frame);
    }
  
  //ASSIGNMENT OPERATOR
  WP_NonAnimatedModel& operator=(const WP_NonAnimatedModel &namodel)
    {
      if (this == &namodel)
	return *this;
      
      WP_Model::operator=(namodel);
      
      delete frame;
      frame = new WP_Frame(*namodel.frame);
      
      return *this;
    }
  
  OPCODE_Model* getCollisionModel(const WP_Object *object) 
    {
      return &frame->collision_model;
    }

  WP_Vertex* getVertex(const WP_Object *object, unsigned int index) 
    {
      return &frame->vertices[index];
    }
  
  virtual void drawOpenGL(const WP_Matrix3D& matrix, WP_Object *object) = 0;
  virtual bool initModel() = 0;
  
 protected:
  WP_Frame* frame;

  bool finalizeAll();
};


////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * this abstract class represents an animated model
 */
 
class WP_AnimatedModel: public WP_Model
 {
 public:
  WP_AnimatedModel(const string& name):WP_Model(name),
    frames(0), numberFrames(0){}; 

  //COPY CONSTRUCTOR
  WP_AnimatedModel(const WP_AnimatedModel& amodel);

  virtual ~WP_AnimatedModel()
    {
      delete[] frames;

      //empty map
      map<string, WP_FrameCategory*>::iterator i = categories.begin();
      while(i != categories.end())
	  {
	    delete (*i).second;
	    i++;
	  }
    }

  //ASSIGNMENT OPERATOR
  WP_AnimatedModel& operator=(const WP_AnimatedModel &amodel);

  virtual void drawOpenGL(const WP_Matrix3D& matrix, WP_Object *object) = 0;
  virtual bool initModel() = 0;

  OPCODE_Model* getCollisionModel(const WP_Object *object) 
    {
      static WP_AnimationManager *ani = WP_AnimationManager::getInstance();
      unsigned short cf = ani->getCurrentFrame(object);
      cf += categories[ani->getCategory(object)]->start_frame;
      return &frames[cf].collision_model;
    }

  WP_Vertex* getVertex(const WP_Object *object, unsigned int index) 
    {
      static WP_AnimationManager *ani = WP_AnimationManager::getInstance();
      unsigned short cf = ani->getCurrentFrame(object);
      cf += categories[ani->getCategory(object)]->start_frame;
      return &frames[cf].vertices[index];
    }

  unsigned short getCategoryStartFrame(const string& category) 
    {
      if (categories.count(category))
	  return categories[category]->start_frame;
      return 0;
    }

  unsigned short getCategoryNumberFrames(const string& category) 
    {
      if (categories.count(category))
	  return categories[category]->number_frames;
      return 0;
    }

  string getCategoryName(unsigned int index) const
    {
      map<string, WP_FrameCategory*>::const_iterator i = categories.begin();
      unsigned int count = 0;

      while(i != categories.end())
	{
	  if (index == count)
	    return (*i).first;
	  i++;
	  count++;
	}

      return ""; 
    }

  unsigned int getNumberCategories() const
    {
      return categories.size();
    }

protected:

  WP_Frame* frames;
  unsigned int numberFrames;

  //nested class 
  class WP_FrameCategory
    {
    public:
      WP_FrameCategory(){};
      WP_FrameCategory(unsigned short start): start_frame(start), number_frames(1){};
      ~WP_FrameCategory(){};   

      unsigned short start_frame;
      unsigned short number_frames;
    };

  map<string, WP_FrameCategory*> categories;

  bool finalizeAll();
};

////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * this class is used for the creation of models in ID's MD2 file format, as used in the computer game Quake2\n
 */
class WP_Model_MD2: public WP_AnimatedModel
{
 public:
  /**
   * @param name the name of the file containing the .md2 model
   */
  WP_Model_MD2(const string& name);

  //COPY CONSTRUCTOR
  WP_Model_MD2(const WP_Model_MD2 &md2model);

  ~WP_Model_MD2()
    {
      list<WP_TriangleGroup*>::iterator i = triangle_groups.begin();
      while (i != triangle_groups.end())
	{
	  delete *i;
	  i++;
	}
    };

  //ASSIGNMENT OPERATOR
  WP_Model_MD2& operator=(const WP_Model_MD2 &md2model);

  void drawOpenGL(const WP_Matrix3D& matrix, WP_Object *object);
  
  /**
   * this function is used for reading the model file and initializing the model by filling the variables of the base class with the appropriate read values. This function is automatically called by the base class WP_Model by a call to its <i>init</i> function
   */
  bool initModel();

 private:

  static WP_Vector3D quake2_normals[];

  // nested class WP_MD2_HEADER 
  class WP_MD2_HEADER
    {
  public:
    int magic; 
    int version; 
    int skinWidth; 
    int skinHeight; 
    int frameSize; 
    int numSkins; 
    int numVertices; 
    int numTexCoords; 
    int numTriangles; 
    int numGlCommands; 
    int numFrames; 
    int offsetSkins; 
    int offsetTexCoords; 
    int offsetTriangles; 
    int offsetFrames; 
    int offsetGlCommands; 
    int offsetEnd; 
  }; 

  list<WP_TriangleGroup*> triangle_groups;
};

//////////////////////////////// WP_MetaBall /////////////////////////////////////////////////////

class WP_MetaBall: public WP_NonAnimatedModel
{
 public:
  WP_MetaBall(const string& name);
  WP_MetaBall(const WP_MetaBall &ball);

  WP_MetaBall::~WP_MetaBall(){};

  WP_MetaBall& operator=(const WP_MetaBall &ball);

  void drawOpenGL(const WP_Matrix3D& matrix, WP_Object *object);
  
  /**
   * this function is used for reading the model file and initializing the model by filling the variables of the base class with the appropriate read values. This function is automatically called by the base class WP_Model by a call to its <i>init</i> function
   */
  bool initModel();

  WP_Point3D center;
};
}
#endif

