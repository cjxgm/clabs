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

#ifndef WP_OBJECTMANAGER_H
#define WP_OBJECTMANAGER_H

#include <string>
#include <list>
#include "WPCG.h"

namespace WPCG
{
//forward declarations
class WP_Model;
class WP_Camera;
class WP_Object;

/**
 * this abstract class represents an object in the 3D scene.Every object consists of the necessary matrices, a name and a pointer to a model, in this way it is possible to use the same 3D models for different instances of the object\n
 * @author Copyright (C) 2001-2003 W.P. van Paassen   peter@paassen.tmfweb.nl
 *
 *  This program is free software; you can redistribute it and/or modify it under
 *  the terms of the GNU General Public License as published by the Free
 *  Software Foundation; either version 2 of the License, or (at your
 *  option) any later version.
 *
 *  This program is distributed in the hope that it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; see the file COPYING.  If not, write to the Free
 *  Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  
 */
  //class for raytracing 
  class WP_RayHitPoint
    {
    public:
      WP_RayHitPoint(const WP_Vector3D& n, const WP_Point3D& p, const WP_Color& c, const WP_Object* o):normal(n),hitpoint(p),color(c),obj(o){};
      ~WP_RayHitPoint(){};

      WP_Vector3D normal;
      WP_Point3D hitpoint;
      WP_Color color;
      const WP_Object* obj;
    };

class WP_Object
{
 public: 
  virtual ~WP_Object(){}
  
  /**
   * this function is used for drawing the object in OpenGL
   */
  void drawOpenGL(); 

  /**
   * this WP_Matrix3D object holds the world matrix of the object. This matrix is used to render the object with the correct position in the correct orientation in the 3D scene
   */
  WP_Matrix3D matrix;

  /**
   * a string containing the name of the object
   */
  const string object_name;

  /**
   * this WP_Vector3D object represents the direction which the object is facing. With the help of this vector and the up vector, it is possible to determine the object's coordinate system and for example to attach a camera to this object in any orientation. This vector is not used for storing velocity information!
   */
  WP_Vector3D dir;

  /**
   * this WP_Vector3D object represents the 'upness' of the object. With the help of this vector and the dir vector it is possible to determine the object's coordinate system and for example to attach a camera to this object in any orientation
   */
  WP_Vector3D up;

  /**
   * a pointer to a WP_Model object containing the 3D representation of this object
   */
  WP_Model* model;
  
  /**
   * the heading in degrees of the object. In aviation terms this is called yaw
   */
  scalar heading; 
  
  /**
   * Pitch is an aviation term and the pitch of an airplane is the angle that its longitudinal axis (running from tail to nose makes with the horizontal plane
   */
  scalar pitch;

  /**
   * Roll is an aviation term and an airplane rolls by rotating about its longitudinal axis (running from tail to nose). The roll is the amount of rotation relative to the horizontal plane.
   */
  scalar roll;

  PlanesCache planesCache;
  
  /**
   * this function prints the actual state of the object to standard output
   */
  virtual void print() const;

  /**
   * this function returns the x position of the center of the object in the 3D scene
   * @return the x position of the center of the object in the 3D scene
   */
  scalar getXPos() const
    {
      return matrix.data[12];
    };

  virtual void onCollision() = 0;
  
  /**
   * this function returns the y position of the center of the object in the 3D scene
   * @return the y position of the center of the object in the 3D scene
   */
  scalar getYPos() const
    {
      return matrix.data[13];
    };
  
  /**
   * this function returns the z position of the center of the object in the 3D scene
   * @return the z position of the center of the object in the 3D scene
   */
  scalar getZPos() const
    {
      return matrix.data[14];
    };

  /**
   * this function sets the x position of the center of the object in the 3D scene
   * @param x the x position of the center of the object in the 3D scene
   */
  void setXPos(scalar x) 
    {
      matrix.data[12] = x;
    };
  
  /**
   * this function sets the y position of the center of the object in the 3D scene
   * @param y the y position of the center of the object in the 3D scene
   */
  void setYPos(scalar y) 
    {
      matrix.data[13] = y;
    };
  
  /**
   * this function sets the z position of the center of the object in the 3D scene
   * @param z the z position of the center of the object in the 3D scene
   */
  void setZPos(scalar z) 
    {
      matrix.data[14] = z;
    };

  bool isAnimated() const;

  unsigned short getAnimationCategories(string **strings) const;

  bool setAnimationCategory(const string &category) const;

  string getAnimationCategory() const;

  /**
   * this boolean indicates if the object was in the camera's viewing volume (frustum) the last time the scene was rendered
   */
  bool inFrustum;

  /**
   * this integer represents the name of the object. In this way it is possible for OpenGL to distinct between objects. This is for example used by OpenGL for 3D object picking
   */
  GLuint name_id;

 protected:
  WP_Object(){};
  
  /**
   * @param _matrix a WP_Matrix3D object holding the world matrix of this object
   * @param name the name of the object
   */
  WP_Object(const WP_Matrix3D& _matrix, const string& name);
};

/**
 * this class represents a static object in the 3D scene. A static object doesn't change its position and rotation, for example a tree or a wall\n
 * @author Copyright (C) 2001-2003 W.P. van Paassen   peter@paassen.tmfweb.nl
 *
 *  This program is free software; you can redistribute it and/or modify it under
 *  the terms of the GNU General Public License as published by the Free
 *  Software Foundation; either version 2 of the License, or (at your
 *  option) any later version.
 *
 *  This program is distributed in the hope that it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; see the file COPYING.  If not, write to the Free
 *  Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  
 */
class WP_StaticObject: public WP_Object
{
  friend class WP_ObjectManager;
  
 public:
  virtual ~WP_StaticObject(){};

  void onCollision()
    {
      // collision handling 
    }

 protected:
  WP_StaticObject(){};
  
  /**
   * @param _matrix a WP_Matrix3D object holding the world matrix of this object
   * @param name the name of the object
   */
  WP_StaticObject(const WP_Matrix3D& _matrix, const string& name):
    WP_Object(_matrix, name){};

};

/**
 * this class represents a dynamic object in a 3D scene. A dynamic object is an object which is able to change its position and its orientation\n
 * @author Copyright (C) 2001-2003 W.P. van Paassen   peter@paassen.tmfweb.nl
 *
 *  This program is free software; you can redistribute it and/or modify it under
 *  the terms of the GNU General Public License as published by the Free
 *  Software Foundation; either version 2 of the License, or (at your
 *  option) any later version.
 *
 *  This program is distributed in the hope that it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; see the file COPYING.  If not, write to the Free
 *  Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  
 */
class WP_DynamicObject: public WP_Object
{
  friend class WP_ObjectManager;
  
 public:
  virtual ~WP_DynamicObject(){};
  
  /**
   * a WP_Vector3D object representing the velocity vector of the object
   */
  WP_Vector3D velocity;;
 
  /**
   * the speed of the object, this is the length (square root) of the velocity vector
   */
  scalar speed;

  /**
   * this function changes the heading of the object
   * @param delta_degrees the amount of change in degrees of the heading according to the current heading
   */
  void changeHeading(scalar delta_degrees);

  /**
   * this function changes the pitch of the object
   * @param delta_degrees the amount of change in degrees of the pitch according to the current pitch
   */
  void changePitch(scalar delta_degrees);

  /**
   * this function changes the roll of the object
   * @param delta_degrees the amount of change in degrees of the roll according to the current roll
   */
  void changeRoll(scalar delta_degrees);

  /**
   * this function changes the speed of the object and computes the new velocity vector
   * @param delta_speed the amount of change in speed according to the current speed
   */
  void changeSpeed(scalar delta_speed);

  /**
   * this function sets the heading of the object to the new_heading
   * @param new_heading the new heading of the object
   */
  void setNewHeading(scalar new_heading);

  /**
   * this function sets the pitch of the object to the new_pitch
   * @param new_pitch the new pitch of the object
   */
  void setNewPitch(scalar new_pitch);

  /**
   * this function sets the roll of the object to the new_roll
   * @param new_roll the new roll of the object
   */
  void setNewRoll(scalar new_roll);

  /**
   * this function sets the speed of the object to the new_speed
   * @param new_speed the new speed of the object
   */
  void setNewSpeed(scalar new_speed);

  /**
   * this function sets the velocity vector to the given vector. The speed, heading and pitch are corrected to match the new velocity vector
   * @param vector the new velocity vector of the object
   */
  void setVelocityVector(const WP_Vector3D& vector);

  /**
   * this function moves the object according its velocity vector
   */
  void move();

  /**
   * this function prints the actual state of the dynamic object to standard output
   */
  void print() const;

  void onCollision()
    {
      // collision handling
    }

 protected:
  WP_DynamicObject(){};

  /**
   * @param _matrix a WP_Matrix3D object holding the world matrix of this object
   * @param name the name of the object
   * @param _velocity a WP_Vector3D object representing the velocity vector of this object
   */
  WP_DynamicObject(const WP_Matrix3D& _matrix, const string& name, const WP_Vector3D& _velocity);

  /**
   * @param _matrix a WP_Matrix3D object holding the world matrix of this object
   * @param name the name of the object
   * @param _heading the heading in degrees of the object
   * @param _speed the speed of the object
   * @param _pitch the pitch in degrees of the object
   */
  WP_DynamicObject(const WP_Matrix3D& _matrix, const string& name, scalar _heading, scalar _speed, scalar _pitch);

  /**
   * this function computes the heading of the object according to its velocity vector
   * @return the heading in degrees of the object
   */
  scalar computeHeading() const;

  /**
   * this function computes the pitch of the object according to its velocity vector
   * @return the pitch in degrees of the object
   */
  scalar computePitch() const;

  /**
   * this function computes the velocity vector of the object according to the object its speed, heading and pitch
   * @return a WP_Vector3D object representing the velocity vector of the object
   */
  WP_Vector3D computeVelocityVector();

  /**
   * a pointer to the only instance of the WP_Math class
   */
  static WP_Math* math;
};

/**
 * this singleton class creates and manages all created static and dynamic objects\n
 * @author Copyright (C) 2001-2003 W.P. van Paassen   peter@paassen.tmfweb.nl
 *
 *  This program is free software; you can redistribute it and/or modify it under
 *  the terms of the GNU General Public License as published by the Free
 *  Software Foundation; either version 2 of the License, or (at your
 *  option) any later version.
 *
 *  This program is distributed in the hope that it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; see the file COPYING.  If not, write to the Free
 *  Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  
 */
class WP_ObjectManager
{
public:
  ~WP_ObjectManager();

  /**
   * this function is used to obtain a pointer to the only instance of this class (singleton)
   * @return a pointer to the only instance of this class
   */
  static WP_ObjectManager* getInstance()
    {
      if (!om_instance)
	{
	  om_instance = new WP_ObjectManager();
	}
      return om_instance;
    }

  /**
   * this function is used for the creation of a static object
   * @param matrix a WP_Matrix3D object holding the world matrix of this object
   * @param object_name the name of the object
   * @param model_name the name of the 3D model belonging to the object
   * @return a pointer to a WP_Object representing the newly created static object
   */
  WP_StaticObject* createStaticObject(const WP_Matrix3D& matrix, const string& object_name, const string& model_name);

  /**
   * this function is used for the creation of a dynamic object
   * @param matrix a WP_Matrix3D object holding the world matrix of this object
   * @param object_name the name of the object
   * @param model_name the name of the 3D model belonging to the object
   * @param velocity a WP_Vector3D object representing the velocity vector of the object
   * @return a pointer to a WP_Object representing the newly created dynamic object
   */
  WP_DynamicObject* createDynamicObject(const WP_Matrix3D& matrix, const string& object_name, const string& model_name, 
				 const WP_Vector3D& velocity = WP_Vector3D());

  void addLight(const WP_Point3D& position, const WP_Color& ambient, const WP_Color& diffuse, const WP_Color& specular, const WP_Color& emissive, bool remote = true);  

  WP_Light* getLight(int index);

  /**
   * this function returns a pointer to an object with name <i>name</i>
   * @param name the name of the to be retrieved object
   * @return a pointer to a WP_Object representing the object with name <i>name</i>
   */
  WP_Object* getObject(const string& name) const;

  /**
   * this function returns the next static object following <i>last</i> in the object manager's internal static object list
   * @param last a pointer to a WP_StaticObject to indicate the position of the object preceeding the wanted object. If this pointer is 0, the first object in the static object list is returned. 
   * @return a pointer to a Object representing the next object (0 in case of no available objects)
   */
  WP_StaticObject* getNextStaticObject(const WP_StaticObject* last) const;

  /**
   * this function returns the next dynamic object following <i>last</i> in the object manager's internal dynamic object list
   * @param last a pointer to a WP_DynamicObject to indicate the position of the object preceeding $the wanted object. If this pointer is 0, the first object in the dynamic object list is returned. 
   * @return a pointer to a WP_Object representing the next object (0 in case of no available objects)
   */
  WP_DynamicObject* getNextDynamicObject(const WP_DynamicObject* last) const;

  /**
   * this function returns the first static object in the object manager's internal static object list
   * @return a pointer to a WP_Static object representing the first object in the static object list
   */
  WP_StaticObject* getStaticObject() const;

  /**
   * this function returns the first dynamic object in the object manager's internal dynamic object list
   * @return a pointer to a WP_Dynamic object representing the first object in the dynamic object list
   */
  WP_DynamicObject* getDynamicObject() const;

  /**
   * this function returns the static object from the object manager's internal static object list with name <i>name</i>
   * @param name the name of the static object
   * @return a pointer to a WP_StaticObject object matching <i>name</i>
   */
  WP_StaticObject* getStaticObject(const string& name) const;

  /**
   * this function returns the dynamic object from the object manager's internal dynamic object list with name <i>name</i>
   * @param name the name of the dynamic object
   * @return a pointer to a WP_DynamicObject object matching <i>name</i>
   */
  WP_DynamicObject* getDynamicObject(const string& name) const;

  /**
   * this function removes the first object it encounters with name <i>name</i> from the static or dynamic object list
   * @param name the name of the to be removed object
   * @return a boolean indicating of an object was removed
   */
  bool removeObject(const string& name);

  /**
   * this function removes the first object it encounters with name <i>name</i> from the static object list
   * @param name the name of the to be removed object
   * @return a boolean indicating of an object was removed
   */
  bool removeStaticObject(const string& name);

  /**
   * this function removes the first object it encounters with name <i>name</i> from the dynamic object list
   * @param name the name of the to be removed object
   * @return a boolean indicating of an object was removed
   */
  bool removeDynamicObject(const string& name);

  /**
   * this function removes all objects called <i>name</i> from the static and dynamic object list
   * @param name the name of the to be removed objects
   * @return a boolean indicating if all objects called <i>name</i> were removed succesfully
   */
  bool removeSameObjects(const string& name);

  /**
   * this function removes all objects from the object manager's internal static and dynamic object lists
   * @return a boolean indicating if all objects were removed succesfully
   */
  bool removeAll();

  /**
   * this function draws all objects currently in the camera's viewing volume to the selection buffer. In this way it is possible to determine which object in the 3D scene was picked by the user
   */
  void drawObjectsSelection();
  
  /**
   * this function determines if an object was picked according to mouse input and returns this object
   * @param x the x coordinate of the mouse-click
   * @param y the y coordinate of the mouse-click
   * @return a pointer to a WP_Object object which is the picked object. 0 if no object was picked
   */
  WP_Object* pickObject(int x, int y);

  unsigned int number_collisions;

  void createCollisionPairs();

  void updateAll();

  bool notInterpolatedFrame();

  /**
   * this function draws all objects currently in the camera's viewing volume (frustum)
   */
  void drawObjects();

  WP_RayHitPoint castRay(const WP_Ray3D& ray, bool check_frustum=true);
  bool inShadow(const WP_Point3D& p, const WP_Point3D& light);

private:
  WP_ObjectManager();

  void checkCollisions();

  /**
   * a list of pointers to WP_StaticObject objects containing all static objects
   */
  list<WP_StaticObject*> static_objects;

  /**
   * a list of pointers to WP_DynamicObject objects containing all dynamic objects
   */
  list<WP_DynamicObject*> dynamic_objects; 

  /**
   * a list of remote or local lights in the scene
   */
  list<WP_Light*> lights;

  unsigned char number_lights;

  /**
   * a pointer to the only instance of the WP_ObjectManager class (singleton)
   */
  static WP_ObjectManager* om_instance;	 

  /**
   * this function checks the extension of a file
   * @param file the file name
   * @param extension the extension which is checked
   * @return a boolean indicating if the file has the extension
   */
  //FIXME this function is also used in WP_Image.C, therefore better to create a WP_File class and park this function there
  inline bool hasValidExtension(const string &file, const string &extension)
    {
      int pos = file.find('.' + extension);
      return pos != string::npos;
    }

  /**
   * this function searches all available static and dynamic objects and checks if their using the model with name <i>model_name</i>, if so this model can be reused by another object.
   * @param model_name the name of the searched model
   * @return a pointer to a WP_Model object. This pointer is 0 if no model matching <i>model_name</i> was found
   */
  WP_Model* findInstance(const string& model_name);

  /**
   * this function counts the number of objects in the object manager's internal static object list matching name <i>name</i>
   * @param name the name of the object
   * @return the number of static objects matching name <i>name</i>
   */
  int countStaticObjects(const string& name) const;

  /**
   * this function counts the number of objects in the object manager's internal dynamic object list matching name <i>name</i>
   * @param name the name of the object
   * @return the number of dynamic objects matching name <i>name</i>
   */
  int countDynamicObjects(const string& name) const;

  /**
   * a pointer to the only instance of the WP_Camera
   */
  WP_Camera* cam;

  /**
   * this unsigned integer is used to give all created objects a unique identifier
   */
  unsigned int unique;

  static const unsigned char num_internal_models;

  /**
   * static array of const strings representing internal models like for example WP_Square
   */
  static const string internal_models[];

  // nested class 

  class WP_CollisionPair
    {
    public:
      WP_CollisionPair(WP_Object *obj1, WP_Object *obj2);
      ~WP_CollisionPair(){};
      
      WP_Object *object1;
      WP_Object *object2;
      BVTCache cache;
    };

  /**
   * a list of pointers to WP_CollisionPair objects
   */
  list<WP_ObjectManager::WP_CollisionPair*> collision_pairs; 

  PlanesCollider PC;
  AABBTreeCollider TC;
  RayCollider RC;
  RayCollider SF; //shadowfeeler
  CollisionFaces CF;

  //OPCODE CALLBACK
  static void ColCallback(udword triangleindex, VertexPointers &triangle, uqword user_data);
};
}
#endif


